/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_map.c
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
#include "../element/cc_pair.h"

#include "cc_pair_collection.h"

//===========================================================================
#include "cc_map.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_map_lower_bound(cc_map_t* ctx, const void* first)
{
	cc_debug_assert(ctx != NULL);


	size_t low;
	size_t high;
	size_t mid;


	low = 0u;
	high = cc_map_count(ctx);
	while (low < high)
	{
		// mid = (low + high) / 2u;
		// (x+y)/2 => x&y + (x^y)/2
		mid = (low & high) + ((low ^ high) / 2);

		void* existing_element_first = cc_pair_collection_element_first(&ctx->collection, mid);
		if (true == ctx->less(existing_element_first, first))
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
cc_api void cc_map_initialize(cc_map_t* ctx, const cc_equal_t equal, const cc_less_t less, const cc_pair_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(equal != NULL);
	cc_debug_assert(less != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	ctx->equal = equal;
	ctx->less = less;
	cc_pair_collection_initialize(&ctx->collection, elements, max_count, param);
}

cc_api uintptr_t cc_map_param(cc_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_param(&ctx->collection);
}

//===========================================================================
cc_api void cc_map_clear(cc_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_pair_collection_clear(&ctx->collection);
}

cc_api bool cc_map_erase(cc_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_erase(&ctx->collection, index);
}

cc_api bool cc_map_add(cc_map_t* ctx, const void* first, const void* second)
{
	cc_debug_assert(ctx != NULL);


	// find insert position
	size_t index = cc_map_lower_bound(ctx, first);


	// if key already exists, do not insert
	if (index < cc_map_count(ctx))
	{
		void* existing_element_first = cc_pair_collection_element_first(&ctx->collection, index);
		if (true == ctx->equal(existing_element_first, first))
		{
			return false;
		}
	}

	// insert at found position (shifts elements)
	return cc_pair_collection_insert(&ctx->collection, index, first, second);
}

//===========================================================================
cc_api cc_pair_t* cc_map_at(cc_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_at(&ctx->collection, index);
}

cc_api void* cc_map_element_first(cc_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_element_first(&ctx->collection, index);
}

cc_api void* cc_map_element_second(cc_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_element_second(&ctx->collection, index);
}

cc_api size_t cc_map_find(cc_map_t* ctx, const void* first)
{
	cc_debug_assert(ctx != NULL);


	size_t index = cc_map_lower_bound(ctx, first);
	if (index < cc_map_count(ctx))
	{
		void* existing_element_first = cc_pair_collection_element_first(&ctx->collection, index);
		if (true == ctx->equal(existing_element_first, first))
		{
			return index;
		}
	}

	return cc_invalid_index;
}

cc_api void* cc_map_element_second_by_first(cc_map_t* ctx, const void* first)
{
	cc_debug_assert(ctx != NULL);


	size_t index = cc_map_find(ctx, first);
	return cc_pair_collection_element_second(&ctx->collection, index);
}

//===========================================================================
cc_api size_t cc_map_count(cc_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_count(&ctx->collection);
}

cc_api bool cc_map_empty(cc_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_pair_collection_empty(&ctx->collection);
}
