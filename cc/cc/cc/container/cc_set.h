#ifndef cc_set_h
#define cc_set_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_set.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_set_t
{
	cc_equal_t equal;
	cc_less_t less;
	cc_collection_t collection;
}
cc_set_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_set_lower_bound(cc_set_t* ctx, const void* first);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_set_initialize(cc_set_t* ctx, const cc_equal_t equal, const cc_less_t less, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_set_param(cc_set_t* ctx);

cc_api void cc_set_clear(cc_set_t* ctx);
cc_api bool cc_set_erase(cc_set_t* ctx, const size_t index);
cc_api bool cc_set_add(cc_set_t* ctx, const void* element);

cc_api cc_element_t* cc_set_at(cc_set_t* ctx, const size_t index);
cc_api void* cc_set_element(cc_set_t* ctx, const size_t index);

cc_api size_t cc_set_find(cc_set_t* ctx, const void* element);
cc_api void* cc_set_element_by_value(cc_set_t* ctx, const void* element);

cc_api size_t cc_set_count(cc_set_t* ctx);
cc_api bool cc_set_empty(cc_set_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_set_h

