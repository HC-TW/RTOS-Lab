/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define RESOURCE1_PRIORITY      1
#define RESOURCE2_PRIORITY      2
#define TASK1_PRIORITY      3
#define TASK2_PRIORITY      4
#define TASK3_PRIORITY      5

INT32U limits[][2] = {
    {0, 0},
    {60, 100},
    {60, 100},
    {90, 100}
};

OS_EVENT    *resource1;
OS_EVENT    *resource2;

/* Prints "Hello World" and sleeps for three seconds */
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

    resource1 = OSMutexCreate(RESOURCE1_PRIORITY, &err);
    resource2 = OSMutexCreate(RESOURCE2_PRIORITY, &err);

    OSTimeSet(0);

    OSTimeDly(50);

    printf("Task 1 start\n");

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource2, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 30;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource1, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 30;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPost(resource1);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 30;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;
    OSMutexPost(resource2);
    OSTimeDly(1000);

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
    INT8U err;

    pdata = pdata;

//    OSTimeDly(40);

    printf("Task 2 start\n");

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;

    OSMutexPend(resource1, 0, &err);

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 60;
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

    OS_ENTER_CRITICAL();
    OSTCBCur->compTime = 20;
    OS_EXIT_CRITICAL();

    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
        ;
    OSMutexPost(resource1);

    OSTimeDly(1000);


}

//void  task3 (void *pdata)
//{
////	printf("2nd\n");
//#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
//    OS_CPU_SR  cpu_sr;
//#endif
//    INT32U start = 0; //the start time
//    INT32U end;                 //the end time
//    INT16U toDelay;
//    INT8U err;
//
//    pdata = pdata;
//
//    printf("Task 3 start\n");
//
//    OS_ENTER_CRITICAL();
//    OSTCBCur->compTime = 20;
//    OS_EXIT_CRITICAL();
//
//    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
//            ;
//    OSMutexPend(resource1, 0, &err);
//
//    OS_ENTER_CRITICAL();
//    OSTCBCur->compTime = 70;
//    OS_EXIT_CRITICAL();
//
//    while (OSTCBCur->compTime > 0)  // Blocked here: Computing
//            ;
//
//    OSMutexPost(resource1);
//    OSTimeDly(1000);
//
//}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
    OSInit();                                              /* Initialize uC/OS-II                      */

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


//	    OSTaskCreateExt(task3,
//	                    NULL,
//	                    (void *)&task3_stk[TASK_STACKSIZE-1],
//	                    TASK3_PRIORITY,
//	                    TASK3_PRIORITY,
//	                    task3_stk,
//	                    TASK_STACKSIZE,
//	                    (void *)limits[3],
//	                    0);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
