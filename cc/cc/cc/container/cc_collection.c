/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_collection.c
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

//===========================================================================
#include "cc_collection.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_collection_initialize(cc_collection_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	ctx->elements = (cc_element_t*)elements;
	ctx->max_count = max_count;
	ctx->param = param;

	ctx->count = 0;

	for (size_t i = 0; i < ctx->max_count; i++)
	{
		cc_element_initialize(&ctx->elements[i]);
	}
}

cc_api uintptr_t cc_collection_param(cc_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->param;
}

//===========================================================================
cc_api void cc_collection_clear(cc_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->count = 0;
}

cc_api bool cc_collection_erase(cc_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		for (size_t i = index; i < ctx->count - 1; i++)
		{
			cc_element_copy(&ctx->elements[i], &ctx->elements[i + 1]);
		}
		ctx->count--;

		return true;
	}

	return false;
}

cc_api bool cc_collection_add(cc_collection_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	size_t index = ctx->count;
	if (index < ctx->max_count)
	{
		cc_element_set(&ctx->elements[index], element);
		ctx->count++;

		return true;
	}

	return false;
}

cc_api bool cc_collection_insert(cc_collection_t* ctx, size_t index, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (index <= ctx->count && ctx->count < ctx->max_count)
	{
		for (size_t i = ctx->count; i > index; i--)
		{
			cc_element_copy(&ctx->elements[i], &ctx->elements[i - 1]);
		}
		cc_element_set(&ctx->elements[index], element);
		ctx->count++;

		return true;
	}

	return false;
}

//===========================================================================
cc_api cc_element_t* cc_collection_at(cc_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		return &ctx->elements[index];
	}
	return NULL;
}

cc_api void* cc_collection_element(cc_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		return cc_element_get(&ctx->elements[index]);
	}
	return NULL;
}

//===========================================================================
cc_api size_t cc_collection_count(cc_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->count;
}

cc_api bool cc_collection_empty(cc_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (ctx->count == 0) ? true : false;
}
