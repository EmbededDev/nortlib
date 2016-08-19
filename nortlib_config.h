
#ifndef __NORTLIB_H__
#define __NORTLIB_H__

#include <stdlib.h>
#include <malloc.h>

#include <stdio.h>

#define U64 unsigned long
#define U32 unsigned int
#define U16 unsigned short
#define U8 unsigned char



#define MAX_TIMER_SUPPORT   64

// define functions
#define MEM_MALLOC  malloc
#define MEM_FREE    free

#define ENTER_CRITICAL()   // do{printf("enter critical!\n");}while(0)
#define EXIT_CRITICAL()    // do{printf("exit critical!\n");}while(0)


#define BASIC_DELAY()   do{}while(0)        // do something in order to freeze the program for a while




#endif /* __NORTLIB_H__ */
