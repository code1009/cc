#ifndef cc_pair_collection_h
#define cc_pair_collection_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_pair_collection.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_pair_collection_t
{
	cc_pair_t* elements;
	size_t max_count;
	uintptr_t param;

	size_t count;
}
cc_pair_collection_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_pair_collection_initialize(cc_pair_collection_t* ctx, const cc_pair_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_pair_collection_param(cc_pair_collection_t* ctx);

cc_api void cc_pair_collection_clear(cc_pair_collection_t* ctx);
cc_api bool cc_pair_collection_erase(cc_pair_collection_t* ctx, const size_t index);
cc_api bool cc_pair_collection_add(cc_pair_collection_t* ctx, const void* first, const void* second);
cc_api bool cc_pair_collection_insert(cc_pair_collection_t* ctx, const size_t index, const void* first, const void* second);

cc_api cc_pair_t* cc_pair_collection_at(cc_pair_collection_t* ctx, const size_t index);
cc_api void* cc_pair_collection_element_first(cc_pair_collection_t* ctx, const size_t index);
cc_api void* cc_pair_collection_element_second(cc_pair_collection_t* ctx, const size_t index);

cc_api size_t cc_pair_collection_count(cc_pair_collection_t* ctx);
cc_api bool cc_pair_collection_empty(cc_pair_collection_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_pair_collection_h

