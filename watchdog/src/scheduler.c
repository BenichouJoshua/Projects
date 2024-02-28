/*
	Coder : Josh Benichou
	Date : 04/07/2023
	Reviewer : Moshe
*/	

#include <stdio.h> /* printf fscanf */
#include <assert.h> /* asserts */
#include <stdlib.h> /* malloc free */
#include <unistd.h> /* sleep time */
#include <string.h> /* strcmp */

#include "scheduler.h" /* our scheduler functions */
#include "priority_queue.h" /* our priority queue functions */
#include "task.h" /* our task functions */

/* #define STOP_SIGNAL (0) */
#define SUCCESS (0)
#define FAILURE (-1)

struct scheduler
{
    p_queue_t *tasks_pq;
    int can_run_flag;
};

static int TimePriority(const void *queue_data, void *new_data);
static int FindTask(const void *queue_data, void *uid);

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
	if (NULL == scheduler)
	{
		return NULL;
	}

	scheduler->tasks_pq = PQueueCreate(&TimePriority);
	if (NULL == scheduler->tasks_pq)
	{
		free(scheduler);
		return NULL;
	}
	scheduler->can_run_flag = ALLOWED_TO_RUN;

	return scheduler;
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	SchedulerClear(scheduler);	

	PQueueDestroy(scheduler->tasks_pq);
	scheduler->tasks_pq = NULL;

	free(scheduler);
}

ilrd_uid_t SchedulerAdd(scheduler_t *scheduler
	, scheduler_operation_t task_func, void *params
		, scheduler_clean_func_t clean_func ,time_t time_to_run
												, size_t time_interval)
{
	task_t *task = NULL;

	assert(NULL != scheduler);
	assert(NULL != task_func);
	assert(NULL != params);
	assert(NULL != clean_func);

	task = TaskCreate(task_func, clean_func, params, time_to_run, time_interval);
	if (NULL == task)
	{	
		return GetBadUID();
	}

	return SUCCESS == PQueueEnqueue(scheduler->tasks_pq, task) ? 
												TaskGetUID(task) : GetBadUID();
}

int SchedulerRemove(scheduler_t *scheduler, ilrd_uid_t uid)
{
	void *data = NULL;
	int status = FAILURE;
	assert(NULL != scheduler);

	data = PQueueRemove(scheduler->tasks_pq, &uid, &FindTask);
	if (NULL != data)
	{
		TaskDestroy((task_t *)data);
		status = SUCCESS;
	}

	return status;
}

size_t SchedulerSize(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	return PQueueSize(scheduler->tasks_pq);
}

int IsSchedulerEmpty(const scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	return IsPQueueEmpty(scheduler->tasks_pq);
}

void SchedulerClear(scheduler_t *scheduler)
{
	void *to_free = NULL;

	assert(NULL != scheduler);

	while (1 != IsPQueueEmpty(scheduler->tasks_pq))
	{
		to_free = PQueueDequeue(scheduler->tasks_pq);
		TaskDestroy((task_t *)to_free);
	}
}

int SchedulerRun(scheduler_t *scheduler)
{
	task_t *curr_task = NULL;
	time_t time_stamp = 0;
	time_t time_to_sleep = 0;
	int status = SUCCESS;
	int buffer = 1;
	FILE *user_input = NULL;

	assert(NULL != scheduler);

	user_input = fopen(SIGNAL_FILE_NAME, "w+");
	if (NULL == user_input)
	{
		return FAILURE;
	}

	while(CANNOT_RUN != scheduler->can_run_flag 
		&& 0 == status
		&& 1 != IsSchedulerEmpty(scheduler))
	{
		curr_task = (task_t *)PQueueDequeue(scheduler->tasks_pq);
		time_stamp = time(NULL);
		if (FAILURE == time_stamp)
		{
			fclose(user_input);
			return FAILURE;
		}

		time_to_sleep = TaskGetStartTime(curr_task) - time_stamp;
		
		sleep(time_to_sleep > 0 ? time_to_sleep : 0);
		status = TaskExecute(curr_task);
		if (0 < status)
		{
			TaskSetFrequency(curr_task, status);
		}
		else if (0 > status)
		{
			fclose(user_input);
			return FAILURE;
		}		

		if (0 != TaskGetFrequency(curr_task))
		{	
			time_stamp = time(NULL);
			if (FAILURE == time_stamp)
			{
				fclose(user_input);
				return FAILURE;
			}

			TaskSetStartTime(curr_task, time_stamp + TaskGetFrequency(curr_task));
			status = PQueueEnqueue(scheduler->tasks_pq, curr_task);
			if (SUCCESS != status)
			{
				fclose(user_input);
				return FAILURE;
			}			
		}
		else
		{
			TaskDestroy(curr_task);
		}
		rewind(user_input);
		fscanf(user_input, "%d", &buffer);

		if (STOP_SIGNAL == buffer)
		{
			SchedulerStop(scheduler);
		}
	}
	fclose(user_input);
	
	scheduler->can_run_flag = ALLOWED_TO_RUN;

	return status;
}

void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);

	scheduler->can_run_flag = CANNOT_RUN;
}

static int TimePriority(const void *queue_data, void *new_data)
{
    assert(NULL != queue_data);
    assert(NULL != new_data);

    return TaskGetStartTime((task_t*)queue_data) - TaskGetStartTime((task_t*)new_data);
}

static int FindTask(const void *queue_data, void *uid)
{
	ilrd_uid_t queue_data_uid = TaskGetUID((task_t *)queue_data);

    assert(NULL != queue_data);
    assert(NULL != uid);

    return IsSameUID(queue_data_uid, *(ilrd_uid_t *)uid);
}
