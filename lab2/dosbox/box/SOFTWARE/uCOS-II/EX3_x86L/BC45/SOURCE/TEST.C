/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #3
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define   TASK_STACKSIZE       2048

#define TASK1_PRIORITY      11
#define TASK2_PRIORITY      12
#define TASK3_PRIORITY      13

/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

INT32U limits[][2] = {
    {0, 0},
    {1, 4},
    {2, 5},
    {2, 10}
};

OS_STK    worker_stk[TASK_STACKSIZE];
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  task1 (void *pdata);
static  void  task2 (void *pdata);
static  void  task3 (void *pdata);



/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void  main (void)
{
    OSInit();                                              /* Initialize uC/OS-II                      */

    PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */

    PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

    OSTaskCreateExt(task1,
                    NULL,
                    (void *)&task1_stk[TASK_STACKSIZE-1],
                    TASK1_PRIORITY,
                    TASK1_PRIORITY,
                    task1_stk,
                    TASK_STACKSIZE,
                    (void *)limits[1],
                    0);


    OSTaskCreateExt(task2,
                    NULL,
                    (void *)&task2_stk[TASK_STACKSIZE-1],
                    TASK2_PRIORITY,
                    TASK2_PRIORITY,
                    task2_stk,
                    TASK_STACKSIZE,
                    (void *)limits[2],
                    0);

    
    OSTaskCreateExt(task3,
                    NULL,
                    (void *)&task3_stk[TASK_STACKSIZE-1],
                    TASK3_PRIORITY,
                    TASK3_PRIORITY,
                    task3_stk,
                    TASK_STACKSIZE,
                    (void *)limits[3],
                    0);
    OSStart();                                             /* Start multitasking                       */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

void  task1 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

    INT32U start = 0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;

    pdata = pdata;

    OSTimeSet(0);
    OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
    PC_VectSet(0x08, OSTickISR);
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    printf("Task 1 start\n");

    while (1)
    {
        while (OSTCBCur->compTime > 0)  // Blocked here: Computing
            ;
        OS_ENTER_CRITICAL();
        end = OSTimeGet(); // end time of computation
        if (end > start + OSTCBCur->period)
        {
            printf("Task 1 miss the deadline\n");
            toDelay = 0;
        }
        else
            toDelay = (OSTCBCur->period) - (end - start);
        start = start + (OSTCBCur->period); // next start time

        OSTCBCur->deadline = start + (OSTCBCur->period);
        OSTCBCur->compTime = 1;
        OS_EXIT_CRITICAL();

        OSTimeDly(toDelay); // delay and wait (P-C) times
    }
}


void  task2 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT32U start = 0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;

    pdata = pdata;

    printf("Task 2 start\n");

    while (1)
    {
        while (OSTCBCur->compTime > 0)  // Blocked here: Computing
            ;
        OS_ENTER_CRITICAL();
        end = OSTimeGet(); // end time of computation
        if (end > start + OSTCBCur->period)
        {
            printf("Task 2 miss the deadline\n");
            toDelay = 0;
        }
        else
            toDelay = (OSTCBCur->period) - (end - start);
        start = start + (OSTCBCur->period); // next start time

        OSTCBCur->deadline = start + (OSTCBCur->period);
        OSTCBCur->compTime = 2;
        OS_EXIT_CRITICAL();

        OSTimeDly(toDelay); // delay and wait (P-C) times
    }
}

void  task3 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT32U start = 0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;

    pdata = pdata;

    printf("Task 3 start\n");

    while (1)
    {
        while (OSTCBCur->compTime > 0)  // Blocked here: Computing
            ;
        OS_ENTER_CRITICAL();
        end = OSTimeGet(); // end time of computation
        if (end > start + OSTCBCur->period)
        {
            printf("Task 3 miss the deadline\n");
            toDelay = 0;
        }
        else
            toDelay = (OSTCBCur->period) - (end - start);
        start = start + (OSTCBCur->period); // next start time

        OSTCBCur->deadline = start + (OSTCBCur->period);
        OSTCBCur->compTime = 2;
        OS_EXIT_CRITICAL();

        OSTimeDly(toDelay); // delay and wait (P-C) times
    }
}
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*********************************************************************************************************
*/
void  OSInitHookBegin (void)
{
}

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*********************************************************************************************************
*/
void  OSInitHookEnd (void)
{
}

/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*********************************************************************************************************
*/
void  OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*********************************************************************************************************
*/
void  OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*********************************************************************************************************
*/
void  OSTaskIdleHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*********************************************************************************************************
*/

void  OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*********************************************************************************************************
*/
void  OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*********************************************************************************************************
*/
void  OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*********************************************************************************************************
*/
void  OSTimeTickHook (void)
{
}
