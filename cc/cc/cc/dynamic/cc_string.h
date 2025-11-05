#ifndef cc_string_h
#define cc_string_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_string.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct _cc_string_t
{
	cc_string_allocator_t* allocator;
	size_t size;
	size_t capacity;
	char* data;
} cc_string_t;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api bool cc_string_create(cc_string_t* ctx, cc_string_allocator_t* allocator);
cc_api bool cc_string_destroy(cc_string_t* ctx);

cc_api bool cc_string_reserve(cc_string_t* ctx, size_t capacity);

cc_api bool cc_string_append(cc_string_t* ctx, const char* s);
cc_api bool cc_string_append_n(cc_string_t* ctx, const char* s, size_t n);
cc_api bool cc_string_append_string(cc_string_t* ctx, const cc_string_t* s);

cc_api bool cc_string_assign(cc_string_t* ctx, const char* s);
cc_api bool cc_string_assign_n(cc_string_t* ctx, const char* s, size_t n);
cc_api bool cc_string_assign_string(cc_string_t* ctx, const cc_string_t* s);

cc_api void cc_string_clear(cc_string_t* ctx);

cc_api const char* cc_string_c_str(const cc_string_t* ctx);
cc_api size_t cc_string_size(const cc_string_t* ctx);
cc_api size_t cc_string_capacity(const cc_string_t* ctx);

cc_api char cc_string_at(const cc_string_t* ctx, size_t index);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_string_h

