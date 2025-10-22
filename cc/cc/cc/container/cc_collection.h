#ifndef cc_collection_h
#define cc_collection_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_collection.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_collection_t
{
	cc_element_t* elements;
	size_t max_count;
	uintptr_t param;

	size_t count;
}
cc_collection_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_collection_initialize(cc_collection_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_collection_param(cc_collection_t* ctx);

cc_api void cc_collection_clear(cc_collection_t* ctx);
cc_api bool cc_collection_erase(cc_collection_t* ctx, const size_t index);
cc_api bool cc_collection_add(cc_collection_t* ctx, const void* element);
cc_api bool cc_collection_insert(cc_collection_t* ctx, const size_t index, const void* element);

cc_api cc_element_t* cc_collection_at(cc_collection_t* ctx, const size_t index);
cc_api void* cc_collection_element(cc_collection_t* ctx, const size_t index);

cc_api size_t cc_collection_count(cc_collection_t* ctx);
cc_api bool cc_collection_empty(cc_collection_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_collection_h

