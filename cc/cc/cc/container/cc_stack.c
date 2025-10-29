/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_stack.c
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
#include "../element/cc_element.h"

#include "cc_collection.h"
#include "cc_deque.h"

//===========================================================================
#include "cc_stack.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_stack_initialize(cc_stack_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);


	cc_deque_initialize(&ctx->deque, elements, max_count, param);
}

cc_api uintptr_t cc_stack_param(cc_stack_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_param(&ctx->deque);
}

//===========================================================================
cc_api bool cc_stack_push(cc_stack_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_push_back(&ctx->deque, element);
}

cc_api void* cc_stack_pop(cc_stack_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_pop_back(&ctx->deque);
}

//===========================================================================
cc_api void* cc_stack_top_element(cc_stack_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_back_element(&ctx->deque);
}

//===========================================================================
cc_api size_t cc_stack_count(cc_stack_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_count(&ctx->deque);
}

cc_api bool cc_stack_empty(cc_stack_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_deque_empty(&ctx->deque);
}
