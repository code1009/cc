#ifndef cc_string_heap_memory_h
#define cc_string_heap_memory_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string_heap_memory.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_default_string_heap_memory_initialize(void);
cc_api void cc_default_string_heap_memory_uninitialize(void);

cc_api cc_heap_memory_t* cc_default_string_heap_memory(void);
cc_api cc_vallocator_t* cc_default_string_heap_memory_allocator(void);

cc_api void cc_default_string_heap_memory_dump(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_string_heap_memory_h

