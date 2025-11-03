/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	uint16_t key1;
	uint16_t key2;
	uint32_t value;
} item_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api static cc_hash_value_t item_hash(const void* item)
{
	test_assert(item != NULL);


	return cc_hash_djb2(item, 4);
}

cc_api static bool item_equal(const void* left, const void* right)
{
	test_assert(left != NULL);
	test_assert(right != NULL);


	const item_t* litem_pointer = (const item_t*)left;
	const item_t* ritem_pointer = (const item_t*)right;


	if ((litem_pointer->key1 == ritem_pointer->key1) &&
		(litem_pointer->key2 == ritem_pointer->key2))
	{
		return true;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 32

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
	cc_bucket_t elements[item_max_count];
	cc_unordered_set_t container;
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

	cc_unordered_set_initialize(
		&_items.container,
		item_hash, item_equal, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_unordered_set_count(&_items.container) << test_tendl;

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

	count = 512;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_alloc();
		if (item_pointer)
		{
			item_pointer->key1 = (uint16_t)i;
			item_pointer->key2 = (uint16_t)i + 10;
			item_pointer->value = (uint32_t)i + 1000;
		}
		else
		{
			test_out << "item_pool_alloc() failed:" << test_tindex(i) << test_tendl;
			break;
		}

		rv = cc_unordered_set_add(&_items.container, item_pointer);
		if (false == rv)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(item_max_count == i);
			break;
		}
	}
}

static void print(void)
{
	item_t* item_pointer;
	cc_hash_value_t hash_value;


	size_t i;
	size_t count;

	count = cc_unordered_set_table_size(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_unordered_set_element(&_items.container, i);

		if (item_pointer != NULL)
		{
			hash_value = item_hash(item_pointer);

			test_out
				<< test_tindex(cc_hash_value_index(hash_value, cc_unordered_set_table_size(&_items.container)))
				<< "+"
				<< cc_hash_calc_attempt(hash_value, cc_unordered_set_table_size(&_items.container), i)
				<< "->"
				<< test_tindex(i)
				<< "="
				<< item_pointer->key1  << ","
				<< item_pointer->key2  << ","
				<< item_pointer->value
				<< " hash="
				<< hash_value
				<< test_tendl
				;
		}
	}
}

static void release(void)
{
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_unordered_set_table_size(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_unordered_set_element(&_items.container, i);
		if (item_pointer != NULL)
		{
			item_pool_free(item_pointer);
		}
	}
}

static void find_and_erase(void)
{
	bool rv;

	item_t* item_pointer;
	item_t item;

	item.key1 = 1;
	item.key2 = 11;


	size_t index;

	index = cc_unordered_set_find(&_items.container, &item);
	if (index != cc_invalid_index)
	{
		item_pointer = (item_t*)cc_unordered_set_element(&_items.container, index);
		test_out << "find:" << test_tindex(index) << item_pointer->key1 << "-" << item_pointer->key2 << test_tendl;
		rv = cc_unordered_set_erase(&_items.container, index);
		if (false == rv)
		{
			test_out << "erase failed:" << test_tindex(index) << test_tendl;
			test_assert(0);
		}
		else
		{
			test_out << "erase success:" << test_tindex(index) << item_pointer->key1 << "-" << item_pointer->key2 << test_tendl;
		}
		item_pool_free(item_pointer);
	}
	else
	{
		test_out << "not found" << test_tendl;
		test_assert(0);
	}
}

static void run(void)
{
	add();

	find_and_erase();

	print();
	release();
}

//===========================================================================
void test_case_cc_unordered_set_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
