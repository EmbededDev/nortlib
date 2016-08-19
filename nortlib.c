#include "nortlib.h"

void _initData(void);
NORT_Timer_t* _removeListNode(NORT_Timer_t *delNode);
void _addListNode(NORT_Timer_t* newNode);
void _pushCallback(NORT_TimerCbQueue_t* newNode);
NORT_TimerCbQueue_t* _popCallback(void);

static NORT_Timer_t* _TimerListHead;
static NORT_TimerCbQueue_t* _CallbackQueueHead;
static NORT_TimerCbQueue_t* _CallbackQueueTail;
static volatile U32 _Ticks;
static volatile U32 _EventReg;

void _initData(void)
{
    _TimerListHead = NULL;
    _CallbackQueueHead = NULL;
    _CallbackQueueTail = _CallbackQueueHead;
    _Ticks = 0;
}

NORT_Timer_t* _removeListNode(NORT_Timer_t *delNode)
{
    if(!delNode) return NULL;

    ENTER_CRITICAL();
    if(delNode->prev){
        delNode->prev->next = delNode->next;
    }else{
        _TimerListHead = delNode->next;
    }

    if(delNode->next){
        delNode->next->prev = delNode->prev;
    }
    EXIT_CRITICAL();
    return delNode;
}


void _addListNode(NORT_Timer_t* newNode)
{
    // add as new HEAD
    newNode->prev = NULL;
    newNode->next = _TimerListHead;

    ENTER_CRITICAL();
    if(_TimerListHead){
        _TimerListHead->prev = newNode;
    }
    _TimerListHead = newNode;

    EXIT_CRITICAL();
}


void _pushCallback(NORT_TimerCbQueue_t* newNode)
{
//    NORT_TimerCbQueue_t *MEM_MALLOC(sizeof NORT_TimerCbQueue_t);
    newNode->prev = NULL;
    newNode->next = _CallbackQueueHead;

    ENTER_CRITICAL();
    if(_CallbackQueueHead){
        _CallbackQueueHead->prev = newNode;
        _CallbackQueueHead = newNode;
    }else{
        _CallbackQueueHead = newNode;
        _CallbackQueueTail = _CallbackQueueHead;
    }
    EXIT_CRITICAL();
}


NORT_TimerCbQueue_t* _popCallback(void)
{
    if(!_CallbackQueueTail){
        return NULL;
    } else
    {

        ENTER_CRITICAL();
        NORT_TimerCbQueue_t* delNode = _CallbackQueueTail;
        if(delNode->prev){
            _CallbackQueueTail = delNode->prev;
            _CallbackQueueTail->next = NULL;
        }else {
            _CallbackQueueHead = _CallbackQueueTail = NULL;
        }
        EXIT_CRITICAL();
        return delNode;
    }
}



extern NORT_Status_t NORT_AddTimer(U32 interval, U16 repeat, NORT_TimerCb_t cb, void *param)
{
    NORT_Timer_t *newNode = NULL;

    newNode = MEM_MALLOC(sizeof(NORT_Timer_t));
    if(!newNode) return NORT_STATUS_MEM_FAILED;

    newNode->cb = cb;
    newNode->param = param;
    newNode->counter = 0;
    newNode->repeat = repeat;
    newNode->interval = interval;
    newNode->prev = NULL;
    newNode->next = NULL;

    _addListNode(newNode);
    return NORT_STATUS_OK;
}

extern void NORT_Init(void)
{
    _initData();
}

// run the function in system timer ISR
extern void NORT_Tick(void)
{
        NORT_Timer_t *entry = _TimerListHead;
        while(entry){
            NORT_Timer_t *next = entry->next;

            if(entry->repeat == 0) {
                entry = next;
                continue;
            }

            // timeout
            if(++entry->counter >= entry->interval){
                entry->counter = 0;

                // push callback into queue
                NORT_TimerCbQueue_t* newNode = MEM_MALLOC(sizeof(NORT_TimerCbQueue_t));
                if(!newNode) return;
                newNode->cb = entry->cb;
                newNode->param = entry->param;
                _pushCallback(newNode);

                // delete entry if dont need repeat
                if(entry->repeat != REPEAT_FOREVER && --entry->repeat <= 0){
                    NORT_Timer_t* delNode = _removeListNode(entry);
                    if(delNode) MEM_FREE(delNode);
                }

            }else {
    //            entry->counter ++;
            }
            entry = next;

        }

    // tick counts

    if(_Ticks+1 > NORT_MAX_TICKS){
        _Ticks = 0;
    }else{
        _Ticks ++;
    }
}


// call earliest callback in the queue and pop it
extern void NORT_InnerLoop(void)
{
    NORT_TimerCbQueue_t *node = NULL;

    node = _popCallback();
    if(node){
        if(node->cb)
            node->cb(node->param);

        MEM_FREE(node);
    }
}

extern void NORT_DelayBlock(U32 delay)
{
    if(_Ticks + delay <= NORT_MAX_TICKS){
        U32 future = _Ticks + delay;
        while (_Ticks < future);
    }else {
        U32 start = _Ticks;
        U32 future_overflow = (U64)(_Ticks + delay) - NORT_MAX_TICKS;

        while(_Ticks >= start || _Ticks < future_overflow);
    }

}

extern U32 NORT_GetTicks(void)
{
    return _Ticks;
}



// event
extern U32 NORT_ClearEvent(U32 event)
{
    _EventReg &= ~event;

    return _EventReg;
}

extern U32 NORT_SetEvent(U32 event)
{
    _EventReg |= event;

    return _EventReg;
}

extern U8 NORT_IsEventSet(U32 event)
{
    return (_EventReg & event) == event;
}

