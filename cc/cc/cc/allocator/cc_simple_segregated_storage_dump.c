/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_simple_segregated_storage_dump.c
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <stdio.h>

//===========================================================================
#include "../cc_export.h"

//===========================================================================
#include "cc_simple_segregated_storage.h"

//===========================================================================
#include "cc_simple_segregated_storage_dump.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
static inline int64_t cc_offset_address(void* pointer, uintptr_t base_address)
{
	uintptr_t address = (uintptr_t)pointer;
	
	if (address >= base_address)
	{
		return (int64_t)(address - base_address);
	}
	return -1;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_simple_segregated_storage_dump(cc_simple_segregated_storage_t* simple_segregated_storage, size_t number, uintptr_t base_address)
{
	//-----------------------------------------------------------------------
	printf("# [%lld] %p(%4lld): simple_segregated_storage \n", (int64_t)number, simple_segregated_storage, cc_offset_address(simple_segregated_storage, base_address));


	//-----------------------------------------------------------------------
	printf("- count              = %lld       \n", (int64_t)simple_segregated_storage->count);
	printf("- memory_pointer     = %p(%4lld)  \n", (void*)simple_segregated_storage->memory_pointer, cc_offset_address(simple_segregated_storage->memory_pointer, base_address));
	printf("- memory_size        = %lld bytes \n", (int64_t)simple_segregated_storage->memory_size);
	printf("- data_size          = %lld bytes \n", (int64_t)simple_segregated_storage->data_size);
	printf("- max_count          = %lld       \n", (int64_t)simple_segregated_storage->max_count);
	printf("- chunk_size         = %lld bytes \n", (int64_t)simple_segregated_storage->chunk_size);
	printf("- free_chunk_head    = %p(%4lld)  \n", (void*)simple_segregated_storage->free_chunk_head, cc_offset_address(simple_segregated_storage->free_chunk_head, base_address));


	//-----------------------------------------------------------------------
	cc_simple_segregated_storage_chunk_t* free_chunk;
	size_t free_chunk_count = 0;
	
	size_t free_chunk_index;
	size_t free_chunk_next_index;

	free_chunk = simple_segregated_storage->free_chunk_head;
	while (free_chunk != NULL)
	{
		free_chunk_index = cc_simple_segregated_storage_get_pointer_index(simple_segregated_storage, free_chunk);
		free_chunk_next_index = cc_simple_segregated_storage_get_pointer_index(simple_segregated_storage, free_chunk->next_free_chunk);


		printf(
			"- free_chunk [%3lld]   = %p(%4lld): index=%3lld, next_free_chunk = %3lld : %p(%4lld)\n",
			(int64_t)free_chunk_count,
			(void*)free_chunk, cc_offset_address(free_chunk, base_address), 
			(int64_t)free_chunk_index, (int64_t)free_chunk_next_index,
			(void*)free_chunk->next_free_chunk, cc_offset_address(free_chunk->next_free_chunk, base_address)
			);
		
		free_chunk_count++;


		free_chunk = free_chunk->next_free_chunk;
	}


	printf("- free_chunk_count   = %lld       \n", (int64_t)free_chunk_count);
}
