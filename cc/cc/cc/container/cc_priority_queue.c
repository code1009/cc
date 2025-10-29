/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_priority_queue.c
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

#include "../element/cc_element.h"

#include "cc_collection.h"

//===========================================================================
#include "cc_priority_queue.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/* 
binary heap (min-heap): 부모가 자식보다 작아야 함.
less(a,b) == true  => a < b
*/
static inline size_t cc_priority_queue_parent_index(const size_t i)
{ 
    return (i - 1) / 2; 
}

static inline size_t cc_priority_queue_left_index(const size_t i)
{ 
    return 2 * i + 1; 
}

static inline size_t cc_priority_queue_right_index(const size_t i)
{ 
    return 2 * i + 2; 
}

static inline void cc_priority_queue_heapify_up(cc_priority_queue_t* ctx, size_t index)
{
    cc_debug_assert(ctx != NULL);
    cc_debug_assert(ctx->less != NULL);


    size_t current_index = index;
    while (current_index > 0)
    {
        size_t parent_index = cc_priority_queue_parent_index(current_index);

        void* current = cc_element_get(&ctx->collection.elements[current_index]);
        void* parent = cc_element_get(&ctx->collection.elements[parent_index]);

        // 현재가 부모보다 작으면 위로
        if (true == ctx->less(current, parent))
        {
            cc_element_swap(&ctx->collection.elements[current_index], &ctx->collection.elements[parent_index]);
            current_index = parent_index;
        }
        else
        {
            break;
        }
    }
}

static inline void cc_priority_queue_heapify_down(cc_priority_queue_t* ctx, size_t index)
{
    cc_debug_assert(ctx != NULL);
    cc_debug_assert(ctx->less != NULL);


    size_t count = ctx->collection.count;
    size_t current_index = index;
    for (;;)
    {
        size_t left_index = cc_priority_queue_left_index(current_index);
        size_t right_index = cc_priority_queue_right_index(current_index);

        if (left_index >= count)
        {
            // 자식 없음
            break;
        }

        // 더 작은 자식을 선택
        size_t smallest_index = left_index;
        if (right_index < count)
        {
            void* left = cc_element_get(&ctx->collection.elements[left_index]);
            void* right = cc_element_get(&ctx->collection.elements[right_index]);
            if (true == ctx->less(right, left))
            {
                smallest_index = right_index;
            }
        }

        void* current = cc_element_get(&ctx->collection.elements[current_index]);
        void* child = cc_element_get(&ctx->collection.elements[smallest_index]);

        if (true == ctx->less(child, current))
        {
            cc_element_swap(&ctx->collection.elements[current_index], &ctx->collection.elements[smallest_index]);
            current_index = smallest_index;
        }
        else
        {
            break;
        }
    }
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
cc_api void cc_priority_queue_initialize(cc_priority_queue_t* ctx, const cc_less_t less, const cc_element_t* elements, const size_t max_count, const uintptr_t param)
{
	cc_debug_assert(ctx != NULL);


	ctx->less = less;
	cc_collection_initialize(&ctx->collection, elements, max_count, param);
}

cc_api uintptr_t cc_priority_queue_param(cc_priority_queue_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_param(&ctx->collection);
}

//===========================================================================
cc_api bool cc_priority_queue_push(cc_priority_queue_t* ctx, const void* element)
{
	cc_debug_assert(ctx != NULL);


    size_t index = ctx->collection.count;
    if (index >= ctx->collection.max_count)
    {
        return false;
    }


    // 마지막 위치에 추가하고 heapify up
	cc_element_set(&ctx->collection.elements[index], element);


    ctx->collection.count++;
    cc_priority_queue_heapify_up(ctx, index);

    return true;
}

cc_api void* cc_priority_queue_pop(cc_priority_queue_t* ctx)
{
	cc_debug_assert(ctx != NULL);


    if (ctx->collection.count == 0)
    {
        return NULL;
    }


    void* top = cc_element_get(&ctx->collection.elements[0]);


    size_t last_index = ctx->collection.count - 1;
    if (last_index == 0)
    {
        // 한 개만 있던 경우
        ctx->collection.count = 0;
        return top;
    }


    // 마지막 요소를 루트로 옮기고 카운트 감소 후 heapify down
    cc_element_set(&ctx->collection.elements[0], cc_element_get(&ctx->collection.elements[last_index]));
    

    ctx->collection.count--;
    cc_priority_queue_heapify_down(ctx, 0);

    return top;
}

//===========================================================================
cc_api void* cc_priority_queue_top_element(cc_priority_queue_t* ctx)
{
	cc_debug_assert(ctx != NULL);


    if (ctx->collection.count == 0)
    {
        return NULL;
    }


    return cc_element_get(&ctx->collection.elements[0]);
}

//===========================================================================
cc_api size_t cc_priority_queue_count(cc_priority_queue_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_count(&ctx->collection);
}

cc_api bool cc_priority_queue_empty(cc_priority_queue_t* ctx)
{
	cc_debug_assert(ctx != NULL);


	return cc_collection_empty(&ctx->collection);
}
