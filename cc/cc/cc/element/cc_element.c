/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_element.c
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





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_element_config_macro_function)

cc_api void cc_element_initialize(cc_element_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->pointer = NULL;
}

cc_api void cc_element_set(cc_element_t* ctx, const void* pointer)
{
	cc_debug_assert(ctx != NULL);


	ctx->pointer = (void*)pointer;
}

cc_api void* cc_element_get(cc_element_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->pointer;
}

//===========================================================================
cc_api void cc_element_copy(cc_element_t* dst, const cc_element_t* src)
{
	cc_debug_assert(dst != NULL);
	cc_debug_assert(src != NULL);


	dst->pointer = src->pointer;
}

cc_api void cc_element_swap(cc_element_t* left, cc_element_t* right)
{
	cc_debug_assert(left != NULL);
	cc_debug_assert(right != NULL);

	
	void* temp;
	
	temp = left->pointer;
	left->pointer = right->pointer;
	right->pointer = temp;
}

#endif
