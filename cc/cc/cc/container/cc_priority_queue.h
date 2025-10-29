#ifndef cc_priority_queue_h
#define cc_priority_queue_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_priority_queue.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_priority_queue_t
{
	cc_less_t less;
	cc_collection_t collection;
}
cc_priority_queue_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_priority_queue_initialize(cc_priority_queue_t* ctx, const cc_less_t less, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_priority_queue_param(cc_priority_queue_t* ctx);

cc_api bool cc_priority_queue_push(cc_priority_queue_t* ctx, const void* element);
cc_api void* cc_priority_queue_pop(cc_priority_queue_t* ctx);

cc_api void* cc_priority_queue_top_element(cc_priority_queue_t* ctx);

cc_api size_t cc_priority_queue_count(cc_priority_queue_t* ctx);
cc_api bool cc_priority_queue_empty(cc_priority_queue_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_priority_queue_h

