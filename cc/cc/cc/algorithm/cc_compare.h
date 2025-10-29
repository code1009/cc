#ifndef cc_compare_h
#define cc_compare_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_compare.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*cc_equal_t)(const void* left, const void* right);
typedef bool (*cc_less_t )(const void* left, const void* right);

#ifdef __cplusplus
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_equal_pvalue_int32_t (const void* left, const void* right);
cc_api bool cc_equal_pvalue_int16_t (const void* left, const void* right);
cc_api bool cc_equal_pvalue_int8_t  (const void* left, const void* right);

cc_api bool cc_equal_pvalue_uint32_t(const void* left, const void* right);
cc_api bool cc_equal_pvalue_uint16_t(const void* left, const void* right);
cc_api bool cc_equal_pvalue_uint8_t (const void* left, const void* right);

cc_api bool cc_equal_pvalue_size_t  (const void* left, const void* right);

cc_api bool cc_equal_pvalue_char    (const void* left, const void* right);

cc_api bool cc_equal_c_str          (const void* left, const void* right);

//===========================================================================
cc_api bool cc_less_pvalue_int32_t  (const void* left, const void* right);
cc_api bool cc_less_pvalue_int16_t  (const void* left, const void* right);
cc_api bool cc_less_pvalue_int8_t   (const void* left, const void* right);

cc_api bool cc_less_pvalue_uint32_t(const void* left, const void* right);
cc_api bool cc_less_pvalue_uint16_t(const void* left, const void* right);
cc_api bool cc_less_pvalue_uint8_t  (const void* left, const void* right);

cc_api bool cc_less_pvalue_size_t   (const void* left, const void* right);

cc_api bool cc_less_pvalue_char     (const void* left, const void* right);

cc_api bool cc_less_c_str           (const void* left, const void* right);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_equal_int32_t(const void* left, const void* right);
cc_api bool cc_equal_int16_t(const void* left, const void* right);
cc_api bool cc_equal_int8_t(const void* left, const void* right);

cc_api bool cc_equal_uint32_t(const void* left, const void* right);
cc_api bool cc_equal_uint16_t(const void* left, const void* right);
cc_api bool cc_equal_uint8_t(const void* left, const void* right);

cc_api bool cc_equal_size_t(const void* left, const void* right);

cc_api bool cc_equal_char(const void* left, const void* right);

//===========================================================================
cc_api bool cc_less_int32_t(const void* left, const void* right);
cc_api bool cc_less_int16_t(const void* left, const void* right);
cc_api bool cc_less_int8_t(const void* left, const void* right);

cc_api bool cc_less_uint32_t(const void* left, const void* right);
cc_api bool cc_less_uint16_t(const void* left, const void* right);
cc_api bool cc_less_uint8_t(const void* left, const void* right);

cc_api bool cc_less_size_t(const void* left, const void* right);

cc_api bool cc_less_char(const void* left, const void* right);




#endif




