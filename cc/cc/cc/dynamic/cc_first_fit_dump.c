/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_first_fit_dump.c
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
#include "cc_first_fit.h"

//===========================================================================
#include "cc_first_fit_dump.h"





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
cc_api void cc_first_fit_dump(cc_first_fit_t* first_fit, size_t number, uintptr_t base_address)
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
