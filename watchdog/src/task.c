/*
	Coder : Josh Benichou
	Date : 04/07/2023
	Reviewer : 
*/	

#include <stdlib.h> /* printf */
#include <assert.h> /* assert */

#include "task.h" /* task function */

task_t *TaskCreate(task_func_t taskfunc, clean_func_t clean_func
                        , void *task_params, time_t start_run_time, time_t frequency)
{	
	task_t *task = (task_t *)malloc(sizeof(task_t));
	if (NULL == task)
	{
		return NULL;
	}

	assert(NULL != taskfunc);
	assert(NULL != task_params);

	task->taskfunc = taskfunc;
	task->clean_func = clean_func;
	task->data = task_params;
	task->start_run_time = start_run_time;
	task->frequency = frequency;
	task->uid = UIDCreate();

	return task;
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);

	task->clean_func(task);

	free(task);
}

ilrd_uid_t TaskGetUID(task_t *task)
{
	assert(NULL != task);

	return task->uid;
}

int TaskExecute(task_t *task)
{
	assert(NULL != task);

	return (task->taskfunc)(task->data);
}

void *TaskGetData(task_t *task)
{
	assert(NULL != task);

	return task->data;
}

time_t TaskGetStartTime(task_t *task)
{
	assert(NULL != task);

	return task->start_run_time;
}

time_t TaskGetFrequency(task_t *task)
{
	assert(NULL != task);

	return task->frequency;	
}

void TaskSetFrequency(task_t *task, int new_frequency)
{
	assert(NULL != task);

	task->frequency = (time_t)new_frequency;
}

task_func_t TaskGetAction(task_t *task)
{
	assert(NULL != task);

	return task->taskfunc;
}

void TaskSetStartTime(task_t *task, time_t new_time_to_set)
{
	assert(NULL != task);

	task->start_run_time = new_time_to_set;
}