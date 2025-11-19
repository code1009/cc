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

#include <algorithm>
#include <numeric>

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
    oss << " cc count=" << cc << std::endl;
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
    oss << "crt count=" << crt << std::endl;
    ptrs.clear();


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
    
    std::vector<void*> cc_ptrs;
	cc_ptrs.reserve(allocate_count);

    std::vector<void*> crt_ptrs;
    crt_ptrs.reserve(allocate_count);


    void* ptr;

    std::vector<uint32_t> random_sizes = generate_random_sizes(allocate_count);


    //-----------------------------------------------------------------------
    oss << std::endl;
    oss << "# cc_heap_memory vs crt:malloc()/free() random size performance" << std::endl;
    oss << std::endl;


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, " cc allocate");
		size_t count = random_sizes.size();
        for (size_t i = 0; i < count; i++)
        {
            ptr = cc_heap_memory_allocate(&_cc_heap_memory, random_sizes[i]);
            cc_ptrs.push_back(ptr);
        }
    }


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, "crt malloc  ");
        size_t count = random_sizes.size();
        for (size_t i = 0; i < count; i++)
        {
            ptr = malloc(random_sizes[i]);
            crt_ptrs.push_back(ptr);
        }
    }


    //-----------------------------------------------------------------------
#if 1
    {
        size_t count = cc_ptrs.size();
        if (count == crt_ptrs.size() && count > 1)
        {
            std::vector<size_t> indices(count);
            std::iota(indices.begin(), indices.end(), 0);

            std::random_device rd;
            std::mt19937 mt(rd());
            std::shuffle(indices.begin(), indices.end(), mt);

            std::vector<void*> cc_shuffled;
            std::vector<void*> crt_shuffled;
            cc_shuffled.reserve(count);
            crt_shuffled.reserve(count);

            for (size_t i = 0; i < count; ++i)
            {
                size_t idx = indices[i];
                cc_shuffled.push_back(cc_ptrs[idx]);
                crt_shuffled.push_back(crt_ptrs[idx]);
            }

            cc_ptrs.swap(cc_shuffled);
            crt_ptrs.swap(crt_shuffled);
        }
    }
#endif


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, " cc free    ");
		size_t count = cc_ptrs.size();
        for (size_t i = 0; i < count; i++)
        {
            cc_heap_memory_free(&_cc_heap_memory, cc_ptrs[i]);
        }
    }


    //-----------------------------------------------------------------------
    {
        test_scoped_timer timer(oss, "crt free    ");
        size_t count = crt_ptrs.size();
        for (size_t i = 0; i < count; i++)
        {
           free(crt_ptrs[i]);
        }
    }


    //-----------------------------------------------------------------------
    cc = cc_ptrs.size();
    oss << " cc count=" << cc << std::endl;
    cc_ptrs.clear();
    

    //-----------------------------------------------------------------------
    crt = crt_ptrs.size();
    oss << "crt count=" << crt << std::endl;
    crt_ptrs.clear();


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
