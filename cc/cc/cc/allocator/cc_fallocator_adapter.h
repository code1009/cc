#ifndef cc_fallocator_adapter_h
#define cc_fallocator_adapter_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_fallocator_adapter.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_simple_segregated_storage_fallocator_initialize(
	cc_fallocator_t* fallocator,
	cc_simple_segregated_storage_t* simple_segregated_storage, const void* memory_pointer, const size_t memory_size, const size_t data_size, const size_t max_count
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_fallocator_adapter_h

