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
	cc_dnode_t elements[item_max_count];
	cc_list_t container;
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

	rv = cc_list_initialize(&_items.container, _items.elements, item_max_count, sizeof(item_t));
	if (rv == false)
	{
		test_assert(0);
		return false;
	}

	return true;
}

static void items_uninitialize()
{
	test_out << "cc_list_count():" << cc_list_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_assert(true == cc_list_empty(&_items.container));


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

		rv = cc_list_add(&_items.container, item_pointer);
		if (rv == false)
		{
			test_out << "cc_list_add() failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(0);
		}
	}

	rv = cc_list_add(&_items.container, NULL);
	test_assert(rv == false);

	test_assert(item_max_count == cc_list_count(&_items.container));

	test_assert(item_max_count == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void print(void)
{
	test_assert(false == cc_list_empty(&_items.container));


	item_t* item_pointer;

	size_t index = 0;


	cc_dnode_t* node;
	for (node = cc_list_first(&_items.container); node != NULL; node = cc_dnode_next(node))
	{
		item_pointer = (item_t*)cc_list_node_element(&_items.container, node);
		test_assert(item_pointer != NULL);

		test_out << test_tindex(index) << "=" << item_pointer->first << "," << item_pointer->second << test_tendl;

		index++;
	}
}

static void clear(void)
{
	cc_list_clear(&_items.container);


	test_assert(true == cc_list_empty(&_items.container));


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void release(void)
{
	item_t* item_pointer;

	cc_dnode_t* node;
	cc_dnode_t* next;
	node = cc_list_first(&_items.container);
	while (node != NULL)
	{
		next = cc_dnode_next(node);


		item_pointer = (item_t*)cc_list_node_element(&_items.container, node);
		item_pool_free(item_pointer);


		node = next;
	}


	clear();
}

static void erase(void)
{
	test_assert(false == cc_list_empty(&_items.container));


	bool rv;

	item_t* item_pointer;


	cc_dnode_t* node;
	cc_dnode_t* next;
	node = cc_list_first(&_items.container);
	while (node != NULL)
	{
		next = cc_dnode_next(node);

		
		item_pointer = (item_t*)cc_list_node_element(&_items.container, node);
		item_pool_free(item_pointer);

		rv = cc_list_erase(&_items.container, node);
		test_assert(rv == true);


		node = next;
	}


	test_assert(0 == cc_list_count(&_items.container));


	test_assert(0 == cc_simple_segregated_storage_count(&_item_pool.simple_segregated_storage));
}

static void insert_and_order(void)
{
	test_assert(true == cc_list_empty(&_items.container));


	item_t* d1 = item_pool_allocate();
	item_t* d2 = item_pool_allocate();
	item_t* d3 = item_pool_allocate();
	test_assert(d1 != NULL);
	test_assert(d2 != NULL);
	test_assert(d3 != NULL);

	d1->first = 1; d1->second = 11;
	d2->first = 2; d2->second = 21;
	d3->first = 3; d3->second = 33;


	cc_dnode_t* e;
	e = cc_list_add(&_items.container, d1);
	test_assert(cc_list_erase(&_items.container, e)==true);


	cc_dnode_t* e1;
	cc_dnode_t* e2;
	cc_dnode_t* e3;

	e1 = cc_list_add(&_items.container, d1);
	e3 = cc_list_add(&_items.container, d3);
	test_assert(e1 != NULL);
	test_assert(e3 != NULL);

	test_assert(e1->next == e3);
	test_assert(e1->previous == NULL);

	test_assert(e3->next == NULL);
	test_assert(e3->previous == e1);



	cc_dnode_t* e1_iterator;
	//cc_dnode_t* e2_iterator;
	cc_dnode_t* e3_iterator;


	e1_iterator = cc_list_first(&_items.container);
	test_assert(e1_iterator != NULL);

	e3_iterator = cc_dnode_next(e1);
	test_assert(e3_iterator != NULL);

	test_assert(e1 == e1_iterator);
	test_assert(e3 == e3_iterator);


	e2 = cc_list_insert(&_items.container, e3, d2);
	test_assert(e2 != NULL);
	test_assert(e2->next == e3);
	test_assert(e2->previous == e1);

	test_assert(cc_list_count(&_items.container) == 3);


	print();


	item_t* item_pointer;
	int expected = 1;

	cc_dnode_t* node;
	cc_dnode_t* next;
	node = cc_list_first(&_items.container);
	while (node != NULL)
	{
		next = cc_dnode_next(node);


		item_pointer = (item_t*)cc_list_node_element(&_items.container, node);
		test_assert(item_pointer != NULL);
		test_assert(item_pointer->first == expected);
		expected++;


		node = next;
	}


	erase();
}

static void erase_middle(void)
{
	test_assert(true == cc_list_empty(&_items.container));


	bool rv;


	item_t* da = item_pool_allocate();
	item_t* db = item_pool_allocate();
	item_t* dc = item_pool_allocate();
	test_assert(da && db && dc);
	da->first = 0; 
	db->first = 1; 
	dc->first = 2;

	test_assert(cc_list_add(&_items.container, da));
	test_assert(cc_list_add(&_items.container, db));
	test_assert(cc_list_add(&_items.container, dc));
	test_assert(cc_list_count(&_items.container) == 3);

	
	cc_dnode_t* first_node = cc_list_first(&_items.container);
	cc_dnode_t* next_node = cc_dnode_next(first_node);
	test_assert(next_node != NULL);
	void* next = cc_list_node_element(&_items.container, next_node);
	test_assert(next != NULL);


	item_t* item_pointer = (item_t*)next;
	test_assert(item_pointer->first == 1);

	rv = cc_list_erase(&_items.container, next_node);
	test_assert(rv == true);
	item_pool_free(item_pointer);



	cc_dnode_t* node;
	
	node = cc_list_first(&_items.container);
	test_assert(node != NULL);
	item_t* p0 = (item_t*)cc_list_node_element(&_items.container, node);
	test_assert(p0->first == 0);


	node = cc_dnode_next(node);
	test_assert(node != NULL);
	item_t* p2 = (item_t*)cc_list_node_element(&_items.container, node);
	test_assert(p2->first == 2);


	erase();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
	add();
	print();
	release();

	insert_and_order();

	erase_middle();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_list_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
