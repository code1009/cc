#ifndef test_output_hpp
#define test_output_hpp





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_text_endl
{
public:
    const char* _value;
public:
    explicit test_text_endl();
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_text_index
{
public:
    size_t _value;
public:
    explicit test_text_index(size_t v);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_output
{
public:
    test_output& operator<<(char v);
    test_output& operator<<(short int v);
    test_output& operator<<(int v);
    test_output& operator<<(long int v);
    test_output& operator<<(long long int v);

    test_output& operator<<(unsigned char v);
    test_output& operator<<(unsigned short int v);
    test_output& operator<<(unsigned int v);
    test_output& operator<<(unsigned long int v);
    test_output& operator<<(unsigned long long int v);

    test_output& operator<<(float v);
    test_output& operator<<(double v);

    test_output& operator<<(bool v);
    test_output& operator<<(const char* v);
    test_output& operator<<(const void* v);

public:
    test_output& operator<<(const test_text_index& v);
    test_output& operator<<(const test_text_endl& v);
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
extern test_output test_out;
extern test_text_endl test_tendl;
using test_tindex = test_text_index;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // test_output_hpp