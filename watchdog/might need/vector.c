#include <stdlib.h> /* malloc, realloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <stdio.h> /* printf */

#include "vector.h" /* my functions */


enum status {FAILURE = 1, SUCCESS = 0};

struct vector
{
	size_t capacity;
	size_t element_size;
	size_t ratio;
	void* base;
	void* end;
	
};

vector_t *VectorCreate(size_t init_capacity, size_t size_of_one_element)
{
	vector_t *vector = malloc(sizeof(vector_t));		
	if (NULL == vector)
	{
		return NULL;
	}

	vector->base = malloc(init_capacity * size_of_one_element);
	if (NULL == vector->base)
	{
		free(vector);
		return NULL;
	}

	vector->end = vector->base;
	vector->capacity = init_capacity;
	vector->element_size = size_of_one_element;
	vector->ratio = 2; 	

	return vector;
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);

	free(vector->base);
	vector->base = NULL;

	free(vector);
}

void *VectorGetAccessToElement(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	if(index < VectorSize(vector))
	{
		return (char*)vector->base + (index * vector->element_size);
	}

	return NULL;
}

size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);

	return ((char*)vector->end - (char*)vector->base) / vector->element_size;	
}

int VectorReserve(vector_t *vector, size_t new_capacity)
{
	void *temp_vec = NULL;
	assert(NULL != vector);

	temp_vec = realloc(vector->base, new_capacity * (vector->element_size));

	if (NULL == temp_vec)
	{
		return FAILURE;
	}

	vector->base = temp_vec;

	return SUCCESS;
}

int VectorPushBack(vector_t *vector, const void *value)
{
	size_t slots_left = 0;
	int realloc_failure = 0;
	
	assert(NULL != vector);

	slots_left = (VectorCapacity(vector) - VectorSize(vector));

	if (0 == slots_left)
	{
		/*printf("No slots_left, reserving... \n");*/														

		realloc_failure = VectorReserve(vector, vector->capacity * vector->ratio);
		if (FAILURE == realloc_failure)
		{
			return FAILURE;
		}

		vector->end = (char*)vector->base + (vector->capacity * vector->element_size);
		vector->capacity = vector->capacity * vector->ratio;
	}

	if (NULL == memcpy(vector->end, value, vector->element_size))
	{
		return FAILURE;
	}	
	vector->end = (char*)vector->end + vector->element_size;

	return SUCCESS;
}

int VectorShrink(vector_t *vector)
{
	int status = 0;
	assert(NULL != vector);

	status = VectorReserve(vector, vector->capacity / vector->ratio);

	if (FAILURE == status)
	{
		return FAILURE;
	}

	vector->capacity = vector->capacity / vector->ratio;
	vector->end = (char*)vector->base + (vector->capacity * vector->element_size);

	return SUCCESS;
}

void VectorPopBack(vector_t *vector)
{
	size_t slots_left = 0;
	int realloc_failure = 0;
	
	assert(NULL != vector);

	if (vector->end == vector->base)
	{	
		/*printf("Nothing to pop :\n");*/
		return;
	}

	slots_left = (VectorCapacity(vector) - VectorSize(vector));

	if ((vector->capacity / vector->ratio) == slots_left)
	{
		/*printf("To much slots spare, shrinking...\n");	*/													

		realloc_failure = VectorShrink(vector);
		if (FAILURE == realloc_failure)
		{
			return;
		}
	}

	vector->end = (char*)vector->end - vector->element_size;
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);

	return vector->capacity;
}
