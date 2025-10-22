/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_allocator_helper.c
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
#include "cc_allocator.h"
#include "cc_simple_segregated_storage.h"

//===========================================================================
#include "cc_allocator_helper.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_simple_segregated_storage_allocator_initialize(
	cc_allocator_t* allocator,
	cc_simple_segregated_storage_t* simple_segregated_storage, 
	const void* memory_pointer, const size_t memory_size, const size_t data_size, const cc_ssize_t max_count
)
{
	cc_debug_assert(allocator != NULL);
	cc_debug_assert(simple_segregated_storage != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(data_size != 0);
	cc_debug_assert(max_count != 0);


	bool rv;


	rv = cc_simple_segregated_storage_initialize(
		simple_segregated_storage, 
		memory_pointer, memory_size, data_size, max_count
	);
	if (rv == false)
	{
		allocator->handle = NULL;
		allocator->alloc = NULL;
		allocator->free = NULL;

		return false;
	}


	cc_allocator_initialize(
		allocator, 
		simple_segregated_storage, (cc_alloc_t)cc_simple_segregated_storage_allocate, (cc_free_t)cc_simple_segregated_storage_free
	);

	return true;
}
