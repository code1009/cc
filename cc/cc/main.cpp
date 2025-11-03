/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc/test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_version();

void test_case_cc_code_template();

void test_case_cc_assert_1();
void test_case_cc_assert_2();

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

void test_case_cc_collection_sort_1();

void test_case_cc_first_fit_storage_1();
void test_case_cc_heap_memory_1();





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
static void test_suite_cc_collection_sort ()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_collection_sort_1", test_case_cc_collection_sort_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_suite_cc_collection_sort";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

//===========================================================================
static void test_case_cc_first_fit_storage()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_first_fit_storage_1", test_case_cc_first_fit_storage_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_case_cc_first_fit_storage";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}

static void test_case_cc_heap_memory()
{
	test_case_t test_cases[] =
	{
		{ "test_case_cc_heap_memory_1", test_case_cc_heap_memory_1 },
		{ NULL, NULL }
	};


	test_suite_t test_suite;
	test_suite.name = "test_case_cc_heap_memory";
	test_suite.items = test_cases;
	test_suite.count = sizeof(test_cases) / sizeof(test_case_t) - 1;
	test_runner(&test_suite);
}


//===========================================================================
static void test_run()
{
#if 0
	test_suite_cc_version();

//	test_suite_cc_code_template();
//	test_suite_cc_assert();
		
	test_suite_cc_vector();

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

	test_suite_cc_collection_sort();
#endif

	test_case_cc_first_fit_storage();
	test_case_cc_heap_memory();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
int main()
{
	test_run();

	return 0;
}

