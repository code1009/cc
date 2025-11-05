#ifndef cc_string_allocator_h
#define cc_string_allocator_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string_allocator.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_string_allocator_t
{
	cc_lf_heap_t lf_heap;
} cc_string_allocator_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_default_string_allocator_initialize(void);
cc_api void cc_default_string_allocator_uninitialize(void);

cc_api cc_string_allocator_t* cc_default_string_allocator(void);
cc_api void cc_default_string_allocator_dump(void);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_string_allocator_h

