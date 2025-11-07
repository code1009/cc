/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "../allocator/cc_simple_segregated_storage.h"
#include "cc_first_fit.h"
#include "cc_lf_heap.h"

#include "cc_vallocator.h"

//===========================================================================
#include "cc_string.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline void cc_string_strncpy(char* dest, const char* src, size_t size)
{
	cc_debug_assert(dest != NULL);
	cc_debug_assert(src != NULL);


	for (size_t i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}

static inline size_t cc_string_strlen(const char* src)
{
	if (src == NULL)
	{
		return 0;
	}


	size_t length = 0;
	char* p = (char*)src;
	while (*p != '\0')
	{
		p++;
		length++;
	}

	return length;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline size_t cc_string_next_capacity(size_t current, size_t needed)
{
	if (current == 0) current = 1;
	while (current < needed) current <<= 1;
	return current;
}

static inline char* cc_string_allocate(cc_string_t* ctx, const size_t size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(size != 0);

	
	return (char*)ctx->allocator->allocate(ctx->allocator->handle, size);
}

static inline bool cc_string_free(cc_string_t* ctx, char* data)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(data != NULL);


	bool rv = ctx->allocator->free(ctx->allocator->handle, data);
	cc_debug_assert(rv != false);
	return rv;
}

static inline char* cc_string_reallocate(cc_string_t* ctx, char* old_data, const size_t old_size, const size_t new_size)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(old_data != NULL);
	cc_debug_assert(old_size != 0);
	cc_debug_assert(new_size != 0);


	char* new_data = cc_string_allocate(ctx, new_size);
	if (new_data != NULL)
	{
		size_t copy_size = (old_size < new_size) ? old_size : new_size;
		cc_string_strncpy(new_data, old_data, copy_size);
		cc_string_free(ctx, old_data);
	}
	return new_data;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_string_create(cc_string_t* ctx, cc_vallocator_t* allocator)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(allocator != NULL);


	if (allocator == NULL)
	{
		return false;
	}


	ctx->allocator = allocator;
	ctx->length = 0;
	ctx->capacity = 0;
	ctx->data = NULL;


	return true;
}

cc_api bool cc_string_destroy(cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	bool rv = true;
	if (ctx->data)
	{
		rv = cc_string_free(ctx, ctx->data);
	}

	ctx->allocator = NULL;
	ctx->length = 0;
	ctx->capacity = 0;
	ctx->data = NULL;

	return rv;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_string_reserve(cc_string_t* ctx, size_t capacity)
{
	cc_debug_assert(ctx != NULL);


	if (capacity == 0) return true; // nothing to do

	// keep space for null terminator
	size_t need_bytes = capacity + 1;

	if (ctx->capacity >= capacity)
	{
		return true;
	}

	// choose growth policy (power-of-two)
	size_t current_bytes = (ctx->capacity == 0) ? 0 : (ctx->capacity + 1);
	size_t new_bytes = cc_string_next_capacity(current_bytes, need_bytes);
	size_t new_capacity = (new_bytes > 0) ? (new_bytes - 1) : 0;

	if (ctx->data == NULL)
	{
		char* p = cc_string_allocate(ctx, new_bytes);
		if (p == NULL) return false;
		p[0] = '\0';
		ctx->data = p;
		ctx->capacity = new_capacity;
		return true;
	}
	else
	{
		// reallocate existing buffer; pass old buffer size in bytes
		char* p = cc_string_reallocate(ctx, ctx->data, ctx->capacity + 1, new_bytes);
		if (p == NULL) return false;
		ctx->data = p;
		ctx->capacity = new_capacity;
		// ensure null terminator preserved (reallocate copies min(old,new) bytes)
		if (ctx->length > ctx->capacity) ctx->length = ctx->capacity;
		ctx->data[ctx->length] = '\0';
		return true;
	}
}

//===========================================================================
cc_api bool cc_string_append_n(cc_string_t* ctx, const char* s, size_t n)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	if (n == 0) return true;

	size_t needed = ctx->length + n;
	if (needed > ctx->capacity)
	{
		if (!cc_string_reserve(ctx, needed)) return false;
	}

	// copy n bytes
	cc_string_strncpy(ctx->data + ctx->length, s, n);
	ctx->length = needed;
	ctx->data[ctx->length] = '\0';
	return true;
}

cc_api bool cc_string_append(cc_string_t* ctx, const char* s)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	size_t n = cc_string_strlen(s);
	return cc_string_append_n(ctx, s, n);
}

cc_api bool cc_string_append_string(cc_string_t* ctx, const cc_string_t* s)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	return cc_string_append_n(ctx, s->data, s->length);
}

//===========================================================================
cc_api bool cc_string_assign_n(cc_string_t* ctx, const char* s, size_t n)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	// ensure capacity then copy
	if (!cc_string_reserve(ctx, n)) return false;

	cc_string_strncpy(ctx->data, s, n);
	ctx->length = n;
	ctx->data[ctx->length] = '\0';
	return true;
}

cc_api bool cc_string_assign(cc_string_t* ctx, const char* s)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	size_t n = cc_string_strlen(s);
	return cc_string_assign_n(ctx, s, n);
}

cc_api bool cc_string_assign_string(cc_string_t* ctx, const cc_string_t* s)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(s != NULL);


	return cc_string_assign_n(ctx, s->data, s->length);
}

//===========================================================================
cc_api void cc_string_clear(cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->length = 0;
	if (ctx->data) ctx->data[0] = '\0';
}

//===========================================================================
cc_api const char* cc_string_c_str(const cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	static const char empty_string = '\0';
	if (ctx->data) return ctx->data;
	return &empty_string;
}

cc_api size_t cc_string_length(const cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->length;
}

cc_api size_t cc_string_capacity(const cc_string_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->capacity;
}

//===========================================================================
cc_api char cc_string_at(const cc_string_t* ctx, size_t index)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(index < ctx->length);


	if (index >= ctx->length)
	{
		return '\0';
	}
	return ctx->data[index];
}

