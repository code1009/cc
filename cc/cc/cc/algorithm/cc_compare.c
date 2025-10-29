/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_compare.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <string.h>

//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_compare.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_equal_pvalue(T,a,b) \
	T* ap;                      \
	T* bp;                      \
	T  av;                      \
	T  bv;                      \
	                            \
	                            \
	cc_debug_assert((a)!=NULL); \
	cc_debug_assert((b)!=NULL); \
	                            \
	                            \
	ap = (T*)(a);               \
	bp = (T*)(b);               \
	av = *ap;                   \
	bv = *bp;                   \
	                            \
	                            \
	if (av == bv)               \
	{                           \
		return true;            \
	}                           \
	                            \
	return false

//===========================================================================
#define cc_less_pvalue(T,a,b) \
	T* ap;                      \
	T* bp;                      \
	T  av;                      \
	T  bv;                      \
	                            \
	                            \
	cc_debug_assert((a)!=NULL); \
	cc_debug_assert((b)!=NULL); \
	                            \
	                            \
	ap = (T*)(a);               \
	bp = (T*)(b);               \
	av = *ap;                   \
	bv = *bp;                   \
	                            \
	                            \
	if (av < bv)                \
	{                           \
		return true;            \
	}                           \
	                            \
	return false





//===========================================================================
cc_api bool cc_equal_pvalue_int32_t (const void* left, const void* right) { cc_equal_pvalue(int32_t, left, right); }
cc_api bool cc_equal_pvalue_int16_t (const void* left, const void* right) { cc_equal_pvalue(int16_t, left, right); }
cc_api bool cc_equal_pvalue_int8_t  (const void* left, const void* right) { cc_equal_pvalue(int8_t , left, right); }

cc_api bool cc_equal_pvalue_uint32_t(const void* left, const void* right) { cc_equal_pvalue(uint32_t, left, right); }
cc_api bool cc_equal_pvalue_uint16_t(const void* left, const void* right) { cc_equal_pvalue(uint16_t, left, right); }
cc_api bool cc_equal_pvalue_uint8_t (const void* left, const void* right) { cc_equal_pvalue(uint8_t , left, right); }

cc_api bool cc_equal_pvalue_size_t  (const void* left, const void* right) { cc_equal_pvalue(size_t  , left, right); }

cc_api bool cc_equal_pvalue_char    (const void* left, const void* right) { cc_equal_pvalue(char    , left, right); }

cc_api bool cc_equal_c_str          (const void* left, const void* right)
{
	char* ap;
	char* bp;


	cc_debug_assert(left  != NULL);
	cc_debug_assert(right != NULL);


	ap = (char*)left;
	bp = (char*)right;

	if (0==strcmp(ap, bp))
	{
		return true;
	}

	return false;
}





//===========================================================================
cc_api bool cc_less_pvalue_int32_t (const void* left, const void* right) { cc_less_pvalue(int32_t , left, right); }
cc_api bool cc_less_pvalue_int16_t (const void* left, const void* right) { cc_less_pvalue(int16_t , left, right); }
cc_api bool cc_less_pvalue_int8_t  (const void* left, const void* right) { cc_less_pvalue(int8_t  , left, right); }

cc_api bool cc_less_pvalue_uint32_t(const void* left, const void* right) { cc_less_pvalue(uint32_t, left, right); }
cc_api bool cc_less_pvalue_uint16_t(const void* left, const void* right) { cc_less_pvalue(uint16_t, left, right); }
cc_api bool cc_less_pvalue_uint8_t (const void* left, const void* right) { cc_less_pvalue(uint8_t , left, right); }

cc_api bool cc_less_pvalue_size_t  (const void* left, const void* right) { cc_less_pvalue(size_t  , left, right); }

cc_api bool cc_less_pvalue_char    (const void* left, const void* right) { cc_less_pvalue(char    , left, right); }

cc_api bool cc_less_c_str(const void* left, const void* right)
{
	char* ap;
	char* bp;


	cc_debug_assert(left != NULL);
	cc_debug_assert(right != NULL);


	ap = (char*)left;
	bp = (char*)right;

	if (0 > strcmp(ap, bp))
	{
		return true;
	}

	return false;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_equal(T,a,b) \
	T  av;                      \
	T  bv;                      \
	                            \
	                            \
	av = (T)((uintptr_t)(a));   \
	bv = (T)((uintptr_t)(b));   \
	                            \
	                            \
	if (av == bv)               \
	{                           \
		return true;            \
	}                           \
	                            \
	return false

//===========================================================================
#define cc_less(T,a,b) \
	T  av;                      \
	T  bv;                      \
	                            \
	                            \
	av = (T)((uintptr_t)(a));   \
	bv = (T)((uintptr_t)(b));   \
	                            \
	                            \
	if (av < bv)                \
	{                           \
		return true;            \
	}                           \
	                            \
	return false





//===========================================================================
cc_api bool cc_equal_int32_t (const void* left, const void* right) { cc_equal(int32_t , left, right); }
cc_api bool cc_equal_int16_t (const void* left, const void* right) { cc_equal(int16_t , left, right); }
cc_api bool cc_equal_int8_t  (const void* left, const void* right) { cc_equal(int8_t  , left, right); }

cc_api bool cc_equal_uint32_t(const void* left, const void* right) { cc_equal(uint32_t, left, right); }
cc_api bool cc_equal_uint16_t(const void* left, const void* right) { cc_equal(uint16_t, left, right); }
cc_api bool cc_equal_uint8_t (const void* left, const void* right) { cc_equal(uint8_t , left, right); }

cc_api bool cc_equal_size_t  (const void* left, const void* right) { cc_equal(size_t  , left, right); }

cc_api bool cc_equal_char    (const void* left, const void* right) { cc_equal(char    , left, right); }





//===========================================================================
cc_api bool cc_less_int32_t (const void* left, const void* right) { cc_less(int32_t , left, right); }
cc_api bool cc_less_int16_t (const void* left, const void* right) { cc_less(int16_t , left, right); }
cc_api bool cc_less_int8_t  (const void* left, const void* right) { cc_less(int8_t  , left, right); }

cc_api bool cc_less_uint32_t(const void* left, const void* right) { cc_less(uint32_t, left, right); }
cc_api bool cc_less_uint16_t(const void* left, const void* right) { cc_less(uint16_t, left, right); }
cc_api bool cc_less_uint8_t (const void* left, const void* right) { cc_less(uint8_t , left, right); }

cc_api bool cc_less_size_t  (const void* left, const void* right) { cc_less(size_t  , left, right); }

cc_api bool cc_less_char    (const void* left, const void* right) { cc_less(char    , left, right); }
