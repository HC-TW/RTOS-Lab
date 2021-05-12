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
#include <stdio.h>
#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define          TASK_STK_SIZE     2048                /* Size of each task's stacks (# of WORDs)       */

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

OS_STK    worker_stk[TASK_STK_SIZE];
OS_STK    task1_stk[TASK_STK_SIZE];
OS_STK    task2_stk[TASK_STK_SIZE];
OS_STK    task3_stk[TASK_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  task1(void *data);
        void  task2(void *data);
        void  task3(void *data);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

int  main (void)
{
	OSInit();                                              /* Initialize uC/OS-II                      */

	//PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */

	//PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

	OSTaskCreateExt(task1,
					NULL,
					(void *)&task1_stk[TASK_STK_SIZE-1],
					TASK1_PRIORITY,
					TASK1_PRIORITY,
					task1_stk,
					TASK_STK_SIZE,
					(void *)limits[1],
					0);


	OSTaskCreateExt(task2,
					NULL,
					(void *)&task2_stk[TASK_STK_SIZE-1],
					TASK2_PRIORITY,
					TASK2_PRIORITY,
					task2_stk,
					TASK_STK_SIZE,
					(void *)limits[2],
					0);

	OSTaskCreateExt(task3,
						NULL,
						(void *)&task3_stk[TASK_STK_SIZE-1],
						TASK3_PRIORITY,
						TASK3_PRIORITY,
						task3_stk,
						TASK_STK_SIZE,
						(void *)limits[3],
						0);
	OSStart();                                             /* Start multitasking                       */
    return 0;
}

void  task1 (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif

    INT32U start=0; //the start time
    INT32U end;                 //the end time
    INT16U toDelay;

    pdata = pdata;
    OSTimeSet(0);
    //OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
    //PC_VectSet(0x08, OSTickISR);
    //PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    //OS_EXIT_CRITICAL();

    //OSStatInit();                                          /* Initialize uC/OS-II's statistics         */


    printf("Task 1 start\n");

    while (1)
    {
        while (OSTCBCur->compTime > 0)
        {

        }  // Blocked here: Computing;

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


/* Prints "Hello World" and sleeps for three seconds */
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

        while (OSTCBCur->compTime > 0)
        {

        }// Blocked here: Computing;

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

        while (OSTCBCur->compTime > 0)
        {

        }// Blocked here: Computing;

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


