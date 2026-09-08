/* FreeModbus port: event queue (single consumer: eMBPoll). */
#include "mb.h"
#include "mbport.h"

#define EV_QUEUE_SIZE 16

static eMBEventType eEventQueue[EV_QUEUE_SIZE];
static int          iEventQueueHead;
static int          iEventQueueTail;

BOOL xMBPortEventInit(void)
{
    iEventQueueHead = 0;
    iEventQueueTail = 0;
    return TRUE;
}

BOOL xMBPortEventPost(eMBEventType eEvent)
{
    BOOL bResult = TRUE;

    ENTER_CRITICAL_SECTION();
    if ((iEventQueueTail + 1) % EV_QUEUE_SIZE == iEventQueueHead)
    {
        bResult = FALSE; /* queue full */
    }
    else
    {
        eEventQueue[iEventQueueTail] = eEvent;
        iEventQueueTail              = (iEventQueueTail + 1) % EV_QUEUE_SIZE;
    }
    EXIT_CRITICAL_SECTION();

    return bResult;
}

BOOL xMBPortEventGet(eMBEventType *eEvent)
{
    BOOL bResult = FALSE;

    ENTER_CRITICAL_SECTION();
    if (iEventQueueHead != iEventQueueTail)
    {
        *eEvent         = eEventQueue[iEventQueueHead];
        iEventQueueHead = (iEventQueueHead + 1) % EV_QUEUE_SIZE;
        bResult         = TRUE;
    }
    EXIT_CRITICAL_SECTION();

    return bResult;
}
