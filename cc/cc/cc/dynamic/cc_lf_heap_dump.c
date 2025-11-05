/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_lf_heap_dump.c
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

//===========================================================================
#include "../allocator/cc_simple_segregated_storage_dump.h"
#include "cc_first_fit_dump.h"

//===========================================================================
#include "cc_lf_heap_dump.h"





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
cc_api void cc_lf_heap_bucket_region_dump(cc_lf_heap_bucket_region_head_t* lf_heap_bucket_region, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%llu] %p(%4lld): lf_heap_bucket_region\n", (int64_t)number, lf_heap_bucket_region, cc_offset_address(lf_heap_bucket_region, base_address));


	//-----------------------------------------------------------------------
	printf("- next               = %p(%4lld) \n", lf_heap_bucket_region->next, cc_offset_address(lf_heap_bucket_region->next, base_address));


	cc_simple_segregated_storage_dump(&lf_heap_bucket_region->simple_segregated_storage, number, base_address);
}

cc_api void cc_lf_heap_bucket_dump(cc_lf_heap_bucket_t* lf_heap_bucket, size_t number, uintptr_t base_address)
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
		cc_lf_heap_bucket_region_dump(region, i, base_address);
		region = region->next;
	}
}

cc_api void cc_lf_heap_dump(cc_lf_heap_t* lf_heap, size_t number, uintptr_t base_address)
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
			cc_lf_heap_bucket_dump(lf_heap_bucket, i, base_address);
		}
	}
}
