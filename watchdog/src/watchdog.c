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

/*************************** HEADER INCLUDES ******************************/
#include "wd.h" /*the internal watchdog api*/
#include "watchdog.h" /* user watchdog api */


/****************************** GLOBAL VARIABLES ****************************/
pthread_t app_thread = RESET;

/************************* API FUNCTIONS DEFINITIONS *************************/

watchdog_status_t WatchdogStart(int argc, char *argv[], size_t timeout_interval
                                                , size_t missed_signal_threshold)
{
    info_t *app_thr_info = CreateInfoStruct(APP_SEMA, WD_SEMA);

    if (2 > argc)
    {
		printf("Usage : 1 argument\n - watchdog executable is missing\n");
		return WD_FAIL_START;
    }

    if (NULL == getenv(WATCHDOG_ON))
    {   
        char *args[3] = {NULL}; 
        SetEnvVar(argv, timeout_interval, missed_signal_threshold);
        FillArgs(args);
        app_thr_info->target_pid = ForkNExec(args);

    }
    #ifndef NDEBUG
        else
        {
            printf("WATCHDOG IS ON\n");
        }
    #endif    

    if (SUCCESS != pthread_create(&app_thread, NULL, InitScheduler, app_thr_info))
    {
        perror("thread_create failed\n");
    }

    return SUCCESS;
}

watchdog_status_t WatchdogStop(void)
{    
    if(SUCCESS != pthread_kill(app_thread, SIGUSR2))
    {
        printf("APP THREAD DID NOT RECEIVE SIGUSR2\n");
        
    }      

    if(SUCCESS != pthread_join(app_thread, NULL))
    {
        perror("couldnt join thread\n");
    }
 
    return SUCCESS;
}
