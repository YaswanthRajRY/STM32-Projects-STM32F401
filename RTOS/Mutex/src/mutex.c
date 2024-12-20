#include "mutex.h"

extern TCB_Typedef* getCurrentTask(void);
extern TCB_Typedef* idle_task;			// defined in task.c
extern ready_task_list* ReadyHead;      // defined in task.c

void pushWaitingQueue(mutexList** mutexListHead, TCB_Typedef* current_task)
{
    removeFromReadyList(&current_task);

    if ((*mutexListHead)->head  == NULL)
    {
        (*mutexListHead)->head = current_task;
    }
    else
    {
        TCB_Typedef* temp = (*mutexListHead)->head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = current_task;
    }
}

TCB_Typedef* popWaitingQueue(mutexList** mutexListHead)
{
    if ((*mutexListHead)->head == NULL)
    {
        return NULL;
    }

    TCB_Typedef* next_task = (*mutexListHead)->head;
    (*mutexListHead)->head = next_task->next;
    
    return next_task;
}

void MutexTake(Mutex_Typedef** mutex)
{
    __disable_irq();

    TCB_Typedef* current_task = getCurrentTask();
    
    if (!((*mutex)->lock))
    {
        (*mutex)->lock = 1;
        (*mutex)->owner = current_task;
        return;
    }
    else
    {
        current_task->state = BLOCKED;      // change state from running to waiting / blocked
        pushWaitingQueue(&((*mutex)->waiting_task), current_task);
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
    }
    __enable_irq();
}

void MutexGive(Mutex_Typedef** mutex)
{
    __disable_irq();

    TCB_Typedef* current_task = getCurrentTask();
    
    if ((*mutex)->owner != current_task)
    {
        __enable_irq();
        return;
    }

    if ((*mutex)->waiting_task->head == NULL)
    {
        (*mutex)->lock = 0;
        (*mutex)->owner = NULL;
    }
    else
    {
        TCB_Typedef* next_task = popWaitingQueue(&((*mutex)->waiting_task));
        if (next_task != NULL)
        {
            next_task->state = READY;
            addToReadyList(&next_task);
            (*mutex)->owner = next_task;
            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
        }
    }
    __enable_irq();
}

Mutex_Typedef* createMutex(void)
{
    Mutex_Typedef* newMutex = (Mutex_Typedef*)malloc(sizeof(Mutex_Typedef));
    if (newMutex == NULL)
    {
        return NULL;
    }

    memset(newMutex, 0, sizeof(Mutex_Typedef));

    mutexList* dummy = (mutexList*)malloc(sizeof(mutexList));
    newMutex->waiting_task = dummy;
    newMutex->waiting_task->head = NULL;

    return newMutex;
}