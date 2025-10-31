/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_simple_segregated_storage.c
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
#include "cc_simple_segregated_storage.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static size_t cc_simple_segregated_storage_alignment_size(void)
{
	return sizeof(void*);
}

static bool cc_simple_segregated_storage_is_aligned_address(const uintptr_t address)
{
	return (0U == (address % cc_simple_segregated_storage_alignment_size()));
}

//===========================================================================
static size_t cc_simple_segregated_storage_chunk_alignment_size(void)
{
	return cc_simple_segregated_storage_alignment_size();
}

static size_t cc_simple_segregated_storage_calc_chunk_size(const size_t data_size)
{
	size_t alignment_size;
	size_t count;

	size_t chunk_size;


	alignment_size = cc_simple_segregated_storage_chunk_alignment_size();
	count = data_size / alignment_size;
	if (0U != (data_size % alignment_size))
	{
		count++;
	}
	chunk_size = alignment_size * count;


	return chunk_size;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_simple_segregated_storage_calc_memory_size(const size_t data_size, const size_t max_count)
{
	size_t chunk_size;
	size_t memory_size;
	size_t max_memory_size;


	chunk_size = cc_simple_segregated_storage_calc_chunk_size(data_size);
	max_memory_size = (size_t)-1;
	if (max_count > (max_memory_size / chunk_size))
	{
		return max_memory_size;
	}


	memory_size = chunk_size * max_count;


	return memory_size;
}

cc_api bool cc_simple_segregated_storage_validate_pointer(const cc_simple_segregated_storage_t* ctx, const void* pointer)
{
	cc_debug_assert(ctx != NULL);


	uintptr_t begin;
	uintptr_t end;
	uintptr_t current;

	begin = (uintptr_t)(ctx->memory_pointer);
	end = begin + ctx->chunk_size * ctx->max_count;
	current = (uintptr_t)pointer;

	if (begin > current)
	{
		return false;
	}
	if (end <= current)
	{
		return false;
	}
	if (0 != ((current - begin) % ctx->chunk_size))
	{
		return false;
	}

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_simple_segregated_storage_initialize(cc_simple_segregated_storage_t* ctx, const void* memory_pointer, const size_t memory_size, const size_t data_size, const size_t max_count)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);
	cc_debug_assert(data_size != 0);
	cc_debug_assert(max_count != 0);


	//-----------------------------------------------------------------------
	if (cc_simple_segregated_storage_is_aligned_address((uintptr_t)memory_pointer) == false)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	ctx->memory_pointer = (uint8_t*)memory_pointer;
	ctx->memory_size = memory_size;


	//-----------------------------------------------------------------------
	ctx->data_size = data_size;
	ctx->max_count = max_count;


	//-----------------------------------------------------------------------
	ctx->chunk_size = cc_simple_segregated_storage_calc_chunk_size(data_size);
	ctx->count = 0;
	ctx->free_chunk_head = NULL;


	//-----------------------------------------------------------------------
	if (ctx->memory_size < cc_simple_segregated_storage_calc_memory_size(data_size, max_count))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	cc_simple_segregated_storage_chunk_t* free_chunk_head;
	cc_simple_segregated_storage_chunk_t* free_chunk;

	uintptr_t address;

	size_t i;
	size_t count;


	address = (uintptr_t)ctx->memory_pointer;


	free_chunk_head = (cc_simple_segregated_storage_chunk_t*)(address);
	count = ctx->max_count;


	free_chunk = free_chunk_head;
	for (i = 1; i < count; i++)
	{
		free_chunk->next_free_chunk = (cc_simple_segregated_storage_chunk_t*)(address + (i * ctx->chunk_size));
		free_chunk = free_chunk->next_free_chunk;
	}
	free_chunk->next_free_chunk = NULL;


	//-----------------------------------------------------------------------
	ctx->free_chunk_head = free_chunk_head;


	return true;
}

//===========================================================================
cc_api void* cc_simple_segregated_storage_allocate(cc_simple_segregated_storage_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(ctx->count <= ctx->max_count);


	//-----------------------------------------------------------------------
	if (NULL == ctx->free_chunk_head)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	void* pointer;


	pointer = ctx->free_chunk_head;
	ctx->free_chunk_head = ctx->free_chunk_head->next_free_chunk;
	ctx->count++;


	return pointer;
}

cc_api bool cc_simple_segregated_storage_free(cc_simple_segregated_storage_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(pointer != NULL);
	cc_debug_assert(ctx->count > 0);


	//-----------------------------------------------------------------------
	if (false == cc_simple_segregated_storage_validate_pointer(ctx, pointer))
	{
		return false;
	}


	//-----------------------------------------------------------------------
	cc_simple_segregated_storage_chunk_t* free_chunk;


	free_chunk = ctx->free_chunk_head;
	ctx->free_chunk_head = (cc_simple_segregated_storage_chunk_t*)(pointer);
	ctx->free_chunk_head->next_free_chunk = free_chunk;
	ctx->count--;


	return true;
}

cc_api cc_ssize_t cc_simple_segregated_storage_count(cc_simple_segregated_storage_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	return ctx->count;
}
