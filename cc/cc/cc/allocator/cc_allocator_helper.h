#ifndef cc_allocator_helper_h
#define cc_allocator_helper_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_allocator_helper.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_simple_segregated_storage_allocator_initialize(
	cc_allocator_t* allocator,
	cc_simple_segregated_storage_t* simple_segregated_storage, const void* memory_pointer, const size_t memory_size, const size_t data_size, const size_t max_count
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_allocator_helper_h

