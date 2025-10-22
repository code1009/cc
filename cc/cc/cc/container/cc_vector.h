#ifndef cc_vector_h
#define cc_vector_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_vector.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_vector_t
{
	cc_collection_t collection;
}
cc_vector_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_vector_initialize(cc_vector_t* ctx, const cc_element_t* elements, const size_t max_count, const uintptr_t param);
cc_api uintptr_t cc_vector_param(cc_vector_t* ctx);

cc_api void cc_vector_clear(cc_vector_t* ctx);
cc_api bool cc_vector_erase(cc_vector_t* ctx, const size_t index);
cc_api bool cc_vector_add(cc_vector_t* ctx, const void* element);
cc_api bool cc_vector_insert(cc_vector_t* ctx, const size_t index, const void* element);

cc_api cc_element_t* cc_vector_at(cc_vector_t* ctx, const size_t index);
cc_api void* cc_vector_element(cc_vector_t* ctx, const size_t index);

cc_api size_t cc_vector_count(cc_vector_t* ctx);
cc_api bool cc_vector_empty(cc_vector_t* ctx);






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_vector_h

