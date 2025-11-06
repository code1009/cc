/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_forward_list.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
//   @ Singly-linked list.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "../allocator/cc_fallocator.h"
#include "../allocator/cc_simple_segregated_storage.h"

#include "../element/cc_element.h"
#include "../element/cc_snode.h"

//===========================================================================
#include "cc_forward_list.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_debug)
#define cc_forward_list_config_enable_node_validation 1
#else
#define cc_forward_list_config_enable_node_validation 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline cc_snode_t* cc_forward_list_snode_allocate(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (cc_snode_t*)ctx->nallocator.allocate((void*)ctx->nallocator.handle);
}

static inline bool cc_forward_list_snode_free(cc_forward_list_t* ctx, const cc_snode_t* node)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(node != NULL);


	return ctx->nallocator.free((void*)ctx->nallocator.handle, (void*)node);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_forward_list_initialize(cc_forward_list_t* ctx, const cc_snode_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);

	bool rv;


	rv = cc_simple_segregated_storage_initialize(
		&ctx->nstorage,
		elements, sizeof(cc_snode_t) * max_count, sizeof(cc_snode_t), max_count
	);
	if (rv == false)
	{
		ctx->nallocator.handle = NULL;
		ctx->nallocator.allocate = NULL;
		ctx->nallocator.free = NULL;

		return false;
	}


	cc_fallocator_initialize(
		&ctx->nallocator,
		&ctx->nstorage, (cc_fallocate_t)cc_simple_segregated_storage_allocate, (cc_ffree_t)cc_simple_segregated_storage_free
	);


	ctx->elements = (cc_snode_t*)elements;
	ctx->max_count = max_count;
	ctx->param = param;

	ctx->count = 0;
	ctx->first = NULL;

	return true;
}

cc_api uintptr_t cc_forward_list_param(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->param;
}

//===========================================================================
cc_api void cc_forward_list_clear(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	// free all nodes allocated from the storage
	cc_snode_t* node = ctx->first;
	cc_snode_t* next;
	bool rv;


	while (node != NULL)
	{
		next = cc_snode_next(node);


#if (1==cc_forward_list_config_enable_node_validation)
		// validate pointer before freeing (optional safety)
		if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)node) == false)
		{
			cc_debug_assert(0);
			break;
		}
#endif


		rv = cc_forward_list_snode_free(ctx, node);
		if (rv == false)
		{
			cc_debug_assert(0);
			break;
		}


		node = next;
	}

	ctx->count = 0;
	ctx->first = NULL;
}

cc_api bool cc_forward_list_push_front(cc_forward_list_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->count >= ctx->max_count)
	{
		return false;
	}


	cc_snode_t* new_node = cc_forward_list_snode_allocate(ctx);
	if (new_node == NULL)
	{
		return false;
	}


	cc_snode_set(new_node, ctx->first, element);
	ctx->first = new_node;
	ctx->count++;

	return true;
}

cc_api void* cc_forward_list_pop_front(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->count == 0)
	{
		return NULL;
	}


	cc_snode_t* node = ctx->first;
	void* element = cc_snode_element(node);


	ctx->first = cc_snode_next(node);
	ctx->count--;


	bool rv = cc_forward_list_snode_free(ctx, node);
	if (rv == false)
	{
		cc_debug_assert(0);
	}


	return element;
}

cc_api cc_snode_t* cc_forward_list_insert_after(cc_forward_list_t* ctx, const cc_snode_t* node, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (element == NULL)
	{
		return NULL;
	}
	if (ctx->count >= ctx->max_count)
	{
		return NULL;
	}


	// if node == NULL, insert at front 
	if (node == NULL)
	{
		if (cc_forward_list_push_front(ctx, element) == true)
		{
			return ctx->first;
		}
		return NULL;
	}


#if (1==cc_forward_list_config_enable_node_validation)
	if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)node) == false)
	{
		cc_debug_assert(0);
		return NULL;
	}
#endif


	cc_snode_t* new_node = cc_forward_list_snode_allocate(ctx);
	if (new_node == NULL)
	{
		return NULL;
	}


	cc_snode_set(new_node, cc_snode_next((cc_snode_t*)node), element);
	((cc_snode_t*)node)->next = new_node;
	ctx->count++;


	return new_node;
}

cc_api bool cc_forward_list_erase_after(cc_forward_list_t* ctx, const cc_snode_t* node)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->count == 0)
	{
		return false;
	}


	// erase first if node == NULL
	if (node == NULL)
	{
		// pop front
		void* e = cc_forward_list_pop_front(ctx);
		(void)e;
		return true;
	}



#if (1==cc_forward_list_config_enable_node_validation)
	if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)node) == false)
	{
		cc_debug_assert(0);
		return false;
	}
#endif


	cc_snode_t* target = cc_snode_next((cc_snode_t*)node);
	if (target == NULL)
	{
		return false;
	}


#if (1==cc_forward_list_config_enable_node_validation)
	if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)target) == false)
	{
		cc_debug_assert(0);
		return false;
	}
#endif


	((cc_snode_t*)node)->next = cc_snode_next(target);
	ctx->count--;


	bool rv = cc_forward_list_snode_free(ctx, target);
	if (rv == false)
	{
		cc_debug_assert(0);
		return false;
	}

	return true;
}

//===========================================================================
cc_api cc_snode_t* cc_forward_list_first(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->first;
}

cc_api void* cc_forward_list_node_element(cc_forward_list_t* ctx, const cc_snode_t* node)
{
	cc_debug_assert(ctx != NULL);


	if (node == NULL)
	{
		return NULL;
	}
	if (ctx->count == 0)
	{
		return NULL;
	}


#if (1==cc_forward_list_config_enable_node_validation)
	bool rv;

	rv = cc_simple_segregated_storage_validate_pointer(
		&ctx->nstorage, (void*)node
	);
	if(rv == false)
	{
		cc_debug_assert(0);

		return NULL;
	}
#endif


	return cc_snode_element((cc_snode_t*)node);
}

//===========================================================================
cc_api size_t cc_forward_list_count(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->count;
}

cc_api bool cc_forward_list_empty(cc_forward_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (ctx->count == 0);
}
