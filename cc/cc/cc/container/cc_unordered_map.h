#ifndef cc_unordered_map_h
#define cc_unordered_map_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_unordered_map.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_unordered_map_t
{
	cc_hash_generate_t generate;
	cc_equal_t equal;
	cc_pair_bucket_t* table;
	size_t max_count;
	uintptr_t param;

	size_t count;
}
cc_unordered_map_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_unordered_map_initialize(cc_unordered_map_t* ctx, const cc_hash_generate_t generate, const cc_equal_t equal, const cc_pair_bucket_t* table, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_unordered_map_param(cc_unordered_map_t* ctx);

cc_api void cc_unordered_map_clear(cc_unordered_map_t* ctx);
cc_api bool cc_unordered_map_erase(cc_unordered_map_t* ctx, const size_t index);
cc_api bool cc_unordered_map_add(cc_unordered_map_t* ctx, const void* first, const void* second);

cc_api cc_pair_bucket_t* cc_unordered_map_at(cc_unordered_map_t* ctx, const size_t index);
cc_api void* cc_unordered_map_element_first(cc_unordered_map_t* ctx, const size_t index);
cc_api void* cc_unordered_map_element_second(cc_unordered_map_t* ctx, const size_t index);

cc_api size_t cc_unordered_map_find(cc_unordered_map_t* ctx, const void* first);
cc_api void* cc_unordered_map_element_second_by_first(cc_unordered_map_t* ctx, const void* first);

cc_api size_t cc_unordered_map_count(cc_unordered_map_t* ctx);
cc_api bool cc_unordered_map_empty(cc_unordered_map_t* ctx);

cc_api size_t cc_unordered_map_table_size(cc_unordered_map_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_unordered_map_h

