#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#include "stdio.h"
//#define  NULL  (0) 

typedef  unsigned char    uint8;
//typedef  unsigned int     uint32;
typedef  unsigned long     uint32;



#define wd(x)      do { x } while (__LINE__ == -1)

typedef long dos_int_state_t; /* 32bit CPU */ 

extern void hw_interrupt_enable(dos_int_state_t level);
extern dos_int_state_t hw_interrupt_disable(void);

#define ENTER_CRITICAL_SECTION(x)   wd( x = hw_interrupt_disable(); )
#define EXIT_CRITICAL_SECTION(x)    wd( hw_interrupt_enable(x); )
#define CRITICAL_STATEMENT(x)       wd( dos_int_state_t _s; ENTER_CRITICAL_SECTION(_s); x; EXIT_CRITICAL_SECTION(_s); )






#endif
