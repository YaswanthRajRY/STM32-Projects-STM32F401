#include "mutex.h"

extern TCB_Typedef* getCurrentTask(void);       // define in scheduler.c
extern TCB_Typedef* idle_task;			// defined in task.c
extern ready_task_list* ReadyHead;      // defined in task.c

/********************************* Function to add task in mutex waiting List *******************************/
void pushWaitingQueue(mutexList** mutexListHead, TCB_Typedef* current_task)
{
    removeFromReadyList(&current_task);         // remove from ready list

    if ((*mutexListHead)->head  == NULL)
    {
        (*mutexListHead)->head = current_task;          // first node if mutexHead is null
    }
    else
    {
        TCB_Typedef* temp = (*mutexListHead)->head;
        while (temp->next != NULL)
        {
            temp = temp->next;          // iterate to last node
        }
        temp->next = current_task;      // add new node in last
    }
}

/****************************** Function to remove task in mutex waiting List *******************************/
TCB_Typedef* popWaitingQueue(mutexList** mutexListHead)
{
    if ((*mutexListHead)->head == NULL) 
    {
        return NULL;        // return if list is empty
    }

    TCB_Typedef* next_task = (*mutexListHead)->head;    // copy the first node
    next_task->next = NULL;                             // detach from mutex list

    (*mutexListHead)->head = next_task->next;           // move to next node in mutex list
    
    return next_task;       // return the poped node
}

/************************************** Function to aquire mutex lock ***************************************/
void MutexTake(Mutex_Typedef** mutex)
{
    __disable_irq();

    TCB_Typedef* current_task = getCurrentTask();       // get current task
    
    if (!((*mutex)->lock))
    {
        (*mutex)->lock = 1;         // set lock
        (*mutex)->owner = current_task;
        (*mutex)->priority = current_task->priority;        // copy mutex aquired task priority for priority inheritance
        return;
    }
    else
    {
        current_task->state = BLOCKED;      // change state from running to waiting / blocked
        pushWaitingQueue(&((*mutex)->waiting_task), current_task);      // push to mutex list

        if (current_task->priority < (*mutex)->priority)
        {
            (*mutex)->owner->priority = current_task->priority;         // waiting task priority is higher than mutex aquired task do priority inheritance
        }

        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;        // trigger PendSV interrupt for context switching
    }
    __enable_irq();
}

/************************************** Function to return mutex lock ***************************************/
void MutexGive(Mutex_Typedef** mutex)
{
    __disable_irq();

    TCB_Typedef* current_task = getCurrentTask();       // get current running task
    
    if ((*mutex)->owner != current_task)
    {
        __enable_irq();
        return;         // return if no task aquired the mutex
    }

    if ((*mutex)->waiting_task->head == NULL)
    {
        (*mutex)->lock = 0;
        (*mutex)->owner->priority = (*mutex)->priority;     // return the old priority of the task
        (*mutex)->owner = NULL;
    }
    else
    {
        TCB_Typedef* next_task = popWaitingQueue(&((*mutex)->waiting_task));        // pop next task from mutex list
        
        if (next_task != NULL)
        {
            next_task->state = READY;       // change next task state to ready
            addToReadyList(&next_task);     // add in ready list

            (*mutex)->owner->priority = (*mutex)->priority;     // return task previous priority
            (*mutex)->owner = next_task;            // change the task that aquired mutex lock
            (*mutex)->priority = next_task->priority;       // copy task priority for priority inheritance

            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;        // trigger PendSV interrupt for context switching
        }
    }
    __enable_irq();
}

/**************************************** Function to create mutex ******************************************/
Mutex_Typedef* createMutex(void)
{
    Mutex_Typedef* newMutex = (Mutex_Typedef*)malloc(sizeof(Mutex_Typedef));        // allocate memory for mutex
    if (newMutex == NULL)
    {
        return NULL;        // return if failed to allocate memory for mutex
    }

    memset(newMutex, 0, sizeof(Mutex_Typedef));

    mutexList* dummy = (mutexList*)malloc(sizeof(mutexList));       // allocate memory for mutex list
    newMutex->waiting_task = dummy;
    newMutex->waiting_task->head = NULL;

    return newMutex;        // return newly created mutex
}