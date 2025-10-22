/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_snode.c
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
#include "cc_snode.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_snode_config_macro_function)

cc_api void cc_snode_initialize(cc_snode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->next = NULL;
	cc_element_initialize(&ctx->element);
}

cc_api void cc_snode_set(cc_snode_t* ctx, const cc_snode_t* next, const void* element)
{
	cc_debug_assert(ctx != NULL);


	ctx->next = (cc_snode_t*)next;
	cc_element_set(&ctx->element, element);
}

cc_api cc_snode_t* cc_snode_next(cc_snode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->next;
}

cc_api void* cc_snode_element(cc_snode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_element_get(&ctx->element);
}

#endif
