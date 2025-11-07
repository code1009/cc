/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	size_t next_free_index;

	int value;
} item_t;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 5





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	item_t memory[item_max_count];
	size_t free_index_head;
	size_t max_count;
	size_t count;
}
item_pool_t;

//===========================================================================
static item_pool_t _item_pool;

//===========================================================================
static const size_t item_null_index = (size_t)-1;

static const size_t item_allocated_bit_number = (sizeof(size_t)*8-1);
static const size_t item_allocated_bit_mask = ((size_t)1 << item_allocated_bit_number);

//===========================================================================
static void item_set_allocated(item_t* ctx, size_t my_index)
{
	ctx->next_free_index = my_index | item_allocated_bit_mask;
}

static bool item_is_allocated(item_t* ctx)
{
	return (0 != (ctx->next_free_index & item_allocated_bit_mask));
}

static size_t item_allocated_index(item_t* ctx)
{
	if (!item_is_allocated(ctx))
	{
		return item_null_index;
	}

	return ctx->next_free_index & (~item_allocated_bit_mask);
}

static size_t item_get_next_free_index(item_t* ctx)
{
	if (item_is_allocated(ctx))
	{
		return item_null_index;
	}

	return ctx->next_free_index;
}

//===========================================================================
static void item_pool_dump(void)
{
	test_out
		<< "# item_pool_dump()" << test_tendl
		;

	if (_item_pool.free_index_head == item_null_index)
	{
		test_out << "- free_index_head = null" << test_tendl;
	}
	else
	{
		test_out << "- free_index_head = " << _item_pool.free_index_head << test_tendl;
	}
	test_out << "- max_count  = " << _item_pool.max_count << test_tendl;
	test_out << "- count      = " << _item_pool.count << test_tendl;

	
	size_t free_index = _item_pool.free_index_head;
	size_t free_count = 0;
	while (free_index != item_null_index)
	{
		item_t* pointer = &_item_pool.memory[free_index];

		size_t next_free_index = item_get_next_free_index(pointer);

		if (next_free_index == item_null_index)
		{
			test_out
				<< "- [" << free_count
				<< "] free_index:" << free_index
				<< " -> next_free_index: null"
				<< test_tendl
				;
		}
		else
		{
			test_out
				<< "- [" << free_count
				<< "] free_index:" << free_index
				<< " -> next_free_index:" << next_free_index
				<< test_tendl
				;
		}


		free_count++;


		free_index = next_free_index;
	}

	test_out << "- free_count = " << free_count << test_tendl;
	test_out << test_tendl;
}

//===========================================================================
static bool item_pool_initialize()
{
	size_t index;
	size_t count = item_max_count -1;


	for (index = 0; index < count; index++)
	{
		_item_pool.memory[index].next_free_index = (int)(index + 1);
	}
	_item_pool.memory[index].next_free_index = item_null_index;
	
	
	_item_pool.max_count = item_max_count;
	
	_item_pool.count = 0;

	return true;
}

static void item_pool_uninitialize()
{

}

static item_t* item_pool_allocate()
{
	//-----------------------------------------------------------------------
	if (item_null_index == _item_pool.free_index_head)
	{
		return NULL;
	}


	//-----------------------------------------------------------------------
	item_t* pointer;
	size_t pointer_index = _item_pool.free_index_head;
	pointer = &_item_pool.memory[pointer_index];


	_item_pool.free_index_head = _item_pool.memory[pointer_index].next_free_index;


	item_set_allocated(pointer, pointer_index);
	
	
	//-----------------------------------------------------------------------
	_item_pool.count++;

	
	//-----------------------------------------------------------------------
	item_pool_dump();


	return pointer;
}

static void item_pool_free(item_t* pointer)
{
	//-----------------------------------------------------------------------
	test_assert(pointer != NULL);
	test_assert(item_is_allocated(pointer) == true);


	//-----------------------------------------------------------------------
	size_t free_index = _item_pool.free_index_head;


	_item_pool.free_index_head = item_allocated_index(pointer);
	_item_pool.memory[_item_pool.free_index_head].next_free_index = free_index;


	//-----------------------------------------------------------------------
	_item_pool.count--;


	//-----------------------------------------------------------------------
	item_pool_dump();
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
	test_out
		<< "#items_uninitialize()" << test_tendl
		;


	test_out << "cc_vector_count():" << cc_vector_count(&_items.container) << test_tendl;

	item_pool_uninitialize();
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
	i = 2;


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
	i = 3;


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
