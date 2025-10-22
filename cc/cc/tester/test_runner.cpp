/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_output.hpp"
#include "test_runner.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_runner(test_suite_t* suite)
{
	size_t i;
	size_t count;

	test_case_t* test_case;


	test_out << "==========================================================================" << test_tendl;
	test_out << "test suite: " << suite->name << test_tendl;
	test_out << "==========================================================================" << test_tendl;
	test_out << test_tendl;

	count = suite->count;
	for (i = 0; i < count; i++)
	{
		test_case = &suite->items[i];

		test_out << "--------------------------------------------------------------------------" << test_tendl;
		test_out << "test case: " << test_case->name << test_tendl;
		test_out << "--------------------------------------------------------------------------" << test_tendl;

		test_case->execute();

		test_out << test_tendl;
	}

	test_out << test_tendl;
	test_out << test_tendl;
}
