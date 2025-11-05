/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================

//===========================================================================
static void run(void)
{
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_string_1()
{
	if (!cc_default_string_allocator_initialize())
	{
		return;
	}

	cc_default_string_allocator_dump();

	cc_lf_heap_t* lf_heap = &cc_default_string_allocator()->lf_heap;
	cc_first_fit_t* first_fit = &lf_heap->first_fit;
	for (size_t i = 0; i < lf_heap->buckets.count; i++)
	{
		//cc_lf_heap_bucket_t* bucket = &lf_heap->buckets.elements[i];
		cc_lf_heap_add_bucket_region(lf_heap, i);
	}

	cc_default_string_allocator_dump();

	for (size_t i = 0; i < lf_heap->buckets.count; i++)
	{
		//cc_lf_heap_bucket_t* bucket = &lf_heap->buckets.elements[i];
		cc_lf_heap_add_bucket_region(lf_heap, i);
	}

	cc_default_string_allocator_dump();

	run();

	cc_default_string_allocator_dump();

	cc_default_string_allocator_uninitialize();
}
