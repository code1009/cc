/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_vallocator_helper.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_vallocator.h"
#include "cc_first_fit.h"

#include "cc_simple_segregated_storage.h"
#include "cc_heap_memory.h"

//===========================================================================
#include "cc_vallocator_helper.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_vallocator_initialize(
	cc_vallocator_t* vallocator,
	cc_first_fit_t* first_fit, const void* memory_pointer, const size_t memory_size
)
{
	cc_debug_assert(vallocator != NULL);
	cc_debug_assert(first_fit != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);


	bool rv;

	rv = cc_first_fit_initialize(
		first_fit, 
		memory_pointer, memory_size
	);
	if (rv == false)
	{
		vallocator->handle = NULL;
		vallocator->allocate = NULL;
		vallocator->free = NULL;

		return false;
	}


	cc_vallocator_initialize(
		vallocator, 
		first_fit, (cc_vallocate_t)cc_first_fit_allocate, (cc_vfree_t)cc_first_fit_free
	);

	return true;
}

//===========================================================================
cc_api bool cc_heap_memory_vallocator_initialize(
	cc_vallocator_t* vallocator,
	cc_heap_memory_t* heap_memory, const void* memory_pointer, const size_t memory_size, const cc_heap_bucket_descriptors_t* bucket_descriptors
)
{
	cc_debug_assert(vallocator != NULL);
	cc_debug_assert(heap_memory != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(bucket_descriptors != NULL);


	bool rv;

	rv = cc_heap_memory_initialize(
		heap_memory,
		memory_pointer, memory_size,
		bucket_descriptors
	);
	if (rv == false)
	{
		vallocator->handle = NULL;
		vallocator->allocate = NULL;
		vallocator->free = NULL;
		return false;
	}


	cc_vallocator_initialize(
		vallocator,
		heap_memory, (cc_vallocate_t)cc_heap_memory_allocate, (cc_vfree_t)cc_heap_memory_free
	);

	return true;
}