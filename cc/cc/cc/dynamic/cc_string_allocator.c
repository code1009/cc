/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string_allocator.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "../allocator/cc_simple_segregated_storage.h"
#include "cc_first_fit.h"
#include "cc_lf_heap.h"

#include "cc_vallocator.h"
#include "cc_vallocator_helper.h"

//===========================================================================
#include "cc_string_allocator.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_first_fit_block_status_free      0U
#define cc_first_fit_block_status_allocated 1U

//===========================================================================
static inline size_t cc_first_fit_block_head_flags_make(const size_t size, const bool allocated)
{
	cc_debug_assert(size <= (((size_t)-1) >> 1));


	return (size << 1) | (allocated ? 1u : 0u);
}

static inline size_t cc_first_fit_block_head_flags_size(const size_t flags)
{
	return flags >> 1;
}

static inline bool cc_first_fit_block_head_flags_allocated(const size_t flags)
{
	return (flags & 1u) == cc_first_fit_block_status_allocated;
}

//===========================================================================
static inline size_t cc_first_fit_block_head_get_size(cc_first_fit_block_head_t* ctx)
{
	return cc_first_fit_block_head_flags_size(ctx->flags);
}

static inline bool cc_first_fit_block_head_is_allocated(cc_first_fit_block_head_t* ctx)
{
	return cc_first_fit_block_head_flags_allocated(ctx->flags);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline int64_t cc_offset_address(void* pointer, uintptr_t base_address)
{
	uintptr_t address = (uintptr_t)pointer;
	
	if (address >= base_address)
	{
		return (int64_t)(address - base_address);
	}
	return -1;
}

//===========================================================================
static inline void cc_first_fit_dump(cc_first_fit_t* first_fit, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): first_fit \n", (int64_t)number, first_fit, cc_offset_address(first_fit, base_address));


	//-----------------------------------------------------------------------
	printf("- count              = %lld       \n", (int64_t)first_fit->count);
	printf("- memory_pointer     = %p(%4lld)  \n", (void*)first_fit->memory_pointer, cc_offset_address(first_fit->memory_pointer, base_address));
	printf("- memory_size        = %lld bytes \n", (int64_t)first_fit->memory_size);
	printf("- free_size          = %lld bytes \n", (int64_t)first_fit->free_size);
	printf("- min_ever_free_size = %lld bytes \n", (int64_t)first_fit->min_ever_free_size);


	//-----------------------------------------------------------------------
	uintptr_t begin_address = (uintptr_t)first_fit->memory_pointer;
	uintptr_t end_address   = (uintptr_t)first_fit->end_block;
	uintptr_t max_free_size = end_address - begin_address;
	printf("- max_free_size      = %lld       \n", (int64_t)max_free_size);
	printf("- first_pointer      = %p(%4lld)  \n", (void*)first_fit->first_pointer_address, cc_offset_address((void*)first_fit->first_pointer_address, base_address));


	//-----------------------------------------------------------------------
	uintptr_t block_head_size = first_fit->first_pointer_address - begin_address;
	printf("- block_head_size    = %lld       \n", (int64_t)block_head_size);


	//-----------------------------------------------------------------------
	cc_first_fit_block_head_t* begin_block;
	cc_first_fit_block_head_t* end_block;


	begin_block = (cc_first_fit_block_head_t*)first_fit->memory_pointer;
	end_block = first_fit->end_block;
	printf("- begin_block        = %p(%4lld)        \n", (void*)begin_block, cc_offset_address(begin_block, base_address));
	printf("- end_block          = %p(%4lld)        \n", (void*)end_block, cc_offset_address(end_block, base_address));


	//-----------------------------------------------------------------------
	size_t block_count;
	size_t total_block_size;
	block_count = 0;
	total_block_size = 0;


	uintptr_t block_address;
	size_t block_size;
	bool block_is_allocated;


	cc_first_fit_block_head_t* block;
	for (block = begin_block; block < end_block; block_count++)
	{
		block_address = (uintptr_t)block;

		block_is_allocated = cc_first_fit_block_head_is_allocated(block);
		block_size = cc_first_fit_block_head_get_size(block);

		total_block_size += block_size;


		printf(
			"- block[%3lld] address = %p(%4lld), pointer = %p(%4lld) size = %6lld bytes %s \n",
			(int64_t)block_count,
			(void*)block, cc_offset_address(block, base_address),
			(void*)(block_address + block_head_size), cc_offset_address((void*)(block_address + block_head_size), base_address),
			(int64_t)block_size,
			(block_is_allocated ? "allocated" : "free     ")
		);


		block = (cc_first_fit_block_head_t*)(block_address + block_size);
	}


	printf("- block_count        = %lld       \n", (int64_t)block_count);
	printf("- total_block_size   = %lld bytes \n", (int64_t)total_block_size);
}

static inline void cc_simple_segregated_storage_dump(cc_simple_segregated_storage_t* simple_segregated_storage, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): simple_segregated_storage \n", (int64_t)number, simple_segregated_storage, cc_offset_address(simple_segregated_storage, base_address));


	//-----------------------------------------------------------------------
	printf("- count              = %lld       \n", (int64_t)simple_segregated_storage->count);
	printf("- memory_pointer     = %p(%4lld)  \n", (void*)simple_segregated_storage->memory_pointer, cc_offset_address(simple_segregated_storage->memory_pointer, base_address));
	printf("- memory_size        = %lld bytes \n", (int64_t)simple_segregated_storage->memory_size);
	printf("- data_size          = %lld bytes \n", (int64_t)simple_segregated_storage->data_size);
	printf("- max_count          = %lld       \n", (int64_t)simple_segregated_storage->max_count);
	printf("- chunk_size         = %lld bytes \n", (int64_t)simple_segregated_storage->chunk_size);
	printf("- free_chunk_head    = %p(%4lld)  \n", (void*)simple_segregated_storage->free_chunk_head, cc_offset_address(simple_segregated_storage->free_chunk_head, base_address));
}

static inline void cc_cc_lf_heap_bucket_region_dump(cc_lf_heap_bucket_region_head_t* lf_heap_bucket_region, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%llu] %p(%4lld): lf_heap_bucket_region\n", (int64_t)number, lf_heap_bucket_region, cc_offset_address(lf_heap_bucket_region, base_address));


	//-----------------------------------------------------------------------
	printf("- next               = %p(%4lld) \n", lf_heap_bucket_region->next, cc_offset_address(lf_heap_bucket_region->next, base_address));


	cc_simple_segregated_storage_dump(&lf_heap_bucket_region->simple_segregated_storage, number, base_address);
}

static inline void cc_cc_lf_heap_bucket_dump(cc_lf_heap_bucket_t* lf_heap_bucket, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): lf_heap_bucket\n", (int64_t)number, lf_heap_bucket, cc_offset_address(lf_heap_bucket, base_address));


	//-----------------------------------------------------------------------
	printf("- descriptor.size    = %lld bytes \n", (int64_t)lf_heap_bucket->descriptor.size);
	printf("- descriptor.count   = %lld       \n", (int64_t)lf_heap_bucket->descriptor.count);
	printf("- regions            = %p(%4lld)  \n", (void*)lf_heap_bucket->regions, cc_offset_address((void*)lf_heap_bucket->regions, base_address));
	printf("- cache_region       = %p(%4lld)  \n", (void*)lf_heap_bucket->cache_region, cc_offset_address((void*)lf_heap_bucket->cache_region, base_address));


	//-----------------------------------------------------------------------
	cc_lf_heap_bucket_region_head_t* region = lf_heap_bucket->regions;
	for (size_t i = 0; region != NULL; i++)
	{
		cc_cc_lf_heap_bucket_region_dump(region, i, base_address);
		region = region->next;
	}
}

static inline void cc_cc_lf_heap_dump(cc_lf_heap_t* lf_heap, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): lf_heap\n", (int64_t)number, lf_heap, cc_offset_address(lf_heap, base_address));


	//-----------------------------------------------------------------------
	printf("- count              = %lld \n", (int64_t)cc_lf_heap_count(lf_heap));
	printf("- buckets            = %lld \n", (int64_t)lf_heap->buckets.count);


	//-----------------------------------------------------------------------
	cc_first_fit_t* first_fit = &lf_heap->first_fit;
	cc_first_fit_dump(first_fit, number, base_address);


	//-----------------------------------------------------------------------
	if (NULL != lf_heap->buckets.elements)
	{
		for (size_t i = 0; i < lf_heap->buckets.count; i++)
		{
			cc_lf_heap_bucket_t* lf_heap_bucket = &lf_heap->buckets.elements[i];
			cc_cc_lf_heap_bucket_dump(lf_heap_bucket, i, base_address);
		}
	}
}

cc_api void cc_string_allocator_dump(cc_string_allocator_t* string_allocator, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): string_allocator\n", (int64_t)number, string_allocator, cc_offset_address(string_allocator, base_address));


	//-----------------------------------------------------------------------
	cc_cc_lf_heap_dump(&string_allocator->lf_heap, number, (uintptr_t)(&string_allocator->lf_heap.first_fit.memory_pointer[0]));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_string_allocator_lf_heap_bucket_count 4

#if (1==cc_config_platform_64bit)
#define cc_string_allocator_memory_size ( \
	0 \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_t) * cc_string_allocator_lf_heap_bucket_count) \
	\
    + ( \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 16 * 27 /*+  0*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 32 * 61 /*+ 16*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 64 * 14 /*+ 48*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + (128 *  7 /*+ 48*/)) \
	) * 2 \
	+ sizeof(cc_first_fit_block_head_t) \
	)
#endif

#if (1==cc_config_platform_32bit)
#define cc_string_allocator_memory_size ( \
	0 \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_t) * cc_string_allocator_lf_heap_bucket_count) \
	\
    + ( \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 16 * 29 /*+  8*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 32 * 62 /*+ 24*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + ( 64 * 15 /*+ 24*/)) \
	+ sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t) + (128 *  7 /*+ 88*/)) \
	) * 2 \
	+ sizeof(cc_first_fit_block_head_t) \
	)
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_string_allocator_initialize(cc_string_allocator_t* ctx, const void* memory_pointer, const size_t memory_size)
{
	cc_lf_heap_bucket_descriptor_t lf_heap_bucket_descriptor_elements[cc_string_allocator_lf_heap_bucket_count] =
	{
#if (1==cc_config_platform_64bit)
		// cc_first_fit_block_head_t:16 + cc_lf_heap_bucket_region_head_t:64 = 80
//		  8, ( 512 - 80) /   8, // ( 512 - 80) /   8 =  432 /   8 = 54  (나머지   0)
		 16, ( 512 - 80) /  16, // ( 512 - 80) /  16 =  432 /  16 = 27  (나머지   0)
		 32, (2048 - 80) /  32, // (2048 - 80) /  32 = 1968 /  32 = 61  (나머지  16)
//		 48, (2048 - 80) /  48, // (2048 - 80) /  48 = 1968 /  48 = 41  (나머지   0)
		 64, (1024 - 80) /  64, // (1024 - 80) /  64 =  944 /  64 = 14  (나머지  48)
//		 96, (1024 - 80) /  96, // (1024 - 80) /  96 =  944 /  96 =  9  (나머지  80)
		128, (1024 - 80) / 128  // (1024 - 80) / 128 =  944 / 128 =  7  (나머지  48)
//		256, (4096 - 80) / 256  // (4096 - 80) / 256 = 4016 / 256 = 15  (나머지 176)
#endif
#if (1==cc_config_platform_32bit)
		// cc_first_fit_block_head_t:8 + cc_lf_heap_bucket_region_head_t:32 = 40
//		  8, (512  - 40) /   8, // ( 512 - 40) /   8 =  472 /   8 = 59 (나머지   0)
		 16, (512  - 40) /  16, // ( 512 - 40) /  16 =  472 /  16 = 29 (나머지   8)
		 32, (2048 - 40) /  32, // (2048 - 40) /  32 = 2008 /  32 = 62 (나머지  24)
//		 48, (2048 - 40) /  48, // (2048 - 40) /  48 = 2008 /  48 = 41 (나머지  40)
		 64, (1024 - 40) /  64, // (1024 - 40) /  64 =  984 /  64 = 15 (나머지  24)
//		 96, (1024 - 40) /  96, // (1024 - 40) /  96 =  984 /  96 = 10 (나머지  24)
		128, (1024 - 40) / 128  // (1024 - 40) / 128 =  984 / 128 =  7 (나머지  88)
//		256, (4096 - 40) / 256  // (4096 - 40) / 256 = 4056 / 256 = 15 (나머지 216)
#endif
};


	cc_lf_heap_bucket_descriptors_t lf_heap_bucket_descriptors;
	lf_heap_bucket_descriptors.elements = lf_heap_bucket_descriptor_elements;
	lf_heap_bucket_descriptors.count = sizeof(lf_heap_bucket_descriptor_elements) / sizeof(cc_lf_heap_bucket_descriptor_t);


	return cc_lf_heap_vallocator_initialize(
		&ctx->iallocator,
		&ctx->lf_heap,
		memory_pointer, memory_size,
		&lf_heap_bucket_descriptors
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static cc_string_allocator_t _cc_default_string_allocator;
static uint8_t _default_cc_string_allocator_memory[cc_string_allocator_memory_size];

//===========================================================================
cc_api bool cc_default_string_allocator_initialize(void)
{
	return cc_string_allocator_initialize(&_cc_default_string_allocator, &_default_cc_string_allocator_memory[0], sizeof(_default_cc_string_allocator_memory));
}

cc_api void cc_default_string_allocator_uninitialize(void)
{
	cc_lf_heap_uninitialize(&_cc_default_string_allocator.lf_heap);
}

cc_api cc_string_allocator_t* cc_default_string_allocator(void)
{
	return &_cc_default_string_allocator;
}

cc_api void cc_default_string_allocator_dump(void)
{
	//-----------------------------------------------------------------------
	printf("\n");
	printf("# default_string_allocator \n");


	//-----------------------------------------------------------------------
	cc_string_allocator_dump(&_cc_default_string_allocator, 0, 0);


	//-----------------------------------------------------------------------
	printf("\n");
}




