/*
	Coder : Josh Benichou
	Date : 02/07/2023
	Reviewer : Liran
*/	


#ifndef __ILRD_PQUEUE_H__
#define __ILRD_PQUEUE_H__

#include <stddef.h> /* size_t */
#include "sorted_linked_list.h" /* my functions */

typedef struct p_queue p_queue_t;
typedef int (*priority_comparefunc_t)(const void *queuedata, void *comparedata);
typedef int (*priority_matchfunc_t)(const void *listdata, void *matchdata);


/*
* DESCRIPTION:
*   Creates an empty priotity queue data structure.
*   Allocates memory for the queue which needs to be cleaned
*   If failed to create returns NULL.
*   
*   Time comlexity O(1)
*   Space complexity O(1)
* 
* PARAMS:
*   None.
*
* RETURN:
*   Reference to priority queue type data structure.
*   NULL if fails.
*/
p_queue_t *PQueueCreate(priority_comparefunc_t func);

/*
* DESCRIPTION:
*   Cleans the priority queue and frees it's memory.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue:  Priority Queue to be destroyed.
*
* RETURN:
*   None.
*/
void PQueueDestroy(p_queue_t *queue);

/*
* DESCRIPTION:
*   Inserts new element to the priority queue, depending on 
*   given priority.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue:  Pointer to the priority queue to be altered.
*   data:   New value to be inserted to the queue.
*
* RETURN:
*   0	if the operation succeeded.
*	-1	if the operation failed.
*/
int PQueueEnqueue(p_queue_t *queue, void *data);

/*
* DESCRIPTION:
*   Removes first element from the priority queue. Dequeue from an empty pqueue is
*   not valid and will lead to undefined behaviour.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue: Pointer to the priority queue to be altered.
*
* RETURN:
*   (void *) to the data of the removed element.
*    
*/
void *PQueueDequeue(p_queue_t *queue);

/*
* DESCRIPTION:
*   Returns a reference to the data of the next-to-be-dequeued element in the
*   priority queue.
* 
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue to be evaluated.
*
* RETURN:
*   Returns reference to next-to-be-dequeued element's data.
*/
void *PQueuePeek(p_queue_t *queue);

/*
* DESCRIPTION:
*   Is priority queue empty.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue: Queue to be evaluated.
*
* RETURN:
*   Non 0 - If the given queue is empty.
*   0 - If the given queue is not empty.
*/  
int IsPQueueEmpty(const p_queue_t *queue);

/*
* DESCRIPTION:
*   Returns the number of items in the priority queue.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue: Priority queue to be evaluated.
*
* RETURN:
*   Returns number of elements in the priority queue.
*/
size_t PQueueSize(const p_queue_t *queue);

/*
* DESCRIPTION:
*   The function clears all the elements of the priority queue.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue: The priority queue to be cleared.
*
* RETURN:
*   None.
*/  
void PQueueClear(p_queue_t *queue);

/*
* DESCRIPTION:
*   The function removes from the queue the element matching 
*   matchdata according to given matchfunc.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   queue:      The priority queue to remove from.
*   matchdata:  The data wanted to be removed.
*   matchfunc:  Matching function to find the element's place.
*
* RETURN:
*   Returns the data removed from the priority queue, if succeed.
*   If fails - returns NULL.
*/  
void *PQueueRemove(p_queue_t *queue, void *matchdata, priority_matchfunc_t matchfunc);

sorted_list_t *GetListInQueue(p_queue_t *queue);

#endif /* __ILRD_PQUEUE_H__ */