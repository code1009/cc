#ifndef cc_unordered_set_h
#define cc_unordered_set_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_unordered_set.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_unordered_set_t
{
	cc_hash_generate_t generate;
	cc_equal_t equal;
	cc_bucket_t* table;
	size_t max_count;
	uintptr_t param;

	size_t count;
}
cc_unordered_set_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_unordered_set_initialize(cc_unordered_set_t* ctx, const cc_hash_generate_t generate, const cc_equal_t equal, const cc_bucket_t* table, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_unordered_set_param(cc_unordered_set_t* ctx);

cc_api void cc_unordered_set_clear(cc_unordered_set_t* ctx);
cc_api bool cc_unordered_set_erase(cc_unordered_set_t* ctx, const size_t index);
cc_api bool cc_unordered_set_add(cc_unordered_set_t* ctx, const void* element);

cc_api cc_bucket_t* cc_unordered_set_at(cc_unordered_set_t* ctx, const size_t index);
cc_api void* cc_unordered_set_element(cc_unordered_set_t* ctx, const size_t index);

cc_api size_t cc_unordered_set_find(cc_unordered_set_t* ctx, const void* element);
cc_api void* cc_unordered_set_element_by_key(cc_unordered_set_t* ctx, const void* element);

cc_api size_t cc_unordered_set_count(cc_unordered_set_t* ctx);
cc_api bool cc_unordered_set_empty(cc_unordered_set_t* ctx);

cc_api size_t cc_unordered_set_table_size(cc_unordered_set_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_unordered_set_h

