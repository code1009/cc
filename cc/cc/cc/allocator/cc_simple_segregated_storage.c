/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_simple_segregated_storage.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
//   @ Simple segregated storage.
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
static inline bool cc_simple_segregated_storage_mul_overflow(const size_t a, const size_t b)
{
	if (a == 0 || b == 0)
	{
		return false;
	}
	size_t c = ((size_t)-1) / b;
	return (a > c);
}

static inline size_t cc_simple_segregated_storage_alignment(void)
{
	return sizeof(void*);
}

static inline size_t cc_simple_segregated_storage_align(const size_t value, const size_t alignment)
{
	cc_debug_assert(alignment != 0);


	size_t count;
	count = value / alignment;
	if (0U != (value % alignment))
	{
		count++;
	}
	if (cc_simple_segregated_storage_mul_overflow(alignment, count))
	{
		return cc_invalid_size;
	}
	return alignment * count;
}

static inline bool cc_simple_segregated_storage_is_aligned(const uintptr_t value, const size_t alignment)
{
	cc_debug_assert(alignment != 0);


	return (0U == (value % alignment));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_simple_segregated_storage_chunk_alignment_size(void)
{
	return cc_simple_segregated_storage_alignment();
}

static inline size_t cc_simple_segregated_storage_chunk_size(const size_t data_size)
{
	return cc_simple_segregated_storage_align(data_size, cc_simple_segregated_storage_chunk_alignment_size());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_simple_segregated_storage_memory_size(const size_t data_size, const size_t max_count)
{
	size_t chunk_size = cc_simple_segregated_storage_chunk_size(data_size);
	if (cc_invalid_size == chunk_size)
	{
		return cc_invalid_size;
	}
	if (cc_simple_segregated_storage_mul_overflow(max_count, chunk_size))
	{
		return cc_invalid_size;
	}
	return chunk_size * max_count;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
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

cc_api bool cc_simple_segregated_storage_is_free_chunk(const cc_simple_segregated_storage_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	cc_simple_segregated_storage_chunk_t* free_chunk;


	free_chunk = ctx->free_chunk_head;
	while (free_chunk != NULL)
	{
		if (free_chunk == (cc_simple_segregated_storage_chunk_t*)pointer)
		{
			return true;
		}

		free_chunk = free_chunk->next_free_chunk;
	}

	return false;
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
	if (cc_simple_segregated_storage_is_aligned((uintptr_t)memory_pointer, cc_simple_segregated_storage_alignment()) == false)
	{
		return false;
	}


	size_t required_memory_size;
	required_memory_size = cc_simple_segregated_storage_memory_size(data_size, max_count);
	if (cc_invalid_size == required_memory_size)
	{
		return false;
	}

	if (memory_size < required_memory_size)
	{
		return false;
	}


	//-----------------------------------------------------------------------
	ctx->memory_pointer = (uint8_t*)memory_pointer;
	ctx->memory_size = memory_size;
	ctx->data_size = data_size;
	ctx->max_count = max_count;
	ctx->chunk_size = cc_simple_segregated_storage_chunk_size(data_size);


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

	ctx->count = 0;

	return true;
}

//===========================================================================
cc_api void* cc_simple_segregated_storage_allocate(cc_simple_segregated_storage_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


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
	cc_simple_segregated_storage_chunk_t* free_chunk;


	//-----------------------------------------------------------------------
#if (1==cc_config_debug)
	if (false == cc_simple_segregated_storage_validate_pointer(ctx, pointer))
	{
		cc_debug_assert(0);
		return false;
	}
	if (false == cc_simple_segregated_storage_is_free_chunk(ctx, pointer))
	{
		cc_debug_assert(0);
		return false;
	}
#endif


	//-----------------------------------------------------------------------
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
