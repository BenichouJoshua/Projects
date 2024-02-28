/****************************************************
 *  WATCHDOG                                        *   
 *                                                  *           
 *  Written by: Josh BENICHOU                       *   
 *                                                  *
 *  Reviewed by: MICHAL                             *
 *                                                  *
 *  Completed on - 31/08/23                         *
 *                                                  *
 ****************************************************/
/* 
    A a multi-threaded C program designed to monitor other programs for 
    potential issues such as crashes or stoppage, and to reset them if
    necessary. This program utilizes synchronization and concurrency tools, 
    signals, and a custom module of a callbacks scheduler to facilitate 
    communication between processes. 
*/

#ifndef __ILRD_WATCHDOG_H__
#define __ILRD_WATCHDOG_H__

#include <stddef.h> /* size_t */

typedef enum
{
    WD_SUCCESS,
    WD_FAIL_START
}watchdog_status_t;

/*
 * DESCRIPTION:
 *  The function will start a watchdog process.
 *  The signals SIGUSR1 and SIGUSR2 must not be modified.
 *
 * PARAMS:
 *  argc                        -   number of arguments in argument vector.
 *  argv                        -   argument vector, should hold name of the process to execute.
 *  timeout_interval            -   The timeout interval for the watchdog.
 *  missed_signal_threshold     -   The number of missed signals allowed before 
 *                                  watchdog restarts the app.
 *
 * RETURN:
 *  WD_SUCCESS if succeeded,
 *  other watchdog_status_t if failed
 *
 */
watchdog_status_t WatchdogStart(int argc, char *argv[], size_t timeout_interval, size_t missed_signal_threshold); 

/*
 * DESCRIPTION:
 *  Terminates the watchdog process, by sending
 *
 * TIME COMPLEXITY:
 *  O(1)
 * SPACE COMPLEXITY:
 *  O(1)
 *
 * PARAMS:
 *
 * RETURN:
 *  WD_SUCCESS on success.
 *  other watchdog_status_t if failed
 *
 */
watchdog_status_t WatchdogStop(void);

#endif /* __ILRD_WATCHDOG_H__ */