#ifndef cc_stack_h
#define cc_stack_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_stack.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_stack_t
{
	cc_deque_t deque;
}
cc_stack_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_stack_initialize(cc_stack_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_stack_param(cc_stack_t* ctx);

cc_api bool cc_stack_push(cc_stack_t* ctx, const void* element);
cc_api void* cc_stack_pop(cc_stack_t* ctx);

cc_api void* cc_stack_top_element(cc_stack_t* ctx);

cc_api size_t cc_stack_count(cc_stack_t* ctx);
cc_api bool cc_stack_empty(cc_stack_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_stack_h

