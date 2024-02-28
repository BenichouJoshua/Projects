#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/types.h> /* time_t */
#include "uid.h" /* our uid functions */

#define STOP_SIGNAL (0)
#define SIGNAL_FILE_NAME ("scheduler_run_flag.txt")
#define ALLOWED_TO_RUN (1)
#define CANNOT_RUN (0)

typedef struct scheduler scheduler_t;

/*
 * DESCRIPTION:
 *   Callback function performs an operation on user passed data.
 *   Optional - change time interval.
 *
 * PARAMS:
 *   void pointer.
 * RETURN:
 *   0 if success,
 *  -1 if fail
 *  positive - new time interval to assign.
 */
typedef int (*scheduler_operation_t)(void*);

/*
 * DESCRIPTION:
 *   Callback function that cleans user changes,
 *   Such as memory allocation.
 *
 * PARAMS:
 *   void pointer.
 * 
 */
typedef void (*scheduler_clean_func_t)(void*);

/*
 * DESCRIPTION:
 *   Create a new scheduler.
 * 
 *   Time complexity: O(1)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   none
 * 
 * RETURN:
 *   Returns a pointer to the newly created scheduler, or NULL on failure.
 */
scheduler_t *SchedulerCreate(void);

/*
 * DESCRIPTION:
 *   Destroy a scheduler and free its resources.
 * 
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler to be destroyed.
 */
void SchedulerDestroy(scheduler_t *scheduler);

/*
 * DESCRIPTION:
 *   Add a new task to the scheduler.
 * 
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   task_function -    A pointer to the task function to be executed.
 *   time_to_run -      The time at which the task should be executed.
 *   operation_data -   Argumentf or the operation function.
 *   time_interval -    The interval between repeated executions of the task.
 *                      0 tells task to run only once.
 *   scheduler -        Pointer to the scheduler.
 * 
 * RETURN:
 *   Returns the unique identifier (uid) of the added task, or an invalid uid on failure.
 */
ilrd_uid_t SchedulerAdd(scheduler_t *scheduler, scheduler_operation_t task_func, void *params, scheduler_clean_func_t clean_func ,time_t time_to_run, size_t time_interval);

/*
 * DESCRIPTION:
 *   Remove a task from the scheduler.
 * 
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 *   uid - The unique identifier (uid) of the task to be removed.
 * RETURN:
 *   0 - If succeeded
 *   Not 0 - If failes
 */
int SchedulerRemove(scheduler_t *scheduler, ilrd_uid_t uid);

/*
 * DESCRIPTION:
 *   Run the scheduler, executing all pending tasks.
 *   Function creates or open the file "scheduler_run_flag.txt" and set flag to 1.
 *
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 * 
 * RETURN:
 *   Returns 0 on success, or a non-zero value on failure.
 */
int SchedulerRun(scheduler_t *scheduler);

/*
 * DESCRIPTION:
 *   Stop the scheduler, preventing further execution of tasks.
 *   command to stop is using the file "scheduler_run_flag.txt"
 *   Flags: 0 - stop. 1 - run.
 *
 *   Time complexity: O(1)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 */
void SchedulerStop(scheduler_t *scheduler);

/*
 * DESCRIPTION:
 *   Get the number of tasks in the scheduler.
 * 
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 * 
 * RETURN:
 *   Returns the number of tasks in the scheduler.
 */
size_t SchedulerSize(const scheduler_t *scheduler);

/*
 * DESCRIPTION:
 *   Check if the scheduler is empty (has no tasks).
 * 
 *   Time complexity: O(1)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 * 
 * RETURN:
 *   Returns 1 if the scheduler is empty, or 0 if it has tasks.
 */
int IsSchedulerEmpty(const scheduler_t *scheduler);

/*
 * DESCRIPTION:
 *   Clear all tasks from the scheduler.
 * 
 *   Time complexity: O(n)
 *   Space complexity: O(1)
 * 
 * PARAMS:
 *   scheduler - A pointer to the scheduler.
 */
void SchedulerClear(scheduler_t *scheduler);

#endif /* SCHEDULER_H */
