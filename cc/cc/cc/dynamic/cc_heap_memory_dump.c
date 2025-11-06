/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_heap_memory_dump.c
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
#include "cc_vallocator_adapter.h"

#include "cc_heap_memory.h"

#include "../allocator/cc_simple_segregated_storage_dump.h"
#include "cc_first_fit_dump.h"
#include "cc_lf_heap_dump.h"

//===========================================================================
#include "cc_heap_memory_dump.h"





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
cc_api void cc_heap_memory_dump(cc_heap_memory_t* heap_memory, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): heap_memory\n", (int64_t)number, heap_memory, cc_offset_address(heap_memory, base_address));


	//-----------------------------------------------------------------------
	cc_lf_heap_dump(&heap_memory->lf_heap, number, (uintptr_t)(&heap_memory->lf_heap.first_fit.memory_pointer[0]));
}
