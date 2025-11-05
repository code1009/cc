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
#define cc_lf_heap_bucket_count  1

//===========================================================================
#define item_max_count 4
#define item_memory_size ( \
	(sizeof(cc_first_fit_block_head_t      )                           ) + \
	(sizeof(cc_lf_heap_bucket_t            ) * cc_lf_heap_bucket_count ) + \
	\
	(sizeof(cc_first_fit_block_head_t      )                           ) + \
	(sizeof(cc_lf_heap_bucket_region_head_t)                           ) + \
	(sizeof(item_t                         ) * item_max_count          ) + \
	\
	(sizeof(cc_first_fit_block_head_t      )                           ) )
/*
# 64bit
@ first_fit_block[0] = cc_first_fit_block_head_t:16 + cc_lf_heap_bucket_t:32*1                             = 48
@ first_fit_block[1] = cc_first_fit_block_head_t:16 + cc_lf_heap_bucket_region_head_t:64 + item_t[4]:256*4 = 16 + 64 + 1024 = 16 + 1088 = 1104
  48 + 1104 = 1152
@ first_fit_block[2] = cc_first_fit_block_head_t:16 = 16(end_block)
  1152 + 16 = 1168 = 16 * 73
*/





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_lf_heap_t heap_memory;
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
		<< "first_fit.min_ever_free_size:" << _item_pool.heap_memory.first_fit.min_ever_free_size << test_tendl
		<< "first_fit.free_size         :" << _item_pool.heap_memory.first_fit.free_size << test_tendl
		<< "first_fit.count             :" << _item_pool.heap_memory.first_fit.count << test_tendl
		<< test_tendl
		;
}

//===========================================================================
static uintptr_t _begin_address = 0;
static uintptr_t _end_address = 0;

//===========================================================================
static bool item_pool_initialize()
{
	test_out
		<< "#item_pool_initialize()" << test_tendl
		;


	bool rv;

	cc_lf_heap_bucket_descriptor_t cc_lf_heap_bucket_descriptor_elements[] = {
		{ sizeof(item_t), item_max_count }
	};
	cc_lf_heap_bucket_descriptors_t cc_lf_heap_bucket_descriptors;
	cc_lf_heap_bucket_descriptors.elements = cc_lf_heap_bucket_descriptor_elements;
	cc_lf_heap_bucket_descriptors.count = cc_lf_heap_bucket_count;


	rv = cc_lf_heap_vallocator_initialize(
		&_item_pool.allocator,
		&_item_pool.heap_memory,
		&_item_pool.memory[0], sizeof(_item_pool.memory),
		&cc_lf_heap_bucket_descriptors
	);
	if (rv == false)
	{
		test_out << "cc_lf_heap_vallocator_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}


	item_pool_dump();

	_begin_address = (uintptr_t)&_item_pool.memory[0];
	_end_address = (uintptr_t)_item_pool.heap_memory.first_fit.end_block;

	test_out
		<< "memory_size  :" << (void*)_item_pool.heap_memory.first_fit.memory_size << "(" << _item_pool.heap_memory.first_fit.memory_size << ")" << test_tendl
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
		<< "cc_lf_heap_count():" << cc_lf_heap_count(&_item_pool.heap_memory) << test_tendl
		;
	test_out
		<< "cc_first_fit_count():" << cc_first_fit_count(&_item_pool.heap_memory.first_fit) << test_tendl
		;

	cc_lf_heap_uninitialize(&_item_pool.heap_memory);
}

static item_t* item_pool_allocate(size_t size)
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.allocate(&_item_pool.heap_memory, size);
	if (item_pointer == NULL)
	{
		test_out << "_item_pool.allocator.allocate() failed" << test_tendl;
		//test_assert(0);
	}

	uintptr_t address = (uintptr_t)item_pointer;
	address -= _begin_address;
	test_out
		<< "-allocate:" << "address=" << address << test_tendl
		;
	item_pool_dump();

	return item_pointer;
}

static void item_pool_free(item_t* item)
{
	bool rv;

	rv = _item_pool.allocator.free(&_item_pool.heap_memory, item);
	if (rv == false)
	{
		test_out << "_item_pool.allocator.free() failed" << test_tendl;
		test_assert(0);
	}

	uintptr_t address = (uintptr_t)item;
	address -= _begin_address;
	test_out
		<< "-free:" << "address=" << address << test_tendl
		;
	item_pool_dump();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static item_t* _p0 = NULL;
static item_t* _p1 = NULL;
static item_t* _p2 = NULL;
static item_t* _p3 = NULL;

static uintptr_t _p0_address = 0;
static uintptr_t _p1_address = 0;
static uintptr_t _p2_address = 0;
static uintptr_t _p3_address = 0;

//===========================================================================
static void allocate(void)
{
	test_out
		<< "@allocate()" << test_tendl
		;

	_p0 = item_pool_allocate(sizeof(item_t) * 1);
	_p1 = item_pool_allocate(sizeof(item_t) * 1);
	_p2 = item_pool_allocate(sizeof(item_t) * 1);
	_p3 = item_pool_allocate(sizeof(item_t) * 1);

	_p0_address = (uintptr_t)_p0;
	_p1_address = (uintptr_t)_p1;
	_p2_address = (uintptr_t)_p2;
	_p3_address = (uintptr_t)_p3;

	_p0_address -= _begin_address;
	_p1_address -= _begin_address;
	_p2_address -= _begin_address;
	_p3_address -= _begin_address;

	
	uintptr_t offset =
		sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_t) * cc_lf_heap_bucket_count) +
		sizeof(cc_first_fit_block_head_t) + (sizeof(cc_lf_heap_bucket_region_head_t))
		;
	_p0_address -= offset;
	_p1_address -= offset;
	_p2_address -= offset;
	_p3_address -= offset;


	test_assert(_p0_address == ( (sizeof(item_t) * 0) ));
	test_assert(_p1_address == ( (sizeof(item_t) * 1) ));
	test_assert(_p2_address == ( (sizeof(item_t) * 2) ));
	test_assert(_p3_address == ( (sizeof(item_t) * 3) ));
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
	if (_p3 != NULL)
	{
		item_pool_free(_p3);
	}
}

//===========================================================================
static void run(void)
{
	allocate();
	release();

	allocate();
	release();
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_lf_heap_1()
{
	if (!item_pool_initialize())
	{
		return;
	}

	run();

	item_pool_uninitialize();
}
