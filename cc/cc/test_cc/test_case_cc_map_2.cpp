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
	cc_simple_segregated_storage_t simple_segregated_storage;
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
		&_item_pool.simple_segregated_storage, &_item_pool.memory[0], sizeof(_item_pool.memory), sizeof(item_t), item_max_count
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
	test_out << "item storage count:" << cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage) << test_tendl;
}

static item_t* item_pool_allocate()
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.allocate(&_item_pool.simple_segregated_storage);
	if (item_pointer == NULL)
	{
		test_out << "_item_pool.allocator.allocate() failed" << test_tendl;
		//test_assert(0);
	}
	return item_pointer;
}

static void item_pool_free(item_t* item)
{
	bool rv;

	rv = _item_pool.allocator.free(&_item_pool.simple_segregated_storage, item);
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
	cc_pair_t elements[item_max_count];
	cc_map_t container;
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

	cc_map_initialize(&_items.container, cc_equal_int32_t, cc_less_int32_t, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_map_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_assert(true == cc_map_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = 5;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		if (item_pointer)
		{
			item_pointer->first = (int)i;
			item_pointer->second = 10 + (int)i;
		}
		else
		{
			test_out << "item_pool_allocate() failed:" << test_tindex(i) << test_tendl;
			break;
		}

		rv = cc_map_add(&_items.container, (void*)((uintptr_t)item_pointer->first), item_pointer);
		if (false == rv)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			break;
		}
	}
}

static void add2(void)
{
	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = 10;
	for (i = 5; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		if (item_pointer)
		{
			item_pointer->first = (int)i;
			item_pointer->second = 10 + (int)i;
		}
		else
		{
			test_out << "item_pool_allocate() failed:" << test_tindex(i) << test_tendl;
			break;
		}

		rv = cc_map_add(&_items.container, (void*)((uintptr_t)item_pointer->first), item_pointer);
		if (false == rv)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			break;
		}
	}
}

static void print(void)
{
	test_assert(false == cc_map_empty(&_items.container));


	cc_pair_t* element_pointer;
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_map_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		element_pointer = cc_map_at(&_items.container, i);
		test_assert(element_pointer != NULL);

		item_pointer = (item_t*)element_pointer->second.pointer;
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

static void clear(void)
{
	cc_map_clear(&_items.container);


	test_assert(true == cc_map_empty(&_items.container));


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void release(void)
{
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_map_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_map_element_second(&_items.container, i);
		if (item_pointer != NULL)
		{
			item_pool_free(item_pointer);
		}
	}


	clear();
}

static void lbound(void)
{
	test_assert(false == cc_map_empty(&_items.container));


	size_t index;
	index = cc_map_lower_bound(&_items.container, (void*)11);
	test_out << "lower bound of 11:" << test_tindex(index) << test_tendl;
	test_assert(index == cc_map_count(&_items.container));
}

static void find_and_erase(void)
{
	test_assert(false == cc_map_empty(&_items.container));


	cc_pair_t* element_pointer;
	item_t* item_pointer;
	bool rv;


	size_t index = cc_map_find(&_items.container, (void*)5);
	if (index != cc_invalid_index)
	{
		element_pointer = cc_map_at(&_items.container, index);
		item_pointer = (item_t*)element_pointer->second.pointer;
		test_out << "find:" << test_tindex(index) << item_pointer->first << "," << item_pointer->second << test_tendl;


		rv = cc_map_erase(&_items.container, index);
		if (false == rv)
		{
			test_out << "erase failed:" << test_tindex(index) << test_tendl;
			test_assert(0);
		}
		else
		{
			test_out << "erase success:" << test_tindex(index) << item_pointer->first << "," << item_pointer->second << test_tendl;
		}
		item_pool_free(item_pointer);
	}
	else
	{
		test_out << "not found" << test_tendl;
		test_assert(0);
	}


	item_pointer = (item_t*)cc_map_element_second_by_first(&_items.container, (void*)4);
	if (item_pointer)
	{
		test_out << "element:" << item_pointer->first << "," << item_pointer->second << test_tendl;
	}
	else
	{
		test_out << "not found" << test_tendl;
		test_assert(0);
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();
	print();
	release();

	add();
	lbound();

	add2();
	lbound();

	find_and_erase();
	print();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_map_2()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
