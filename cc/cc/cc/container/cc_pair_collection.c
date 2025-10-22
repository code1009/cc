/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_pair_collection.c
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
#include "../element/cc_pair.h"

//===========================================================================
#include "cc_pair_collection.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_pair_collection_initialize(cc_pair_collection_t* ctx, const cc_pair_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	ctx->elements = (cc_pair_t*)elements;
	ctx->max_count = max_count;
	ctx->param = param;

	ctx->count = 0;

	for (size_t i = 0; i < ctx->max_count; i++)
	{
		cc_pair_initialize(&ctx->elements[i]);
	}
}

cc_api uintptr_t cc_pair_collection_param(cc_pair_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->param;
}

//===========================================================================
cc_api void cc_pair_collection_clear(cc_pair_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->count = 0;
}

cc_api bool cc_pair_collection_erase(cc_pair_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		for (size_t i = index; i < ctx->count - 1; i++)
		{
			cc_pair_copy(&ctx->elements[i], &ctx->elements[i + 1]);
		}
		ctx->count--;

		return true;
	}

	return false;
}

cc_api bool cc_pair_collection_add(cc_pair_collection_t* ctx, const void* first, const void* second)
{
	cc_debug_assert(ctx != NULL);


	size_t index = ctx->count;
	if (index < ctx->max_count)
	{
		cc_pair_set(&ctx->elements[index], first, second);
		ctx->count++;

		return true;
	}

	return false;
}

cc_api bool cc_pair_collection_insert(cc_pair_collection_t* ctx, size_t index, const void* first, const void* second)
{
	cc_debug_assert(ctx != NULL);


	if (index <= ctx->count && ctx->count < ctx->max_count)
	{
		for (size_t i = ctx->count; i > index; i--)
		{
			cc_pair_copy(&ctx->elements[i], &ctx->elements[i - 1]);
		}
		cc_pair_set(&ctx->elements[index], first, second);
		ctx->count++;

		return true;
	}

	return false;
}

//===========================================================================
cc_api cc_pair_t* cc_pair_collection_at(cc_pair_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		return &ctx->elements[index];
	}

	return NULL;
}

cc_api void* cc_pair_collection_element_first(cc_pair_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		return cc_pair_first(&ctx->elements[index]);
	}

	return NULL;
}

cc_api void* cc_pair_collection_element_second(cc_pair_collection_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->count)
	{
		return cc_pair_second(&ctx->elements[index]);
	}

	return NULL;
}

//===========================================================================
cc_api size_t cc_pair_collection_count(cc_pair_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->count;
}

cc_api bool cc_pair_collection_empty(cc_pair_collection_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (ctx->count == 0) ? true : false;
}
