#ifndef cc_forward_list_h
#define cc_forward_list_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_forward_list.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_forward_list_t
{
	cc_simple_segregated_storage_t nstorage;
	cc_fallocator_t nallocator;

	cc_snode_t* elements;
	size_t max_count;
	uintptr_t param;

	size_t count;
	cc_snode_t* first;
}
cc_forward_list_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_forward_list_initialize(cc_forward_list_t* ctx, const cc_snode_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_forward_list_param(cc_forward_list_t* ctx);

cc_api void  cc_forward_list_clear(cc_forward_list_t* ctx);
cc_api bool  cc_forward_list_push_front(cc_forward_list_t* ctx, const void* element);
cc_api void* cc_forward_list_pop_front(cc_forward_list_t* ctx);
cc_api cc_snode_t* cc_forward_list_insert_after(cc_forward_list_t* ctx, const cc_snode_t* node, const void* element);
cc_api bool cc_forward_list_erase_after(cc_forward_list_t* ctx, const cc_snode_t* node);

cc_api cc_snode_t* cc_forward_list_first(cc_forward_list_t* ctx);
cc_api void* cc_forward_list_node_element(cc_forward_list_t* ctx, const cc_snode_t* node);

cc_api size_t cc_forward_list_count(cc_forward_list_t* ctx);
cc_api bool cc_forward_list_empty(cc_forward_list_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_forward_list_h

