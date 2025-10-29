#ifndef cc_list_h
#define cc_list_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_list.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_list_t
{
	cc_simple_segregated_storage_t nstorage;
	cc_allocator_t nallocator;

	cc_dnode_t* elements;
	size_t max_count;
	uintptr_t param;

	size_t count;
	cc_dnode_t* first;
	cc_dnode_t* last;
}
cc_list_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_list_initialize(cc_list_t* ctx, const cc_dnode_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_list_param(cc_list_t* ctx);

cc_api void cc_list_clear(cc_list_t* ctx);
cc_api bool cc_list_erase(cc_list_t* ctx, const cc_dnode_t* node);
cc_api cc_dnode_t* cc_list_add(cc_list_t* ctx, const void* element);
cc_api cc_dnode_t* cc_list_insert(cc_list_t* ctx, const cc_dnode_t* node, const void* element);

cc_api cc_dnode_t* cc_list_first(cc_list_t* ctx);
cc_api cc_dnode_t* cc_list_last(cc_list_t* ctx);
cc_api void* cc_list_node_element(cc_list_t* ctx, const cc_dnode_t* node);

cc_api size_t cc_list_count(cc_list_t* ctx);
cc_api bool cc_list_empty(cc_list_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_list_h

