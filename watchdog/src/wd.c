/****************************************************
 *  WATCHDOG INTERNAL                               *   
 *                                                  *           
 *  Written by: Josh BENICHOU                       *   
 *                                                  *
 *  Reviewed by: MICHAL                             *
 *                                                  *
 *  Completed on - 31/08/23                         *
 *                                                  *
 ****************************************************/

#include "wd.h"

 
/****************************** GLOBAL VARIABLES ****************************/

static atomic_int g_threshold_counter = 0;
static atomic_int g_should_i_stop = 0;
static size_t g_is_wd = 0;

/************************ STATIC FUNCTIONS DECLARATIONS **********************/

static int TASKSendSignal(void *data);
static int TASKCheckTreshRevive(void *data);
static int TASKShouldITerminate(void *data);

static void SigHandler(int signum);
static void SetSignalsToHandle(struct sigaction *act);
static void SetTaskInScheduler(info_t *info);
static void SetSemaphores(info_t *app_thr_info, char  *sem_1, char  *sem_2);
static void UnsetEnvVar();

static void CheckMAlloc(void *ptr, char *calling_func);
static void CheckSemOpen(void *sem, char *calling_func, char *named_sem);
static void CheckSemClose(void *sem, char *calling_func, char *named_sem);
static void CheckSemUnlink(char *calling_func, char *named_sem);
static void CheckUnsetEnv(int status, char *calling_func, char *env_name);
static void CheckSchedulerAdd(ilrd_uid_t uid_to_check);
static void CleanUp(info_t *info);

static void CleanUpStubFunc(void *data);

/************************* API FUNCTIONS DEFINITIONS *************************/

info_t *CreateInfoStruct(char  *sem_1, char  *sem_2)
{
    info_t *info = malloc(sizeof(info_t));
    CheckMAlloc(info, "CreateInfoStruct"); 

    info->scheduler = SchedulerCreate();
    CheckMAlloc(info->scheduler,"SchedulerCreate");

    SetSemaphores(info, sem_1, sem_2);

    info->target_pid = getppid();

    return info;
}

void SetEnvVar(char *argv[], size_t timeout_interval, size_t missed_signal_threshold)
{
    char interval[20] = {0};
    char thres[20] = {0};

    sprintf(interval, "%ld", timeout_interval);
    sprintf(thres, "%ld", missed_signal_threshold);

    setenv(WATCHDOG_ON, "1", 1);

    setenv(APP, argv[0], 1);
    setenv(WATCH_DOG_ENV, argv[1], 1);
    setenv(INTERVALS, interval, 1);
    setenv(THRESHOLD, thres, 1);
}

void* InitScheduler(void *data)
{
    struct sigaction act = {0};
    info_t *info = (info_t *)data;

    assert(data);

    SetSignalsToHandle(&act);
    SetTaskInScheduler(info);

    sem_post(info->sem_1);
    #ifndef NDEBUG
        printf("\n    %s - OPERATIONAL\n", g_is_wd == WATCH_DOG ?
                                             "\t WATCHDOG" : "\tAPP THREAD");
    #endif
    sem_wait(info->sem_2);

    SchedulerRun(info->scheduler);

    CleanUp(info);

    #ifndef NDEBUG
        printf("    %s - All clean - EXITING\n", g_is_wd == WATCH_DOG ?
                                             "    WATCHDOG" : "    APP THREAD");
    #endif

    return NULL;
}

int ForkNExec(char *args[])
{
    pid_t pid = fork();

    if (CHILD == pid) 
    {
        execvp(args[WATCH_DOG == g_is_wd ? 0 : 1], args);
        perror("execvp failed ");
        return FAILURE;
    } 
    else if (0 > pid)
    {
        perror("fail to fork\n");
    }

    return pid;
}

void FillArgs(char *args[])
{
    args[0] = getenv(APP);
    args[1] = getenv(WATCH_DOG_ENV);
}

void SetWDIndicator(size_t wd_flag)
{
    atomic_store(&g_is_wd, wd_flag);
}

/*********************** STATIC FUNCTIONS DEFINITIONS ************************/

static int TASKSendSignal(void *data)
{
    info_t *info = (info_t *)data;

    #ifndef NDEBUG
        printf("%s : ", g_is_wd == WATCH_DOG ? "WATCHDOG" : "APP THREAD");
        printf("%d    ->    ", getpid() % 100);
        printf("%s : ", g_is_wd == APP_THREAD ? "WATCHDOG" : "APP THREAD");
        printf("%d\n", info->target_pid % 100);

    #endif

    kill(info->target_pid, SIGUSR1);

    return 0;
}

static int TASKCheckTreshRevive(void *data)
{
    info_t *info = (info_t *)data;

    assert(data);

    atomic_fetch_add(&g_threshold_counter, 1);
    if (g_threshold_counter > atol(getenv("THRESHOLD")))
    {
        char *args[3] = {NULL}; 
        FillArgs(args);

/*         SchedulerStop(info->scheduler);
 */        kill(info->target_pid, SIGKILL);
        wait(&info->target_pid);

        info->target_pid = ForkNExec(args);

        sem_post(info->sem_1);
        sem_wait(info->sem_2);

        atomic_store(&g_threshold_counter, RESET);
/*         SchedulerRun(info->scheduler);
 */    }

    return 0;
}

static int TASKShouldITerminate(void *data)
{
    if (g_should_i_stop)
    {
        info_t *info = (info_t *)data;
        SchedulerStop(info->scheduler);
        if (APP_THREAD == g_is_wd)
        {
            if(SUCCESS != kill(info->target_pid, SIGUSR2))
            {
                printf("WD DID NOT RECEIVE SIGUSR2\n");
            } 
        }

    }

    return 0;
}

static void SigHandler(int signum)
{ 
    if (SIGUSR1 == signum)
    { 
        atomic_store(&g_threshold_counter, RESET);
    }
    else if (SIGUSR2 == signum)
    {
        atomic_store(&g_should_i_stop, 1);
    }
}

static void SetSignalsToHandle(struct sigaction *act)
{
    assert(act);

    act->sa_handler = SigHandler;
    sigaction(SIGUSR1, act, NULL); 
    sigaction(SIGUSR2, act, NULL); 
}

static void SetTaskInScheduler(info_t *info)
{
    ilrd_uid_t returned_uid = {0,0,0};
    assert(info);

    returned_uid = SchedulerAdd(info->scheduler, TASKSendSignal, info, CleanUpStubFunc, time(NULL), atol(getenv("INTERVALS")));
    CheckSchedulerAdd(returned_uid);
    returned_uid = SchedulerAdd(info->scheduler, TASKCheckTreshRevive, info, CleanUpStubFunc, time(NULL) , atol(getenv("INTERVALS")));
    CheckSchedulerAdd(returned_uid);
    returned_uid = SchedulerAdd(info->scheduler, TASKShouldITerminate, info, CleanUpStubFunc, time(NULL), atol(getenv("INTERVALS")));
    CheckSchedulerAdd(returned_uid);
}

static void SetSemaphores(info_t *app_thr_info, char  *sem_1, char  *sem_2)
{
    assert(app_thr_info);
    assert(sem_1);
    assert(sem_2);

	app_thr_info->sem_1 = sem_open(sem_1, O_CREAT, 0666, 0);
	app_thr_info->sem_2 = sem_open(sem_2, O_CREAT, 0666, 0);

    CheckSemOpen(app_thr_info->sem_1, "SetSemaphores", APP_SEMA);
    CheckSemOpen(app_thr_info->sem_2, "SetSemaphores", WD_SEMA);
}

static void UnsetEnvVar()
{
    CheckUnsetEnv(unsetenv(WATCHDOG_ON), "UnsetEnvVar", WATCHDOG_ON);  
    CheckUnsetEnv(unsetenv(APP), "UnsetEnvVar", APP);  
    CheckUnsetEnv(unsetenv(WATCH_DOG_ENV), "UnsetEnvVar", WATCH_DOG_ENV);  
    CheckUnsetEnv(unsetenv(INTERVALS), "UnsetEnvVar", INTERVALS);  
    CheckUnsetEnv(unsetenv(THRESHOLD), "UnsetEnvVar", THRESHOLD);  
}

static void CleanUp(info_t *info)
{
    UnsetEnvVar();

    CheckSemClose(info->sem_1, "CleanUp", "wd_sem");
    CheckSemClose(info->sem_2, "CleanUp", "app_sem");

    if (WATCH_DOG == g_is_wd)
    {
        CheckSemUnlink("CleanUp", APP_SEMA);
        CheckSemUnlink("CleanUp", WD_SEMA);
    }

    atomic_store(&g_threshold_counter, RESET);
    atomic_store(&g_should_i_stop, RESET);

    SchedulerDestroy(info->scheduler);
    free(info);
}

static void CheckUnsetEnv(int status, char *calling_func, char *env_name)
{
    if (SUCCESS != status)
    {
        perror(calling_func);
        printf("%s\n", env_name);
        exit(EXIT_FAILURE);
    }
}

static void CheckMAlloc(void *ptr, char *calling_func)
{
    if (NULL == ptr)
    {
        perror(calling_func);
        exit(EXIT_FAILURE);
    }
}

static void CheckSemOpen(void *sem, char *calling_func, char *named_sem)
{
    if (SEM_FAILED == sem)
    {
        perror(calling_func);
        printf("%s\n", named_sem);
        exit(EXIT_FAILURE);
    }  
}

static void CheckSemClose(void *sem, char *calling_func, char *named_sem)
{
    if (SUCCESS != sem_close(sem))
    {
        perror(calling_func);
        printf("%s\n", named_sem);
        exit(EXIT_FAILURE);
    } 
}

static void CheckSemUnlink(char *calling_func, char *named_sem)
{
    if (SUCCESS != sem_unlink(named_sem))
    {
        perror(calling_func);
        printf("%s\n", named_sem);
        exit(EXIT_FAILURE);
    } 
}

static void CheckSchedulerAdd(ilrd_uid_t uid_to_check)
{
    if (IsSameUID(GetBadUID(), uid_to_check))
    {
        perror("Failed adding task to scheduler\n");
        exit(EXIT_FAILURE);
    }
}

static void CleanUpStubFunc(void *data)
{ 
    (UNUSED)data;
}


