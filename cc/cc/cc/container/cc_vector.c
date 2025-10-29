/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_vector.c
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
#include "cc_vector.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_vector_initialize(cc_vector_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);


	cc_collection_initialize(&ctx->collection, elements, max_count, param);
}

cc_api uintptr_t cc_vector_param(cc_vector_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_param(&ctx->collection);
}

//===========================================================================
cc_api void cc_vector_clear(cc_vector_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_collection_clear(&ctx->collection);
}

cc_api bool cc_vector_erase(cc_vector_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_erase(&ctx->collection, index);
}

cc_api bool cc_vector_add(cc_vector_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_add(&ctx->collection, element);
}

cc_api bool cc_vector_insert(cc_vector_t* ctx, const size_t index, const void* element)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_insert(&ctx->collection, index, element);
}

//===========================================================================
cc_api cc_element_t* cc_vector_at(cc_vector_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_at(&ctx->collection, index);
}

cc_api void* cc_vector_element(cc_vector_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_element(&ctx->collection, index);
}

//===========================================================================
cc_api size_t cc_vector_count(cc_vector_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_count(&ctx->collection);
}

cc_api bool cc_vector_empty(cc_vector_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_empty(&ctx->collection);
}
