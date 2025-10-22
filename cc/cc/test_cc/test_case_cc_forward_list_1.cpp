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
	cc_snode_t elements[item_max_count];
	cc_forward_list_t container;
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

	rv = cc_forward_list_initialize(&_items.container, _items.elements, item_max_count, sizeof(item_t));
	if (rv == false)
	{
		test_assert(0);
		return false;
	}

	return true;
}

static void items_uninitialize()
{
	test_out << "elements count:" << cc_forward_list_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_assert(true == cc_forward_list_empty(&_items.container));


	bool rv;

	item_t* item_pointer;
	cc_snode_t* previous_node = NULL;
	cc_snode_t* current_node = NULL;


	size_t i;
	size_t count;

	count = item_max_count;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_alloc();
		test_assert(item_pointer != NULL);

		item_pointer->first = (int)i;
		item_pointer->second = 100 + (int)i;

		current_node = cc_forward_list_insert_after(&_items.container, previous_node, item_pointer);
		if (current_node == NULL)
		{
			test_out << "add failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}

		previous_node = current_node;
	}


	rv = cc_forward_list_insert_after(&_items.container, previous_node, NULL);
	test_assert(rv == false);

	test_assert(item_max_count == cc_forward_list_count(&_items.container));

	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.storage));
}

static void print(void)
{
	test_assert(false == cc_forward_list_empty(&_items.container));


	item_t* item_pointer;

	size_t index = 0;


	cc_snode_t* node;
	for (node = cc_forward_list_first(&_items.container); node != NULL; node = cc_snode_next(node))
	{
		item_pointer = (item_t*)cc_forward_list_node_element(&_items.container, node);
		test_assert(item_pointer != NULL);

		test_out << test_tindex(index) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;

		index++;
	}
}

static void clear(void)
{
	cc_forward_list_clear(&_items.container);


	test_assert(true == cc_forward_list_empty(&_items.container));


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.storage));
}

static void release(void)
{
	test_assert(false == cc_forward_list_empty(&_items.container));


	item_t* item_pointer;

	cc_snode_t* node;
	cc_snode_t* next;
	node = cc_forward_list_first(&_items.container);
	while (node != NULL)
	{
		next = cc_snode_next(node);


		item_pointer = (item_t*)cc_forward_list_node_element(&_items.container, node);
		item_pool_free(item_pointer);


		node = next;
	}


	clear();
}

static void erase(void)
{
	test_assert(false == cc_forward_list_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	cc_snode_t* first;
	cc_snode_t* node;
	

	first = cc_forward_list_first(&_items.container);
	while ((first != NULL) && cc_snode_next(first) != NULL)
	{
		node = cc_snode_next(first);


		item_pointer = (item_t*)cc_forward_list_node_element(&_items.container, node);
		cc_debug_assert(item_pointer != NULL);
		item_pool_free(item_pointer);


		rv = cc_forward_list_erase_after(&_items.container, first);
		test_assert(rv == true);
	}


	while (!cc_forward_list_empty(&_items.container))
	{
		item_pointer = (item_t*)cc_forward_list_pop_front(&_items.container);
		test_assert(item_pointer != NULL);
		item_pool_free(item_pointer);
	}


	test_assert(0 == cc_forward_list_count(&_items.container));


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.storage));
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();
	print();
	release();

	add();
	erase();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_forward_list_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
