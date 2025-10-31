#ifndef cc_simple_segregated_storage_h
#define cc_simple_segregated_storage_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_simple_segregated_storage.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_simple_segregated_storage_chunk_t
{
	struct _cc_simple_segregated_storage_chunk_t* next_free_chunk;
}
cc_simple_segregated_storage_chunk_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_simple_segregated_storage_t
{
	uint8_t* memory_pointer;
	size_t memory_size;

	size_t data_size;
	size_t max_count;

	size_t chunk_size;

	cc_simple_segregated_storage_chunk_t* free_chunk_head;

	cc_ssize_t count;
}
cc_simple_segregated_storage_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_simple_segregated_storage_calc_memory_size(const size_t data_size, const size_t max_count);
cc_api bool   cc_simple_segregated_storage_validate_pointer(const cc_simple_segregated_storage_t* ctx, const void* pointer);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_simple_segregated_storage_initialize(cc_simple_segregated_storage_t* ctx, const void* memory_pointer, const size_t memory_size, const size_t data_size, const size_t max_count);

cc_api void* cc_simple_segregated_storage_allocate(cc_simple_segregated_storage_t* ctx);
cc_api bool  cc_simple_segregated_storage_free(cc_simple_segregated_storage_t* ctx, const void* pointer);

cc_api cc_ssize_t cc_simple_segregated_storage_count(cc_simple_segregated_storage_t* ctx);





#endif




