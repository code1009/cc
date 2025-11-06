/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_heap_memory.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "../allocator/cc_simple_segregated_storage.h"
#include "cc_first_fit.h"
#include "cc_lf_heap.h"

#include "cc_vallocator.h"
#include "cc_vallocator_adapter.h"

//===========================================================================
#include "cc_heap_memory.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_heap_memory_copy(char* dest, const char* src, size_t size)
{
	cc_debug_assert(dest != NULL);
	cc_debug_assert(src != NULL);


	for (size_t i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_initialize(cc_heap_memory_t* ctx,
	const void* memory_pointer, const size_t memory_size, const cc_lf_heap_bucket_descriptors_t* lf_heap_bucket_descriptors)
{
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(lf_heap_bucket_descriptors != NULL);


	return cc_lf_heap_vallocator_initialize(
		&ctx->allocator,
		&ctx->lf_heap,
		memory_pointer, memory_size,
		lf_heap_bucket_descriptors
	);
}

cc_api void cc_heap_memory_uninitialize(cc_heap_memory_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	cc_lf_heap_uninitialize(&ctx->lf_heap);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void* cc_heap_memory_allocate(cc_heap_memory_t* ctx, const size_t size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(size != 0);


	return ctx->allocator.allocate(&ctx->allocator.handle, size);
}

cc_api bool cc_heap_memory_free(cc_heap_memory_t* ctx, void* data)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(data != NULL);


	bool rv = ctx->allocator.free(&ctx->allocator.handle, data);
	cc_debug_assert(rv != false);

	return rv;
}

cc_api void* cc_heap_memory_reallocate(cc_heap_memory_t* ctx, char* old_data, const size_t old_size, const size_t new_size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(old_data != NULL);
	cc_debug_assert(old_size != 0);
	cc_debug_assert(new_size != 0);


	void* new_data = cc_heap_memory_allocate(ctx, new_size);
	if (new_data != NULL)
	{
		size_t copy_size = (old_size < new_size) ? old_size : new_size;
		cc_heap_memory_copy(new_data, old_data, copy_size);
		cc_heap_memory_free(ctx, old_data);
	}
	return new_data;
}
