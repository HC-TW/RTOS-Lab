# RTOS-Lab
## Lab2
uC/OS-II: EDF Scheduler
### Implementation
- ucos_ii.h
  - Add some variables in os_tcb:
    1. INT8U		   compTime
	  2. INT8U		   period
    3. INT32U      deadline
- os_core.c
  - OSNextTask() (User-defined function)
    - Choose the next highest priority task according to their deadline
  - OSIntExit()
    - Replace the original function for choosing the next priority task into OSNextTask()
    - Print the "preempt" message
  - OSStart()
    - Replace the original function for choosing the next priority task into OSNextTask()
  - OSTimeTick()
    - OSTCBCur->compTime--
  - OS_Sched()
    - Replace the original function for choosing the next priority task into OSNextTask()
    - Print the "complete" message
  - OS_TCBInit()
    - Initialize some variables:
        1. ptcb->compTime = ((INT32U *)pext)[0]
        2. ptcb->period = ((INT32U *)pext)[1]
        3. ptcb->deadline = ((INT32U *)pext)[1]    
## Lab3
uC/OS-II: Ceiling Priority Protocol Scheduler
### Implementation
- ucos_ii.h
  - Add some variables in os_tcb:
    1. INT8U		   compTime
	  2. INT8U		   period
- os_core.c
  - OSTimeTick()
    - OSTCBCur->compTime--
  - OS_Sched()
    - Print the "complete" message
  - OS_TCBInit()
    - Initialize some variables:
        1. ptcb->compTime = ((INT32U *)pext)[0]
        2. ptcb->period = ((INT32U *)pext)[1]
- os_mutex.c
  - OSMutexPend()
    - Raise priority when the task acquires the mutex lock
    - Print the "lock resource" message
  - OSMutexPost()
    - Print the "unlock resource" message
