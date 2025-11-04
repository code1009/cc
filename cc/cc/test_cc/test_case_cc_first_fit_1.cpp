/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	uint8_t data[256];
} item_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 4
#define item_allocation_count 3
#define item_memory_size ( \
	(sizeof(item_t                   ) * item_max_count       ) + \
	(sizeof(cc_first_fit_block_head_t)                        ) + \
	(sizeof(cc_first_fit_block_head_t) * item_allocation_count) )





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_first_fit_t first_fit;
	uint8_t memory[item_memory_size];
	cc_vallocator_t allocator;
}
item_pool_t;

//===========================================================================
static item_pool_t _item_pool;

//===========================================================================
static void item_pool_dump(void)
{
	test_out
		<< "first_fit.min_ever_free_size:" << _item_pool.first_fit.min_ever_free_size << test_tendl
		<< "first_fit.free_size         :" << _item_pool.first_fit.free_size          << test_tendl
		<< "first_fit.count             :" << _item_pool.first_fit.count              << test_tendl
		<< test_tendl
		;
}

//===========================================================================
static uintptr_t _begin_address = 0;
static uintptr_t _end_address = 0;

//===========================================================================
static bool item_pool_initialize()
{
	bool rv;
	rv = cc_first_fit_vallocator_initialize(
		&_item_pool.allocator,
		&_item_pool.first_fit, &_item_pool.memory[0], sizeof(_item_pool.memory)
	);
	if (rv == false)
	{
		test_out << "cc_first_fit_vallocator_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}

	item_pool_dump();

	_begin_address = (uintptr_t)&_item_pool.memory[0];
	_end_address = (uintptr_t)_item_pool.first_fit.end_block;

	test_out
		<< "#item_pool_initialize()" << test_tendl
		<< "memory_size  :" << (void*)_item_pool.first_fit.memory_size << "(" << _item_pool.first_fit.memory_size << ")" << test_tendl
		<< "begin_address:" << (void*)_begin_address << test_tendl
		<< "end_address  :" << (void*)_end_address << test_tendl
		<< "end-begin    :" << _end_address - _begin_address << test_tendl
		<< test_tendl
		;


	return true;
}

static void item_pool_uninitialize()
{
	test_out
		<< "#item_pool_uninitialize()" << test_tendl
		;

	item_pool_dump();

	test_out
		<< "cc_first_fit_count():" << cc_first_fit_count(&_item_pool.first_fit) << test_tendl
		;
}

static item_t* item_pool_allocate(size_t size)
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.allocate(&_item_pool.first_fit, size);
	if (item_pointer == NULL)
	{
		test_out << "_item_pool.allocator.allocate() failed" << test_tendl;
		//test_assert(0);
	}

	uintptr_t address = (uintptr_t)item_pointer;
	address -= _begin_address;
	test_out
		<< "-item_pool_allocate():" << "address=" << address << test_tendl
		;
	item_pool_dump();

	return item_pointer;
}

static void item_pool_free(item_t* item)
{
	bool rv;

	rv = _item_pool.allocator.free(&_item_pool.first_fit, item);
	if (rv == false)
	{
		test_out << "_item_pool.allocator.free() failed" << test_tendl;
		test_assert(0);
	}

	uintptr_t address = (uintptr_t)item;
	address -= _begin_address;
	test_out
		<< "-item_pool_free():" << "address=" << address << test_tendl
		;
	item_pool_dump();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static item_t* _p0 = NULL;
static item_t* _p1 = NULL;
static item_t* _p2 = NULL;

static uintptr_t _p0_address = 0;
static uintptr_t _p1_address = 0;
static uintptr_t _p2_address = 0;

//===========================================================================
static void allocate(void)
{
	test_out
		<< "@allocate()" << test_tendl
		;

	_p0 = item_pool_allocate(sizeof(item_t) * 1);
	_p1 = item_pool_allocate(sizeof(item_t) * 2);
	_p2 = item_pool_allocate(sizeof(item_t) * 1);

	_p0_address = (uintptr_t)_p0;
	_p1_address = (uintptr_t)_p1;
	_p2_address = (uintptr_t)_p2;

	_p0_address -= _begin_address;
	_p1_address -= _begin_address;
	_p2_address -= _begin_address;

	test_assert(_p0_address == ((sizeof(cc_first_fit_block_head_t) * 1) + (sizeof(item_t) * 0)));
	test_assert(_p1_address == ((sizeof(cc_first_fit_block_head_t) * 2) + (sizeof(item_t) * 1)));
	test_assert(_p2_address == ((sizeof(cc_first_fit_block_head_t) * 3) + (sizeof(item_t) * 3)));
}

static void allocate1(void)
{
	test_out
		<< "@allocate1()" << test_tendl
		;

	_p0 = item_pool_allocate(sizeof(item_t) * 1);
	_p1 = item_pool_allocate(sizeof(item_t) * 2);
	_p2 = item_pool_allocate(sizeof(item_t) * 1);

	_p0_address = (uintptr_t)_p0;
	_p1_address = (uintptr_t)_p1;
	_p2_address = (uintptr_t)_p2;

	_p0_address -= _begin_address;
	_p1_address -= _begin_address;
	_p2_address -= _begin_address;

	test_assert(_p0_address == ((sizeof(cc_first_fit_block_head_t) * 1) + (sizeof(item_t) * 0)));
	test_assert(_p1_address == ((sizeof(cc_first_fit_block_head_t) * 2) + (sizeof(item_t) * 1)));
	test_assert(_p2_address == ((sizeof(cc_first_fit_block_head_t) * 3) + (sizeof(item_t) * 3)));


	item_pool_free(_p1);


	_p1 = item_pool_allocate(sizeof(item_t) * 1);
	_p1_address = (uintptr_t)_p1;
	_p1_address -= _begin_address;
	test_assert(_p1_address == ((sizeof(cc_first_fit_block_head_t) * 2) + (sizeof(item_t) * 1)));


	void* _p3;
	uintptr_t _p3_address;

	_p3 = item_pool_allocate(sizeof(item_t) - sizeof(cc_first_fit_block_head_t));
	test_assert(_p3 != NULL);

	_p3_address = (uintptr_t)_p3;
	_p3_address -= _begin_address;
	test_assert(_p3_address == ((sizeof(cc_first_fit_block_head_t) * 3) + (sizeof(item_t) * 2)));

	item_pool_free((item_t*)_p3);
}

static void release(void)
{
	test_out
		<< "@release()" << test_tendl
		;

	if (_p0 != NULL)
	{
		item_pool_free(_p0);
	}
	if (_p1 != NULL)
	{
		item_pool_free(_p1);
	}
	if (_p2 != NULL)
	{
		item_pool_free(_p2);
	}
}

//===========================================================================
static void run(void)
{
	allocate();
	release();

	allocate();
	release();

	allocate1();
	release();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_first_fit_1()
{
	if (!item_pool_initialize())
	{
		return;
	}

	run();

	item_pool_uninitialize();
}
