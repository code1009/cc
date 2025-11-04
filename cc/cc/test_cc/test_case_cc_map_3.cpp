/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "test_cc.hpp"

#include <ostream>
#include <string>

#include <chrono>
#include <iostream>
#include <iomanip>

#include <random>
#include <sstream>

#include <vector>
#include <map>

#include "../tester/test_timer.hpp"






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _item_t
{
	uint32_t key;
	uint32_t value;
} item_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define item_max_count 65536





/////////////////////////////////////////////////////////////////////////////
// 
// source
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static uint32_t random_range_uint32_t(uint32_t min_inclusive, uint32_t max_inclusive)
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<uint32_t> dist(min_inclusive, max_inclusive);
    return dist(mt);
}

static std::vector<item_t> generate_source_items(size_t count)
{
    if (count == 0)
    {
        return {};
    }

    if (count > item_max_count)
    {
        count = item_max_count;
    }


    std::vector<uint32_t> keys;
    keys.reserve(item_max_count);

    uint32_t key = 0;
    for (uint32_t i = 0; i < item_max_count; ++i)
    {
        key = random_range_uint32_t(key+1, key + 9);
        keys.push_back(key);
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(keys.begin(), keys.end(), mt);


    std::vector<item_t> items;
    items.reserve(count);
    for (size_t i = 0; i < count; ++i)
    {
        item_t item;

        item.key = keys[i];
        item.value = static_cast<uint32_t>(i);

        items.push_back(item);
    }

    return items;
}





/////////////////////////////////////////////////////////////////////////////
// 
// cc
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api static inline uint32_t cc_item_key(const item_t* item_pointer)
{
	return item_pointer->key;
}

cc_api static bool cc_item_key_equal(const void* left, const void* right)
{
	uint32_t lkey = (uint32_t)((uintptr_t)left);
	uint32_t rkey = (uint32_t)((uintptr_t)right);


	if (lkey == rkey)
	{
		return true;
	}

	return false;
}

cc_api static bool cc_item_key_less(const void* left, const void* right)
{
    uint32_t lkey = (uint32_t)((uintptr_t)left);
    uint32_t rkey = (uint32_t)((uintptr_t)right);


    return (lkey < rkey) ? true : false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_item_pool_t
{
	cc_simple_segregated_storage_t simple_segregated_storage;
	item_t memory[item_max_count];
	cc_fallocator_t allocator;
}
cc_item_pool_t;

//===========================================================================
static cc_item_pool_t _cc_item_pool;

//===========================================================================
static bool cc_item_pool_initialize()
{
	bool rv;
	rv = cc_simple_segregated_storage_fallocator_initialize(
		&_cc_item_pool.allocator,
		&_cc_item_pool.simple_segregated_storage, &_cc_item_pool.memory[0], sizeof(_cc_item_pool.memory), sizeof(item_t), item_max_count
	);
	if (rv == false)
	{
		test_out << "cc_simple_segregated_storage_fallocator_initialize() failed" << test_tendl;
		test_assert(0);
		return false;
	}
	return true;
}

static void cc_item_pool_uninitialize()
{
    test_out << "cc_simple_segregated_storage_count():" << cc_simple_segregated_storage_count(&_cc_item_pool.simple_segregated_storage) << test_tendl;
}

static item_t* cc_item_pool_allocate()
{
	item_t* item_pointer = (item_t*)_cc_item_pool.allocator.allocate(&_cc_item_pool.simple_segregated_storage);
	if (item_pointer == NULL)
	{
		test_out << "_cc_item_pool.allocator.allocate() failed" << test_tendl;
		//test_assert(0);
	}
	return item_pointer;
}

static void cc_item_pool_free(item_t* item)
{
	bool rv;
	
	rv = _cc_item_pool.allocator.free(&_cc_item_pool.simple_segregated_storage, item);
	if (rv == false)
	{
		test_out << "_cc_item_pool.allocator.free() failed" << test_tendl;
		test_assert(0);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_items_t
{
	cc_pair_t elements[item_max_count];
	cc_map_t container;
}
cc_items_t;

//===========================================================================
static cc_items_t _cc_items;

//===========================================================================
static bool cc_items_initialize()
{
	bool rv;

	rv = cc_item_pool_initialize();
	if (rv == false)
	{
		test_assert(0);
		return false;
	}

	cc_map_initialize(
		&_cc_items.container,
        cc_item_key_equal, cc_item_key_less, _cc_items.elements, item_max_count, sizeof(item_t));

	return true;
}

static void cc_items_uninitialize()
{
	test_out << "cc_map_count():" << cc_map_count(&_cc_items.container) << test_tendl;

	cc_item_pool_uninitialize();
}





/////////////////////////////////////////////////////////////////////////////
// 
// STL
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static std::map<uint32_t, item_t*> _stl_items;




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void cc_add(std::vector<item_t>& source_items)
{
	size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
        item_t* item_pointer = cc_item_pool_allocate();
        if (item_pointer == NULL)
        {
            continue;
        }
        item_pointer->key = source_items[i].key;
		item_pointer->value = source_items[i].value;

        bool rv;
        rv = cc_map_add(&_cc_items.container, (void*)(uintptr_t)cc_item_key(item_pointer), item_pointer);
        if (rv == false)
        {
            cc_item_pool_free(item_pointer);
        }
	}
}

static void stl_add(std::vector<item_t>& source_items)
{
    size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
		item_t* item_pointer = new(std::nothrow) item_t();
        if (item_pointer == NULL)
        {
            continue;
        }
        item_pointer->key = source_items[i].key;
        item_pointer->value = source_items[i].value;

        _stl_items.insert(std::make_pair(cc_item_key(item_pointer), item_pointer));
    }
}

//===========================================================================
static void cc_clear()
{
	size_t count = cc_map_count(&_cc_items.container);
    for (size_t i = 0; i < count; i++)
    {
        item_t* item_pointer = (item_t*)cc_map_element_second(&_cc_items.container, i);
        if (item_pointer != NULL)
        {
            cc_item_pool_free(item_pointer);
        }
	}

    cc_map_clear(&_cc_items.container);
}

static void stl_clear()
{
    for (auto& pair : _stl_items)
    {
        delete pair.second;
    }
    _stl_items.clear();
}

//===========================================================================
static void cc_find(std::vector<item_t>& source_items)
{
    size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
        uint32_t key = source_items[i].key;
        uint32_t value = source_items[i].value;

        item_t* item_pointer = (item_t*)cc_map_element_second_by_first(&_cc_items.container, (void*)(uintptr_t)key);
        if (item_pointer != NULL)
        {
            test_assert(item_pointer->key == key);
            test_assert(item_pointer->value == value);
        }
    }
}

static void stl_find(std::vector<item_t>& source_items)
{
    size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
        uint32_t key = source_items[i].key;
        uint32_t value = source_items[i].value;

        auto it = _stl_items.find(key);
        if (it != _stl_items.end())
        {
            item_t* item_pointer = it->second;            
            if (item_pointer != NULL)
            {
                test_assert(item_pointer->key == key);
                test_assert(item_pointer->value == value);
            }
        }
    }
}

//===========================================================================
static void cc_erase(std::vector<item_t>& source_items)
{
    size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
        uint32_t key = source_items[i].key;
		uint32_t value = source_items[i].value;

        size_t index = cc_map_find(&_cc_items.container, (void*)(uintptr_t)key);
        if (index != cc_invalid_index)
        {
            item_t* item_pointer = (item_t*)cc_map_element_second(&_cc_items.container, index);
        
            test_assert(item_pointer->key == key);
            test_assert(item_pointer->value == value);


            bool rv;
            rv = cc_map_erase(&_cc_items.container, index);
            test_assert(rv == true);


			cc_item_pool_free(item_pointer);
        }
    }
}

static void stl_erase(std::vector<item_t>& source_items)
{
    size_t count = source_items.size();
    for (size_t i = 0; i < count; ++i)
    {
        uint32_t key = source_items[i].key;
        uint32_t value = source_items[i].value;

        auto it = _stl_items.find(key);
        if (it != _stl_items.end())
        {
            item_t* item_pointer = it->second;
            
            test_assert(item_pointer->key == key);
            test_assert(item_pointer->value == value);


            _stl_items.erase(it);


            delete item_pointer;
        }
    }
}

//===========================================================================
static void cc_print_items(size_t items_count, size_t percent)
{
    test_out
        << "@ count=" << items_count
        << "(" << percent << "%%)"
        << test_tendl;


    item_t* item_pointer;


    const size_t max_printed_items = 32;
    size_t printed = 0;


    size_t i;
    size_t count;


    count = cc_map_count(&_cc_items.container);
    for (i = 0; i < count; i++)
    {
        item_pointer = (item_t*)cc_map_element_second(&_cc_items.container, i);

        if (item_pointer != NULL)
        {
            test_out
                << test_tindex(i)
                << "="
                << item_pointer->key << ","
                << item_pointer->value
                << test_tendl
                ;

            printed++;

            if (printed > max_printed_items)
            {
                break;
            }
        }
    }
}





/////////////////////////////////////////////////////////////////////////////
// 
// 성능 비교 시험
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void performance(std::ostream& oss, size_t count)
{
    std::vector<item_t> source_items = generate_source_items(count);

    size_t percent;
    percent = (count * 100) / item_max_count;

    oss
        << "@" << percent << "%%:"
        << "count=" << count
        << std::endl
        ;


    {
        test_scoped_timer timer(oss, " cc add  ");
        cc_add(source_items);                  
    }                                          
    {                                          
        test_scoped_timer timer(oss, "stl add  ");
        stl_add(source_items);                 
    }
    test_assert(count == _stl_items.size());
    test_assert(count == cc_map_count(&_cc_items.container));


    cc_print_items(count, percent);


    {
        test_scoped_timer timer(oss, " cc find ");
        cc_find(source_items);
    }
    {
        test_scoped_timer timer(oss, "stl find ");
        stl_find(source_items);
    }
    {
        test_scoped_timer timer(oss, " cc erase");
        cc_erase(source_items);
    }
    {
        test_scoped_timer timer(oss, "stl erase");
        stl_erase(source_items);
    }
    test_assert(0 == _stl_items.size());
    test_assert(0 == cc_map_count(&_cc_items.container));


    {
        test_scoped_timer timer(oss, " cc clear");
        cc_clear();
    }
    {
        test_scoped_timer timer(oss, "stl clear");
        stl_clear();
    }


    test_assert(cc_map_count(&_cc_items.container) == _stl_items.size());


	oss << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void run(void)
{
    std::ostringstream oss;

    oss << std::endl;
    oss << "# cc_map vs std::map performance" << std::endl;
    oss << std::endl;

    performance(oss, item_max_count * 4 / 4);
    performance(oss, item_max_count * 3 / 4);
    performance(oss, item_max_count * 2 / 4);
    performance(oss, item_max_count * 1 / 4);

	test_out << oss.str().c_str();
}

//===========================================================================
void test_case_cc_map_3()
{
	if (!cc_items_initialize())
	{
		return;
	}

	run();

	cc_items_uninitialize();
}
