/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_first_fit.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
// @ first-fit allocator with boundary-tag coalescing
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_first_fit.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_alignment(void)
{
	return sizeof(void*);
}

static inline size_t cc_first_fit_align(size_t value, size_t alignment)
{
	cc_debug_assert(alignment != 0);


	size_t count;
	count = value / alignment;
	if (0U != (value % alignment))
	{
		count++;
	}
	return alignment * count;
}

static inline bool cc_first_fit_is_aligned(const uintptr_t value, size_t alignment)
{
	cc_debug_assert(alignment != 0);


	return (0U == (value % alignment));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_first_fit_block_status_free      0U
#define cc_first_fit_block_status_allocated 1U

//===========================================================================
static inline size_t cc_first_fit_block_head_flags_make(size_t size, bool allocated)
{
	return (size << 1) | (allocated ? 1u : 0u);
}

static inline size_t cc_first_fit_block_head_flags_size(size_t flags)
{
	return flags >> 1;
}

static inline bool cc_first_fit_block_head_flags_allocated(size_t flags)
{
	return (flags & 1u) == cc_first_fit_block_status_allocated;
}

//===========================================================================
static inline size_t cc_first_fit_block_head_get_size(cc_first_fit_block_head_t* ctx)
{
	return cc_first_fit_block_head_flags_size(ctx->flags);
}

static inline bool cc_first_fit_block_head_is_allocated(cc_first_fit_block_head_t* ctx)
{
	return cc_first_fit_block_head_flags_allocated(ctx->flags);
}

static inline void cc_first_fit_block_head_set_flags(cc_first_fit_block_head_t* ctx, size_t size, bool allocated)
{	
	ctx->flags = cc_first_fit_block_head_flags_make(size, allocated);
}

static inline void cc_first_fit_block_head_set_allocated(cc_first_fit_block_head_t* ctx, bool allocated)
{
	cc_first_fit_block_head_set_flags(
		ctx,
		cc_first_fit_block_head_get_size(ctx),
		allocated
	);
}

static inline void cc_first_fit_block_head_set_size(cc_first_fit_block_head_t* ctx, size_t size)
{
	cc_first_fit_block_head_set_flags(
		ctx,
		size,
		cc_first_fit_block_head_is_allocated(ctx)
	);
}

static inline void cc_first_fit_block_head_add_size(cc_first_fit_block_head_t* ctx, size_t size)
{
	cc_first_fit_block_head_set_size(
		ctx,
		cc_first_fit_block_head_get_size(ctx) + size
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_block_head_aligned_size(void)
{
	return  cc_first_fit_align(sizeof(cc_first_fit_block_head_t), cc_first_fit_alignment());
}

//===========================================================================
static inline size_t cc_first_fit_block_payload_minimal_size(void)
{
	return cc_first_fit_block_head_aligned_size();
}

static inline size_t cc_first_fit_block_payload_aligned_size(const size_t block_payload_size)
{
	size_t size;
	if (block_payload_size < cc_first_fit_block_payload_minimal_size())
	{
		size = cc_first_fit_block_payload_minimal_size();
	}
	else
	{
		size = block_payload_size;
	}
	return  cc_first_fit_align(size, cc_first_fit_alignment());
}

//===========================================================================
static inline size_t cc_first_fit_block_alignment_size(void)
{
	return cc_first_fit_block_head_aligned_size();
}

static inline size_t cc_first_fit_block_aligned_size(const size_t block_payload_size)
{
	return  cc_first_fit_align(
		cc_first_fit_block_head_aligned_size() + cc_first_fit_block_payload_aligned_size(block_payload_size),
		cc_first_fit_block_alignment_size()
	);
}

//===========================================================================
static inline size_t cc_first_fit_block_size(const size_t payload_size)
{
	return cc_first_fit_block_aligned_size(payload_size);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void* cc_first_fit_block_payload_pointer(const cc_first_fit_t* ctx, const cc_first_fit_block_head_t* block_head)
{
	uintptr_t address = (uintptr_t)block_head;
	return (void*)(address + cc_first_fit_block_head_aligned_size());
}

static inline cc_first_fit_block_head_t* cc_first_fit_block_head_pointer(const cc_first_fit_t* ctx, const void* block_payload)
{
	uintptr_t address = (uintptr_t)block_payload;
	return (cc_first_fit_block_head_t*)(address - cc_first_fit_block_head_aligned_size());
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_first_fit_insert_block_into_free_blocks(cc_first_fit_t* ctx, cc_first_fit_block_head_t* block_to_insert)
{
	cc_first_fit_block_head_t* block;

	// 이전 블럭 찾기
	for (block = &ctx->start_block; block->next_free_block < block_to_insert; block = block->next_free_block)
	{
		;
	}


	uintptr_t address = (uintptr_t)block;
	if ((address + cc_first_fit_block_head_get_size(block)) == ((uintptr_t)block_to_insert))
	{
		cc_first_fit_block_head_add_size(block, cc_first_fit_block_head_get_size(block_to_insert));
		block_to_insert = block;
	}


	address = (uintptr_t)block_to_insert;
	if ((address + cc_first_fit_block_head_get_size(block_to_insert)) == ((uintptr_t)block->next_free_block))
	{
		if (block->next_free_block != ctx->end_block)
		{
			cc_first_fit_block_head_add_size(block_to_insert, cc_first_fit_block_head_get_size(block->next_free_block));
			block_to_insert->next_free_block = block->next_free_block->next_free_block;
		}
		else
		{
			block_to_insert->next_free_block = ctx->end_block;
		}
	}
	else
	{
		block_to_insert->next_free_block = block->next_free_block;
	}


	if (block != block_to_insert)
	{
		block->next_free_block = block_to_insert;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_validate_pointer(const cc_first_fit_t* ctx, const void* pointer)
{
	cc_debug_assert(ctx != NULL);


	uintptr_t begin;
	uintptr_t end;
	uintptr_t current;

	begin = (uintptr_t)ctx->memory_pointer;
	end = (uintptr_t)ctx->end_block;
	current = (uintptr_t)cc_first_fit_block_head_pointer(ctx, pointer);
	if (begin > current)
	{
		return false;
	}
	if (end <= current)
	{
		return false;
	}


	cc_first_fit_block_head_t* block;
	uintptr_t address;

	address = begin;
	while(address != end)
	{
		block = (cc_first_fit_block_head_t*)address;
		if (pointer == cc_first_fit_block_payload_pointer(ctx, block))
		{
			if (cc_first_fit_block_head_is_allocated(block))
			{
				return true;
			}
		}

		address += cc_first_fit_block_head_get_size(block);
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_initialize(cc_first_fit_t* ctx, const void* memory_pointer, const size_t memory_size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);


	//-----------------------------------------------------------------------
	if (cc_first_fit_is_aligned((uintptr_t)memory_pointer, cc_first_fit_alignment()) == false)
	{
		return false;
	}
	if (memory_size < cc_first_fit_block_head_aligned_size())
	{
		return false;
	}


	//-----------------------------------------------------------------------
	ctx->memory_pointer = (uint8_t*)memory_pointer;
	ctx->memory_size = memory_size;


	//-----------------------------------------------------------------------
	ctx->start_block.next_free_block = (cc_first_fit_block_head_t*)memory_pointer;
	ctx->start_block.flags = cc_first_fit_block_head_flags_make(0, cc_first_fit_block_status_free);


	//-----------------------------------------------------------------------
	cc_first_fit_block_head_t* begin_block;
	cc_first_fit_block_head_t* end_block;

	uintptr_t address;
	size_t count;
	size_t offset;


	address = (uintptr_t)ctx->memory_pointer;
	begin_block = (cc_first_fit_block_head_t*)(address);

	count = ctx->memory_size / cc_first_fit_block_head_aligned_size();
	offset = (count - 1) * cc_first_fit_block_head_aligned_size();
	end_block = (cc_first_fit_block_head_t*)(address + offset);


	//-----------------------------------------------------------------------
	uintptr_t begin_block_address = (uintptr_t)(begin_block);
	uintptr_t end_block_address = (uintptr_t)(end_block);
	size_t begin_block_size  = end_block_address - begin_block_address;
	

	begin_block->flags = cc_first_fit_block_head_flags_make(begin_block_size, cc_first_fit_block_status_free);
	begin_block->next_free_block = end_block;

	end_block->next_free_block = NULL;
	end_block->flags = cc_first_fit_block_head_flags_make(0, cc_first_fit_block_status_free);


	//-----------------------------------------------------------------------
	ctx->end_block = end_block;
	ctx->min_ever_free_size = begin_block_size;
	ctx->free_size = begin_block_size;
	ctx->first_pointer_address = (uintptr_t)cc_first_fit_block_payload_pointer(ctx, begin_block);
	ctx->count = 0;


	return true;
}

//===========================================================================
// First-fit
cc_api void* cc_first_fit_allocate(cc_first_fit_t* ctx, const size_t size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	if(size == 0)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	size_t block_size = cc_first_fit_block_size(size);
	if (ctx->free_size  < block_size)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	cc_first_fit_block_head_t* previous_block = &ctx->start_block;
	cc_first_fit_block_head_t* block = previous_block->next_free_block;
	while ((cc_first_fit_block_head_get_size(block) < block_size) && (block->next_free_block != NULL))
	{
		previous_block = block;
		block = block->next_free_block;
	}


	//-----------------------------------------------------------------------
	void* block_payload_pointer = NULL;
	
	if (block != ctx->end_block)
	{
		block_payload_pointer = cc_first_fit_block_payload_pointer(ctx, previous_block->next_free_block);

		
		previous_block->next_free_block = block->next_free_block;


		size_t remaining_size = cc_first_fit_block_head_get_size(block) - block_size;
		if (remaining_size > cc_first_fit_block_aligned_size(0))
		{
			uintptr_t address = (uintptr_t)block;

			cc_first_fit_block_head_t* new_free_block = (cc_first_fit_block_head_t*)(address + block_size);
			cc_first_fit_block_head_set_flags(new_free_block, remaining_size, false);


			cc_first_fit_block_head_set_size(block, block_size);


			cc_first_fit_insert_block_into_free_blocks(ctx, new_free_block);
		}


		size_t allocated_block_size = cc_first_fit_block_head_get_size(block);
		ctx->free_size -= allocated_block_size;
		if (ctx->free_size < ctx->min_ever_free_size)
		{
			ctx->min_ever_free_size = ctx->free_size;
		}


		cc_first_fit_block_head_set_allocated(block, true);
		block->next_free_block = NULL;


		ctx->count++;
	}

	return block_payload_pointer;
}

// boundary-tag coalescing
cc_api bool cc_first_fit_free(cc_first_fit_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(pointer != NULL);
	cc_debug_assert(ctx->count > 0);


	//-----------------------------------------------------------------------
	cc_first_fit_block_head_t* block = cc_first_fit_block_head_pointer(ctx, pointer);
	cc_debug_assert(cc_first_fit_block_head_is_allocated(block));
	cc_debug_assert(block->next_free_block == NULL);


	if (cc_first_fit_block_head_is_allocated(block) == false)
	{
		return false;
	}

	if (block->next_free_block != NULL)
	{
		return false;
	}

	
	size_t free_block_size = cc_first_fit_block_head_get_size(block);
	ctx->free_size += free_block_size;
	
	
	cc_first_fit_block_head_set_allocated(block, false);
	cc_first_fit_insert_block_into_free_blocks(ctx, block);


	ctx->count--;


	return true;
}

cc_api cc_ssize_t cc_first_fit_count(cc_first_fit_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	return ctx->count;
}
