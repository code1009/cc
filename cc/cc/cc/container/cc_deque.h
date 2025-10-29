#ifndef cc_deque_h
#define cc_deque_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_deque.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_deque_t
{
	cc_collection_t collection;
	size_t begin;
	size_t end;
}
cc_deque_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_deque_initialize(cc_deque_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_deque_param(cc_deque_t* ctx);

cc_api void* cc_deque_pop_front(cc_deque_t* ctx);
cc_api void* cc_deque_pop_back(cc_deque_t* ctx);
cc_api bool cc_deque_push_back(cc_deque_t* ctx, const void* element);
cc_api bool cc_deque_push_front(cc_deque_t* ctx, const void* element);

cc_api cc_element_t* cc_deque_at(cc_deque_t* ctx, const size_t index);
cc_api void* cc_deque_front_element(cc_deque_t* ctx);
cc_api void* cc_deque_back_element(cc_deque_t* ctx);

cc_api size_t cc_deque_count(cc_deque_t* ctx);
cc_api bool cc_deque_empty(cc_deque_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_deque_h

