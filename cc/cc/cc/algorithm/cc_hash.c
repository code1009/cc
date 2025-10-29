/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_unordered_set.c
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
#include "cc_hash.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define cc_hash_config_linear_probing     1
#define cc_hash_config_quadratic_probing  0





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api cc_hash_value_t cc_hash_djb2(const void* data, const size_t length)
{
	cc_debug_assert(data != NULL);
	cc_debug_assert(length != 0);


	cc_hash_value_t hash = 5381;
	uint8_t* ptr = (uint8_t*)data;
	for (size_t i = 0; i < length; i++)
	{
		hash = ((hash << 5) + hash) + ptr[i]; // hash * 33 + c
	}

	return hash;
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/*

@ Open Addressing hash methods
- Linear Probing
- Quadratic Probing
- Double Hashing

@ Separate Chaining hash methods
- Linked List: C++ STL unordered_map/unordered_set

*/

//===========================================================================
#if (1==cc_hash_config_linear_probing)
// Linear probing
static size_t cc_hash_linear_probe(const size_t index, const size_t attempt, const size_t size)
{
	cc_debug_assert(size != 0);

	// h(i) = (index + i) % size
	return (index + attempt) % size;
}
#endif

#if (1==cc_hash_config_quadratic_probing)
// Quadratic probing
static size_t cc_hash_quadratic_probe(const size_t index, const size_t attempt, const size_t size)
{
	cc_debug_assert(size != 0);

	// h(i) = (index + i + i^2) % size : 50% 이상 데이터를 채우면 삽입할 공간을 찾지 못하는 경우가 발생할 수 있음
	//return (index + attempt + (attempt * attempt)) % size;

	// h(i) = (index + i^2) % size
	return (index + (attempt * attempt)) % size;
}
#endif

cc_api size_t cc_hash_probe(const size_t index, const size_t attempt, const size_t size)
{
#if   (1==cc_hash_config_linear_probing)
	return cc_hash_linear_probe(index, attempt, size);
#elif (1==cc_hash_config_quadratic_probing)
	return cc_hash_quadratic_probe(index, attempt, size);
#endif
}




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api size_t cc_hash_value_index(cc_hash_value_t hash_value, size_t table_size)
{
	cc_debug_assert(table_size != 0);


	size_t index = hash_value % table_size;


	return index;
}

cc_api size_t cc_hash_calc_attempt(cc_hash_value_t hash_value, size_t table_size, size_t target)
{
	cc_debug_assert(table_size != 0);


	size_t index = cc_hash_value_index(hash_value, table_size);


	for (size_t attemp = 0; attemp < table_size; attemp++)
	{
		if (cc_hash_probe(index, attemp, table_size) == target)
		{
			return attemp;
		}
	}

	return cc_invalid_index;
}
