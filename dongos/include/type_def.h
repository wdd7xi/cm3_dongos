#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#include "stdio.h"

#ifndef NULL
   #define NULL       ((void*)0)
#endif

typedef unsigned char       BOOL;

//芯片硬件字长 //数据总线宽度: 32 bit 
typedef unsigned int       hal_data_align_t;

// Unsigned numbers
typedef  unsigned char    uint8;
typedef  unsigned char    byte;
typedef  unsigned short   uint16;
//typedef  unsigned int     uint32;
typedef  unsigned long    uint32;

// Signed numbers
typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;


#ifndef FALSE
  #define FALSE       0
#endif

#ifndef ARRAY_NULL
   #define ARRAY_NULL '\0'
#endif

#ifndef TRUE
   #define TRUE       1
#endif

#ifndef OPEN
   #define OPEN       1
#endif

#ifndef CLOSE
   #define CLOSE      0
#endif

#ifndef HIGH
   #define HIGH       1
#endif

#ifndef LOW
   #define LOW        0
#endif

#if 0
#ifndef SUCCESS
  #define SUCCESS     1
#endif

#ifndef ERROR
  #define ERROR       0
#endif
#endif



#define wd(x)      do { x } while (__LINE__ == -1)
#define DOS_ALIGN(size, align)      (((size) + (align) - 1) & ~((align) - 1))

typedef long dos_int_state_t; /* 32bit CPU */ 

extern void hw_interrupt_enable(dos_int_state_t level);
extern dos_int_state_t hw_interrupt_disable(void);

#define ENTER_CRITICAL_SECTION(x)   wd( x = hw_interrupt_disable(); )
#define EXIT_CRITICAL_SECTION(x)    wd( hw_interrupt_enable(x); )
#define CRITICAL_STATEMENT(x)       wd( dos_int_state_t _s; ENTER_CRITICAL_SECTION(_s); x; EXIT_CRITICAL_SECTION(_s); )






#endif
