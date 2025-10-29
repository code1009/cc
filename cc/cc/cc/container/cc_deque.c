/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_deque.c
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

//===========================================================================
#include "cc_deque.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_deque_initialize(cc_deque_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	cc_collection_initialize(&ctx->collection, elements, max_count, param);

	ctx->begin = 0;
	ctx->end = 0;
}

cc_api uintptr_t cc_deque_param(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_param(&ctx->collection);
}

//===========================================================================
cc_api void* cc_deque_pop_front(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->collection.count == 0)
	{
		return NULL;
	}

	size_t index = ctx->begin;
	void* element = cc_element_get(&ctx->collection.elements[index]);
	cc_element_set(&ctx->collection.elements[index], NULL);

	ctx->begin = (index + 1) % ctx->collection.max_count;
	ctx->collection.count--;


	return element;
}

cc_api void* cc_deque_pop_back(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->collection.count == 0)
	{
		return NULL;
	}

	size_t index = (ctx->end == 0) ? (ctx->collection.max_count - 1) : (ctx->end - 1);
	void* element = cc_element_get(&ctx->collection.elements[index]);
	cc_element_set(&ctx->collection.elements[index], NULL);

	ctx->end = index;
	ctx->collection.count--;


	return element;
}

cc_api bool cc_deque_push_back(cc_deque_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->collection.count < ctx->collection.max_count)
	{
		cc_element_set(&ctx->collection.elements[ctx->end], element);
		ctx->end = (ctx->end + 1) % ctx->collection.max_count;
		ctx->collection.count++;

		return true;
	}

	return false;
}

cc_api bool cc_deque_push_front(cc_deque_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->collection.count < ctx->collection.max_count)
	{
		ctx->begin = (ctx->begin == 0) ? (ctx->collection.max_count - 1) : (ctx->begin - 1);
		cc_element_set(&ctx->collection.elements[ctx->begin], element);
		ctx->collection.count++;

		return true;
	}

	return false;
}

//===========================================================================
cc_api cc_element_t* cc_deque_at(cc_deque_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);
	

	if (index < ctx->collection.count)
	{
		size_t real_index = (ctx->begin + index) % ctx->collection.max_count;
		return &ctx->collection.elements[real_index];
	}

	return NULL;
}

cc_api void* cc_deque_front_element(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_element_get(&ctx->collection.elements[ctx->begin]);
}

cc_api void* cc_deque_back_element(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	size_t index = (ctx->end == 0) ? (ctx->collection.max_count - 1) : (ctx->end - 1);
	return cc_element_get(&ctx->collection.elements[index]);
}

//===========================================================================
cc_api size_t cc_deque_count(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_count(&ctx->collection);
}

cc_api bool cc_deque_empty(cc_deque_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_empty(&ctx->collection);
}
