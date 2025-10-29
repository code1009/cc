#ifndef cc_pair_bucket_h
#define cc_pair_bucket_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_pair_bucket.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_pair_bucket_t
{
	cc_pair_t element;
	cc_bucket_status_t status;
}
cc_pair_bucket_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_pair_bucket_config_macro_function 1
#else
#define cc_pair_bucket_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_pair_bucket_config_macro_function)

cc_api void cc_pair_bucket_initialize(cc_pair_bucket_t* ctx);
cc_api void* cc_pair_bucket_element_first(cc_pair_bucket_t* ctx);
cc_api void* cc_pair_bucket_element_second(cc_pair_bucket_t* ctx);
cc_api cc_bucket_status_t cc_pair_bucket_status(cc_pair_bucket_t* ctx);
cc_api void cc_pair_bucket_fill(cc_pair_bucket_t* ctx, const void* first, const void* second);
cc_api void cc_pair_bucket_remove(cc_pair_bucket_t* ctx);
cc_api void cc_pair_bucket_clear(cc_pair_bucket_t* ctx);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_pair_bucket_config_macro_function)

#define cc_pair_bucket_initialize(ctx) \
	do { \
		cc_pair_bucket_clear((ctx)); \
	} while(0)

#define cc_pair_bucket_element_first(ctx) \
	(cc_pair_first(&((ctx)->element)))

#define cc_pair_bucket_element_second(ctx) \
	(cc_pair_second(&((ctx)->element)))

#define cc_pair_bucket_status(ctx) \
	((ctx)->status)

#define cc_pair_bucket_fill(ctx, _cc_pair_bucket_fill_first, _cc_pair_bucket_fill_second) \
	do { \
		(ctx)->status = cc_bucket_status_filled; \
		cc_pair_set(&((ctx)->element), (_cc_pair_bucket_fill_first), (_cc_pair_bucket_fill_second)); \
	} while(0)

#define cc_pair_bucket_remove(ctx) \
	do { \
		(ctx)->status = cc_bucket_status_removed; \
		cc_pair_set(&((ctx)->element), NULL, NULL); \
	} while(0)

#define cc_pair_bucket_clear(ctx) \
	do { \
		(ctx)->status = cc_bucket_status_empty; \
		cc_pair_set(&((ctx)->element), NULL, NULL); \
	} while(0)

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_pair_bucket

