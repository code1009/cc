/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_fallocator.c
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
#include "cc_fallocator.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_fallocator_initialize(cc_fallocator_t* ctx, const void* handle, const cc_fallocate_t allocate, const cc_ffree_t free)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(handle != NULL); 
	cc_debug_assert(allocate != NULL);
	cc_debug_assert(free != NULL);


	ctx->handle = (void*)handle;
	ctx->allocate = allocate;
	ctx->free = free;
}
