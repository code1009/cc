#ifndef cc_vallocator_helper_h
#define cc_vallocator_helper_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_vallocator_helper.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_first_fit_vallocator_initialize(
	cc_vallocator_t* vallocator,
	cc_first_fit_t* first_fit, const void* memory_pointer, const size_t memory_size
);

cc_api bool cc_heap_memory_vallocator_initialize(
	cc_vallocator_t* vallocator,
	cc_heap_memory_t* heap_memory, const void* memory_pointer, const size_t memory_size, const cc_heap_memory_config_t* heap_memory_config
);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_vallocator_helper_h

