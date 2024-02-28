/*
	Coder : Josh Benichou
	Date : 02/07/2023
	Reviewer : Liran
*/	


#include<stdio.h> /* printf */
#include <stdlib.h> /* malloc free */
#include <assert.h> /* asserts */

#include "priority_queue.h" /* my functions */
#include "sorted_linked_list.h" /* my functions */

struct p_queue
{
	sorted_list_t *queue;
};

p_queue_t *PQueueCreate(priority_comparefunc_t func)
{
	p_queue_t *new_queue = (p_queue_t *)malloc(sizeof(p_queue_t));
	if (NULL == new_queue)
	{
		return NULL;
	}

	assert(NULL != func);

	new_queue->queue = SortedListCreate(func);
	if (NULL == new_queue->queue)
	{
		free(new_queue);
		return NULL;
	}

	return new_queue;
}

void PQueueDestroy(p_queue_t *queue)
{
	assert(NULL != queue);

	SortedListDestroy(queue->queue);
	queue->queue = NULL;
	free(queue);
}

int PQueueEnqueue(p_queue_t *queue, void *data)
{
	assert(NULL != queue);
	assert(NULL != data);

	return NULL != SortedListInsert(queue->queue, data).iter ? 0 : -1 ;
}

void *PQueueDequeue(p_queue_t *queue)
{
	assert(NULL != queue);

	return SortedListPopBack(queue->queue);
}

size_t PQueueSize(const p_queue_t *queue)
{
	assert(NULL != queue);

	return SortedListCount(queue->queue);
}

void *PQueuePeek(p_queue_t *queue)
{
	assert(NULL != queue);

	return SortedListGetData(SortedListPrev(SortedListEnd(queue->queue)));
}

int IsPQueueEmpty(const p_queue_t *queue)
{
	assert(NULL != queue);

	return IsSortedListEmpty(queue->queue);
}


void PQueueClear(p_queue_t *queue)
{
	assert(NULL != queue);

	while (1 != IsPQueueEmpty(queue))
	{
		PQueueDequeue(queue);
	}
}

void *PQueueRemove(p_queue_t *queue, void *matchdata, priority_matchfunc_t matchfunc)
{
	void *removed_data = NULL;
	sorted_iter_t found = {0};

	assert(NULL != queue);
	assert(NULL != matchfunc);
	assert(NULL != matchdata);

	found = SortedListFindIf(SortedListBegin(queue->queue), 
							SortedListEnd(queue->queue), matchdata, matchfunc);

	if (0 == IsSortedListIterEqual(found, SortedListEnd(queue->queue)))
	{
		removed_data = SortedListGetData(found);
		SortedListRemove(found);
	}

	return removed_data;
}

void PQueuePrint(const p_queue_t *queue)
{
	assert(NULL != queue);

	SortedListPrint(queue->queue);
}

sorted_list_t *GetListInQueue(p_queue_t *queue)
{
	return queue->queue;
}