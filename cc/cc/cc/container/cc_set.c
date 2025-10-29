/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_set.c
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

#include "../element/cc_element.h"

#include "cc_collection.h"

//===========================================================================
#include "cc_set.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_set_lower_bound(cc_set_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	size_t low;
	size_t high;
	size_t mid;


	low = 0u;
	high = cc_set_count(ctx);
	while (low < high)
	{
		// mid = (low + high) / 2u;
		// (x+y)/2 => x&y + (x^y)/2
		mid = (low & high) + ((low ^ high) / 2);

		void* existing_element = cc_collection_element(&ctx->collection, mid);
		if (true == ctx->less(existing_element, element))
		{
			low = mid + 1u;
		}
		else
		{
			high = mid;
		}
	}

	return low;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_set_initialize(cc_set_t* ctx, const cc_equal_t equal, const cc_less_t less, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(equal != NULL);
	cc_debug_assert(less != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	ctx->equal = equal;
	ctx->less = less;
	cc_collection_initialize(&ctx->collection, elements, max_count, param);
}

cc_api uintptr_t cc_set_param(cc_set_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_param(&ctx->collection);
}

//===========================================================================
cc_api void cc_set_clear(cc_set_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_collection_clear(&ctx->collection);
}

cc_api bool cc_set_erase(cc_set_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_erase(&ctx->collection, index);
}

cc_api bool cc_set_add(cc_set_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	// find insert position
	size_t index = cc_set_lower_bound(ctx, element);


	// if key already exists, do not insert
	if (index < cc_set_count(ctx))
	{
		void* existing_element = cc_collection_element(&ctx->collection, index);
		if (true == ctx->equal(existing_element, element))
		{
			return false;
		}
	}

	// insert at found position (shifts elements)
	return cc_collection_insert(&ctx->collection, index, element);
}

//===========================================================================
cc_api cc_element_t* cc_set_at(cc_set_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_at(&ctx->collection, index);
}

cc_api void* cc_set_element(cc_set_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_element(&ctx->collection, index);
}

cc_api size_t cc_set_find(cc_set_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	size_t index = cc_set_lower_bound(ctx, element);
	if (index < cc_set_count(ctx))
	{
		void* existing_element = cc_collection_element(&ctx->collection, index);
		if (true == ctx->equal(existing_element, element))
		{
			return index;
		}
	}

	return cc_invalid_index;
}

cc_api void* cc_set_element_by_value(cc_set_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	size_t index = cc_set_find(ctx, element);
	return cc_collection_element(&ctx->collection, index);
}

//===========================================================================
cc_api size_t cc_set_count(cc_set_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_count(&ctx->collection);
}

cc_api bool cc_set_empty(cc_set_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_empty(&ctx->collection);
}
