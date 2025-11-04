/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_list.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
//   @ Doubly-linked list.
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
#include "../element/cc_dnode.h"

//===========================================================================
#include "cc_list.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_debug)
#define cc_list_config_enable_node_validation 1
#else
#define cc_list_config_enable_node_validation 0
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static cc_dnode_t* cc_list_dnode_allocate(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (cc_dnode_t*)ctx->nallocator.allocate((void*)ctx->nallocator.handle);
}

static bool cc_list_dnode_free(cc_list_t* ctx, const cc_dnode_t* node)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(node != NULL);


	return ctx->nallocator.free((void*)ctx->nallocator.handle, (void*)node);
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_list_initialize(cc_list_t* ctx, const cc_dnode_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(elements != NULL);
	cc_debug_assert(max_count != 0);

	bool rv;


	rv = cc_simple_segregated_storage_initialize(
		&ctx->nstorage,
		elements, sizeof(cc_dnode_t) * max_count, sizeof(cc_dnode_t), max_count
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


	ctx->elements = (cc_dnode_t*)elements;
	ctx->max_count = max_count;
	ctx->param = param;

	ctx->count = 0;
	ctx->first = NULL;
	ctx->last = NULL;

	return true;
}

cc_api uintptr_t cc_list_param(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->param;
}

//===========================================================================
cc_api void cc_list_clear(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	// free all nodes allocated from the storage
	cc_dnode_t* node = ctx->first;
	cc_dnode_t* next;
	bool rv;


	while (node != NULL)
	{
		next = cc_dnode_next(node);


#if (1==cc_list_config_enable_node_validation)
		// validate pointer before freeing (optional safety)
		if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)node) == false)
		{
			cc_debug_assert(0);
			break;
		}
#endif


		rv = cc_list_dnode_free(ctx, node);
		if (rv == false)
		{
			cc_debug_assert(0);
			break;
		}


		node = next;
	}

	ctx->count = 0;
	ctx->first = NULL;
	ctx->last = NULL;
}

cc_api bool cc_list_erase(cc_list_t* ctx, const cc_dnode_t* node)
{
	cc_debug_assert(ctx != NULL);


	if(node == NULL)
	{
		return false;
	}
	if(ctx->count == 0)
	{
		return false;
	}


	bool rv;

#if (1==cc_list_config_enable_node_validation)
	rv = cc_simple_segregated_storage_validate_pointer(
		&ctx->nstorage, (void*)node
	);
	if (rv == false)
	{
		cc_debug_assert(0);

		return false;
	}
#endif


	cc_dnode_t* previous = node->previous;
	cc_dnode_t* next = node->next;

	if (previous != NULL)
	{
		previous->next = next;
	}
	else
	{
		ctx->first = next;
	}

	if (next != NULL)
	{
		next->previous = previous;
	}
	else
	{
		ctx->last = previous;
	}


	ctx->count--;


	rv = cc_list_dnode_free(ctx, node);
	if (rv == false)
	{
		cc_debug_assert(0);

		return false;
	}

	
	return true;
}

cc_api cc_dnode_t* cc_list_add(cc_list_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->count >= ctx->max_count)
	{
		return NULL;
	}


	cc_dnode_t* new_node = cc_list_dnode_allocate(ctx);
	if (new_node == NULL)
	{
		return NULL;
	}


	cc_dnode_set(new_node, NULL, NULL, element);
	

	if (ctx->first == NULL)
	{
		ctx->first = new_node;
		ctx->last = new_node;
	}
	else
	{
		cc_dnode_t* last = ctx->last;
		last->next = new_node;

		new_node->previous = last;
		ctx->last = new_node;
	}


	ctx->count++;


	return new_node;
}

cc_api cc_dnode_t* cc_list_insert(cc_list_t* ctx, const cc_dnode_t* node, const void* element)
{
	cc_debug_assert(ctx != NULL);


	if (node == NULL)
	{
		return NULL;
	}
	if (ctx->count >= ctx->max_count)
	{
		return NULL;
	}


#if (1==cc_list_config_enable_node_validation)
	if (cc_simple_segregated_storage_validate_pointer(&ctx->nstorage, (void*)node) == false)
	{
		cc_debug_assert(0);

		return NULL;
	}
#endif


	cc_dnode_t* new_node = cc_list_dnode_allocate(ctx);
	if (new_node == NULL)
	{
		return NULL;
	}

	// insert before the provided node
	cc_dnode_set(new_node, node, node->previous, element);

	if (node->previous != NULL)
	{
		node->previous->next = new_node;
	}
	else
	{
		// inserting at first
		ctx->first = new_node;
	}

	// link the provided node back to new_node
	((cc_dnode_t*)node)->previous = new_node;


	ctx->count++;


	return new_node;
}

//===========================================================================
cc_api cc_dnode_t* cc_list_first(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->first;
}

cc_api cc_dnode_t* cc_list_last(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->last;
}

cc_api void* cc_list_node_element(cc_list_t* ctx, const cc_dnode_t* node)
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


#if (1==cc_list_config_enable_node_validation)
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


	return cc_dnode_element((cc_dnode_t*)node);
}

//===========================================================================
cc_api size_t cc_list_count(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->count;
}

cc_api bool cc_list_empty(cc_list_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (ctx->count == 0);
}
