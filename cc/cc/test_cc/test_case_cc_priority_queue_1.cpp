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
cc_api static bool item_less(const void* left, const void* right)
{
	const item_t* litem = (item_t*)left;
	const item_t* ritem = (item_t*)right;


	if (litem->first < ritem->first)
	{
		return true;
	}

	return false;
}




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
	cc_priority_queue_t container;
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

	cc_priority_queue_initialize(&_items.container, item_less, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_priority_queue_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	bool rv;

	int32_t data[item_max_count][2] = { {5, 1}, {3, 2}, {1, 3},  {3, 4}, {5, 5} };
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = item_max_count;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		test_assert(item_pointer != NULL);

		item_pointer->first = data[i][0];
		item_pointer->second = data[i][1];

		rv = cc_priority_queue_push(&_items.container, item_pointer);
		if (rv == false)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}

		test_out << "add:" << test_tindex(i) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;
	}

	rv = cc_priority_queue_push(&_items.container, NULL);
	test_assert(rv == false);
}

static void release(void)
{
	item_t* item_pointer;


	size_t i;
	size_t count;

	count = cc_priority_queue_count(&_items.container);
	for (i = 0; i < count; i++)
	{
		item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
		test_assert(item_pointer != NULL);

		test_out << "release:" << test_tindex(i) << " =" << item_pointer->first << "," << item_pointer->second << test_tendl;

		item_pool_free(item_pointer);
	}
}

static void push_pop1(void)
{
	test_assert(true == cc_priority_queue_empty(&_items.container));


	item_t* d0 = item_pool_allocate();
	item_t* d1 = item_pool_allocate();
	test_assert(d0 != NULL && d1 != NULL);

	d0->first = 20; d1->second = 120;
	d1->first = 10; d0->second = 110;

	test_assert(cc_priority_queue_push(&_items.container, d0) == true);
	test_assert(cc_priority_queue_push(&_items.container, d1) == true);


	item_t* item_pointer;


	test_assert(cc_priority_queue_top_element(&_items.container) == d1);

	item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
	test_assert(item_pointer == d1);
	item_pool_free(item_pointer);


	test_assert(cc_priority_queue_top_element(&_items.container) == d0);

	item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
	test_assert(item_pointer == d0);
	item_pool_free(item_pointer);
}

static void push_pop2(void)
{
	test_assert(true == cc_priority_queue_empty(&_items.container));


	bool rv;
	item_t* a = item_pool_allocate();
	item_t* b = item_pool_allocate();
	item_t* c = item_pool_allocate();
	test_assert(a != NULL && b != NULL && c != NULL);

	a->first = 1; a->second = 11;
	b->first = 2; b->second = 22;
	c->first = 3; c->second = 33;

	rv = cc_priority_queue_push(&_items.container, a);
	test_assert(rv == true);
	rv = cc_priority_queue_push(&_items.container, b);
	test_assert(rv == true);

	
	item_t* item_pointer;

	
	item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
	test_assert(item_pointer == a);
	item_pool_free(item_pointer);

	rv = cc_priority_queue_push(&_items.container, c);
	test_assert(rv == true);

	item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
	test_assert(item_pointer == b);
	item_pool_free(item_pointer);

	item_pointer = (item_t*)cc_priority_queue_pop(&_items.container);
	test_assert(item_pointer == c);
	item_pool_free(item_pointer);

	
	test_assert(0 == cc_priority_queue_count(&_items.container));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();
	release();

	push_pop1();
	push_pop2();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_priority_queue_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
