#ifndef cc_lf_heap_h
#define cc_lf_heap_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_lf_heap.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_lf_heap_bucket_descriptor_t
{
	size_t size;
	size_t count;
}
cc_lf_heap_bucket_descriptor_t;

//===========================================================================
typedef struct _cc_lf_heap_bucket_descriptors_t
{
	cc_lf_heap_bucket_descriptor_t* elements;
	size_t count;
}
cc_lf_heap_bucket_descriptors_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_lf_heap_bucket_region_head_t
{
	struct _cc_lf_heap_bucket_region_head_t* next;
	cc_simple_segregated_storage_t simple_segregated_storage;
}
cc_lf_heap_bucket_region_head_t;

//===========================================================================
typedef struct _cc_lf_heap_bucket_t
{
	cc_lf_heap_bucket_descriptor_t descriptor;
	cc_lf_heap_bucket_region_head_t* regions;
	cc_lf_heap_bucket_region_head_t* cache_region;
}
cc_lf_heap_bucket_t;

typedef struct _cc_lf_heap_buckets_t
{
	cc_lf_heap_bucket_t* elements;
	size_t count;
}
cc_lf_heap_buckets_t;

//===========================================================================
typedef struct _cc_lf_heap_t
{
	cc_first_fit_t first_fit;
	cc_lf_heap_buckets_t buckets;

	cc_ssize_t count;
}
cc_lf_heap_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_lf_heap_validate_pointer(const cc_lf_heap_t* ctx, const void* pointer);
cc_api bool cc_lf_heap_add_bucket_region(cc_lf_heap_t* ctx, size_t index);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_lf_heap_initialize(cc_lf_heap_t* ctx, const void* memory_pointer, const size_t memory_size, const cc_lf_heap_bucket_descriptors_t* bucket_descriptors);
cc_api void cc_lf_heap_uninitialize(cc_lf_heap_t* ctx);

cc_api void* cc_lf_heap_allocate(cc_lf_heap_t* ctx, const size_t size);
cc_api bool  cc_lf_heap_free(cc_lf_heap_t* ctx, const void* pointer);

cc_api cc_ssize_t cc_lf_heap_count(cc_lf_heap_t* ctx);





#endif




