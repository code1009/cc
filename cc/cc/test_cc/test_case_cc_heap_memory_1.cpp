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
#include <unordered_map>

#include "../tester/test_timer.hpp"






/////////////////////////////////////////////////////////////////////////////
// 
// cc
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define         _cc_heap_memory_buffer_size (1024 * 1024 * 64)
static uint8_t* _cc_heap_memory_buffer_pointer = NULL;
static cc_heap_memory_t _cc_heap_memory;

//===========================================================================
static bool cc_heap_memory_create()
{
	_cc_heap_memory_buffer_pointer = (uint8_t*)malloc(_cc_heap_memory_buffer_size);
    if (_cc_heap_memory_buffer_pointer == NULL)
    {
        return false;
	}


	cc_lf_heap_bucket_descriptor_t lf_heap_bucket_descriptor_elements[] =
	{
		  8, (4096*4 - sizeof(cc_lf_heap_bucket_t)) /   8,
		 16, (4096*4 - sizeof(cc_lf_heap_bucket_t)) /  16,
		 32, (4096*4 - sizeof(cc_lf_heap_bucket_t)) /  32,
		 64, (4096*4 - sizeof(cc_lf_heap_bucket_t)) /  64,
		128, (4096*4 - sizeof(cc_lf_heap_bucket_t)) / 128,
        256, (4096*4 - sizeof(cc_lf_heap_bucket_t)) / 256,
        512, (4096*4 - sizeof(cc_lf_heap_bucket_t)) / 512
    };


	cc_lf_heap_bucket_descriptors_t lf_heap_bucket_descriptors;
	lf_heap_bucket_descriptors.elements = lf_heap_bucket_descriptor_elements;
	lf_heap_bucket_descriptors.count = sizeof(lf_heap_bucket_descriptor_elements) / sizeof(cc_lf_heap_bucket_descriptor_t);


	return cc_heap_memory_initialize(
        &_cc_heap_memory,
        _cc_heap_memory_buffer_pointer, _cc_heap_memory_buffer_size,
		&lf_heap_bucket_descriptors
	);
}

static void cc_heap_memory_destroy()
{
	cc_heap_memory_uninitialize(&_cc_heap_memory);

    if (_cc_heap_memory_buffer_pointer)
    {
        free(_cc_heap_memory_buffer_pointer);
    }
    _cc_heap_memory_buffer_pointer = NULL;
}

static void cc_heap_memory_add_bucket_region()
{
    cc_lf_heap_t* lf_heap = &_cc_heap_memory.lf_heap;
    cc_first_fit_t* first_fit = &lf_heap->first_fit;
    for (size_t i = 0; i < lf_heap->buckets.count; i++)
    {
        cc_lf_heap_add_bucket_region(lf_heap, i);
    }
}

static void cc_heap_memory_reserved()
{
    for(size_t i = 0; i < 100; i++)
    {
        cc_heap_memory_add_bucket_region();
	}
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void cc_alloc(std::vector<void*>& ptrs, size_t size, size_t count)
{
    void* ptr;

    for (size_t i = 0; i < count; ++i)
    {
        ptr = cc_heap_memory_allocate(&_cc_heap_memory, size);
        if (ptr == NULL)
        {
            return;
        }
        ptrs.push_back(ptr);
	}
}

static void crt_alloc(std::vector<void*>& ptrs, size_t size, size_t count)
{
    void* ptr;
    for (size_t i = 0; i < count; ++i)
    {
        ptr = malloc(size);
        if (ptr == NULL)
        {
            return;
        }
        ptrs.push_back(ptr);
	}
}






/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void cc_free(std::vector<void*>& ptrs)
{
    size_t count = ptrs.size();
    for (size_t i = 0; i < count; ++i)
    {
        cc_heap_memory_free(&_cc_heap_memory, ptrs[i]);
    }
	ptrs.clear();
}

static void crt_free(std::vector<void*>& ptrs)
{
    size_t count = ptrs.size();
    for (size_t i = 0; i < count; ++i)
    {
        free(ptrs[i]);
    }
	ptrs.clear();
}






/////////////////////////////////////////////////////////////////////////////
// 
// 성능 비교 시험
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void performance(std::ostream& oss, size_t size, size_t count, bool cc_create)
{
    std::vector<void*> ptrs;
    size_t cc;
    size_t crt;


    //-----------------------------------------------------------------------
    oss
        << "@" 
        << " size=" << size 
        << " count=" << count
        << std::endl
        ;

    
    //-----------------------------------------------------------------------
    if (cc_create)
    {
        if (!cc_heap_memory_create())
        {
            return;
        }

#if 0
        cc_heap_memory_reserved();
#endif
    }

	ptrs.reserve(count);



    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, " cc allocate");
        cc_alloc(ptrs, size, count);
    }
    cc = ptrs.size();
    {
        test_scoped_timer timer(oss, " cc free    ");
        cc_free(ptrs);
    }
    ptrs.clear();



    //-----------------------------------------------------------------------
    if (cc_create)
    {
        cc_heap_memory_destroy();
    }



    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, "crt malloc  ");
        crt_alloc(ptrs, size, count);
    }
    crt = ptrs.size();
    {
        test_scoped_timer timer(oss, "crt free    ");
        crt_free(ptrs);
    }
    ptrs.clear();


    //-----------------------------------------------------------------------
    oss
        << "cc  count=" << cc 
        << ", crt count=" << crt
        << std::endl
        ;


    //-----------------------------------------------------------------------
    oss << std::endl;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static void t1(void)
{
    //-----------------------------------------------------------------------
    bool cc_create = true;
    std::ostringstream oss;


    //-----------------------------------------------------------------------
    oss << std::endl;
    oss << "# cc_heap_memory(create) vs crt:malloc()/free() performance" << std::endl;
    oss << std::endl;


    //-----------------------------------------------------------------------
    performance(oss, 16, 1000000, cc_create);
    performance(oss, 256, 100000, cc_create);
    performance(oss, 4096, 10000, cc_create);


    //-----------------------------------------------------------------------
    test_out << oss.str().c_str();
}

static void t2(void)
{
    //-----------------------------------------------------------------------
    if (!cc_heap_memory_create())
    {
        return;
    }
#if 0
    cc_heap_memory_reserved();
#endif


    //-----------------------------------------------------------------------
    bool cc_create = false;
    std::ostringstream oss;


    //-----------------------------------------------------------------------
    oss << std::endl;
    oss << "# cc_heap_memory vs crt:malloc()/free() performance" << std::endl;
    oss << std::endl;


    performance(oss, 16, 1000000, cc_create);
    performance(oss, 256, 100000, cc_create);
    performance(oss, 4096, 10000, cc_create);


    //-----------------------------------------------------------------------
    test_out << oss.str().c_str();


    //-----------------------------------------------------------------------
    cc_first_fit_dump(&_cc_heap_memory.lf_heap.first_fit, 0, 0);
    printf("\n");


    //-----------------------------------------------------------------------
    cc_heap_memory_destroy();
}

static uint32_t random_range_uint32_t(uint32_t min_inclusive, uint32_t max_inclusive)
{
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<uint32_t> dist(min_inclusive, max_inclusive);
    return dist(mt);
}

static std::vector<uint32_t> generate_random_sizes(size_t count)
{
    if (count == 0)
    {
        return {};
    }

    std::vector<uint32_t> sizes;
	sizes.reserve(count);

    for (uint32_t i = 0; i < count; ++i)
    {
        uint32_t size = random_range_uint32_t(16, 512+128);
        sizes.push_back(size);
    }

    return sizes;
}

static void t3(void)
{
    //-----------------------------------------------------------------------
    if (!cc_heap_memory_create())
    {
        return;
    }


    //-----------------------------------------------------------------------
    std::ostringstream oss;


    //-----------------------------------------------------------------------
    size_t cc;
    size_t crt;


    //-----------------------------------------------------------------------
    std::size_t allocate_count = 100000;
    
    std::vector<void*> ptrs;
	ptrs.reserve(allocate_count);
    void* ptr;

    std::vector<uint32_t> random_sizes = generate_random_sizes(allocate_count);


    //-----------------------------------------------------------------------
    oss << std::endl;
    oss << "# cc_heap_memory vs crt:malloc()/free() random size performance" << std::endl;
    oss << std::endl;


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, " cc allocate ");
		size_t count = random_sizes.size();
        for (size_t i = 0; i < count; i++)
        {
            ptr = cc_heap_memory_allocate(&_cc_heap_memory, random_sizes[i]);
            ptrs.push_back(ptr);
        }
    }
    {
        test_scoped_timer timer(oss, " cc free     ");
		size_t count = ptrs.size();
        for (size_t i = 0; i < count; i++)
        {
            cc_heap_memory_free(&_cc_heap_memory, ptrs[i]);
        }
    }
	cc = ptrs.size();
	ptrs.clear();


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, " crt malloc  ");
        size_t count = random_sizes.size();
        for (size_t i = 0; i < count; i++)
        {
            ptr = malloc(random_sizes[i]);
            ptrs.push_back(ptr);
        }
    }
    {
        test_scoped_timer timer(oss, " crt free    ");
        size_t count = ptrs.size();
        for (size_t i = 0; i < count; i++)
        {
           free(ptrs[i]);
        }
    }
	crt = ptrs.size();
	ptrs.clear();


    //-----------------------------------------------------------------------
    oss
        << "cc  count=" << cc
        << ", crt count=" << crt
        << std::endl
        ;


    //-----------------------------------------------------------------------
    oss << std::endl;


    //-----------------------------------------------------------------------
    test_out << oss.str().c_str();


    //-----------------------------------------------------------------------
    cc_first_fit_dump(&_cc_heap_memory.lf_heap.first_fit, 0, 0);
    printf("\n");


    //-----------------------------------------------------------------------
    cc_heap_memory_destroy();
}

static void run(void)
{
    t1();
    t2();
    t3();
}

//===========================================================================
void test_case_cc_heap_memory_1()
{
	run();
}
