#include <stdlib.h> /* malloc free */
#include <stdio.h> /* printf */
#include <assert.h> /* asserts */

#include "sorted_linked_list.h" /* our sorted linked list header */

struct sorted_list
{
    dll_t *list;
    sort_comparefunc_t cmp_func;
};


static sorted_iter_t FindMyPlace(sorted_list_t *list, void *data);

sorted_list_t *SortedListCreate(sort_comparefunc_t func)
{
    sorted_list_t *new_list = NULL;

    assert(NULL != func);

    new_list = (sorted_list_t *)malloc(sizeof(struct sorted_list));
    if (NULL == new_list)
    {
        return NULL;
    }
    new_list->cmp_func = func;
    new_list->list = DLLCreate();
    if (NULL == new_list->list)
    {   
        free(new_list);
        return NULL;
    }

    return new_list;
}

void SortedListDestroy(sorted_list_t *list)
{
    assert(NULL != list);

    DLLDestroy(list->list);
    list->list = NULL;
    
    free(list);
    list = NULL;
}

void *SortedListGetData(sorted_iter_t iterator)
{
    return DLLGetData(iterator.iter);
}

sorted_iter_t SortedListInsert(sorted_list_t *list, void *data)
{
    sorted_iter_t iterator;

    assert(NULL != list);
    assert(NULL != data);

    iterator = FindMyPlace(list, data);

    iterator.iter = DLLInsertBefore(list->list, iterator.iter, data);
    #ifndef NDEBUG
        iterator.list = list;        
    #endif

    return iterator;
}

static sorted_iter_t FindMyPlace(sorted_list_t *list, void *data)
{
    sorted_iter_t runner = SortedListBegin(list);
    assert(NULL != list);

    while (runner.iter != DLLEnd(list->list))
    {
        if (0 > list->cmp_func(SortedListGetData(runner), data))
        {
            break;
        }
        runner = SortedListNext(runner);
    }
    return runner;
}

sorted_iter_t SortedListBegin(const sorted_list_t *list)
{
    sorted_iter_t iterator;

    assert(NULL != list);

    iterator.iter = DLLBegin(list->list);
    #ifndef NDEBUG
        iterator.list = (sorted_list_t *)list;
    #endif
    
    return iterator; 
}    

sorted_iter_t SortedListEnd(const sorted_list_t *list)
{
    sorted_iter_t iterator;

    assert(NULL != list);

    iterator.iter = DLLEnd(list->list);
    #ifndef NDEBUG
        iterator.list = (sorted_list_t *)list;
    #endif

    return iterator; 
}

sorted_iter_t SortedListNext(sorted_iter_t iterator)
{
    iterator.iter = DLLNext(iterator.iter);

    return iterator; 
}

sorted_iter_t SortedListPrev(sorted_iter_t iterator)
{
    iterator.iter = DLLPrev(iterator.iter);

    return iterator; 
}

int IsSortedListEmpty(const sorted_list_t *list)
{
    assert(NULL != list);

    return IsDLLEmpty(list->list);
}

void SortedListMerge(sorted_list_t *dest, sorted_list_t *src)
{
    sorted_iter_t src_runner;
    sorted_iter_t dest_runner;
    sorted_iter_t match;
    int status = 0;

    assert(NULL != dest);
    assert(NULL != src);

    src_runner = SortedListBegin(src);
    dest_runner = SortedListBegin(dest);

    while (!IsSortedListIterEqual(src_runner, SortedListEnd(src)) && 
                        !IsSortedListIterEqual(dest_runner, SortedListEnd(dest)))
    {
        status = src->cmp_func(DLLGetData(dest_runner.iter), DLLGetData(src_runner.iter));

        if (0 > status)
        {
            #ifndef NDEBUG
                src_runner.list = dest;
            #endif   

            match = src_runner;
            src_runner = SortedListNext(src_runner);    

            DLLSplice(dest_runner.iter, match.iter, SortedListNext(match).iter);
        }
        else
        {
            dest_runner = SortedListNext(dest_runner);
        }
    }

    if (!IsSortedListIterEqual(src_runner, SortedListEnd(src)))
    {
        DLLSplice(dest_runner.iter, src_runner.iter, SortedListEnd(src).iter);
    }    
}

sorted_iter_t SortedListRemove(sorted_iter_t iterator)
{
    iterator.iter = DLLRemove(NULL, iterator.iter);

    return iterator;
}

int IsSortedListIterEqual(sorted_iter_t iterator1, sorted_iter_t iterator2)
{
    return IsDLLIterEqual(iterator1.iter, iterator2.iter);
}

size_t SortedListCount(const sorted_list_t *list)
{
    assert(NULL != list);

    return DLLSize(list->list);
}

void *SortedListPopFront(sorted_list_t *list)
{
    assert(NULL != list);

    return DLLPopFront(list->list);    
}

void *SortedListPopBack(sorted_list_t *list)
{
    assert(NULL != list);

    return DLLPopBack(list->list);    
}

int SortedListForEachElement(sorted_iter_t from, sorted_iter_t to, void *userdata, sort_actionfunc_t func)
{
    assert(NULL != func);
    assert(from.list == to.list);

    return DLLForEachElement(from.iter, to.iter, userdata, func);
}

sorted_iter_t SortedListFindIf(sorted_iter_t from, sorted_iter_t to, void *matchdata, sort_matchfunc_t func)
{
    sorted_iter_t return_iter;

    assert(NULL != func);
    assert(from.list == to.list);

    return_iter.iter = DLLFind(from.iter, to.iter, matchdata, func);

    return return_iter;
}    

sorted_iter_t SortedListFind(sorted_list_t *list, sorted_iter_t from, sorted_iter_t to, void *matchdata)
{
    sorted_iter_t return_iter;

    assert(from.list == to.list);

    (void)from;
    (void)to;

    return_iter = FindMyPlace(list, matchdata);    

    return return_iter; 
}


void SortedListPrint(sorted_list_t *list)
{
    sorted_iter_t runner;

    assert(NULL != list);

    runner.iter = DLLBegin(list->list);

    while(runner.iter != DLLEnd(list->list))
    {
        printf("%d -> ", *(int*)SortedListGetData(runner) );

        runner.iter = DLLNext(runner.iter);
    }
    printf("\n");
}