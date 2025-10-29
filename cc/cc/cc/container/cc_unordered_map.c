/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_unordered_map.c
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
#include "../algorithm/cc_compare.h"
#include "../algorithm/cc_hash.h"

#include "../element/cc_element.h"
#include "../element/cc_pair.h"
#include "../element/cc_bucket.h"
#include "../element/cc_pair_bucket.h"

//===========================================================================
#include "cc_unordered_map.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_unordered_map_initialize(cc_unordered_map_t* ctx, const cc_hash_generate_t generate, const cc_equal_t equal, const cc_pair_bucket_t* table, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(generate != NULL);
	cc_debug_assert(equal != NULL);
	cc_debug_assert(table != NULL);
	cc_debug_assert(max_count != 0);


	ctx->generate = generate;
	ctx->equal = equal;
	ctx->table = (cc_pair_bucket_t*)table;
	ctx->max_count = max_count;
	ctx->param = param;

	ctx->count = 0;


	size_t i;
	for(i = 0; i < ctx->max_count; i++)
	{
		cc_pair_bucket_initialize(&ctx->table[i]);
	}
}

cc_api uintptr_t cc_unordered_map_param(cc_unordered_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->param;
}

//===========================================================================
cc_api void cc_unordered_map_clear(cc_unordered_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	ctx->count = 0;


	for (size_t i = 0; i < ctx->max_count; i++)
	{
		cc_pair_bucket_clear(&ctx->table[i]);
	}
}

cc_api bool cc_unordered_map_erase(cc_unordered_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->max_count)
	{
		cc_pair_bucket_t* bucket = &ctx->table[index];

		if (cc_pair_bucket_status(bucket) == cc_bucket_status_filled)
		{
			cc_pair_bucket_remove(bucket);
			ctx->count--;

			return true;
		}

		return false;
	}

	return false;
}

cc_api bool cc_unordered_map_add(cc_unordered_map_t* ctx, const void* first, const void* second)
{
	cc_debug_assert(ctx != NULL);


	if (ctx->count >= ctx->max_count)
	{
		return false;
	}


	cc_hash_value_t hash_value = ctx->generate(first);
	size_t index = cc_hash_value_index(hash_value, ctx->max_count);


	for(size_t attempt = 0; attempt < ctx->max_count; attempt++)
	{
		size_t probe_index = cc_hash_probe(index, attempt, ctx->max_count);
		cc_pair_bucket_t* bucket = &ctx->table[probe_index];
		cc_bucket_status_t status = cc_pair_bucket_status(bucket);


		if ( (status == cc_bucket_status_empty) ||
			 (status == cc_bucket_status_removed) )
		{
			cc_pair_bucket_fill(bucket, first, second);
			ctx->count++;

			return true;
		}
		else if(status == cc_bucket_status_filled)
		{
			void* existing_element_first = cc_pair_bucket_element_first(bucket);
			if(ctx->equal(existing_element_first, first))
			{
				// already exists, do not add
				return false;
			}

			continue;
		}
		else
		{
			return false;
		}
	}

	return false;
}

//===========================================================================
cc_api cc_pair_bucket_t* cc_unordered_map_at(cc_unordered_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->max_count)
	{
		return &ctx->table[index];
	}

	return NULL;
}

cc_api void* cc_unordered_map_element_first(cc_unordered_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->max_count)
	{
		return cc_pair_bucket_element_first(&ctx->table[index]);
	}

	return NULL;
}

cc_api void* cc_unordered_map_element_second(cc_unordered_map_t* ctx, const size_t index)
{
	cc_debug_assert(ctx != NULL);


	if (index < ctx->max_count)
	{
		return cc_pair_bucket_element_second(&ctx->table[index]);
	}

	return NULL;
}

cc_api size_t cc_unordered_map_find(cc_unordered_map_t* ctx, const void* first)
{
	cc_debug_assert(ctx != NULL);


	cc_hash_value_t hash_value = ctx->generate(first);
	size_t index = cc_hash_value_index(hash_value, ctx->max_count);


	for (size_t attempt = 0; attempt < ctx->max_count; attempt++)
	{
		size_t probe_index = cc_hash_probe(index, attempt, ctx->max_count);
		cc_pair_bucket_t* bucket = &ctx->table[probe_index];
		cc_bucket_status_t status = cc_pair_bucket_status(bucket);


		if (status == cc_bucket_status_filled)
		{
			void* existing_element_first = cc_pair_bucket_element_first(bucket);
			if (ctx->equal(existing_element_first, first))
			{
				return probe_index;
			}
		}
		else if (status == cc_bucket_status_removed)
		{
			continue;
		}
		/*
		else if (status == cc_bucket_status_empty)
		{
			return cc_invalid_index;
		}
		*/
		else
		{
			return cc_invalid_index;
		}
	}


	return cc_invalid_index;
}

cc_api void* cc_unordered_map_element_second_by_first(cc_unordered_map_t* ctx, const void* first)
{
	cc_debug_assert(ctx != NULL);


	size_t index = cc_unordered_map_find(ctx, first);
	if (index != cc_invalid_index)
	{
		return cc_pair_bucket_element_second(&ctx->table[index]);
	}


	return NULL;
}

//===========================================================================
cc_api size_t cc_unordered_map_count(cc_unordered_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->count;
}

cc_api bool cc_unordered_map_empty(cc_unordered_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return (ctx->count == 0) ? true : false;
}

cc_api size_t cc_unordered_map_table_size(cc_unordered_map_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return ctx->max_count;
}
