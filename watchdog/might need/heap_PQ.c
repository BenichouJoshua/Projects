/*
	Coder : Josh Benichou
	Date : 02/07/2023
	Reviewer : *****
*/	

/*************************** LIBRARY INCLUDES ******************************/
#include <assert.h> /*asserts*/
#include <stdio.h> /*printf */
#include <stdlib.h> /* malloc free */
#include <string.h> /* memmove */

/*************************** HEADER INCLUDES ******************************/

#include <heap.h> /* our heap API */
#include "heap_PQ.h"
#include "vector.h"

/************************** TYPEDEFS & STRUCTS ****************************/

struct p_queue
{
	heap_t *heap;
};

/************************ STATIC FUNCTIONS DECLARATIONS **********************/


/************************* API FUNCTIONS DEFINITIONS *************************/

p_queue_t *PQueueCreate(priority_comparefunc_t func)
{
	p_queue_t *new_queue = (p_queue_t *)malloc(sizeof(p_queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}

	assert(NULL != func);

	new_queue->heap = HeapCreate(func);
	if (NULL == new_queue->heap)
	{
		free(new_queue);
		return NULL;
	}

	return new_queue;
}

void PQueueDestroy(p_queue_t *queue)
{
	assert(NULL != queue);

	HeapDestroy(queue->heap);
	queue->heap = NULL;
	
	free(queue);
}

int PQueueEnqueue(p_queue_t *queue, void *data)
{
	assert(NULL != queue);
	assert(NULL != data);

	return 0 == HeapPush(queue->heap, data) ? 0 : -1 ;
}

void *PQueueDequeue(p_queue_t *queue)
{
	void *dequeued_data = NULL;
	
	assert(NULL != queue);
	
	dequeued_data = HeapPeek(queue->heap);

	HeapPop(queue->heap);

	return dequeued_data;	
}

size_t PQueueSize(const p_queue_t *queue)
{
	assert(NULL != queue);

	return HeapSize(queue->heap);
}

void *PQueuePeek(p_queue_t *queue)
{
	void *peeked_data = NULL;

	assert(NULL != queue);

	peeked_data = HeapPeek(queue->heap);

	return peeked_data;
}

int IsPQueueEmpty(const p_queue_t *queue)
{
	assert(NULL != queue);

	return IsHeapEmpty(queue->heap);
}

void PQueueClear(p_queue_t *queue)
{
	assert(NULL != queue);

	while (1 != IsPQueueEmpty(queue))
	{
		HeapPop(queue->heap);
	}
}

void *PQueueRemove(p_queue_t *queue, void *matchdata, priority_matchfunc_t matchfunc)
{

	assert(NULL != queue);
	assert(NULL != matchfunc);
	assert(NULL != matchdata);

	return HeapRemove(queue->heap, matchfunc, matchdata);

}

#ifndef NDEBUG
	void PrintQueue(p_queue_t *queue)
	{
		PrintHeap(queue->heap);
	}
#endif
