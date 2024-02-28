#include <stddef.h> /* size_t */
#include <stdlib.h> /* printf */
#include <assert.h> /* asserts */
#include <stdio.h> /* printf */

#include "d_linked_list.h" /* my funtions */

typedef struct dll_node dll_node_t;
struct dll 
{
	dll_iterator_t first;
	dll_iterator_t last;
};

struct iterator
{
	void *data;
	dll_iterator_t next;
	dll_iterator_t prev;	
};

static dll_iterator_t DLLNewNode();
static int PlusOne(void *iteratordata, void *userdata);

dll_t *DLLCreate(void)
{
	dll_t *dll = (dll_t *)malloc(sizeof(dll_t));
	if (NULL == dll)
	{
		return NULL;
	}

	dll->first = DLLNewNode();
	if (NULL == dll->first)
	{
		free(dll);
		return NULL;
	}

	dll->last = DLLNewNode();
	if (NULL == dll->last)
	{
		free(dll->first);
		free(dll);
		return NULL;
	}

	dll->first->next = dll->last;
	dll->last->prev = dll->first;

	return dll;
}

void DLLDestroy(dll_t *dll)
{
    dll_iterator_t temp_to_free = dll->first;

    assert(NULL != dll);

    while (NULL != dll->first)
    {
        dll->first = dll->first->next;
        free(temp_to_free);
        temp_to_free = dll->first;
    }

    free(dll); 
}

void *DLLGetData(dll_iterator_t iterator)
{
	assert(NULL != iterator);

	return iterator->data;
}

void DLLSetData(dll_iterator_t iterator, void *data)
{
	assert(NULL != iterator);

	iterator->data = data;
}

dll_iterator_t DLLNext(dll_iterator_t iterator)
{
	assert(NULL != iterator);

	return iterator->next;
}

dll_iterator_t DLLPrev(dll_iterator_t iterator)
{
	assert(NULL != iterator);

	return iterator->prev;
}

dll_iterator_t DLLBegin(const dll_t *dll)
{
	assert(NULL != dll);

	return dll->first->next;
}

dll_iterator_t DLLEnd(const dll_t *dll)
{
	assert(NULL != dll);

	return dll->last;
}

int IsDLLEmpty(const dll_t *dll)
{
	assert(NULL != dll);

	return (dll->first->next == dll->last) ? 1 : 0;
}

int IsDLLIterEqual(dll_iterator_t iterator1, dll_iterator_t iterator2)
{
	assert(NULL != iterator1);
	assert(NULL != iterator2);

	return (iterator1 == iterator2) ? 1 : 0;
}

dll_iterator_t DLLInsertBefore(dll_t *dll, dll_iterator_t iterator, void *data)
{
	dll_iterator_t new_node = DLLNewNode();

	assert(NULL != dll);
	assert(NULL != iterator);

	if (NULL == new_node)
	{
		return DLLEnd(dll);
	}

	new_node->data = data;
	new_node->next = iterator;
	new_node->prev = iterator->prev;
	iterator->prev->next = new_node;
	iterator->prev = new_node;

	return new_node;
}

static dll_iterator_t DLLNewNode()
{
    dll_iterator_t new_node = malloc(sizeof(struct iterator));
    if (NULL == new_node)
    {
        return NULL;
    }

    new_node->next = NULL;
    new_node->prev = NULL;
    new_node->data = NULL;

    return new_node;
}

dll_iterator_t DLLRemove(dll_t *dll, dll_iterator_t iterator)
{
	dll_iterator_t to_return = iterator->next;
	(void)dll;
	assert(NULL != iterator);

	iterator->prev->next = iterator->next;
	iterator->next->prev = iterator->prev;

	free(iterator);
	iterator = NULL;

	return to_return;
}

dll_iterator_t DLLPushFront(dll_t *dll, void *data)
{
	assert(NULL != dll);

	return DLLInsertBefore(dll, DLLBegin(dll), data);
}

dll_iterator_t DLLPushBack(dll_t *dll, void *data)
{
	assert(NULL != dll);

	return DLLInsertBefore(dll, DLLEnd(dll), data);	
}

void *DLLPopFront(dll_t *dll)
{
	void *data_to_return = DLLGetData(DLLBegin(dll));

	assert(NULL != dll);

	if (1 == IsDLLEmpty(dll))
	{
		return NULL;
	}

	DLLRemove(dll, DLLBegin(dll));

	return data_to_return;
}

void *DLLPopBack(dll_t *dll)
{
	void *data_to_return = DLLGetData(DLLPrev(DLLEnd(dll)));

	assert(NULL != dll);

	if (1 == IsDLLEmpty(dll))
	{
		return NULL;
	}

	DLLRemove(dll, DLLPrev(DLLEnd(dll)));

	return data_to_return;
}


int DLLForEachElement(dll_iterator_t from, dll_iterator_t to, void *userdata, dll_actionfunc_t func)
{
    int status = 0;

    assert(NULL != from);
    assert(NULL != to);    
    assert(NULL != userdata);
    assert(NULL != func);

    while (from != to)
    {
        status = func(from->data, userdata);
        if (1 == status)
        {
            break;
        }
        from = DLLNext(from);
    }

    return status;
}

size_t DLLSize(dll_t *dll)
{
    size_t num_of_nodes = 0;
 
    assert(NULL != dll);
    DLLForEachElement(DLLBegin(dll), DLLEnd(dll), &num_of_nodes, &PlusOne);

    return num_of_nodes;	
}

static int PlusOne(void *iteratordata, void *userdata)
{
    (void)iteratordata;
    assert(NULL != userdata);

    *(size_t*)userdata += 1;;

    return 0;
}

void DLLSplice(dll_iterator_t target, dll_iterator_t from, dll_iterator_t to)
{
	dll_iterator_t last_included = to->prev;

    assert(NULL != from);
    assert(NULL != to);    
    assert(NULL != target);
    
    from->prev->next = to;
    to->prev = DLLPrev(from);

    target->prev->next = from;
    last_included->next = target; 

    from->prev = DLLPrev(target);
    target->prev = last_included; 
}

dll_iterator_t DLLFind(dll_iterator_t from, dll_iterator_t to, void *matchdata, dll_matchfunc_t func)
{
    assert(NULL != from);
    assert(NULL != to);    
    assert(NULL != matchdata);
    assert(NULL != func);

    while (from != to)
    {
        if (1 == func(from->data, matchdata))
        {
            return from;
        }
        from = from->next;
    }

    return from;
}

int DLLMultiFind(dll_t *dll, dll_iterator_t from, dll_iterator_t to, void *matchdata, dll_matchfunc_t func)
{
	dll_iterator_t status = NULL;

    assert(NULL != from);
    assert(NULL != to);    
    assert(NULL != func);
    assert(NULL != dll);


    while(from != to)
    {
    	if (1 == func(from->data, matchdata))
    	{
    		status = DLLPushBack(dll, matchdata);
    		if (dll->last == status)
    		{
    			return 0;
    		}
    	}
    	from = from->next;
    }

    return 1;
}

#ifndef NDEBUG
	void DLLPrint(dll_t *list)
	{
		dll_iterator_t runner = list->first->next;

		assert(NULL != list);
		
		while(NULL != runner->next)
		{
			printf("-> %s\n", (char*)runner->data );

			runner = runner->next;
		}
	}
#endif