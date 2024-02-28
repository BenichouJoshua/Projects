#ifndef __ILRD_DLL_H__
#define __ILRD_DLL_H__

#include <stddef.h> /* size_t */

typedef int (*dll_matchfunc_t)(const void *list_data, void *match_data);
typedef int (*dll_actionfunc_t)(void *iterator_data, void *user_data);

typedef struct iterator *dll_iterator_t;
typedef struct dll dll_t;


/*
 * DESCRIPTION:
 * Creates an empty doubly linked list.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  none
 *
 * RETURN:
 *  The function returns a pointer to the doubly linked list.
 *  In case of failure, the function returns NULL.
 *
 */
dll_t *DLLCreate(void);

/*
* DESCRIPTION:
*   Destroys the list, unlinks all the iterators and frees their memory.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   list:  list to be destroyed.
*
* RETURN:
*   None.
*/
void DLLDestroy(dll_t *dll);

/*
* DESCRIPTION:
*   Inserts new element to the linkedlist before the iterator pointed
*    to by the iterator.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   list:  pointer to the list to be altered.
*   iterator: iterator to an element, to insert before.
*   data: new value to be inserted to list.
*
* RETURN:
*   Returns the added iterator.
*/
dll_iterator_t DLLInsertBefore(dll_t *dll, dll_iterator_t iterator, void *data);

/*
* DESCRIPTION:
*   remove element from linkedlist.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   list:       Pointer to list to be altered.
*   iterator:   Iterator of the element to remove.
*
* RETURN:
*   Returns an iterator to the position after the removed iterator.
*/
dll_iterator_t DLLRemove(dll_t *dll, dll_iterator_t iterator);

/*
 * DESCRIPTION:
 *   The DLLGetData() function retrieves the data stored in a iterator,
 *   identified by the provided iterator.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  iterator:       A pointer to the iterator from which the data needs to be retieved.
 *
 * RETURN:
 *  A pointer to the data stored in the iterator.
 *  
 */
 
void *DLLGetData(dll_iterator_t iterator);

/*
 * DESCRIPTION:
 *   The DLLSetData() function sets the data of a iterator in to the specified value.
 *   The iterator is identified by the provided iterator.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *   iterator:      A pointer to the iterator that needs to have its data set.
 *   data:          A pointer to the new data value that will be set in the iterator. 
 *
 * RETURN:
 * Nothing to return. 
 */
void DLLSetData(dll_iterator_t iterator, void *data);

/* DESCRIPTION:
*   DLLForEachElement() iterate through a certain section of the list, and for each
*   element in that section - invoke an "action function", using the value of the current element, 
*   and the value that was recieved from the user - as arguments.
* 
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*   from: start of the section.
*   to: end of the section.
*   func: the action function
*
* RETURN:
*   returns 0 if the action function succeeded, 
*   returns Non Zero if the action function failed in one of the iterations. 
*/
int DLLForEachElement(dll_iterator_t from, dll_iterator_t to, void *userdata, dll_actionfunc_t func);

/*O(1)*/
/*
 * DESCRIPTION:
 *  This function cuts a designated part of a doubly linked list and moves it before
 *  target iterator.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  target : insert before target  
 *  from : iterator from which we want to cut 
 *  to : iterator to which we want to cut, not included
 *
 * RETURN:
 *  nothing
 *
 */
void DLLSplice(dll_iterator_t target, dll_iterator_t from, dll_iterator_t to);

/*
 * DESCRIPTION:
 * DLLMultyFind iterate from From to To params and copy all 
 * elements that matched by func to the target iterator (elements inserted before)
 * 
 * TIME COMPLEXITY: 
 * O(n)
 *
 * SPACE COMPLEXITY: 
 * O(1)
 * 
 * PARAMS:
 *  from: start of the section.
 *  to: end of the section.
 *  target: place to copy found elements to, elements placed before target
 *  func: the match function
 *  matchdata: user data for match func
 *
 * RETURN:
 *  0 if operation was successful
 *  Non zero if faild
 */
int DLLMultiFind(dll_t *dll, dll_iterator_t from, dll_iterator_t to, void *matchdata, dll_matchfunc_t func);

/*
 * DESCRIPTION:
 *  Get the next element after given element.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  iterator: Iterator to get the next element.
 *
 * RETURN:
 *  Iterator of the next element.
 *  
 */
dll_iterator_t DLLNext(dll_iterator_t iterator);

/*
 * DESCRIPTION:
 *  Get the element before the given element.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  iterator: Iterator to get the previous element to.
 *
 * RETURN:
 * Iterator of the previous element.
 *  
 */
dll_iterator_t DLLPrev(dll_iterator_t iterator);

/*
 * DESCRIPTION:
 *  Get iterator to first element in the list.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  dll:        doubly linked list to be evaluated
 *
 * RETURN:
 *  returns iterator to first element.
 *  
 */
dll_iterator_t DLLBegin(const dll_t *dll);

/*
 * DESCRIPTION:
 *  Get iterator to last element in the list.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1) 
 * 
 * PARAMS:
 *  dll:        doubly linked list to be evaluated
 *
 * RETURN:
 *  returns iterator to last element.
 *  
 */
dll_iterator_t DLLEnd(const dll_t *dll);

/*O(n)*/
/*
 * DESCRIPTION:
 *  The function counts the number of elements in the list.
 * 
 * TIME COMPLEXITY: 
 *  O(n)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  dll:        The list to count the elements in.
 *
 * RETURN:
 *  Returns the number of elements in the list.
 *
 */
size_t DLLSize(dll_t *dll);

/*
 * DESCRIPTION:
 *  Adds new element to the start of the list
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  dll:        The list to push an element into.
 *  data:       The data to be pushed.  
 *
 * RETURN:
 *  iterator to the pushed element
 *  
 */
dll_iterator_t DLLPushFront(dll_t *dll, void *data);

/*
 * DESCRIPTION:
 * Function inserts a iterator to the tail of the list.
 * 
 * TIME COMPLEXITY: 
 *  0(1)
 * 
 * SPACE COMPLEXITY: 
 * 0(1)
 * 
 * PARAMS:
 * doubly-linked-list 
 * data - data to be inserted
 * RETURN:
 * dll_iterator_t to the inserted iterator.
 *  
 */
dll_iterator_t DLLPushBack(dll_t *dll, void *data);

/*
 * DESCRIPTION:
 *  Removes the element located in the front of the list.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  dll: The list to be evaluated.
 *
 * RETURN:
 * void pointer to the data stored in popped iterator.
 */
void *DLLPopFront(dll_t *dll);

/*
 * DESCRIPTION:
 *  Removes the element located in the back of the list.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  dll:        double linked list to remove from      
 *
 * RETURN:
 * void pointer to the data stored in popped iterator
 *  
 */
void *DLLPopBack(dll_t *dll); 

/*
 * DESCRIPTION:
 *  Determines if the list is empty or not.
 * 
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
 * 
 * PARAMS:
 *  list:   The list to be evaluated.
 *
 * RETURN:
 *  0       : If the given list is not empty.
 *  Non-Zero: If the given list is empty.
 *
 */
int IsDLLEmpty(const dll_t *list);

/*
* DESCRIPTION:
*   IsDLLIterEqual function takes two iterators and compares between them
*   if they are equal.
*
 * TIME COMPLEXITY: 
 *  O(1)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
*
* PARAMS:
*   iterator1 = first iterator to compare
*   iterator2 = second iterator to compare
*
* RETURN:
*   1 if the compared iterators are same.
*   0 if iterators are different    
*/
int IsDLLIterEqual(dll_iterator_t iterator1, dll_iterator_t iterator2);

/*
* DESCRIPTION:
*   Finds the first occurence of an element in a list using a callback function.
*   The function search from the node noted by the iterator 'from' until,
*   not including, the one noted by 'to'.
*
 * TIME COMPLEXITY: 
 *  O(n)
 * 
 * SPACE COMPLEXITY: 
 *  O(1)
*
* PARAMS:
*   dll_iterator_t from:        iterator to the first node the search from.
*   dll_iterator_t to:          iterator the end of search.
*   void *matchdata:        value of element to search for.
*   dll_matchfunc_t func:   callback function to match between data.
*
* RETURN:
*   Returns iterator to the first node whoes value matches query.          
*/
dll_iterator_t DLLFind(dll_iterator_t from, dll_iterator_t to, void *matchdata, dll_matchfunc_t func);

void DLLPrint(dll_t *list);

#endif /* __ILRD_DLL_H__ */