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
#define item_max_count 5





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
	test_out << "cc_simple_segregated_storage_count():" << cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage) << test_tendl;
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
	cc_element_t elements[item_max_count];
	cc_deque_t container;
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

	cc_deque_initialize(&_items.container, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_deque_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_assert(true==cc_deque_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = item_max_count;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		test_assert(item_pointer != NULL);

		item_pointer->first = (int)i;
		item_pointer->second = 100 + (int)i;

		rv = cc_deque_push_back(&_items.container, item_pointer);
		if (rv == false)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}
	}


	rv = cc_deque_push_back(&_items.container, NULL);
	test_assert(rv == false);

	test_assert(item_max_count == cc_deque_count(&_items.container));

	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void release(void)
{
	test_assert(false == cc_deque_empty(&_items.container));


	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_deque_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_deque_pop_front(&_items.container);
		test_assert(item_pointer != NULL);

		test_out << "release:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;

		item_pool_free(item_pointer);
	}


	test_assert(true == cc_deque_empty(&_items.container));
	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void push_back(void)
{
	test_assert(true == cc_deque_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = item_max_count;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		test_assert(item_pointer != NULL);

		item_pointer->first = (int)i;
		item_pointer->second = 100 + (int)i;

		rv = cc_deque_push_back(&_items.container, item_pointer);
		if (rv == false)
		{
			test_out << "push_back failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}

		test_out << "push_back:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;
	}


	rv = cc_deque_push_back(&_items.container, NULL);
	test_assert(rv == false);


	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void push_front(void)
{
	test_assert(true == cc_deque_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = item_max_count;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		test_assert(item_pointer != NULL);

		item_pointer->first = (int)i;
		item_pointer->second = 100 + (int)i;

		rv = cc_deque_push_front(&_items.container, item_pointer);
		if (rv == false)
		{
			test_out << "push_front failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}

		test_out << "push_front:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;
	}


	rv = cc_deque_push_back(&_items.container, NULL);
	test_assert(rv == false);


	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void pop_front(void)
{
	test_assert(false == cc_deque_empty(&_items.container));


	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_deque_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_deque_pop_front(&_items.container);
		test_assert(item_pointer != NULL);

		test_out << "pop_front:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;

		item_pool_free(item_pointer);
	}


	test_assert(true == cc_deque_empty(&_items.container));
	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void pop_back(void)
{
	test_assert(false == cc_deque_empty(&_items.container));


	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_deque_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_deque_pop_back(&_items.container);
		test_assert(item_pointer != NULL);

		test_out << "pop_back:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;

		item_pool_free(item_pointer);
	}


	test_assert(true == cc_deque_empty(&_items.container));
	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void push_pop(void)
{
	test_assert(true == cc_deque_empty(&_items.container));


	bool rv;

	item_t* d0;
	item_t* d1;


	d0 = item_pool_allocate();
	d1 = item_pool_allocate();
	test_assert(d0 != NULL && d1 != NULL);

	d0->first = 1; d0->second = 11;
	d1->first = 2; d1->second = 22;

	rv = cc_deque_push_back(&_items.container, d0);
	test_assert(rv == true);

	rv = cc_deque_push_front(&_items.container, d1);
	test_assert(rv == true);


	item_t* item_pointer;


	item_pointer = (item_t*)cc_deque_pop_front(&_items.container);
	test_assert(item_pointer == d1);
	item_pool_free(item_pointer);

	item_pointer = (item_t*)cc_deque_pop_back(&_items.container);
	test_assert(item_pointer == d0);
	item_pool_free(item_pointer);


	test_assert(true == cc_deque_empty(&_items.container));
	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();
	release();

	push_back();
	pop_front();

	push_front();
	pop_back();

	push_pop();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_deque_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
