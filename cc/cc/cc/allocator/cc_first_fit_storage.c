/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_first_fit_storage.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
// @ First-fit allocator with boundary-tag coalescing
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_first_fit_storage.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_first_fit_storage_block_status_free      0U
#define cc_first_fit_storage_block_status_allocated 1U





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_storage_block_head_flags_make(size_t size, bool allocated)
{
	return (size << 1) | (allocated ? 1u : 0u);
}

static inline size_t cc_first_fit_storage_block_head_flags_size(size_t flags)
{
	return flags >> 1;
}

static inline bool cc_first_fit_storage_block_head_flags_allocated(size_t flags)
{
	return (flags & 1u) == cc_first_fit_storage_block_status_allocated;
}

//===========================================================================
static inline size_t cc_first_fit_storage_block_head_get_size(cc_first_fit_storage_block_head_t* block_head)
{
	return cc_first_fit_storage_block_head_flags_size(block_head->flags);
}

static inline bool cc_first_fit_storage_block_head_is_allocated(cc_first_fit_storage_block_head_t* block_head)
{
	return cc_first_fit_storage_block_head_flags_allocated(block_head->flags);
}

static inline void cc_first_fit_storage_block_head_set_flags(cc_first_fit_storage_block_head_t* block_head, size_t size, bool allocated)
{	
	block_head->flags = cc_first_fit_storage_block_head_flags_make(size, allocated);
}

static inline void cc_first_fit_storage_block_head_set_allocated(cc_first_fit_storage_block_head_t* block_head, bool allocated)
{
	cc_first_fit_storage_block_head_set_flags(
		block_head,
		cc_first_fit_storage_block_head_get_size(block_head),
		allocated
	);
}

static inline void cc_first_fit_storage_block_head_set_size(cc_first_fit_storage_block_head_t* block_head, size_t size)
{
	cc_first_fit_storage_block_head_set_flags(
		block_head,
		size,
		cc_first_fit_storage_block_head_is_allocated(block_head)
	);
}

static inline void cc_first_fit_storage_block_head_add_size(cc_first_fit_storage_block_head_t* block_head, size_t size)
{
	cc_first_fit_storage_block_head_set_size(
		block_head,
		cc_first_fit_storage_block_head_get_size(block_head) + size
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_storage_calc_aligned_size(size_t v, size_t alignment_size)
{
	cc_debug_assert(alignment_size != 0);


	size_t count;
	count = v / alignment_size;
	if (0U != (v % alignment_size))
	{
		count++;
	}
	return alignment_size * count;
}

//===========================================================================
static inline size_t cc_first_fit_storage_alignment_size(void)
{
	return sizeof(void*);
}

static inline bool cc_first_fit_storage_is_aligned_address(const uintptr_t address)
{
	return (0U == (address % cc_first_fit_storage_alignment_size()));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_storage_block_alignment_size(void)
{
	return sizeof(void*);
}

//===========================================================================
static inline size_t cc_first_fit_storage_calc_block_head_aligned_size(void)
{
	return  cc_first_fit_storage_calc_aligned_size(sizeof(cc_first_fit_storage_block_head_t), cc_first_fit_storage_alignment_size());
}

static inline size_t cc_first_fit_storage_block_payload_minimal_size(void)
{
	return cc_first_fit_storage_calc_block_head_aligned_size();
}

static inline size_t cc_first_fit_storage_calc_block_payload_aligned_size(const size_t block_payload_size)
{
	size_t size;
	if (block_payload_size < cc_first_fit_storage_block_payload_minimal_size())
	{
		size = cc_first_fit_storage_block_payload_minimal_size();
	}
	else
	{
		size = block_payload_size;
	}
	return  cc_first_fit_storage_calc_aligned_size(size, cc_first_fit_storage_alignment_size());
}

static inline size_t cc_first_fit_storage_calc_block_aligned_size(const size_t block_payload_size)
{
	return  cc_first_fit_storage_calc_aligned_size(
		cc_first_fit_storage_calc_block_head_aligned_size() + cc_first_fit_storage_calc_block_payload_aligned_size(block_payload_size),
		cc_first_fit_storage_block_alignment_size()
	);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void* cc_first_fit_storage_block_payload_pointer(const cc_first_fit_storage_t* ctx, const cc_first_fit_storage_block_head_t* block_head)
{
	uintptr_t address = (uintptr_t)block_head;
	return (void*)(address + cc_first_fit_storage_calc_block_head_aligned_size());
}

static inline cc_first_fit_storage_block_head_t* cc_first_fit_storage_block_head_pointer(const cc_first_fit_storage_t* ctx, const void* block_payload)
{
	uintptr_t address = (uintptr_t)block_payload;
	return (cc_first_fit_storage_block_head_t*)(address - cc_first_fit_storage_calc_block_head_aligned_size());
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_first_fit_storage_calc_block_size(const size_t payload_size)
{
	return cc_first_fit_storage_calc_block_aligned_size(payload_size);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_first_fit_storage_insert_block_into_free_blocks(cc_first_fit_storage_t* ctx, cc_first_fit_storage_block_head_t* block_to_insert)
{
	cc_first_fit_storage_block_head_t* iter;

	// 이전 블럭 찾기
	for (iter = &ctx->start_block; iter->next_free_block < block_to_insert; iter = iter->next_free_block)
	{
		;
	}


	uintptr_t address = (uintptr_t)iter;
	if ((address + cc_first_fit_storage_block_head_get_size(iter)) == ((uintptr_t)block_to_insert))
	{
		cc_first_fit_storage_block_head_add_size(iter, cc_first_fit_storage_block_head_get_size(block_to_insert));
		block_to_insert = iter;
	}


	address = (uintptr_t)block_to_insert;
	if ((address + cc_first_fit_storage_block_head_get_size(block_to_insert)) == ((uintptr_t)iter->next_free_block))
	{
		if (iter->next_free_block != ctx->end_block)
		{
			cc_first_fit_storage_block_head_add_size(block_to_insert, cc_first_fit_storage_block_head_get_size(iter->next_free_block));
			block_to_insert->next_free_block = iter->next_free_block->next_free_block;
		}
		else
		{
			block_to_insert->next_free_block = ctx->end_block;
		}
	}
	else
	{
		block_to_insert->next_free_block = iter->next_free_block;
	}


	if (iter != block_to_insert)
	{
		iter->next_free_block = block_to_insert;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_storage_validate_pointer(const cc_first_fit_storage_t* ctx, const void* pointer)
{
	cc_debug_assert(ctx != NULL);


	uintptr_t begin;
	uintptr_t end;
	uintptr_t current;

	begin = (uintptr_t)ctx->memory_pointer;
	end = (uintptr_t)ctx->end_block;
	current = (uintptr_t)cc_first_fit_storage_block_head_pointer(ctx, pointer);
	if (begin > current)
	{
		return false;
	}
	if (end <= current)
	{
		return false;
	}


	cc_first_fit_storage_block_head_t* block;
	uintptr_t address;

	address = begin;
	while(address != end)
	{
		block = (cc_first_fit_storage_block_head_t*)address;
		if (pointer == cc_first_fit_storage_block_payload_pointer(ctx, block))
		{
			return true;
		}

		address += cc_first_fit_storage_block_head_get_size(block);
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_storage_initialize(cc_first_fit_storage_t* ctx, const void* memory_pointer, const size_t memory_size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(memory_pointer != NULL);
	cc_debug_assert(memory_size != 0);


	//-----------------------------------------------------------------------
	if (cc_first_fit_storage_is_aligned_address((uintptr_t)memory_pointer) == false)
	{
		return false;
	}
	if (memory_size < cc_first_fit_storage_calc_block_head_aligned_size())
	{
		return false;
	}



	//-----------------------------------------------------------------------
	ctx->memory_pointer = (uint8_t*)memory_pointer;
	ctx->memory_size = memory_size;


	//-----------------------------------------------------------------------
	ctx->count = 0;
	ctx->allocated_size = 0;
	ctx->minimum_ever_free_size_remaining = 0;
	ctx->free_size_remaining = 0;
	ctx->start_block.next_free_block = (cc_first_fit_storage_block_head_t*)memory_pointer;
	ctx->start_block.flags = cc_first_fit_storage_block_head_flags_make(0, cc_first_fit_storage_block_status_free);
	ctx->end_block = NULL;


	//-----------------------------------------------------------------------
	cc_first_fit_storage_block_head_t* begin_block;
	cc_first_fit_storage_block_head_t* end_block;

	uint8_t* pointer;
	size_t count;
	size_t offset;


	pointer = ctx->memory_pointer;
	begin_block = (cc_first_fit_storage_block_head_t*)(pointer);

	count = ctx->memory_size / cc_first_fit_storage_calc_block_head_aligned_size();
	offset = (count - 1) * cc_first_fit_storage_calc_block_head_aligned_size();
	end_block = (cc_first_fit_storage_block_head_t*)(pointer + offset);


	//-----------------------------------------------------------------------
	uintptr_t begin_address = (uintptr_t)(begin_block);
	uintptr_t end_address = (uintptr_t)(end_block);
	size_t begin_block_size  = end_address - begin_address;
	

	begin_block->flags = cc_first_fit_storage_block_head_flags_make(begin_block_size, cc_first_fit_storage_block_status_free);
	begin_block->next_free_block = end_block;

	end_block->next_free_block = NULL;
	end_block->flags = cc_first_fit_storage_block_head_flags_make(0, cc_first_fit_storage_block_status_free);


	ctx->end_block = end_block;


	//-----------------------------------------------------------------------
	ctx->minimum_ever_free_size_remaining = begin_block_size;
	ctx->free_size_remaining = begin_block_size;


	return true;
}

//===========================================================================
// First-fit
cc_api void* cc_first_fit_storage_allocate(cc_first_fit_storage_t* ctx, const size_t size)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	if(size == 0)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	size_t block_size = cc_first_fit_storage_calc_block_size(size);
	if (ctx->free_size_remaining  < block_size)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	cc_first_fit_storage_block_head_t* previous_block = &ctx->start_block;
	cc_first_fit_storage_block_head_t* block = previous_block->next_free_block;
	while ((cc_first_fit_storage_block_head_get_size(block) < block_size) && (block->next_free_block != NULL))
	{
		previous_block = block;
		block = block->next_free_block;
	}


	//-----------------------------------------------------------------------
	void* block_payload_pointer = NULL;
	
	if (block != ctx->end_block)
	{
		block_payload_pointer = cc_first_fit_storage_block_payload_pointer(ctx, previous_block->next_free_block);

		
		previous_block->next_free_block = block->next_free_block;


		size_t remaining_size = cc_first_fit_storage_block_head_get_size(block) - block_size;
		if (remaining_size > cc_first_fit_storage_calc_block_aligned_size(0))
		{
			uintptr_t address = (uintptr_t)block;

			cc_first_fit_storage_block_head_t* new_free_block = (cc_first_fit_storage_block_head_t*)(address + block_size);
			cc_first_fit_storage_block_head_set_flags(new_free_block, remaining_size, false);


			cc_first_fit_storage_block_head_set_size(block, block_size);


			cc_first_fit_storage_insert_block_into_free_blocks(ctx, new_free_block);
		}


		size_t allocated_block_size = cc_first_fit_storage_block_head_get_size(block);
		ctx->free_size_remaining -= allocated_block_size;
		if (ctx->free_size_remaining < ctx->minimum_ever_free_size_remaining)
		{
			ctx->minimum_ever_free_size_remaining = ctx->free_size_remaining;
		}


		cc_first_fit_storage_block_head_set_allocated(block, true);
		block->next_free_block = NULL;


		ctx->allocated_size += allocated_block_size;


		ctx->count++;
	}

	return block_payload_pointer;
}

// boundary-tag coalescing
cc_api bool cc_first_fit_storage_free(cc_first_fit_storage_t* ctx, const void* pointer)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(pointer != NULL);
	cc_debug_assert(ctx->count > 0);


	//-----------------------------------------------------------------------
	cc_first_fit_storage_block_head_t* block = cc_first_fit_storage_block_head_pointer(ctx, pointer);
	cc_debug_assert(cc_first_fit_storage_block_head_is_allocated(block));
	cc_debug_assert(block->next_free_block == NULL);


	if (cc_first_fit_storage_block_head_is_allocated(block) == false)
	{
		return false;
	}

	if (block->next_free_block != NULL)
	{
		return false;
	}

	
	size_t free_block_size = cc_first_fit_storage_block_head_get_size(block);
	ctx->free_size_remaining += free_block_size;


	ctx->allocated_size -= free_block_size;
	
	
	cc_first_fit_storage_block_head_set_allocated(block, false);
	cc_first_fit_storage_insert_block_into_free_blocks(ctx, block);


	ctx->count--;


	return true;
}

cc_api cc_ssize_t cc_first_fit_storage_count(cc_first_fit_storage_t* ctx)
{
	//-----------------------------------------------------------------------
	cc_debug_assert(ctx != NULL);


	//-----------------------------------------------------------------------
	return ctx->count;
}
