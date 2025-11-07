/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_lf_heap.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
//   @ Low fragmentation heap.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "../allocator/cc_simple_segregated_storage.h"
#include "cc_first_fit.h"

#include "cc_lf_heap.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline bool cc_lf_heap_add_overflow(const size_t a, const size_t b)
{
	return (a > ((size_t)-1 - b));
}

static inline bool cc_lf_heap_mul_overflow(const size_t a, const size_t b)
{
	if (a == 0 || b == 0)
	{
		return false;
	}
	size_t c = ((size_t)-1) / b;
	return (a > c);
}

static inline size_t cc_lf_heap_alignment(void)
{
	return sizeof(void*);
}

static inline size_t cc_lf_heap_align(const size_t value, const size_t alignment)
{
	cc_debug_assert(alignment != 0);


	size_t count;
	count = value / alignment;
	if (0U != (value % alignment))
	{
		count++;
	}
	if (cc_lf_heap_mul_overflow(alignment, count))
	{
		return cc_invalid_size;
	}
	return alignment * count;
}

static inline bool cc_lf_heap_is_aligned(const uintptr_t value, const size_t alignment)
{
	cc_debug_assert(alignment != 0);


	return (0U == (value % alignment));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_lf_heap_bucket_descriptor_copy(cc_lf_heap_bucket_descriptor_t* dst, const cc_lf_heap_bucket_descriptor_t* src)
{
	cc_debug_assert(dst != NULL);
	cc_debug_assert(src != NULL);


	dst->size = src->size;
	dst->count = src->count;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_lf_heap_buckets_alignment_size(void)
{
	return cc_lf_heap_align(sizeof(cc_first_fit_block_head_t), cc_lf_heap_alignment());
}

static inline size_t cc_lf_heap_buckets_aligned_size(const size_t bucket_count)
{
	if (cc_lf_heap_mul_overflow(sizeof(cc_lf_heap_bucket_t), bucket_count))
	{
		return cc_invalid_size;
	}
	return cc_lf_heap_align(sizeof(cc_lf_heap_bucket_t) * bucket_count, cc_lf_heap_buckets_alignment_size());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_lf_heap_bucket_region_head_alignment_size(void)
{
	return cc_lf_heap_alignment();
}

static inline size_t cc_lf_heap_bucket_region_body_alignment_size(void)
{
	return cc_lf_heap_alignment();
}

static inline size_t cc_lf_heap_bucket_region_alignment_size(void)
{
	return cc_lf_heap_alignment();
}

//===========================================================================
static inline size_t cc_lf_heap_bucket_region_head_aligned_size(void)
{
	return cc_lf_heap_align(sizeof(cc_lf_heap_bucket_region_head_t), cc_lf_heap_bucket_region_head_alignment_size());
}

static inline size_t cc_lf_heap_bucket_region_body_aligned_size(const size_t body_size)
{
	return cc_lf_heap_align(body_size, cc_lf_heap_bucket_region_body_alignment_size());
}

static inline size_t cc_lf_heap_bucket_region_aligned_size(const size_t body_size)
{
	size_t head_aligned_size = cc_lf_heap_bucket_region_head_aligned_size();
	size_t body_aligned_size = cc_lf_heap_bucket_region_body_aligned_size(body_size);
	if (cc_lf_heap_add_overflow(head_aligned_size, body_aligned_size))
	{
		return cc_invalid_size;
	}
	return cc_lf_heap_align(
		head_aligned_size + body_aligned_size,
		cc_lf_heap_bucket_region_alignment_size()
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline cc_lf_heap_bucket_t* cc_lf_heap_find_bucket(const cc_lf_heap_t* ctx, const size_t size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	for (size_t i = 0; i < ctx->buckets.count; i++)
	{
		if (size <= ctx->buckets.elements[i].descriptor.size)
		{
			return &ctx->buckets.elements[i];
		}
	}

	return NULL;
}

static inline cc_lf_heap_bucket_region_head_t* cc_lf_heap_new_bucket_region(cc_lf_heap_t* ctx, cc_lf_heap_bucket_t* bucket)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(bucket != NULL);


	//-----------------------------------------------------------------------
	size_t simple_segregated_storage_memory_size = cc_simple_segregated_storage_memory_size(bucket->descriptor.size, bucket->descriptor.count);
	size_t body_size = cc_lf_heap_bucket_region_body_aligned_size(simple_segregated_storage_memory_size);

	
	//-----------------------------------------------------------------------
	size_t head_size = cc_lf_heap_bucket_region_head_aligned_size();


	//-----------------------------------------------------------------------
	size_t memory_size = cc_lf_heap_bucket_region_aligned_size(simple_segregated_storage_memory_size);
	uint8_t* memory_pointer = (uint8_t*)cc_first_fit_allocate(&ctx->first_fit, memory_size);
	if(NULL==memory_pointer)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	bool rv;

	uint8_t* head_pointer = memory_pointer;
	uint8_t* body_pointer = memory_pointer + head_size;

	cc_lf_heap_bucket_region_head_t* bucket_region = (cc_lf_heap_bucket_region_head_t*)head_pointer;
	rv = cc_simple_segregated_storage_initialize(
		&bucket_region->simple_segregated_storage,
		body_pointer,
		body_size,
		bucket->descriptor.size,
		bucket->descriptor.count
	);
	if (rv == false)
	{
		cc_first_fit_free(&ctx->first_fit, memory_pointer);
		return NULL;
	}
	bucket_region->next = NULL;


	//-----------------------------------------------------------------------
	if (NULL == bucket->regions)
	{
		bucket->regions = bucket_region;
	}
	else
	{
		cc_lf_heap_bucket_region_head_t* current;
		current = bucket->regions;
		while (current->next != NULL)
		{
			current = (cc_lf_heap_bucket_region_head_t*)current->next;
		}
		current->next = bucket_region;
	}


	//-----------------------------------------------------------------------
	bucket->cache_region = bucket_region;


	return bucket_region;
}

static inline void* cc_lf_heap_allocate_from_bucket(cc_lf_heap_bucket_t* bucket)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(bucket != NULL);


	//-----------------------------------------------------------------------
	if (NULL != bucket->cache_region)
	{
		void* pointer = cc_simple_segregated_storage_allocate(&bucket->cache_region->simple_segregated_storage);
		if (pointer != NULL)
		{
			return pointer;
		}
	}


	//-----------------------------------------------------------------------
	cc_lf_heap_bucket_region_head_t* current = bucket->regions;
	
	while (current)
	{
		void* pointer = cc_simple_segregated_storage_allocate(&current->simple_segregated_storage);
		if (pointer != NULL)
		{
			bucket->cache_region = current;

			return pointer;
		}

		current = (cc_lf_heap_bucket_region_head_t*)current->next;
	}

	return NULL;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_lf_heap_validate_pointer(const cc_lf_heap_t* ctx, const void* pointer)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);


	uintptr_t begin;
	uintptr_t end;
	uintptr_t current;

	begin = (uintptr_t)ctx->first_fit.memory_pointer;
	end = (uintptr_t)ctx->first_fit.end_block;
	current = (uintptr_t)pointer;
	if (begin > current)
	{
		return false;
	}
	if (end <= current)
	{
		return false;
	}

	for (size_t i = 0; i < ctx->buckets.count; ++i)
	{
		cc_lf_heap_bucket_t* bucket = &ctx->buckets.elements[i];
		cc_lf_heap_bucket_region_head_t* current = bucket->regions;
		while (current)
		{
			if (cc_simple_segregated_storage_validate_pointer(&current->simple_segregated_storage, pointer))
			{
				return true;
			}
			current = (cc_lf_heap_bucket_region_head_t*)current->next;
		}
	}

	return cc_first_fit_validate_pointer(&ctx->first_fit, pointer);
}

cc_api bool cc_lf_heap_add_bucket_region(cc_lf_heap_t* ctx, size_t index)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);

	
	if (ctx->buckets.elements == NULL)
	{
		return false;
	}
	if (ctx->buckets.count == 0)
	{
		return false;
	}


	cc_lf_heap_bucket_t* bucket = &ctx->buckets.elements[index];
	cc_lf_heap_bucket_region_head_t* bucket_region = cc_lf_heap_new_bucket_region(ctx, bucket);
	if (bucket_region == NULL)
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_lf_heap_initialize(cc_lf_heap_t* ctx, const void* memory_pointer, const size_t memory_size, const cc_lf_heap_bucket_descriptors_t* bucket_descriptors)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(bucket_descriptors != NULL);


	//-----------------------------------------------------------------------
	if (cc_lf_heap_is_aligned((uintptr_t)memory_pointer, cc_lf_heap_alignment()) == false)
	{
		return false;
	}
	if (bucket_descriptors->elements == NULL)
	{
		return false;
	}
	if (bucket_descriptors->count == 0)
	{
		return false;
	}


	size_t buckets_size = cc_lf_heap_buckets_aligned_size(bucket_descriptors->count);
	if (cc_invalid_size == buckets_size)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	bool rv; 

	rv = cc_first_fit_initialize(&ctx->first_fit, memory_pointer, memory_size);
	if (rv == false)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	cc_lf_heap_bucket_t* buckets = cc_first_fit_allocate(&ctx->first_fit, buckets_size);
	if (buckets == NULL)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	size_t simple_segregated_storage_memory_size;	

	uintptr_t begin_address = (uintptr_t)ctx->first_fit.memory_pointer;
	uintptr_t end_address = (uintptr_t)cc_first_fit_get_block_payload_pointer(&ctx->first_fit, (cc_first_fit_block_head_t*)ctx->first_fit.memory_pointer);
	size_t first_fit_block_head_size = end_address - begin_address;

	size_t bucket_region_size;


	size_t required_memory_size;

	for (size_t i =0; i < bucket_descriptors->count; i++)
	{
		if (bucket_descriptors->elements[i].size == 0)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}
		if (bucket_descriptors->elements[i].count == 0)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}

		
		simple_segregated_storage_memory_size = cc_simple_segregated_storage_memory_size(bucket_descriptors->elements[i].size, bucket_descriptors->elements[i].count);
		if (cc_invalid_size == simple_segregated_storage_memory_size)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}
		bucket_region_size = cc_lf_heap_bucket_region_aligned_size(simple_segregated_storage_memory_size);
		if (cc_invalid_size == bucket_region_size)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}
		if (cc_lf_heap_add_overflow(first_fit_block_head_size, bucket_region_size))
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}


		required_memory_size = first_fit_block_head_size + bucket_region_size;
		if (ctx->first_fit.free_size < required_memory_size)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}


		cc_lf_heap_bucket_descriptor_copy(&buckets[i].descriptor, &bucket_descriptors->elements[i]);
		buckets[i].regions = NULL;
		buckets[i].cache_region = NULL;
	}


	//-----------------------------------------------------------------------
	ctx->buckets.elements = buckets;
	ctx->buckets.count = bucket_descriptors->count;
	
	ctx->count = 0;

	return true;
}

cc_api void cc_lf_heap_uninitialize(cc_lf_heap_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);


#if (1==cc_config_debug)
	//-----------------------------------------------------------------------
	for (size_t i = 0; i < ctx->buckets.count; ++i)
	{
		cc_lf_heap_bucket_t* bucket = &ctx->buckets.elements[i];
		cc_lf_heap_bucket_region_head_t* current = bucket->regions;
		while (current)
		{
			cc_lf_heap_bucket_region_head_t* next = current->next;

			cc_first_fit_free(&ctx->first_fit, (void*)current);
			current = next;
		}
		bucket->regions = NULL;
	}


	//-----------------------------------------------------------------------
	cc_first_fit_free(&ctx->first_fit, ctx->buckets.elements);
#endif


	//-----------------------------------------------------------------------
	ctx->buckets.elements = NULL;
	ctx->buckets.count = 0;
	ctx->count = 0;
}

//===========================================================================
cc_api void* cc_lf_heap_allocate(cc_lf_heap_t* ctx, const size_t size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);


	//-----------------------------------------------------------------------
	if(size == 0)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	void* pointer;


	//-----------------------------------------------------------------------
	cc_lf_heap_bucket_t* bucket = cc_lf_heap_find_bucket(ctx, size);
	if (bucket == NULL)
	{
		pointer = cc_first_fit_allocate(&ctx->first_fit, size);
		if (pointer != NULL)
		{
			ctx->count++;
			return pointer;
		}

		return NULL;
	}


	//-----------------------------------------------------------------------
	pointer = cc_lf_heap_allocate_from_bucket(bucket);
	if (pointer != NULL)
	{ 
		ctx->count++;
		return pointer;
	}


	//-----------------------------------------------------------------------
	cc_lf_heap_bucket_region_head_t* bucket_region= cc_lf_heap_new_bucket_region(ctx, bucket);
	if (bucket_region == NULL)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	pointer = cc_simple_segregated_storage_allocate(&bucket_region->simple_segregated_storage);
	if (pointer != NULL)
	{
		ctx->count++;
		return pointer;
	}


	cc_debug_assert(false);
	return NULL;
}

cc_api bool cc_lf_heap_free(cc_lf_heap_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(pointer != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);
	cc_debug_assert(ctx->count > 0);


	//-----------------------------------------------------------------------
	for (size_t i=0; i<ctx->buckets.count; i++)
	{
		cc_lf_heap_bucket_t* bucket = &ctx->buckets.elements[i];


		if (bucket->cache_region != NULL)
		{
			if (cc_simple_segregated_storage_validate_pointer(&bucket->cache_region->simple_segregated_storage, pointer))
			{
				bool rv = cc_simple_segregated_storage_free(&bucket->cache_region->simple_segregated_storage, pointer);
				if (rv)
				{
					ctx->count--;
					if (bucket->cache_region->simple_segregated_storage.count == 0)
					{
						cc_lf_heap_bucket_region_head_t* cache_region = bucket->cache_region;
						bucket->cache_region = NULL;


						cc_lf_heap_bucket_region_head_t* current = bucket->regions;
						cc_lf_heap_bucket_region_head_t* previous = NULL;
						while (current)
						{
							if (current == cache_region)
							{
								if (previous == NULL)
								{
									bucket->regions = (cc_lf_heap_bucket_region_head_t*)current->next;
								}
								else
								{
									previous->next = current->next;
								}
								cc_first_fit_free(&ctx->first_fit, (void*)current);
								break;
							}
							previous = current;
							current = (cc_lf_heap_bucket_region_head_t*)current->next;
						}
					}
					return true;
				}
				return false;
			}
		}


		cc_lf_heap_bucket_region_head_t* current = bucket->regions;
		cc_lf_heap_bucket_region_head_t* previous = NULL;
		while (current)
		{
			if (cc_simple_segregated_storage_validate_pointer(&current->simple_segregated_storage, pointer))
			{
				bool rv = cc_simple_segregated_storage_free(&current->simple_segregated_storage, pointer);
				if (rv)
				{
					ctx->count--;

					if (current->simple_segregated_storage.count == 0)
					{
						if (previous == NULL)
						{
							bucket->regions = (cc_lf_heap_bucket_region_head_t*)current->next;
						}
						else
						{
							previous->next = current->next;
						}


						if (bucket->cache_region == current)
						{
							bucket->cache_region = NULL;
						}


						cc_first_fit_free(&ctx->first_fit, (void*)current);
					}

					return true;
				}
				return false;
			}
			previous = current;


			current = (cc_lf_heap_bucket_region_head_t*)current->next;
		}
	}


	//-----------------------------------------------------------------------
	if (cc_first_fit_validate_pointer(&ctx->first_fit, pointer))
	{
		bool rv = cc_first_fit_free(&ctx->first_fit, pointer);
		if (rv)
		{
			ctx->count--;
			return true;
		}
		return false;
	}


	//-----------------------------------------------------------------------
	// Pointer not recognized
	return false;
}

cc_api cc_ssize_t cc_lf_heap_count(cc_lf_heap_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	return ctx->count;
}
