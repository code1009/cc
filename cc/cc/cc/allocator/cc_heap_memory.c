/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_heap_memory.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
// @ Low fragmentation heap memory
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_simple_segregated_storage.h"
#include "cc_first_fit.h"

#include "cc_heap_memory.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_heap_memory_calc_aligned_size(size_t v, size_t alignment_size)
{
	cc_debug_assert(alignment_size != 0);


	size_t count;
	count = v / alignment_size;
	if (0U != (v % alignment_size))
	{
		count++;
	}
	return alignment_size * count;
}

static inline size_t cc_heap_memory_alignment_size(void)
{
	return sizeof(void*);
}

static inline bool cc_heap_memory_is_aligned_address(const uintptr_t address)
{
	return (0U == (address % cc_heap_memory_alignment_size()));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_heap_bucket_descriptor_copy(cc_heap_bucket_descriptor_t* dst, const cc_heap_bucket_descriptor_t* src)
{
	cc_debug_assert(dst != NULL);
	cc_debug_assert(src != NULL);


	dst->size = src->size;
	dst->count = src->count;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_heap_memory_buckets_alignment_size(void)
{
	return cc_heap_memory_calc_aligned_size(sizeof(cc_first_fit_block_head_t), cc_heap_memory_alignment_size());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline cc_heap_bucket_t* cc_heap_memory_find_bucket(const cc_heap_memory_t* ctx, size_t size)
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

static inline size_t cc_heap_memory_bucket_head_alignment_size(void)
{
	return sizeof(void*);
}

static inline size_t cc_heap_memory_bucket_body_alignment_size(void)
{
	return sizeof(void*);
}

static inline size_t cc_heap_memory_bucket_alignment_size(void)
{
	return sizeof(void*);
}

static inline size_t cc_heap_memory_calc_bucket_head_memory_size(void)
{
	size_t head_size = sizeof(cc_heap_bucket_region_head_t);
	size_t head_memory_size = cc_heap_memory_calc_aligned_size(head_size, cc_heap_memory_bucket_head_alignment_size());

	return head_memory_size;
}

static inline size_t cc_heap_memory_calc_bucket_body_size(cc_heap_bucket_t* bucket)
{
	cc_debug_assert(bucket != NULL);


	size_t body_size = bucket->descriptor.size * bucket->descriptor.count;

	return body_size;
}

static inline size_t cc_heap_memory_calc_bucket_body_memory_size(size_t body_size)
{
	size_t body_memory_size = cc_heap_memory_calc_aligned_size(body_size, cc_heap_memory_bucket_body_alignment_size());

	return body_memory_size;
}

static inline cc_heap_bucket_region_head_t* cc_heap_memory_add_bucket_storage(cc_heap_memory_t* ctx, cc_heap_bucket_t* bucket)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(bucket != NULL);


	//-----------------------------------------------------------------------
	size_t head_memory_size = cc_heap_memory_calc_bucket_head_memory_size();

	size_t body_size = cc_heap_memory_calc_bucket_body_size(bucket);
	size_t body_memory_size = cc_heap_memory_calc_bucket_body_memory_size(body_size);

	size_t size = head_memory_size + body_memory_size;
	size_t memory_size = cc_heap_memory_calc_aligned_size(size, cc_heap_memory_bucket_alignment_size());
	uint8_t* memory_pointer = (uint8_t*)cc_first_fit_allocate(&ctx->first_fit, memory_size);
	if(NULL==memory_pointer)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	bool rv;

	uint8_t* head_pointer = memory_pointer;
	uint8_t* body_pointer = memory_pointer + head_memory_size;

	cc_heap_bucket_region_head_t* bucket_storage = (cc_heap_bucket_region_head_t*)head_pointer;
	rv = cc_simple_segregated_storage_initialize(
		&bucket_storage->simple_segregated_storage,
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
	bucket_storage->next = NULL;


	//-----------------------------------------------------------------------
	if (NULL == bucket->regions)
	{
		bucket->regions = bucket_storage;
	}
	else
	{
		cc_heap_bucket_region_head_t* current;
		current = bucket->regions;
		while (current->next != NULL)
		{
			current = (cc_heap_bucket_region_head_t*)current->next;
		}
		current->next = bucket_storage;
	}


	return bucket_storage;
}

static inline void* cc_heap_memory_allocate_from_bucket(const cc_heap_memory_t* ctx, cc_heap_bucket_t* bucket)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(bucket != NULL);


	//-----------------------------------------------------------------------
	cc_heap_bucket_region_head_t* current = bucket->regions;
	
	while (current)
	{
		void* pointer = cc_simple_segregated_storage_allocate(&current->simple_segregated_storage);
		if (pointer != NULL)
		{
			return pointer;
		}

		current = (cc_heap_bucket_region_head_t*)current->next;
	}

	return NULL;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_validate_pointer(const cc_heap_memory_t* ctx, const void* pointer)
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
		cc_heap_bucket_t* bucket = &ctx->buckets.elements[i];
		cc_heap_bucket_region_head_t* current = bucket->regions;
		while (current)
		{
			if (cc_simple_segregated_storage_validate_pointer(&current->simple_segregated_storage, pointer))
			{
				return true;
			}
			current = (cc_heap_bucket_region_head_t*)current->next;
		}
	}

	return cc_first_fit_validate_pointer(&ctx->first_fit, pointer);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_initialize(cc_heap_memory_t* ctx, const void* memory_pointer, const size_t memory_size, const cc_heap_bucket_descriptors_t* bucket_descriptors)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(bucket_descriptors != NULL);


	//-----------------------------------------------------------------------
	if (cc_heap_memory_is_aligned_address((uintptr_t)memory_pointer) == false)
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


	//-----------------------------------------------------------------------
	ctx->count = 0;


	//-----------------------------------------------------------------------
	bool rv;


	rv = cc_first_fit_initialize(&ctx->first_fit, memory_pointer, memory_size);
	if (rv == false)
	{
		return false;
	}


	size_t buckets_size = sizeof(cc_heap_bucket_t) * bucket_descriptors->count;
	size_t buckets_memory_size = cc_heap_memory_calc_aligned_size(buckets_size, cc_heap_memory_buckets_alignment_size());
	cc_heap_bucket_t* buckets = cc_first_fit_allocate(&ctx->first_fit, buckets_memory_size);
	if (buckets == NULL)
	{
		return false;
	}
	if (ctx->first_fit.free_size < cc_heap_memory_calc_bucket_head_memory_size())
	{
		cc_first_fit_free(&ctx->first_fit, buckets);
		return false;
	}

	size_t max_memory_size = ctx->first_fit.free_size - cc_heap_memory_calc_bucket_head_memory_size();
	size_t max_count;
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

		max_count = max_memory_size / bucket_descriptors->elements[i].size;
		if (bucket_descriptors->elements[i].count > max_count)
		{
			cc_first_fit_free(&ctx->first_fit, buckets);
			return false;
		}

		cc_heap_bucket_descriptor_copy(&buckets[i].descriptor, &bucket_descriptors->elements[i]);
		buckets[i].regions = NULL;
	}


	ctx->buckets.elements = buckets;
	ctx->buckets.count = bucket_descriptors->count;

	return true;
}

cc_api void cc_heap_memory_uninitialize(cc_heap_memory_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);


	//-----------------------------------------------------------------------
	for (size_t i = 0; i < ctx->buckets.count; ++i)
	{
		cc_heap_bucket_t* bucket = &ctx->buckets.elements[i];
		cc_heap_bucket_region_head_t* current = bucket->regions;
		while (current)
		{
			cc_heap_bucket_region_head_t* next = current->next;

			cc_first_fit_free(&ctx->first_fit, (void*)current);
			current = next;
		}
		bucket->regions = NULL;
	}


	//-----------------------------------------------------------------------
	cc_first_fit_free(&ctx->first_fit, ctx->buckets.elements);
	ctx->buckets.elements = NULL;
	ctx->buckets.count = 0;
	ctx->count = 0;
}

//===========================================================================
cc_api void* cc_heap_memory_allocate(cc_heap_memory_t* ctx, const size_t size)
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
	cc_heap_bucket_t* bucket = cc_heap_memory_find_bucket(ctx, size);
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
	pointer = cc_heap_memory_allocate_from_bucket(ctx, bucket);
	if (pointer != NULL)
	{ 
		ctx->count++;
		return pointer;
	}


	//-----------------------------------------------------------------------
	cc_heap_bucket_region_head_t* bucket_storage = cc_heap_memory_add_bucket_storage(ctx, bucket);

	if (bucket_storage == NULL)
	{
		return NULL;
	}

	pointer = cc_simple_segregated_storage_allocate(&bucket_storage->simple_segregated_storage);
	if (pointer != NULL)
	{
		ctx->count++;
		return pointer;
	}


	cc_debug_assert(false);
	return NULL;
}

cc_api bool cc_heap_memory_free(cc_heap_memory_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(pointer != NULL);
	cc_debug_assert(ctx->buckets.count != 0);
	cc_debug_assert(ctx->buckets.elements != NULL);
	cc_debug_assert(ctx->count > 0);


	//-----------------------------------------------------------------------
	// Try buckets first (fast-path for small fixed-size allocations)
	for (size_t i=0; i<ctx->buckets.count; i++)
	{
		cc_heap_bucket_t* bucket = &ctx->buckets.elements[i];
		cc_heap_bucket_region_head_t* current = bucket->regions;
		cc_heap_bucket_region_head_t* previous = NULL;
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
							bucket->regions = (cc_heap_bucket_region_head_t*)current->next;
						}
						else
						{
							previous->next = current->next;
						}

						cc_first_fit_free(&ctx->first_fit, (void*)current);
					}

					return true;
				}
				return false;
			}
			previous = current;


			current = (cc_heap_bucket_region_head_t*)current->next;
		}
	}


	//-----------------------------------------------------------------------
	// Not in any bucket => try underlying first-fit storage
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

cc_api cc_ssize_t cc_heap_memory_count(cc_heap_memory_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	return ctx->count;
}
