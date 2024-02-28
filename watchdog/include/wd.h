/****************************************************
 *  WATCHDOG INTERNAL                                *   
 *                                                  *           
 *  Written by: Josh BENICHOU                     *   
 *                                                  *
 *  Reviewed by: MICHAL                              *
 *                                                  *
 *  Completed on - 31/08/23                      *
 *                                                  *
 ****************************************************/

#ifndef __ILRD_ROTWEILER_H__
#define __ILRD_ROTWEILER_H__

#define _POSIX_C_SOURCE 199309L
#define _DEFAULT_SOURCE /* setenv getenv*/

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc free */
#include <assert.h> /* assert */
#include <unistd.h> /* sleep time */
#include <errno.h> /* perror */
#include <signal.h> /* sigaction */
#include <stdatomic.h> /* atomic_int atomic_fetch_add atomic_store*/
#include <pthread.h> /* create join */
#include <semaphore.h> /* semopen semwait sempost */
#include <fcntl.h>  /* O_CREAT*/
#include <sys/wait.h> /* wait */

#include "scheduler.h" /* our scheduler functions */

#define WATCH_DOG (1)
#define APP_THREAD (0)
#define UNUSED void
#define RESET (0)
#define SUCCESS (0)
#define FAILURE (-1)
#define CHILD (0)
#define APP ("APP")
#define APP_SEMA ("/APP_SEM")
#define WD_SEMA ("/WD_SEMA")
#define WATCH_DOG_ENV ("WATCH_DOG_ENV")
#define INTERVALS ("INTERVALS")
#define THRESHOLD ("THRESHOLD")
#define WATCHDOG_ON ("WATCHDOG_ON")

typedef struct info_data
{
    scheduler_t *scheduler;
    sem_t *sem_1;
    sem_t *sem_2;
    int target_pid;
}info_t; 


/*
 * DESCRIPTION:
 *  this func takes two named semaphores, create an info struct
 *  fills it with env variables and returns it.
 *
 * PARAMS:
 *  sem_1           - first semaphore
 *  sem_2           - second semaphore
 *
 * RETURN:
 *  the allocated, filled info struct on success, NULL on failure
 *
 */

info_t *CreateInfoStruct(char  *sem_1, char  *sem_2);

/*
 * DESCRIPTION:
 *  this functions set all env variable.
 *  
 *
 * PARAMS:
 *  argv                        - argument vector
 *  timeout_interval            - given interval
 *  missed_signal_threshold     - given max threshold
 * 
 * 
 * RETURN:
 *  none
 *
 */
void SetEnvVar(char *argv[], size_t timeout_interval, size_t missed_signal_threshold);

/*
 * DESCRIPTION:
 *  The function will init all tools, add task to scheduler and run it.
 *  
 *
 *
 * PARAMS:
 *  data                        -   void ptr to info struct
 *
 * RETURN:
 *  NULL
 */
void* InitScheduler(void *data);

/*
 * DESCRIPTION:
 *  The function fork and execute the given process in args
 * 
 *
 * PARAMS:
 *  args                        -  argument vector with process to run
 *
 * RETURN:
 *  returns the pid of the forked process.
 *
 */
int ForkNExec(char *args[]);

/*
 * DESCRIPTION:
 *  The function fill an argument vector with the env varibales.
 *
 * PARAMS:
 *  args                        - argument vector to fill.
 *
 * RETURN:
 *  none
 *
 */
void FillArgs(char *args[]);

/*
 * DESCRIPTION:
 *  The function will set the global variable that indicates that it is 
 *  the WATCHDOG.
 *
 * PARAMS:
 *  wd_flag                 - flag indicating if WATCHDOG. 
 *
 * RETURN:
 *  none
 *
 */
void SetWDIndicator(size_t wd_flag); 

#endif /* __ILRD_ROTWEILER_H__ */
