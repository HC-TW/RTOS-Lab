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

#define RESOURCE1_PRIORITY      1
#define RESOURCE2_PRIORITY      2
#define TASK1_PRIORITY      3
#define TASK2_PRIORITY      4
#define TASK3_PRIORITY      5

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
    {60, 100},
    {60, 100},
    {90, 100}
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

OS_EVENT    *resource1;  
OS_EVENT    *resource2;                

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
    INT8U err;

    pdata = pdata;

    OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
    PC_VectSet(0x08, OSTickISR);
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    resource1 = OSMutexCreate(RESOURCE1_PRIORITY, &err);                
    resource2 = OSMutexCreate(RESOURCE2_PRIORITY, &err);                

    OSTimeSet(0);

    OSTimeDly(80); 

    printf("Task 1 start\n");

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource1, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource2, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPost(resource2);
    OSMutexPost(resource1);

    OSTimeDly(1000); 
    PC_DOSReturn();                            /* Return to DOS                            */

}


void  task2 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT32U start = 0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;
    INT8U err;

    pdata = pdata;

    OSTimeDly(40); 

    printf("Task 2 start\n");

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource2, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 40;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;
    OSMutexPost(resource2);

    OSTimeDly(1000); 
    PC_DOSReturn();                            /* Return to DOS                            */

}

void  task3 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif
    INT32U start = 0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;
    INT8U err;

    pdata = pdata;

    printf("Task 3 start\n");

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
            ;
    OSMutexPend(resource1, 0, &err);
    
    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 70;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
            ;
    OSMutexPost(resource1);

    OSTimeDly(1000); 
    PC_DOSReturn();                            /* Return to DOS                            */

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
