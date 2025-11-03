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
#include "cc_first_fit_storage.h"

//===========================================================================
#include "cc_vallocator_helper.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_storage_vallocator_initialize(
	cc_vallocator_t* vallocator,
	cc_first_fit_storage_t* first_fit_storage, const void* memory_pointer, const size_t memory_size
)
{
	cc_debug_assert(vallocator != NULL);
	cc_debug_assert(first_fit_storage != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);


	bool rv;


	rv = cc_first_fit_storage_initialize(
		first_fit_storage, 
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
		first_fit_storage, (cc_vallocate_t)cc_first_fit_storage_allocate, (cc_vfree_t)cc_first_fit_storage_free
	);

	return true;
}
