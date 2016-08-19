
#include "nortlib_config.h"

#define REPEAT_FOREVER  0xFFFF
#define NORT_MAX_TICKS  0xFFFF

#ifndef NULL
#define NULL   0
#endif

typedef void (*NORT_TimerCb_t)(void *param);


typedef struct NORT_Timer
{
    struct NORT_Timer *prev;
    struct NORT_Timer *next;

    U32 interval;
    U32 counter;
    U16 repeat;
    U16 _reserved;

    NORT_TimerCb_t cb;
    void *param;
}NORT_Timer_t;


typedef struct NORT_TimerCbQueue
{
    struct NORT_TimerCbQueue *prev;
    struct NORT_TimerCbQueue *next;
    NORT_TimerCb_t cb;
    void *param;

}NORT_TimerCbQueue_t;


typedef enum NORT_Status{
    NORT_STATUS_OK = 0,
    NORT_STATUS_MEM_FAILED = -1
}NORT_Status_t;


extern NORT_Status_t NORT_AddTimer(U32 interval, U16 repeat, NORT_TimerCb_t cb, void *param);
extern void NORT_Init(void);
extern void NORT_Tick(void);
extern void NORT_InnerLoop(void);
extern void NORT_DelayBlock(U32 delay);
extern U32 NORT_GetTicks(void);

extern U32 NORT_ClearEvent(U32 event);
extern U32 NORT_SetEvent(U32 event);
extern U8 NORT_IsEventSet(U32 event);
