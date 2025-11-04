#ifndef cc_first_fit_h
#define cc_first_fit_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_first_fit.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_first_fit_block_head_t
{
	struct _cc_first_fit_block_head_t* next_free_block;
	size_t flags;
}
cc_first_fit_block_head_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_first_fit_t
{
	uint8_t* memory_pointer;
	size_t memory_size;

	cc_first_fit_block_head_t start_block;
	cc_first_fit_block_head_t* end_block;
	size_t min_ever_free_size;
	size_t free_size;
	uintptr_t first_pointer_address;

	cc_ssize_t count;
}
cc_first_fit_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_validate_pointer(const cc_first_fit_t* ctx, const void* pointer);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_initialize(cc_first_fit_t* ctx, const void* memory_pointer, const size_t memory_size);

cc_api void* cc_first_fit_allocate(cc_first_fit_t* ctx, const size_t size);
cc_api bool  cc_first_fit_free(cc_first_fit_t* ctx, const void* pointer);

cc_api cc_ssize_t cc_first_fit_count(cc_first_fit_t* ctx);





#endif




