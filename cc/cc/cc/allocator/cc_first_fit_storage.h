#ifndef cc_first_fit_storage_h
#define cc_first_fit_storage_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_first_fit_storage.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_first_fit_storage_block_head_t
{
	struct _cc_first_fit_storage_block_head_t* next_free_block;
	size_t flags;
}
cc_first_fit_storage_block_head_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_first_fit_storage_t
{
	uint8_t* memory_pointer;
	size_t memory_size;

	size_t allocated_size;
	size_t minimum_ever_free_size_remaining;
	size_t free_size_remaining;
	cc_first_fit_storage_block_head_t start_block;
	cc_first_fit_storage_block_head_t* end_block;

	cc_ssize_t count;
}
cc_first_fit_storage_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_storage_validate_pointer(const cc_first_fit_storage_t* ctx, const void* pointer);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_storage_initialize(cc_first_fit_storage_t* ctx, const void* memory_pointer, const size_t memory_size);

cc_api void* cc_first_fit_storage_allocate(cc_first_fit_storage_t* ctx, const size_t size);
cc_api bool  cc_first_fit_storage_free(cc_first_fit_storage_t* ctx, const void* pointer);

cc_api cc_ssize_t cc_first_fit_storage_count(cc_first_fit_storage_t* ctx);





#endif




