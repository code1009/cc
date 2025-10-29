#ifndef cc_hash_h
#define cc_hash_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_hash.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef size_t cc_hash_value_t;

typedef cc_hash_value_t (*cc_hash_generate_t)(const void* data);





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api cc_hash_value_t cc_hash_djb2(const void* data, const size_t length);

cc_api size_t cc_hash_probe(const size_t index, const size_t attempt, const size_t size);

cc_api size_t cc_hash_value_index(cc_hash_value_t hash_value, size_t table_size);
cc_api size_t cc_hash_calc_attempt(cc_hash_value_t hash_value, size_t table_size, size_t target);




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_hash_h

