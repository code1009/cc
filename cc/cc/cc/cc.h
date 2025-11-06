#ifndef cc_h
#define cc_h

/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc.h
// 
// # Created by: code1009
// # Created on: 09-18, 2025.
// 
// # Description:
//   @ c container
// 
/////////////////////////////////////////////////////////////////////////////
//===========================================================================





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "cc_import.h"

//===========================================================================
#include "cc_version.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include "allocator/cc_simple_segregated_storage.h"
#include "allocator/cc_simple_segregated_storage_dump.h"

#include "allocator/cc_fallocator.h"
#include "allocator/cc_fallocator_adapter.h"

//===========================================================================
#include "algorithm/cc_hash.h"
#include "algorithm/cc_compare.h"

//===========================================================================
#include "element/cc_element.h"
#include "element/cc_pair.h"
#include "element/cc_snode.h"
#include "element/cc_dnode.h"
#include "element/cc_bucket.h"
#include "element/cc_pair_bucket.h"

//===========================================================================
#include "container/cc_collection.h"
#include "container/cc_pair_collection.h"

//===========================================================================
#include "container/cc_vector.h"
#include "container/cc_deque.h"
#include "container/cc_queue.h"
#include "container/cc_stack.h"
#include "container/cc_forward_list.h"
#include "container/cc_list.h"
#include "container/cc_set.h"
#include "container/cc_map.h"
#include "container/cc_unordered_set.h"
#include "container/cc_unordered_map.h"
#include "container/cc_priority_queue.h"

//===========================================================================
#include "algorithm/cc_sort.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if (1==cc_config_dynamic)

#include "dynamic/cc_first_fit.h"
#include "dynamic/cc_first_fit_dump.h"

#include "dynamic/cc_lf_heap.h"
#include "dynamic/cc_lf_heap_dump.h"

#include "dynamic/cc_vallocator.h"
#include "dynamic/cc_vallocator_adapter.h"

#include "dynamic/cc_heap_memory.h"
#include "dynamic/cc_heap_memory_dump.h"

#include "dynamic/cc_string_heap_memory.h"
#include "dynamic/cc_string.h"

#endif // (1==cc_config_dynamic)





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#endif // cc_h

