/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc/test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_version();

//===========================================================================
void test_case_cc_code_template();

//===========================================================================
void test_case_cc_assert_1();
void test_case_cc_assert_2();

//===========================================================================
void test_case_cc_vector_1();
void test_case_cc_vector_2();

void test_case_cc_deque_1();
void test_case_cc_queue_1();
void test_case_cc_stack_1();

void test_case_cc_set_1();

void test_case_cc_map_1();
void test_case_cc_map_2();
void test_case_cc_map_3();

void test_case_cc_forward_list_1();
void test_case_cc_list_1();

void test_case_cc_unordered_set_1();
void test_case_cc_unordered_map_1();
void test_case_cc_unordered_map_2();

void test_case_cc_priority_queue_1();

//===========================================================================
void test_case_cc_sort_1();

//===========================================================================
void test_case_cc_first_fit_1();
void test_case_cc_lf_heap_1();
void test_case_cc_string_1();





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void test_suite_cc_version()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_version", test_case_cc_version},
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_version";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_code_template()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_code_template", test_case_cc_code_template },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_code_template";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_assert()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_assert_1", test_case_cc_assert_1 },
		{ "test_case_cc_assert_2", test_case_cc_assert_2 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_assert";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

//===========================================================================
static void test_suite_cc_vector()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_vector_1", test_case_cc_vector_1 },
		{ "test_case_cc_vector_2", test_case_cc_vector_2 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_vector";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_deque()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_deque_1", test_case_cc_deque_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_deque";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_queue()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_queue_1", test_case_cc_queue_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_queue";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_stack()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_stack_1", test_case_cc_stack_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_stack";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_forward_list()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_forward_list_1", test_case_cc_forward_list_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_forward_list";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_list()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_list_1", test_case_cc_list_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_list";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_set()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_set_1", test_case_cc_set_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_set";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_map()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_map_1", test_case_cc_map_1 },
		{ "test_case_cc_map_2", test_case_cc_map_2 },
		{ "test_case_cc_map_3", test_case_cc_map_3 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_map";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_unordered_set()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_unordered_set_1", test_case_cc_unordered_set_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_unordered_set";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_unordered_map()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_unordered_map_1", test_case_cc_unordered_map_1 },
		{ "test_case_cc_unordered_map_2", test_case_cc_unordered_map_2 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_unordered_map";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_suite_cc_priority_queue()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_priority_queue_1", test_case_cc_priority_queue_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_priority_queue";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

//===========================================================================
static void test_suite_cc_sort ()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_sort_1", test_case_cc_sort_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_sort";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

//===========================================================================
static void test_case_cc_first_fit()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_first_fit_1", test_case_cc_first_fit_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_case_cc_first_fit";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_case_cc_lf_heap()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_lf_heap_1", test_case_cc_lf_heap_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_case_cc_lf_heap";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_case_cc_string()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_string_1", test_case_cc_string_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_case_cc_string";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

//===========================================================================
static void test_run()
{
	test_suite_cc_version();

//	test_suite_cc_code_template();
//	test_suite_cc_assert();
		
#if 1
	test_suite_cc_vector();
#endif

#if 1
	test_suite_cc_deque();
	test_suite_cc_queue();
	test_suite_cc_stack();
	
	test_suite_cc_forward_list();
	test_suite_cc_list();

	test_suite_cc_set();
	test_suite_cc_map();

	test_suite_cc_unordered_set();
	test_suite_cc_unordered_map();

	test_suite_cc_priority_queue();

	test_suite_cc_sort();

	test_case_cc_first_fit();
	test_case_cc_lf_heap_1();
#endif

#if 1
	test_case_cc_string();
#endif
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	test_run();

	return 0;
}

