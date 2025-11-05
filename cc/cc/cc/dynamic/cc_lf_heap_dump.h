#ifndef cc_lf_heap_dump_h
#define cc_lf_heap_dump_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_lf_heap_dump.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_lf_heap_bucket_region_dump(cc_lf_heap_bucket_region_head_t* lf_heap_bucket_region, size_t number, uintptr_t base_address);
cc_api void cc_lf_heap_bucket_dump(cc_lf_heap_bucket_t* lf_heap_bucket, size_t number, uintptr_t base_address);
cc_api void cc_lf_heap_dump(cc_lf_heap_t* lf_heap, size_t number, uintptr_t base_address);




#endif




