/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(_WIN32) || defined(_WIN64)

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

#else

#include <stdio.h>
#include <stdarg.h>

#endif

#include "test_output.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_output test_out;
test_text_endl test_tendl;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_text_endl::test_text_endl() : _value("\n") {}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_text_index::test_text_index(size_t v) : _value(v) {}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if 1
static void debug_printf(const char* format, ...)
{
	static char buffer[1024];
	size_t max_count = sizeof(buffer) / sizeof(buffer[0]);

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, max_count, format, args);
    va_end(args);

	fputs(buffer, stdout);

#if defined(_WIN32) || defined(_WIN64)
    OutputDebugStringA(buffer);
#endif
}
#endif
#if 0
#define debug_printf    printf
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_output& test_output::operator<<(char v)
{
    debug_printf("%3d", static_cast<int>(v));
    return *this;
}

test_output& test_output::operator<<(short int v)
{
    debug_printf("%3hd", static_cast<int>(v));
    return *this;
}

test_output& test_output::operator<<(int v)
{
    debug_printf("%3d", static_cast<int>(v));
    return *this;
}

test_output& test_output::operator<<(long int v)
{
    debug_printf("%3ld", static_cast<int>(v));
    return *this;
}

test_output& test_output::operator<<(long long int v)
{
    debug_printf("%3lld", v);
    return *this;
}

test_output& test_output::operator<<(unsigned char v)
{
    debug_printf("%3u", static_cast<unsigned int>(v));
    return *this;
}

test_output& test_output::operator<<(unsigned short int v)
{
    debug_printf("%3hu", static_cast<unsigned int>(v));
    return *this;
}

test_output& test_output::operator<<(unsigned int v)
{
    debug_printf("%3u", static_cast<unsigned int>(v));
    return *this;
}

test_output& test_output::operator<<(unsigned long int v)
{
    debug_printf("%3lu", static_cast<unsigned int>(v));
    return *this;
}

test_output& test_output::operator<<(unsigned long long int v)
{
    debug_printf("%3llu", v);
    return *this;
}

test_output& test_output::operator<<(float v)
{
    debug_printf("%g", static_cast<double>(v));
    return *this;
}

test_output& test_output::operator<<(double v)
{
    debug_printf("%g", v);
    return *this;
}

test_output& test_output::operator<<(bool v)
{
    if (v)
    {
        debug_printf("true");
    }
    else
    {
        debug_printf("false");
    }

    return *this;
}

test_output& test_output::operator<<(const char* v)
{
    if (!v)
    {
        debug_printf("(null)");
    }
    else
    {
        debug_printf(v);
    }

    return *this;
}

test_output& test_output::operator<<(const void* v)
{
    debug_printf("%p", v);

    return *this;
}

test_output& test_output::operator<<(const test_text_index& v)
{
	// 64bit or 32bit 겸용시 %zu 사용
    debug_printf(" [%3zu] ", v._value);

    return *this;
}

test_output& test_output::operator<<(const test_text_endl& v)
{
    debug_printf(v._value);

    return *this;
}



