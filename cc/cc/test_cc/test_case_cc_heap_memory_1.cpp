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
static bool create_cc_heap_memory()
{
	_cc_heap_memory_buffer_pointer = (uint8_t*)malloc(_cc_heap_memory_buffer_size);
    if (_cc_heap_memory_buffer_pointer == NULL)
    {
        return false;
	}


	cc_lf_heap_bucket_descriptor_t lf_heap_bucket_descriptor_elements[] =
	{
		  8, (4096 - sizeof(cc_lf_heap_bucket_t)) /   8,
		 16, (4096 - sizeof(cc_lf_heap_bucket_t)) /  16,
		 32, (4096 - sizeof(cc_lf_heap_bucket_t)) /  32,
		 64, (4096 - sizeof(cc_lf_heap_bucket_t)) /  64,
		128, (4096 - sizeof(cc_lf_heap_bucket_t)) / 128,
        256, (4096 - sizeof(cc_lf_heap_bucket_t)) / 256,
        512, (4096 - sizeof(cc_lf_heap_bucket_t)) / 512
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

static void destroy_cc_heap_memory()
{
    if (_cc_heap_memory_buffer_pointer)
    {
        free(_cc_heap_memory_buffer_pointer);
    }
    _cc_heap_memory_buffer_pointer = NULL;
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
        if (!create_cc_heap_memory())
        {
            return;
        }
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
        destroy_cc_heap_memory();
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
static void test1(void)
{
    std::ostringstream oss;

    bool cc_create;


    //-----------------------------------------------------------------------
    oss << std::endl;
    oss << "# cc_heap_memory(create) vs crt:malloc()/free() performance" << std::endl;
    oss << std::endl;


    cc_create = true;
    performance(oss, 16, 1000000, cc_create);
    performance(oss, 256, 100000, cc_create);
    performance(oss, 4096, 10000, cc_create);


    //-----------------------------------------------------------------------
    test_out << oss.str().c_str();
}

static void test2(void)
{
    std::ostringstream oss;

    bool cc_create;


	//-----------------------------------------------------------------------
    oss << "# cc_heap_memory vs crt:malloc()/free() performance" << std::endl;
    oss << std::endl;


    cc_create = false;
    if (!create_cc_heap_memory())
    {
        return;
    }

    performance(oss, 16, 1000000, cc_create);
    performance(oss, 256, 100000, cc_create);
    performance(oss, 4096, 10000, cc_create);
    cc_first_fit_dump(&_cc_heap_memory.lf_heap.first_fit, 0, 0);
    printf("\n");


    destroy_cc_heap_memory();


    //-----------------------------------------------------------------------
    test_out << oss.str().c_str();
}

static void run(void)
{
    test1();
    test2();
}

//===========================================================================
void test_case_cc_heap_memory_1()
{
	run();
}
