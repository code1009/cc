#ifndef cc_element_h
#define cc_element_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_element.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_element_t
{
	void* pointer;
}
cc_element_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_element_config_macro_function 1
#else
#define cc_element_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_element_config_macro_function)

cc_api void cc_element_initialize(cc_element_t* ctx);
cc_api void cc_element_set(cc_element_t* ctx, const void* pointer);
cc_api void* cc_element_get(cc_element_t* ctx);

cc_api void cc_element_copy(cc_element_t* dst, const cc_element_t* src);

cc_api void cc_element_swap(cc_element_t* left, cc_element_t* right);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_element_config_macro_function)

#define cc_element_initialize(ctx) \
	do { \
		(ctx)->pointer = NULL; \
	} while(0)

#define cc_element_set(ctx, _cc_element_set_pointer) \
	do { \
		(ctx)->pointer = (void*)(_cc_element_set_pointer); \
	} while(0)

#define cc_element_get(ctx) \
	((ctx)->pointer)

#define cc_element_copy(_cc_element_copy_dst, _cc_element_copy_src) \
	do { \
		(_cc_element_copy_dst)->pointer = (_cc_element_copy_src)->pointer; \
	} while(0)

#define cc_element_swap(_cc_element_swap_left, _cc_element_swap_right) \
	do { \
		void* _cc_element_swap_temp = (_cc_element_swap_left)->pointer; \
		(_cc_element_swap_left)->pointer = (_cc_element_swap_right)->pointer; \
		(_cc_element_swap_right)->pointer = _cc_element_swap_temp; \
	} while(0)

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_element_h

