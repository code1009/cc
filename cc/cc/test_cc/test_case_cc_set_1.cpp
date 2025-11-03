/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	int first;
	int second;
} item_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 10





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_simple_segregated_storage_t storage;
	item_t memory[item_max_count];
	cc_fallocator_t allocator;
}
item_pool_t;

//===========================================================================
static item_pool_t _item_pool;

//===========================================================================
static bool item_pool_initialize()
{
	bool rv;
	rv = cc_simple_segregated_storage_fallocator_initialize(
		&_item_pool.allocator,
		&_item_pool.storage, &_item_pool.memory[0], sizeof(_item_pool.memory), sizeof(item_t), item_max_count
	);
	if (rv == false)
	{
		test_out << "cc_simple_segregated_storage_fallocator_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}
	return true;
}

static void item_pool_uninitialize()
{
	test_out << "item storage count:" << cc_simple_segregated_storage_count(&_item_pool.storage) << test_tendl;
}

static item_t* item_pool_alloc()
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.alloc(&_item_pool.storage);
	if (item_pointer == NULL)
	{
		test_out << "_item_pool.allocator.alloc() failed" << test_tendl;
		//test_assert(0);
	}
	return item_pointer;
}

static void item_pool_free(item_t* item)
{
	bool rv;

	rv = _item_pool.allocator.free(&_item_pool.storage, item);
	if (rv == false)
	{
		test_out << "_item_pool.allocator.free() failed" << test_tendl;
		test_assert(0);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _items_t
{
	cc_element_t elements[item_max_count];
	cc_set_t container;
}
items_t;

//===========================================================================
static items_t _items;

//===========================================================================
static bool items_initialize()
{
	bool rv;

	rv = item_pool_initialize();
	if (rv == false)
	{
		test_assert(0);
		return false;
	}

	cc_set_initialize(&_items.container, cc_equal_pvalue_int32_t, cc_less_pvalue_int32_t, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_set_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = 20;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_alloc();
		if (item_pointer)
		{
			item_pointer->first = 10 - (int)i;
			item_pointer->second = 20 - (int)i;
		}
		else
		{
			test_out << "item_pool_alloc() failed:" << test_tindex(i) << test_tendl;
			test_assert(i == 10);
			break;
		}

		test_out << "add:" << test_tindex(i) << item_pointer->first << "," << item_pointer->second << test_tendl;
		rv = cc_set_add(&_items.container, item_pointer);
		if (false == rv)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
			break;
		}
	}
}

static void print(void)
{
	cc_element_t* element_pointer;
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_set_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		element_pointer = cc_set_at(&_items.container, i);
		test_assert(element_pointer != NULL);

		item_pointer = (item_t*)element_pointer->pointer;
		test_assert(item_pointer != NULL);

		test_out
			<< test_tindex(i)
			<< "="
			<< item_pointer->first
			<< ","
			<< item_pointer->second
			<< test_tendl
			;
	}
}

static void release(void)
{
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_set_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_set_element(&_items.container, i);
		test_assert(item_pointer != NULL);

		item_pool_free(item_pointer);
	}

	cc_set_clear(&_items.container);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();

	print();
	release();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_set_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}