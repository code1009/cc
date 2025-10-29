/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_pair_bucket.c
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
#include "../algorithm/cc_compare.h"

#include "cc_element.h"
#include "cc_pair.h"
#include "cc_bucket.h"

//===========================================================================
#include "cc_pair_bucket.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_pair_bucket_config_macro_function)

cc_api void cc_pair_bucket_initialize(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_pair_bucket_clear(ctx);
}

cc_api void* cc_pair_bucket_element_first(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_first(&ctx->element);
}

cc_api void* cc_pair_bucket_element_second(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_second(&ctx->element);
}

cc_api cc_bucket_status_t cc_pair_bucket_status(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->status;
}

cc_api void cc_pair_bucket_fill(cc_pair_bucket_t* ctx, const void* first, const void* second)
{
	cc_debug_assert(ctx != NULL);

	
	ctx->status = cc_bucket_status_filled;
	cc_pair_set(&ctx->element, first, second);
}

cc_api void cc_pair_bucket_remove(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->status = cc_bucket_status_removed;
	cc_pair_set(&ctx->element, NULL, NULL);
}

cc_api void cc_pair_bucket_clear(cc_pair_bucket_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->status = cc_bucket_status_empty;
	cc_pair_set(&ctx->element, NULL, NULL);
}

#endif
