#ifndef cc_pair_h
#define cc_pair_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_pair.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_pair_t
{
	cc_element_t first;
	cc_element_t second;
}
cc_pair_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_pair_config_macro_function 1
#else
#define cc_pair_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_pair_config_macro_function)

cc_api void cc_pair_initialize(cc_pair_t* ctx);
cc_api void cc_pair_set(cc_pair_t* ctx, const void* first, const void* second);
cc_api void* cc_pair_first(cc_pair_t* ctx);
cc_api void* cc_pair_second(cc_pair_t* ctx);

cc_api void cc_pair_copy(cc_pair_t* dst, const cc_pair_t* src);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_pair_config_macro_function)

#define cc_pair_initialize(ctx) \
	do { \
		cc_element_initialize(&(ctx)->first); \
		cc_element_initialize(&(ctx)->second); \
	} while(0)

#define cc_pair_set(ctx, _cc_pair_set_first, _cc_pair_set_second) \
	do { \
		cc_element_set(&(ctx)->first, (_cc_pair_set_first)); \
		cc_element_set(&(ctx)->second, (_cc_pair_set_second)); \
	} while(0)

#define cc_pair_first(ctx) \
	(cc_element_get(&(ctx)->first))

#define cc_pair_second(ctx) \
	(cc_element_get(&(ctx)->second))

#define cc_pair_copy(_cc_pair_copy_dst, _cc_pair_copy_src) \
	do { \
		cc_element_copy(&(_cc_pair_copy_dst)->first, &(_cc_pair_copy_src)->first); \
		cc_element_copy(&(_cc_pair_copy_dst)->second, &(_cc_pair_copy_src)->second); \
	} while(0)

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_pair_h

