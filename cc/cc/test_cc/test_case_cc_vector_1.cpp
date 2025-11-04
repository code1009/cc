/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	int value;
} item_t;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 32





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static item_t* item_pool_allocate()
{
	return new item_t();
}

static void item_pool_free(item_t* item)
{
	delete item;
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
	test_out
		<< "#items_initialize()" << test_tendl
		;


	cc_vector_initialize(&_items.container, _items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void items_uninitialize()
{
	test_out
		<< "#items_uninitialize()" << test_tendl
		;


	test_out << "cc_vector_count():" << cc_vector_count(&_items.container) << test_tendl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void add(void)
{
	test_out
		<< "@add()" << test_tendl
		;


	bool rv;

	item_t* item_pointer;


	size_t i;
	size_t count;

	count = 512;
	for (i = 0; i < count; i++)
	{
		item_pointer = item_pool_allocate();
		if (item_pointer)
		{
			item_pointer->value = (int)i;
		}
		else
		{
			test_out << "item_pool_allocate() failed:" << test_tindex(i) << test_tendl;
			break;
		}

		rv = cc_vector_add(&_items.container, item_pointer);
		if (false == rv)
		{
			test_out << "cc_vector_add() failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
			test_assert(item_max_count == i);
			break;
		}
	}
}

static void erase(void)
{
	test_out
		<< "@erase()" << test_tendl
		;


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
		test_out << "cc_vector_erase() failed:" << test_tindex(i) << test_tendl;
		test_assert(0);
	}
	else
	{
		test_out << "cc_vector_erase() success:" << test_tindex(i) << item_pointer->value << test_tendl;
		item_pool_free(item_pointer);
	}
}

static void insert(void)
{
	test_out
		<< "@insert()" << test_tendl
		;


	bool rv;

	item_t* item_pointer;


	size_t i;
	i = 5;


	item_pointer = item_pool_allocate();
	if (item_pointer)
	{
		item_pointer->value = 10;
		rv = cc_vector_insert(&_items.container, i, item_pointer);
		if (rv)
		{
			test_out << "cc_vector_insert() success:" << test_tindex(i) << test_tendl;
		}
		else
		{
			test_out << "cc_vector_insert() failed:" << test_tindex(i) << test_tendl;
			item_pool_free(item_pointer);
		}
	}
	else
	{
		test_out << "item_pool_allocate() failed:" << test_tindex(i) << test_tendl;
	}
}

static void print(void)
{
	test_out
		<< "@print()" << test_tendl
		;


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
				<< item_pointer->value
				<< test_tendl
				;
		}
	}
}

static void release(void)
{
	test_out
		<< "@release()" << test_tendl
		;


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
void test_case_cc_vector_1()
{
	if (!items_initialize())
	{
		return;
	}

	run();

	items_uninitialize();
}
