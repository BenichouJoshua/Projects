/*************************** LIBRARY INCLUDES ******************************/
#include <assert.h> /*asserts*/
#include <stdio.h> /*printf */
#include <stdlib.h> /* malloc free */
#include <string.h> /* memmove */

/*************************** HEADER INCLUDES ******************************/

#include "heap.h" /* our heap API */
#include "vector.h" /* our vector API */

/************************** TYPEDEFS & STRUCTS ****************************/

#define EMPTY (0)
#define HEAP_ROOT (1)
#define DUMMY (1)
#define PARENT (0)
#define WORDSIZE (sizeof(void *))
#define PARENT_INDEX(index) (index / 2)
#define LEFT_CHILD_INDEX(index) (index * 2)
#define RIGHT_CHILD_INDEX(index) (index * 2 + 1)
#define NO_CHILDREN (0)

typedef enum
{
    FAILURE = -1,
    SUCCESS
} status_t;

struct heap
{
    vector_t *vector;
    heap_comparefunc_t cmp_func;
};

/************************ STATIC FUNCTIONS DECLARATIONS **********************/

static void HeapifyUp(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func);
static void HeapifyDown(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func);
static size_t FindBigger(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func);
static size_t FindElement(vector_t *vector, heap_matchfunc_t match_func, const void *search_data);
static void GetDataNSwap(vector_t *vector, size_t data1_index, size_t data2_index);
static int GetDataNCmp(vector_t *vector, size_t data1_index, size_t data2_index, heap_comparefunc_t cmp_func);
static void SwapVoid(void **element1, void **element2);

/************************* API FUNCTIONS DEFINITIONS *************************/

heap_t *HeapCreate(heap_comparefunc_t cmp_func)
{
    heap_t *new_heap = (heap_t *)malloc(sizeof(heap_t));
    char *dummy = "DUMMY";

    if(NULL == new_heap)
    {
        return NULL;
    }

    assert(cmp_func);

    new_heap->vector = VectorCreate(4, sizeof(void *));
    if(NULL == new_heap->vector)
    {
        return NULL;
    }
    VectorPushBack(new_heap->vector, dummy); /* SET DUMMY VALUE */

    new_heap->cmp_func = cmp_func;

    return new_heap;
}

void HeapDestroy(heap_t *heap)
{
    assert(heap);

    VectorDestroy(heap->vector);
    heap->vector = NULL;

    free(heap);
}

int HeapPush(heap_t *heap, void *data)
{
    size_t pushed_item_index = 0;
    int status = SUCCESS;

    assert(heap);
    assert(data);   

    pushed_item_index = VectorSize(heap->vector);

    status = VectorPushBack(heap->vector, &data);

    HeapifyUp(heap->vector, pushed_item_index, heap->cmp_func);

    return status;
}

void HeapPop(heap_t *heap)
{
    assert(heap);

    if (IsHeapEmpty(heap))
    {
        return;
    }

    GetDataNSwap(heap->vector, HeapSize(heap), HEAP_ROOT);

    VectorPopBack(heap->vector);

    HeapifyDown(heap->vector, HEAP_ROOT, heap->cmp_func); 
}

void *HeapPeek(const heap_t *heap)
{
    void **data = NULL;
    void *to_return = NULL;
    
    assert(heap);

    data = VectorGetAccessToElement(heap->vector, HEAP_ROOT);
    to_return = *data;

    return to_return;
}

size_t HeapSize(const heap_t *heap)
{
    assert(heap);

    return VectorSize(heap->vector) - 1;
}

int IsHeapEmpty(const heap_t *heap)
{
    assert(heap);

    return EMPTY == HeapSize(heap) ? 1 : 0;
}

void *HeapRemove(heap_t *heap, heap_matchfunc_t match_func, const void *search_data)
{
    size_t found_index = 0;
    void **data_to_return = NULL;
    void *temp = NULL;

    assert(heap);
    assert(match_func);

    found_index = FindElement(heap->vector, match_func, search_data);

    if (0 != found_index)
    {
        data_to_return = VectorGetAccessToElement(heap->vector, found_index);
        temp = *data_to_return;
        GetDataNSwap(heap->vector, found_index, HeapSize(heap));

        VectorPopBack(heap->vector);

        HeapifyDown(heap->vector, found_index, heap->cmp_func); 
    }

    return temp;
}

/**************************** ADVANCED FUNCTIONS *****************************/

/*********************** STATIC FUNCTIONS DEFINITIONS ************************/

static void HeapifyUp(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func)
{
    int child = 0;

    assert(vector);
    assert(cmp_func);

    if (HEAP_ROOT >= curr_index)
    {
        return;
    }

    child = GetDataNCmp(vector, PARENT_INDEX(curr_index), curr_index, cmp_func);
    if (PARENT < child)
    {
        GetDataNSwap(vector, curr_index, PARENT_INDEX(curr_index));
        HeapifyUp(vector, PARENT_INDEX(curr_index), cmp_func);
    }

    return;
}

static void HeapifyDown(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func)
{
    size_t biggest_index = 0;

    assert(vector);
    assert(cmp_func);

    biggest_index = FindBigger(vector, curr_index, cmp_func);

    if(biggest_index > curr_index)
    {
        GetDataNSwap(vector, curr_index, biggest_index);
        HeapifyDown(vector, biggest_index, cmp_func);
    }
}

static size_t FindBigger(vector_t *vector, size_t curr_index, heap_comparefunc_t cmp_func)
{
    size_t biggest_index = curr_index;
    size_t heap_size = VectorSize(vector) - DUMMY;

    assert(vector);
    assert(cmp_func);

    if (LEFT_CHILD_INDEX(curr_index) < heap_size 
                && 0 < GetDataNCmp(vector, curr_index, LEFT_CHILD_INDEX(curr_index), cmp_func))
    {
        biggest_index = LEFT_CHILD_INDEX(curr_index);
    }

    if (RIGHT_CHILD_INDEX(curr_index) < heap_size 
                && 0 < GetDataNCmp(vector, biggest_index, RIGHT_CHILD_INDEX(curr_index), cmp_func))
    {
        biggest_index = RIGHT_CHILD_INDEX(curr_index);
    }

    return biggest_index;
}

static int GetDataNCmp(vector_t *vector, size_t data1_index, size_t data2_index, heap_comparefunc_t cmp_func)
{
    void **data1 = VectorGetAccessToElement(vector, data1_index);
    void **data2 = VectorGetAccessToElement(vector, data2_index);

    assert(vector);
    assert(cmp_func);

    return cmp_func(*data1, *data2);
}

static void GetDataNSwap(vector_t *vector, size_t data1_index, size_t data2_index)
{
    assert(vector);

    SwapVoid(VectorGetAccessToElement(vector, data1_index), 
                        VectorGetAccessToElement(vector, data2_index));
}

static void SwapVoid(void **element1, void **element2) 
{
	void *temp = NULL;

	assert(NULL != element1);
	assert(NULL != element2);

    temp = *element1;
    *element1 = *element2;
    *element2 = temp;
}

static size_t FindElement(vector_t *vector, heap_matchfunc_t match_func, const void *search_data)
{
    size_t heap_size = VectorSize(vector);
    size_t index = 1;
    void **vector_data = NULL;

    assert(vector);
    assert(match_func);
    
    while (index < heap_size)
    {
        vector_data = VectorGetAccessToElement(vector, index);

        if (match_func(*vector_data, (void *)search_data))
        {     
            return index;
        }
        ++index;
    }

    return 0;
}


/*************************** DEBUG UTILS  ****************************/

#ifndef NDEBUG
    void PrintHeap(heap_t *heap)
    {
        void **data = NULL;
        int *temp = NULL;
        size_t heap_size = HeapSize(heap);
        size_t index = 1;

        while (index <= heap_size)
        {
            data = VectorGetAccessToElement(heap->vector, index);
            temp = (int *)(*data);
            printf("%d -> ", *temp);
            ++index;
        }
        printf("\n");
    }
#endif