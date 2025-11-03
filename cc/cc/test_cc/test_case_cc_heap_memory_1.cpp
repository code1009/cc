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
#define item_memory_size ( \
	(sizeof(cc_first_fit_storage_block_head_t)                  ) + \
	(sizeof(cc_heap_bucket_t)                  * 1              ) + \
	(sizeof(cc_first_fit_storage_block_head_t)                  ) + \
	(sizeof(cc_heap_bucket_storage_t)          * 1              ) + \
	(sizeof(item_t)                            * item_max_count ) + \
	(sizeof(cc_first_fit_storage_block_head_t)                  ) + 8)

// cc_first_fit_storage_block_head_t:16 + cc_heap_bucket_t:24 = 40
// cc_first_fit_storage_block_head_t:16 + cc_heap_bucket_storage_t:64 + item_t:256*4 = 16 + 64 + 1024 = 16 + 1088 = 1104
// 40 + 1104 = 1144
// 1144 + 16(end) = 1160 = 16 * 72.5
// 1160 + 8(padding) = 1168 = 16 * 73
// cc_first_fit_storage_block_head_t의 크기는 16바이트 단위로 정렬됨


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_pool_t
{
	cc_heap_memory_t storage;
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
		<< "storage min ever free size:" << _item_pool.storage.storage.min_ever_free_size << test_tendl
		<< "storage          free size:" << _item_pool.storage.storage.free_size << test_tendl
		<< "storage              count:" << _item_pool.storage.storage.count << test_tendl
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

	cc_heap_bucket_config_t buckets[] = {
		{ sizeof(item_t), item_max_count }
	};
	cc_heap_memory_config_t config;
	config.buckets = buckets;
	config.bucket_count = 1;


	rv = cc_heap_memory_initialize(
		&_item_pool.storage,
		&_item_pool.memory[0], sizeof(_item_pool.memory),
		&config
	);
	if (rv == false)
	{
		test_out << "cc_heap_memory_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}
	_item_pool.allocator.handle = &_item_pool.storage;
	_item_pool.allocator.alloc = (cc_valloc_t)cc_heap_memory_allocate;
	_item_pool.allocator.free = (cc_vfree_t)cc_heap_memory_free;


	item_pool_dump();

	_begin_address = (uintptr_t)&_item_pool.memory[0];
	_end_address = (uintptr_t)_item_pool.storage.storage.end_block;

	test_out
		<< "@ item pool initialized:" << test_tendl
		<< " memory_size:   " << (void*)_item_pool.storage.storage.memory_size << "(" << _item_pool.storage.storage.memory_size << ")" << test_tendl
		<< " begin address: " << (void*)_begin_address << test_tendl
		<< " end address:   " << (void*)_end_address << test_tendl
		<< " end-begin:     " << _end_address - _begin_address << test_tendl
		<< test_tendl
		;


	return true;
}

static void item_pool_uninitialize()
{
	test_out
		<< "@ item pool uninitialized:" << test_tendl
		;

	item_pool_dump();

	cc_heap_memory_uninitialize(&_item_pool.storage);
}

static item_t* item_pool_alloc(size_t size)
{
	item_t* item_pointer = (item_t*)_item_pool.allocator.alloc(&_item_pool.storage, size);
	if (item_pointer == NULL)
	{
		test_out << "_item_pool.allocator.alloc() failed" << test_tendl;
		//test_assert(0);
	}

	uintptr_t address = (uintptr_t)item_pointer;
	address -= _begin_address;
	test_out
		<< "-alloc:" << "address=" << address << test_tendl
		;
	item_pool_dump();

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
static void alloc(void)
{
	test_out
		<< "@ alloc:" << test_tendl
		;

	_p0 = item_pool_alloc(sizeof(item_t)*1);
	_p1 = item_pool_alloc(sizeof(item_t)*1);
	_p2 = item_pool_alloc(sizeof(item_t)*1);

	_p0_address = (uintptr_t)_p0;
	_p1_address = (uintptr_t)_p1;
	_p2_address = (uintptr_t)_p2;

	_p0_address -= _begin_address;
	_p1_address -= _begin_address;
	_p2_address -= _begin_address;

	/*
	test_assert(_p0_address == ( (sizeof(cc_heap_memory_block_head_t) * 1) + (sizeof(item_t) * 0) ));
	test_assert(_p1_address == ( (sizeof(cc_heap_memory_block_head_t) * 2) + (sizeof(item_t) * 1) ));
	test_assert(_p2_address == ( (sizeof(cc_heap_memory_block_head_t) * 3) + (sizeof(item_t) * 3) ));
	*/
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
	alloc();
	release();

	alloc();
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
