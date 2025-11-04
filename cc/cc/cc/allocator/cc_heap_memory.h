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
typedef struct _cc_heap_bucket_config_t
{
	size_t size;
	size_t count;
}
cc_heap_bucket_config_t;

//===========================================================================
typedef struct _cc_heap_memory_config_t
{
	cc_heap_bucket_config_t* buckets;
	size_t bucket_count;
}
cc_heap_memory_config_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_heap_bucket_storage_t
{
	struct _cc_heap_bucket_storage_t* next;
	cc_simple_segregated_storage_t storage;
}
cc_heap_bucket_storage_t;

//===========================================================================
typedef struct _cc_heap_bucket_t
{
	cc_heap_bucket_config_t config;
	cc_heap_bucket_storage_t* storages;
}
cc_heap_bucket_t;

//===========================================================================
typedef struct _cc_heap_memory_t
{
	cc_first_fit_t first_fit;

	cc_heap_bucket_t* buckets;
	size_t bucket_count;

	cc_ssize_t count;
}
cc_heap_memory_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_validate_pointer(const cc_heap_memory_t* ctx, const void* pointer);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_heap_memory_initialize(cc_heap_memory_t* ctx, const void* memory_pointer, const size_t memory_size, const cc_heap_memory_config_t* config);
cc_api void cc_heap_memory_uninitialize(cc_heap_memory_t* ctx);

cc_api void* cc_heap_memory_allocate(cc_heap_memory_t* ctx, const size_t size);
cc_api bool  cc_heap_memory_free(cc_heap_memory_t* ctx, const void* pointer);

cc_api cc_ssize_t cc_heap_memory_count(cc_heap_memory_t* ctx);





#endif




