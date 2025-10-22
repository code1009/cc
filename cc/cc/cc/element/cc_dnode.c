/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_dnode.c
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
#include "cc_dnode.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_dnode_config_macro_function)

cc_api void cc_dnode_initialize(cc_dnode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->next = NULL;
	ctx->previous = NULL;
	cc_element_initialize(&ctx->element);
}

cc_api void cc_dnode_set(cc_dnode_t* ctx, const cc_dnode_t* next, const cc_dnode_t* previous, const void* element)
{
	cc_debug_assert(ctx != NULL);


	ctx->next = (cc_dnode_t*)next;
	ctx->previous = (cc_dnode_t*)previous;
	cc_element_set(&ctx->element, element);
}

cc_api cc_dnode_t* cc_dnode_next(cc_dnode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->next;
}

cc_api cc_dnode_t* cc_dnode_previous(cc_dnode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->previous;
}

cc_api void* cc_dnode_element(cc_dnode_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_element_get(&ctx->element);
}

#endif
