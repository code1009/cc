#ifndef cc_bucket_h
#define cc_bucket_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_bucket.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef enum _cc_bucket_status_t
{
	cc_bucket_status_empty = 0,
	cc_bucket_status_removed,
	cc_bucket_status_filled
}
cc_bucket_status_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_bucket_t
{
	cc_element_t element;
	cc_bucket_status_t status;
}
cc_bucket_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_bucket_config_macro_function 1
#else
#define cc_bucket_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_bucket_config_macro_function)

cc_api void cc_bucket_initialize(cc_bucket_t* ctx);
cc_api void* cc_bucket_element(cc_bucket_t* ctx);
cc_api cc_bucket_status_t cc_bucket_status(cc_bucket_t* ctx);
cc_api void cc_bucket_fill(cc_bucket_t* ctx, const void* element);
cc_api void cc_bucket_remove(cc_bucket_t* ctx);
cc_api void cc_bucket_clear(cc_bucket_t* ctx);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_bucket_config_macro_function)

#define cc_bucket_initialize(ctx) \
	do { \
		(ctx)->status = cc_bucket_status_empty; \
		(ctx)->element.pointer = NULL; \
	} while(0)

#define cc_bucket_element(ctx) \
	((ctx)->element.pointer)

#define cc_bucket_status(ctx) \
	((ctx)->status)

#define cc_bucket_fill(ctx, _cc_bucket_fill_element) \
	do { \
		(ctx)->status = cc_bucket_status_filled; \
		(ctx)->element.pointer = (void*)(_cc_bucket_fill_element); \
	} while(0)

#define cc_bucket_remove(ctx) \
	do { \
		(ctx)->status = cc_bucket_status_removed; \
		(ctx)->element.pointer = NULL; \
	} while(0)

#define cc_bucket_clear(ctx) \
	do { \
		(ctx)->status = cc_bucket_status_empty; \
		(ctx)->element.pointer = NULL; \
	} while(0)

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_bucket_h

