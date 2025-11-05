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

#include "../allocator/cc_simple_segregated_storage_dump.h"
#include "cc_first_fit_dump.h"
#include "cc_lf_heap_dump.h"

#include "cc_vallocator.h"
#include "cc_vallocator_adapter.h"

//===========================================================================
#include "cc_string_allocator.h"





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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_string_allocator_dump(cc_string_allocator_t* string_allocator, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): string_allocator\n", (int64_t)number, string_allocator, cc_offset_address(string_allocator, base_address));


	//-----------------------------------------------------------------------
	cc_lf_heap_dump(&string_allocator->lf_heap, number, (uintptr_t)(&string_allocator->lf_heap.first_fit.memory_pointer[0]));
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




