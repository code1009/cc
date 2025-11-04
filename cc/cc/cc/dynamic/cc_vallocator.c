/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_vallocator.c
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_vallocator_initialize(cc_vallocator_t* ctx, const void* handle, const cc_vallocate_t allocate, const cc_vfree_t free)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(handle != NULL); 
	cc_debug_assert(allocate != NULL);
	cc_debug_assert(free != NULL);


	ctx->handle = handle;
	ctx->allocate = allocate;
	ctx->free = free;
}
