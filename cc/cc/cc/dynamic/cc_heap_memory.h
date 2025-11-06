#ifndef cc_heap_memory_h
#define cc_heap_memory_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_heap_memory.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_heap_memory_t
{
	cc_lf_heap_t lf_heap;
	cc_vallocator_t allocator;
} cc_heap_memory_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_initialize(cc_heap_memory_t* ctx, 
	const void* memory_pointer, const size_t memory_size, 
	const cc_lf_heap_bucket_descriptors_t* lf_heap_bucket_descriptors
);

cc_api void cc_heap_memory_uninitialize(cc_heap_memory_t* ctx);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void* cc_heap_memory_allocate(cc_heap_memory_t* ctx, const size_t size);
cc_api bool cc_heap_memory_free(cc_heap_memory_t* ctx, void* data);
cc_api void* cc_heap_memory_reallocate(cc_heap_memory_t* ctx, char* old_data, const size_t old_size, const size_t new_size);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_heap_memory_h

