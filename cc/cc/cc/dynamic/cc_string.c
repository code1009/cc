/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string.c
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
#include "../allocator/cc_simple_segregated_storage.h"
#include "cc_first_fit.h"
#include "cc_lf_heap.h"

#include "cc_vallocator.h"
#include "cc_vallocator_helper.h"

#include "cc_string_allocator.h"

//===========================================================================
#include "cc_string.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static char* cc_string_allocate(cc_string_t* ctx, const size_t size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(size != 0);

	
	return (char*)ctx->allocator->iallocator.allocate(&ctx->allocator->lf_heap, size);
}

static bool cc_string_free(cc_string_t* ctx, char* data)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(data != NULL);


	bool rv = ctx->allocator->iallocator.free(&ctx->allocator->lf_heap, data);
	cc_debug_assert(rv != false);
	return rv;
}

static void cc_string_strncpy(char* dest, const char* src, size_t size)
{
	cc_debug_assert(dest != NULL);
	cc_debug_assert(src != NULL);


	for (size_t i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}

static char* cc_string_reallocate(cc_string_t* ctx, char* old_data, const size_t old_size, const size_t new_size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(old_data != NULL);
	cc_debug_assert(old_size != 0);
	cc_debug_assert(new_size != 0);


	char* new_data = cc_string_allocate(ctx, new_size);
	if (new_data != NULL)
	{
		size_t copy_size = (old_size < new_size) ? old_size : new_size;
		cc_string_strncpy(new_data, old_data, copy_size);
		cc_string_free(ctx, old_data);
	}
	return new_data;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool string_create(cc_string_t* ctx, cc_string_allocator_t* allocator)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(allocator != NULL);


	if (allocator == NULL)
	{
		return false;
	}


	ctx->allocator = allocator;
	ctx->size = 0;
	ctx->capacity = 0;
	ctx->data = NULL;


	return true;
}

cc_api bool string_destroy(cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);

	bool rv = true;
	if (ctx->data)
	{
		rv = cc_string_free(ctx, ctx->data);
	}

	ctx->allocator = NULL;
	ctx->size = 0;
	ctx->capacity = 0;
	ctx->data = NULL;

	return rv;
}
