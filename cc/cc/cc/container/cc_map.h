#ifndef cc_map_h
#define cc_map_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_map.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_map_t
{
	cc_equal_t equal;
	cc_less_t less;
	cc_pair_collection_t collection;
}
cc_map_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_map_lower_bound(cc_map_t* ctx, const void* first);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_map_initialize(cc_map_t* ctx, const cc_equal_t equal, const cc_less_t less, const cc_pair_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_map_param(cc_map_t* ctx);

cc_api void cc_map_clear(cc_map_t* ctx);
cc_api bool cc_map_erase(cc_map_t* ctx, const size_t index);
cc_api bool cc_map_add(cc_map_t* ctx, const void* first, const void* second);

cc_api cc_pair_t* cc_map_at(cc_map_t* ctx, const size_t index);
cc_api void* cc_map_element_first(cc_map_t* ctx, const size_t index);
cc_api void* cc_map_element_second(cc_map_t* ctx, const size_t index);

cc_api size_t cc_map_find(cc_map_t* ctx, const void* first);
cc_api void* cc_map_element_second_by_first(cc_map_t* ctx, const void* first);

cc_api size_t cc_map_count(cc_map_t* ctx);
cc_api bool cc_map_empty(cc_map_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_map_h

