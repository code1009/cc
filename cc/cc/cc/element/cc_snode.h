#ifndef cc_snode_h
#define cc_snode_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_snode.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_snode_t
{
	struct _cc_snode_t* next;
	cc_element_t element;
}
cc_snode_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_snode_config_macro_function 1
#else
#define cc_snode_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_snode_config_macro_function)

cc_api void cc_snode_initialize(cc_snode_t* ctx);
cc_api void cc_snode_set(cc_snode_t* ctx, const cc_snode_t* next, const void* element);
cc_api cc_snode_t* cc_snode_next(cc_snode_t* ctx);
cc_api void* cc_snode_element(cc_snode_t* ctx);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_snode_config_macro_function)

#define cc_snode_initialize(ctx) \
	do { \
		(ctx)->next = NULL; \
		cc_element_initialize(&((ctx)->element)); \
	} while(0)

#define cc_snode_set(ctx, _cc_snode_set_next, _cc_snode_set_element) \
	do { \
		(ctx)->next = (cc_snode_t*)(_cc_snode_set_next); \
		cc_element_set(&((ctx)->element), (_cc_snode_set_element)); \
	} while(0)

#define cc_snode_next(ctx) \
	((ctx)->next)

#define cc_snode_element(ctx) \
	(cc_element_get(&((ctx)->element)))

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_snode_h

