/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_unordered_set.c
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
#include "cc_element.h"

//===========================================================================
#include "cc_bucket.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_bucket_config_macro_function)

cc_api void cc_bucket_initialize(cc_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_bucket_clear(ctx);
}

cc_api void* cc_bucket_element(cc_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->element.pointer;
}

cc_api cc_bucket_status_t cc_bucket_status(cc_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->status;
}

cc_api void cc_bucket_fill(cc_bucket_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);

	
	ctx->status = cc_bucket_status_filled;
	ctx->element.pointer = (void*)element;
}

cc_api void cc_bucket_remove(cc_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->status = cc_bucket_status_removed;
	ctx->element.pointer = NULL;
}

cc_api void cc_bucket_clear(cc_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->status = cc_bucket_status_empty;
	ctx->element.pointer = NULL;
}

#endif
