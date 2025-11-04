/////////////////////////////////////////////////////////////////////////////
// 
// # File: cc_sort.c
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

#include "../container/cc_collection.h"

//===========================================================================
#include "cc_sort.h"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
/* 

In-place quicksort with tail-recursion elimination:
- Pivot: middle element
- Always recurse on the smaller partition and loop on the larger one
  to guarantee O(log n) maximum recursion depth. 

- 함수 호출 스택 크기 계산(32bit)
  함수 인자      크기: 4 * 4개 = 16 bytes
  함수 내부 변수 크기: 6 * 4개 = 24 bytes
  리턴 주소      크기: 4 bytes
  프레임 포인터  크기: 4 bytes
  총 스택 프레임 크기: 16 + 24 + 4 + 4 = 48 bytes

- 최대 재귀 깊이: log2(n)

- 1024개 요소 정렬 시 최대 스택 사용량:
- log2( 1024) * 48 = 10 * 48 = 480 bytes
- log2(65536) * 48 = 16 * 48 = 768 bytes

*/
#if (1==cc_config_algorithm_quicksort)
static void cc_collection_quicksort_range(cc_collection_t* ctx, const cc_less_t less, ptrdiff_t left, ptrdiff_t right)
{
	while (left < right)
	{
		// Choose pivot as middle element
		ptrdiff_t mid = (left + right) / 2;
		void* pivot = cc_element_get(&ctx->elements[mid]);

		ptrdiff_t i = left;
		ptrdiff_t j = right;

		// Partition
		while (i <= j)
		{
			while (less(cc_element_get(&ctx->elements[i]), pivot))
			{
				i++;
			}
			while (less(pivot, cc_element_get(&ctx->elements[j])))
			{
				j--;
			}
			if (i <= j)
			{
				cc_element_swap(&ctx->elements[i], &ctx->elements[j]);
				i++;
				j--;
			}
		}

		// Now [left..j] and [i..right] are the two partitions.
		// Recurse on the smaller one to limit recursion depth,
		// and continue loop on the larger partition.
		ptrdiff_t left_size = (j >= left) ? (j - left + 1) : 0;
		ptrdiff_t right_size = (right >= i) ? (right - i + 1) : 0;

		if (left_size == 0 && right_size == 0)
		{
			// Nothing more to sort in this range
			break;
		}

		if (left_size < right_size)
		{
			if (left < j)
			{
				cc_collection_quicksort_range(ctx, less, left, j); // smaller side recursive
			}
			// Iterate on the larger side
			left = i;
		}
		else
		{
			if (i < right)
			{
				cc_collection_quicksort_range(ctx, less, i, right); // smaller side recursive
			}
			// Iterate on the larger side
			right = j;
		}
	}
}

cc_api void cc_collection_quicksort(cc_collection_t* ctx, const cc_less_t less)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(less != NULL);


	if (ctx->count <= 1)
	{
		return;
	}

	cc_collection_quicksort_range(ctx, less, 0, (ptrdiff_t)ctx->count - 1);
}
#endif





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// heapsort (in-place, non-recursive)
// - Uses cc_less_t less(a,b) which returns true when a < b.
// - Build max-heap using comparator: if less(a,b) == true then b is greater.
// - Then repeatedly swap root(max) with last element and shrink heap.
static void cc_collection_heapify_down(cc_collection_t* ctx, const cc_less_t less, size_t index, size_t heap_size)
{
	for (;;)
	{
		size_t left = index * 2 + 1;
		if (left >= heap_size)
		{
			break;
		}

		size_t right = left + 1;
		size_t largest = index;

		// compare current largest vs left child
		void* largest_val = cc_element_get(&ctx->elements[largest]);
		void* left_val = cc_element_get(&ctx->elements[left]);
		if (less(largest_val, left_val))
		{
			largest = left;
		}

		// compare current largest vs right child (if exist)
		if (right < heap_size)
		{
			void* largest_now_val = cc_element_get(&ctx->elements[largest]);
			void* right_val = cc_element_get(&ctx->elements[right]);
			if (less(largest_now_val, right_val))
			{
				largest = right;
			}
		}

		if (largest == index)
		{
			break;
		}

		cc_element_swap(&ctx->elements[index], &ctx->elements[largest]);
		index = largest;
	}
}

cc_api void cc_collection_heapsort(cc_collection_t* ctx, const cc_less_t less)
{
	cc_debug_assert(ctx != NULL);
	cc_debug_assert(less != NULL);


	size_t n = ctx->count;
	if (n <= 1)
	{
		return;
	}


	// Build max-heap
	// last parent = (n/2) - 1
	for (size_t i = (n / 2); i > 0; i--)
	{
		size_t index = i - 1;
		cc_collection_heapify_down(ctx, less, index, n);
	}

	// Extract elements from heap one by one
	for (size_t end = n; end > 1; end--)
	{
		// swap max (index 0) with element at end-1
		cc_element_swap(&ctx->elements[0], &ctx->elements[end - 1]);


		// heapify root in reduced heap [0 .. end-2]
		cc_collection_heapify_down(ctx, less, 0, end - 1);
	}
}
