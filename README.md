# nortlib
Simple event and timer framework for MCU which has limit resource and don't need RealTime feature.

# Usage
* usage NORT_AddTimer() function to add a task to excute after *interval* ticks, *repeat* times
* call NORT_Tick() in a timer ISR
* call NORT_InnerLoop() in a `main loop`
* NORT_DelayBlock(delay) block the program for *delay* ticks
* NORT_ClearEvent, NORT_SetEvent,NORT_IsEventSet for events

# API
    extern NORT_Status_t NORT_AddTimer(U32 interval, U16 repeat, NORT_TimerCb_t cb, void *param);
    extern void NORT_Init(void);
    extern void NORT_Tick(void);
    extern void NORT_InnerLoop(void);
    
    extern void NORT_DelayBlock(U32 delay);
    extern U32 NORT_GetTicks(void);
    
    extern U32 NORT_ClearEvent(U32 event);
    extern U32 NORT_SetEvent(U32 event);
    extern U8 NORT_IsEventSet(U32 event);
