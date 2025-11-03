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

static item_t* item_pool_allocate()
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.allocate(&_item_pool.storage);
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

	rv = _item_pool.allocator.free(&_item_pool.storage, item);
	if (rv == false)
	{
		test_out << "_item_pool.allocator.free() failed" << test_tendl;
		test_assert(0);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _key_pool_t
{
	cc_simple_segregated_storage_t storage;
	uintptr_t memory[item_max_count];
	cc_fallocator_t allocator;
}
key_pool_t;

//===========================================================================
static key_pool_t _key_pool;

//===========================================================================
static bool key_pool_initialize()
{
	bool rv;
	rv = cc_simple_segregated_storage_fallocator_initialize(
		&_key_pool.allocator,
		&_key_pool.storage, &_key_pool.memory[0], sizeof(_key_pool.memory), sizeof(int), item_max_count
	);
	if (rv == false)
	{
		test_out << "cc_simple_segregated_storage_fallocator_initialize() failed (key)" << test_tendl;
		test_assert(0);
		return false;
	}
	return true;
}

static void key_pool_uninitialize()
{
	test_out << "key storage count:" << cc_simple_segregated_storage_count(&_key_pool.storage) << test_tendl;
}

static int* key_pool_allocate()
{
	int* key_pointer = (int*)_key_pool.allocator.allocate(&_key_pool.storage);
	if (key_pointer == NULL)
	{
		test_out << "_key_pool.allocator.alloc() failed" << test_tendl;
		//test_assert(0);
	}
	return key_pointer;
}

static void key_pool_free(int* key)
{
	bool rv;

	rv = _key_pool.allocator.free(&_key_pool.storage, key);
	if (rv == false)
	{
		test_out << "_key_pool.allocator.free() failed" << test_tendl;
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

	rv = key_pool_initialize();
	if (rv == false)
	{
		item_pool_uninitialize();
		test_assert(0);
		return false;
	}

	cc_map_initialize(&_items.container, cc_equal_pvalue_int32_t, cc_less_pvalue_int32_t, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_map_count(&_items.container) << test_tendl;

	key_pool_uninitialize();
	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_assert(true == cc_map_empty(&_items.container));


	bool rv;

	item_t* item_pointer;
	int* key_pointer;


	size_t i;
	size_t count;

	count = 20;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		if (item_pointer)
		{
			item_pointer->first = 10 - (int)i;
			item_pointer->second = 20 - (int)i;
		}
		else
		{
			test_out << "item_pool_allocate() failed:" << test_tindex(i) << test_tendl;
			test_assert(i == 10);
			break;
		}

		key_pointer = key_pool_allocate();
		if (key_pointer)
		{
			*key_pointer = item_pointer->first;
		}
		else
		{
			test_out << "key_pool_allocate() failed:" << test_tindex(i) << test_tendl;
			test_assert(0);
			item_pool_free(item_pointer);
			break;
		}


		test_out << "add:" << test_tindex(i) << item_pointer->first << "," << item_pointer->second << test_tendl;
		rv = cc_map_add(&_items.container, key_pointer, item_pointer);
		if (false == rv)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			key_pool_free(key_pointer);
			test_assert(0);
			break;
		}
	}


	int k = 0;
	rv = cc_map_add(&_items.container, &k, NULL);
	test_assert(rv == false);

	test_assert(item_max_count == cc_map_count(&_items.container));

	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.storage));
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


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.storage));
}

static void release(void)
{
	int* key_pointer;
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_map_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		key_pointer = (int*)cc_map_element_first(&_items.container, i);
		test_assert(key_pointer != NULL);

		item_pointer = (item_t*)cc_map_element_second(&_items.container, i);
		test_assert(item_pointer != NULL);

		key_pool_free(key_pointer);
		item_pool_free(item_pointer);
	}


	clear();
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
void test_case_cc_map_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}