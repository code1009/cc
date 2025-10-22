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
#define item_max_count 32





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_simple_segregated_storage_t storage;
	item_t memory[item_max_count];
	cc_allocator_t allocator;
}
item_pool_t;

//===========================================================================
static item_pool_t _item_pool;

//===========================================================================
static bool item_pool_initialize()
{
	bool rv;
	rv = cc_simple_segregated_storage_allocator_initialize(
		&_item_pool.allocator,
		&_item_pool.storage, &_item_pool.memory[0], sizeof(_item_pool.memory), sizeof(item_t), item_max_count
	);
	if (rv == false)
	{
		test_out << "cc_simple_segregated_storage_allocator_initialize() failed" << test_tendl;
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
	cc_vector_t container;
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

	cc_vector_initialize(&_items.container, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_vector_count(&_items.container) << test_tendl;

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
			item_pointer->first = (int)i;
			item_pointer->second = 10+(int)i;
		}
		else
		{
			test_out << "item_pool_alloc() failed:" << test_tindex(i) << test_tendl;
			break;
		}

		rv = cc_vector_add(&_items.container, item_pointer);
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


	size_t i;
	size_t count;

	count = cc_vector_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_vector_element(&_items.container, i);

		if (item_pointer != NULL)
		{
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
}

static void release(void)
{
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_vector_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_vector_element(&_items.container, i);
		if (item_pointer != NULL)
		{
			item_pool_free(item_pointer);
		}
	}

	cc_vector_clear(&_items.container);
}

static void erase(void)
{
	bool rv;

	item_t* item_pointer;

	cc_element_t* element_pointer;


	size_t i;
	i = 9;


	element_pointer = cc_vector_at(&_items.container, i);
	test_assert(element_pointer != NULL);

	item_pointer = (item_t*)element_pointer->pointer;
	test_assert(item_pointer != NULL);


	rv = cc_vector_erase(&_items.container, i);
	if (false == rv)
	{
		test_out << "erase failed:" << test_tindex(i) << test_tendl;
		test_assert(0);
	}
	else
	{
		test_out << "erase success:" << test_tindex(i) << item_pointer->first << test_tendl;
		item_pool_free(item_pointer);
	}
}

static void insert(void)
{
	bool rv;

	item_t* item_pointer;


	size_t i;
	i = 5;


	item_pointer = item_pool_alloc();
	if (item_pointer)
	{
		item_pointer->first = 99;
		item_pointer->second = 99;
		rv = cc_vector_insert(&_items.container, i, item_pointer);
		if(rv)
		{
			test_out << "insert success:" << test_tindex(i) << test_tendl;
		}
		else
		{
			test_out << "insert failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
		}
	}
	else
	{
		test_out << "item_pool_alloc() failed:" << test_tindex(i) << test_tendl;
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();

	erase();
	insert();

	print();
	release();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_vector_2()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
