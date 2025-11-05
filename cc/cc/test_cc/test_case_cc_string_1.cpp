/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void string_append(void)
{
	test_out
		<< "@string_append()" << test_tendl
		;
	cc_string_t s0;
	cc_string_create(&s0, cc_default_string_allocator());

	cc_string_t s1;
	cc_string_create(&s1, cc_default_string_allocator());
	cc_string_append(&s1, "<Hello,");
	cc_string_append(&s1, "World!>");


	for (size_t i = 0; i < 10; i++)
	{
		cc_string_append(&s0, cc_string_c_str(&s1));
		test_out
			<< "s0: "
			<< "capacity=" << cc_string_capacity(&s0) << ", "
			<< "length=" << cc_string_length(&s0)
			<< test_tendl
			;
	}

	cc_string_destroy(&s1);

	test_out
		<< "s0: data="
		<< cc_string_c_str(&s0) << test_tendl
		;

	cc_string_destroy(&s0);
}

//===========================================================================
static void run(void)
{
	string_append();

	cc_default_string_allocator_dump();

	string_append();
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
