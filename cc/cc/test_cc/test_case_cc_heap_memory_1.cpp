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
#define cc_heap_bucket_count  1

/*
static inline size_t cc_heap_bucket_padding_size(size_t bucket_count)
{
	uintptr_t aligment_size = sizeof(cc_first_fit_block_head_t);
	uintptr_t source_size;
	uintptr_t padding_size;
	source_size = sizeof(cc_heap_bucket_t) * bucket_count;
	if ((source_size % aligment_size)
	{
		padding_size = aligment_size - (source_size % aligment_size);
	}
	else
	{
		padding_size = 0;
	}

	return padding_size;
}
*/

#define cc_heap_bucket_padding_size(bucket_count) \
	(                                                             \
		(                                                         \
			(                                                     \
				(                                                 \
					(sizeof(cc_heap_bucket_t) * (bucket_count)) + \
					sizeof(cc_first_fit_block_head_t) - 1         \
				)                                                 \
				/ sizeof(cc_first_fit_block_head_t)               \
			)                                                     \
			* sizeof(cc_first_fit_block_head_t)                   \
		)                                                         \
		- (sizeof(cc_heap_bucket_t) * (bucket_count))             \
	)

//===========================================================================
#define item_max_count 4
#define item_memory_size ( \
	(sizeof(cc_first_fit_block_head_t                )                        ) + \
	(sizeof(cc_heap_bucket_t                         ) * cc_heap_bucket_count ) + \
	(cc_heap_bucket_padding_size(cc_heap_bucket_count)                        ) + \
	\
	(sizeof(cc_first_fit_block_head_t                )                        ) + \
	(sizeof(cc_heap_bucket_region_head_t             )                        ) + \
	(sizeof(item_t                                   ) * item_max_count       ) + \
	(sizeof(cc_first_fit_block_head_t                )                        ) )

// 64bit
// cc_first_fit_block_head_t:16 + cc_heap_bucket_t:24 + cc_heap_bucket_padding_size:8 = 48
// cc_first_fit_block_head_t:16 + cc_heap_bucket_region_head_t:64 + item_t:256*4 = 16 + 64 + 1024 = 16 + 1088 = 1104
// 48 + 1104 = 1152
// 1152 + 16(end_block) = 1168 = 16 * 73





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_heap_memory_t heap_memory;
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
	bool rv;

	cc_heap_bucket_descriptor_t cc_heap_bucket_descriptors[] = {
		{ sizeof(item_t), item_max_count }
	};
	cc_heap_memory_config_t config;
	config.elements = cc_heap_bucket_descriptors;
	config.count = cc_heap_bucket_count;


	rv = cc_heap_memory_vallocator_initialize(
		&_item_pool.allocator,
		&_item_pool.heap_memory,
		&_item_pool.memory[0], sizeof(_item_pool.memory),
		&config
	);
	if (rv == false)
	{
		test_out << "cc_heap_memory_vallocator_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}


	item_pool_dump();

	_begin_address = (uintptr_t)&_item_pool.memory[0];
	_end_address = (uintptr_t)_item_pool.heap_memory.first_fit.end_block;

	test_out
		<< "#item_pool_initialize()" << test_tendl
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
		<< "cc_heap_memory_count():" << cc_heap_memory_count(&_item_pool.heap_memory) << test_tendl
		;
	test_out
		<< "cc_first_fit_count():" << cc_first_fit_count(&_item_pool.heap_memory.first_fit) << test_tendl
		;

	cc_heap_memory_uninitialize(&_item_pool.heap_memory);
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

static uintptr_t _p0_address = 0;
static uintptr_t _p1_address = 0;
static uintptr_t _p2_address = 0;

//===========================================================================
static void allocate(void)
{
	test_out
		<< "@ allocate:" << test_tendl
		;

	_p0 = item_pool_allocate(sizeof(item_t)*1);
	_p1 = item_pool_allocate(sizeof(item_t)*1);
	_p2 = item_pool_allocate(sizeof(item_t)*1);

	_p0_address = (uintptr_t)_p0;
	_p1_address = (uintptr_t)_p1;
	_p2_address = (uintptr_t)_p2;

	_p0_address -= _begin_address;
	_p1_address -= _begin_address;
	_p2_address -= _begin_address;

	
	uintptr_t padding_size;
	padding_size = cc_heap_bucket_padding_size(cc_heap_bucket_count);

	uintptr_t offset =
		sizeof(cc_first_fit_block_head_t) + sizeof(cc_heap_bucket_t) + padding_size +
		sizeof(cc_first_fit_block_head_t) + sizeof(cc_heap_bucket_region_head_t)
		;
	_p0_address -= offset;
	_p1_address -= offset;
	_p2_address -= offset;


	test_assert(_p0_address == ( (sizeof(item_t) * 0) ));
	test_assert(_p1_address == ( (sizeof(item_t) * 1) ));
	test_assert(_p2_address == ( (sizeof(item_t) * 2) ));
}

static void release(void)
{
	test_out
		<< "@ release:" << test_tendl
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
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
void test_case_cc_heap_memory_1()
{
	if (!item_pool_initialize())
	{
		return;
	}

	run();

	item_pool_uninitialize();
}
