#ifndef cc_dnode_h
#define cc_dnode_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_dnode.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_dnode_t
{
	struct _cc_dnode_t* next;
	struct _cc_dnode_t* previous;
	cc_element_t element;
}
cc_dnode_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_element_macro_function)
#define cc_dnode_config_macro_function 1
#else
#define cc_dnode_config_macro_function 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (0==cc_dnode_config_macro_function)

cc_api void cc_dnode_initialize(cc_dnode_t* ctx);
cc_api void cc_dnode_set(cc_dnode_t* ctx, const cc_dnode_t* next, const cc_dnode_t* previous, const void* element);
cc_api cc_dnode_t* cc_dnode_next(cc_dnode_t* ctx);
cc_api cc_dnode_t* cc_dnode_previous(cc_dnode_t* ctx);
cc_api void* cc_dnode_element(cc_dnode_t* ctx);

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_dnode_config_macro_function)

#define cc_dnode_initialize(ctx) \
	do { \
		(ctx)->next = NULL; \
		(ctx)->previous = NULL; \
		cc_element_initialize(&((ctx)->element)); \
	} while(0)

#define cc_dnode_set(ctx, _cc_dnode_set_next, _cc_dnode_set_previous, _cc_dnode_set_element) \
	do { \
		(ctx)->next = (cc_dnode_t*)(_cc_dnode_set_next); \
		(ctx)->previous = (cc_dnode_t*)(_cc_dnode_set_previous); \
		cc_element_set(&((ctx)->element), (_cc_dnode_set_element)); \
	} while(0)

#define cc_dnode_next(ctx) \
	((ctx)->next)

#define cc_dnode_previous(ctx) \
	((ctx)->previous)

#define cc_dnode_element(ctx) \
	(cc_element_get(&((ctx)->element)))

#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_dnode_h

