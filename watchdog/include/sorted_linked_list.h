#ifndef __ILRD_SORTED_LIST_H__
#define __ILRD_SORTED_LIST_H__
#include <stddef.h> /* size_t */
#include "d_linked_list.h"

typedef int (*sort_comparefunc_t)(const void *listdata, void *comparedata);
typedef int (*sort_matchfunc_t)(const void *listdata, void *matchdata);
typedef int (*sort_actionfunc_t)(void *iteratordata, void *userdata);

typedef struct sorted_iterator sorted_iter_t;
typedef struct sorted_list sorted_list_t;  

struct sorted_iterator
{
    dll_iterator_t iter;

    #ifndef NDEBUG
        sorted_list_t *list;
    #endif  
};

/*
* DESCRIPTION:
* Creates a new sorted list, accordig to the compare function  
*   
*   Time comlexity O(1)
*   Space complexity O(1)
* 
* PARAMS:
*   func - Compare function to sort the list.
*
* RETURN:
*   Reference to linkedlist type data structure. 
*   NULL if fails.
*/
sorted_list_t *SortedListCreate(sort_comparefunc_t func);

/*
* DESCRIPTION:
*   Destroys the list, unlinks all the nodes and frees their memory.
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
void SortedListDestroy(sorted_list_t *list);

/*
* DESCRIPTION:
*   Function retrieves the data stored in a node,
*   identified by the provided iterator.
* 
*   Time Complexity	: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   iterator 	: A pointer to the node from which the data needs to be retieved.
*
* RETURN: 
*    A pointer to the data stored in the node.
*/
void *SortedListGetData(sorted_iter_t iterator);

/*
* DESCRIPTION:
*   Inserts new element to the sorted list in the right place,
*   according to the order dictated by the compare function of the list.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*	list: pointer to the list to be altered.
* 	data: new value to be inserted to list.
*
* RETURN:
*	Returns iterator to the added iterator.
*/
sorted_iter_t SortedListInsert(sorted_list_t *list, void *data);

/*
* DESCRIPTION:
*   Remove element from linkedlist.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*   iterator:   Iterator of the element to remove.
*
* RETURN:
*   Returns an iterator to the position after the removed node.
*/
sorted_iter_t SortedListRemove(sorted_iter_t iterator);

/*
* DESCRIPTION:
*   Returns number of items in list.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*	list:  list to be evaluated.
*
* RETURN:
*	Returns number of elements in LinkedList.
*/
size_t SortedListCount(const sorted_list_t *list);

/*
* DESCRIPTION:
*   Finds the first occurence of an element in a list using the compare function.
*   The function search from the node noted by the iterator 'from' until,
*   not including, the one noted by 'to'.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*	from:       Iterator to the first node to search from.
*   to:         Iterator to the end of search.
*   matchdata:  Value of the element to search for.
*   list:       The list the function operates on.
*
* RETURN:
*	Returns iterator to the first node whoes value matches query.          
*/
sorted_iter_t SortedListFind(sorted_list_t *list, sorted_iter_t from, sorted_iter_t to, void *matchdata);

/*
* DESCRIPTION:
*   Finds the first occurence of an element in a list using the callback match function.
*   The function search from the node noted by the iterator 'from' until,
*   not including, the one noted by 'to'.
*
*   Time complexity: O(n)
*   Space Complexity: O(1)
*
* PARAMS:
*	iterator_t from:    iterator to the first node the search from.
*   iterator_t to:      iterator the end of search.
*   void *matchdata:    value of element to search for.
*   sort_matchfunc_t func:   callback function to match between data.
*
* RETURN:
*	Returns iterator to the first node whoes value matches query.          
*/
sorted_iter_t SortedListFindIf(sorted_iter_t from, sorted_iter_t to, void *matchdata, sort_matchfunc_t func);
    
/* DESCRIPTION:
*   SortedForEachElement() iterate through a certain section of the
*   list, and for each element in that section - invoke an 
*   "action function", using the value of the current element, 
*   and the value that was recieved from the user - as arguments.
*   (*) Be aware that your "action function" might damage the
*       order of your original list.
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
*   returns 1 if the action function failed in one of the iterations. 
*/
int SortedListForEachElement(sorted_iter_t from, sorted_iter_t to, void *userdata, sort_actionfunc_t func);

/*
* DESCRIPTION:
*   Get iterator to first element in the list.
* 
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*	list:   list to be evaluated.
*
* RETURN:
*	returns iterator to first element
*/
sorted_iter_t SortedListBegin(const sorted_list_t *list);

/*
* DESCRIPTION:
*   Get iterator to last element in the list.
*   
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*	list: list to be evaluated.
*
* RETURN:
*	returns iterator to last element
*/
sorted_iter_t SortedListEnd(const sorted_list_t *list);

/*
* DESCRIPTION:
*   Get the next element after given element.
*   
*   Time complexity:    O(1)
*   Space Complexity:   O(1)
*
* PARAMS:
*   iterator: Iterator to get the next element.
*
* RETURN:
*	Iterator of the next element.
*/
sorted_iter_t SortedListNext(sorted_iter_t iterator);

/*
* DESCRIPTION:
*   Get the previous element of the given element.
*   
*   Time complexity:    O(1)
*   Space Complexity:   O(1)
*
* PARAMS:
*   iterator: Iterator to get previous element.
*
* RETURN:
*	Iterator of the previous element.
*/
sorted_iter_t SortedListPrev(sorted_iter_t iterator);

/*
* DESCRIPTION:
*   Is list empty ?
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*	list =  list to be evaluated.
*
* RETURN:
*	1 if the given list is empty
*   0 if the given list is not empty
*/  
int IsSortedListEmpty(const sorted_list_t *list);

/*
* DESCRIPTION:
*   IsSLLIterEqual function takes two iterators and compares between them
*   if they are equal.
*
*   Time complexity: O(1)
*   Space Complexity: O(1)
*
* PARAMS:
*	iterator1 = first iterator to compare
*   iterator2 = second iterator to compare
*
* RETURN:
*	1 if the compared iterators are same.
*   0 if iterators are different    
*/
int IsSortedListIterEqual(sorted_iter_t iterator1, sorted_iter_t iterator2);

/*
* DESCRIPTION:
*   Merges two sorted linked lists, src and dest, into a single sorted linked list.
*   The merged list is stored in dest, and src becomes an empty list.
*   Time complexity: O(n)
*   Space complexity: O(1)
* PARAMS:
*   dest: Pointer to the destination sorted list.
*   src: Pointer to the source sorted list.
*
* RETURN:
*   None.
*/
void SortedListMerge(sorted_list_t *dest, sorted_list_t *src);
/*

* DESCRIPTION:
*   Removes and returns the data of the first element in the sorted linked list.
*   Time complexity: O(1)
*   Space complexity: O(1)
* PARAMS:
*   list: Pointer to the sorted list.
* RETURN:
*   A pointer to the data of the removed element.
*/
void *SortedListPopFront(sorted_list_t *list);
/*

* DESCRIPTION:
*   Removes and returns the data of the last element in the sorted linked list.
*   Time complexity: O(1)
*   Space complexity: O(1)
* PARAMS:
*   list: Pointer to the sorted list.
* RETURN:
*   A pointer to the data of the removed element.
*/
void *SortedListPopBack(sorted_list_t *list);

void SortedListPrint(sorted_list_t *list);

#endif /* __ILRD_SORTED_LIST_H__ */



         

