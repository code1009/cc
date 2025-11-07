/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void string_append(bool memory_leack)
{
	test_out
		<< "@string_append()" << test_tendl
		;
	test_out << "=============================================================================" << test_tendl;


	cc_string_t s;
	cc_string_t s0;
	cc_string_t s1;


	cc_string_create(&s, cc_default_string_heap_memory_allocator());
	cc_string_append(&s, "Hello");



	cc_string_create(&s0, cc_default_string_heap_memory_allocator());

	cc_string_create(&s1, cc_default_string_heap_memory_allocator());
	cc_string_append(&s1, "<Hello");
	cc_string_append(&s1, "World>");
	cc_string_destroy(&s1);

	cc_string_create(&s1, cc_default_string_heap_memory_allocator());
	cc_string_append(&s1, "<World>");


	for (size_t i = 0; i < 20; i++)
	{
		cc_string_append(&s0, cc_string_c_str(&s1));
		test_out
			<< "s0: "
			<< "capacity=" << cc_string_capacity(&s0) << ", "
			<< "length=" << cc_string_length(&s0)
			<< test_tendl
			;
	}
	cc_string_append_n(&s0, s0.data, s0.length);


	if (memory_leack == false)
	{
		cc_string_destroy(&s1);
	}
	else
	{
		test_out << "Memory Leak Test: Skip cc_string_destroy(&s1)" << test_tendl;
	}

	test_out << "s0: data=" << cc_string_c_str(&s0) << test_tendl;



	if (memory_leack == false)
	{
		cc_string_destroy(&s0);
	}
	else
	{
		size_t alignment = sizeof(void*) * 2;
		size_t s0_string_memory_size = s0.capacity + 1;
		size_t s0_string_memory_aligned_size =
			s0_string_memory_size / alignment * alignment + ((s0_string_memory_size % alignment) ? alignment : 0);

		size_t s0_memory_leak_size =
			sizeof(cc_first_fit_block_head_t) +
			s0_string_memory_aligned_size;
			;

		uintptr_t s0_address = (uintptr_t)&s0.data[0];

		test_out << "Memory Leak Test: Skip cc_string_destroy(&s0):"
			<< " s0_memory_leak_size=" << s0_memory_leak_size
			<< " s0.data =" << (void*)s0_address
			<< test_tendl;
	}


	if (memory_leack == false)
	{
		cc_string_destroy(&s);
	}
	else
	{
		test_out << "Memory Leak Test: Skip cc_string_destroy(&s)" << test_tendl;
	}


	test_out << "=============================================================================" << test_tendl;
}

//===========================================================================
static void run(void)
{
	string_append(false);

	cc_default_string_heap_memory_dump();

	string_append(true);
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_string_1()
{
	//----------------------------------------------------------------------
	if (!cc_default_string_heap_memory_initialize())
	{
		return;
	}


	//----------------------------------------------------------------------
	cc_default_string_heap_memory_dump();

	cc_lf_heap_t* lf_heap = &cc_default_string_heap_memory()->lf_heap;
	cc_first_fit_t* first_fit = &lf_heap->first_fit;
	for (size_t i = 0; i < lf_heap->buckets.count; i++)
	{
		//cc_lf_heap_bucket_t* bucket = &lf_heap->buckets.elements[i];
		cc_lf_heap_add_bucket_region(lf_heap, i);
	}

	cc_default_string_heap_memory_dump();

	for (size_t i = 0; i < lf_heap->buckets.count; i++)
	{
		//cc_lf_heap_bucket_t* bucket = &lf_heap->buckets.elements[i];
		cc_lf_heap_add_bucket_region(lf_heap, i);
	}

	cc_default_string_heap_memory_dump();

	run();

	cc_default_string_heap_memory_dump();


	//----------------------------------------------------------------------
	cc_default_string_heap_memory_uninitialize();
}
