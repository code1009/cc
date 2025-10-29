#ifndef cc_queue_h
#define cc_queue_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_queue.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_queue_t
{
	cc_deque_t deque;
}
cc_queue_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_queue_initialize(cc_queue_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_queue_param(cc_queue_t* ctx);

cc_api bool cc_queue_push(cc_queue_t* ctx, const void* element);
cc_api void* cc_queue_pop(cc_queue_t* ctx);

cc_api void* cc_queue_front_element(cc_queue_t* ctx);
cc_api void* cc_queue_back_element(cc_queue_t* ctx);

cc_api size_t cc_queue_count(cc_queue_t* ctx);
cc_api bool cc_queue_empty(cc_queue_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_queue_h

