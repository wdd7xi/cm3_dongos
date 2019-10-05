#include "type_def.h"

dos_int_state_t sys_int_state;

extern void tick_increase(void);

//void SysTick_Handler(void)
void HAL_SYSTICK_Callback(void)
{
	ENTER_CRITICAL_SECTION(sys_int_state);
	
	tick_increase();
	
	EXIT_CRITICAL_SECTION(sys_int_state);
}

