/*
    Coder : Josh Benichou
    Date : 04/07/2023
    Reviewer : 
*/  

#ifndef __ILRD_TASK_H__
#define __ILRD_TASK_H__

#include <time.h>   /* time_t       */

#include "uid.h"    /* ilrd_uid_t   */

typedef struct task task_t;

typedef int (*task_func_t)(void*);
typedef void (*clean_func_t)(void*);

struct task
{
    task_func_t taskfunc;
    clean_func_t clean_func;
    void *data;
    time_t start_run_time;        /* the time in time_t the task shall be executed*/
    size_t frequency;             /* time between iterations*/
    ilrd_uid_t uid;
};


/*
 * DESCRIPTION:
 *   Creates a task.
 *   
 *   Time complexity:   O(1)
 *   Space complexity:  O(1)
 * 
 * PARAMS:
 *   task_function     - reference to task.
 *   clean_func        - reference to clean function   
 *   task_params       - params for the action func
 *   start_run_time    - the time until the task needs to be executed
 *   frequency         - time between iterations
 *
 * RETURN:
 *   A reference to the task, NULL if failed.
 *
 */
task_t *TaskCreate(task_func_t taskfunc, clean_func_t clean_func
                        , void *task_params, time_t start_run_time, time_t frequency);

/*
 * DESCRIPTION:
 *   The function cleans the memory
 *	 the task was using.
 *   
 *   Time complexity:   O(1)
 *   Space complexity:  O(1)
 * 
 * PARAMS:
 *   task - A reference to the task to-be destroyed.
 *
 * RETURN:
 *   None.
 *
 */
void TaskDestroy(task_t *task);

/*
 * DESCRIPTION:
 *   The function returns the uid of the task.
 *   
 *   Time complexity  O(1)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   The uid of the task.
 */
ilrd_uid_t TaskGetUID(task_t *task);

/*
 * DESCRIPTION:
 *   Executes the action of a task
 *   
 *   Time complexity  O(1) * O(of action)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   0 action success non-zero otherwise.
 */
int TaskExecute(task_t *task);

/*
 * DESCRIPTION:
 *   Gets data stored in task.
 *   
 *   Time complexity  O(1)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   void * to data
 *
 */
void *TaskGetData(task_t *task);

/* 
 * DESCRIPTION:
 *   The function returns the action of the task. 
 *   
 *   Time comlexity:    O(1)
 *   Space complexity:  O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   A function pointer to the action of the task.
 */
task_func_t TaskGetAction(task_t *task);
 

/* 
 * DESCRIPTION:
 *  The function returns the time at which
 *	the function will be executed. 
 *   
 *   Time complexity:    O(1)
 *   Space complexity:   O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   A time_t of the task.
 */
time_t TaskGetStartTime(task_t *task);

/* 
 * DESCRIPTION:
 *   The function returns the frequency of the task.
 *   
 *   Time complexity  O(1)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *
 * RETURN:
 *   The frequency of the task.
 */
time_t TaskGetFrequency(task_t *task);

/* 
 * DESCRIPTION:
 *   The function update the frequency of the task.
 *   
 *   Time complexity  O(1)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *   new_frequency - new_frequency to update
 *  
 * RETURN:
 *   void
 */
void TaskSetFrequency(task_t *task, int new_frequency);

/* 
 * DESCRIPTION:
 *   The function sets the start time of the task.
 *   
 *   Time complexity  O(1)
 *   Space complexity O(1)
 * 
 * PARAMS:
 *   task - reference to task.
 *   new_time_to_set - new time to set   
 * RETURN:
 *   void
 */
void TaskSetStartTime(task_t *task, time_t new_time_to_set);


#endif /* __ILRD_TASK_H__ */




