#include "dongos.h"
#include "dos_tasks.h"
#include "list.h"

#include "type_def.h"

#include "ARMCM3.h"


const uint8 __lowest_bit_bitmap[] =
{
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

int __ffs(uint32 value)
{
	if (value == 0)  return 0;
	
	if (value & 0xff)
		return __lowest_bit_bitmap[value & 0xff] + 1;
	
	if (value & 0xff00)
		return __lowest_bit_bitmap[(value & 0xff00) >> 8] + 9;
	
	if (value & 0xff0000)
		return __lowest_bit_bitmap[(value & 0xff0000) >> 16] + 17;
	
	return __lowest_bit_bitmap[(value & 0xff000000) >> 24] + 25;

}

static void dos_system_run(void)
{
	struct list_node *pos, *tasks_tcb_head;
	register uint32 highest_ready_priority;
	dos_int_state_t int_state;
	
	dos_task_tcb_t *current_tcb;
	
	//uint8 current_task_id;
	//uint8 current_priority;
	
	uint32 current_event_set;
	
	ENTER_CRITICAL_SECTION(int_state);
	
	highest_ready_priority = __ffs(tasks_ready_priority_group) - 1;// - 1 ?
	if (highest_ready_priority > TASKS_PRIOTITY_MAX)
	{
		
		EXIT_CRITICAL_SECTION(int_state);
		return ;
	}
	
	tasks_tcb_head = &tasks_priority_tab[highest_ready_priority];
	
	EXIT_CRITICAL_SECTION(int_state);
	
	if (NULL == tasks_tcb_head)
	{
		return ;
	}
	if (list_empty_careful(tasks_tcb_head))
	{
		return ;
	}
	
	list_for_each(pos, tasks_tcb_head) 
	{
		current_tcb = (dos_task_tcb_t *)pos;
		//current_task_id = current_tcb->task_id;
		if (pos == NULL)
		{
			break;
		}
		//current_priority = current_tcb->priority;
		current_event_set = current_tcb->event_set;
		
		current_event_set = current_tcb->event_set;
		if ((0 != current_event_set)
		 && (NULL != current_tcb->process) )
		{
			current_tcb->process(current_tcb);
			//break;//continue  ?
		}
		
		if (list_is_last(pos, tasks_tcb_head))
		{
			ENTER_CRITICAL_SECTION(int_state);
			//current_tcb = (dos_task_tcb_t *)pos;
			tasks_ready_priority_group &= ~(current_tcb->priority_mask);
			EXIT_CRITICAL_SECTION(int_state);
			break;
		}
	}
	
	if (pos == tasks_tcb_head)
	{
		ENTER_CRITICAL_SECTION(int_state);
		current_tcb = (dos_task_tcb_t *)pos->prev;
		tasks_ready_priority_group &= ~(current_tcb->priority_mask);
		EXIT_CRITICAL_SECTION(int_state);
	}
	
	return ;
}

#define TICK_PER_SECOND       100

void dos_system_init(void)
{
	SysTick_Config( SystemCoreClock / TICK_PER_SECOND );
	
	dos_tasks_init();
}

void dos_system_start(void)
{
	while (1)
	{
		dos_system_run();
	}
}

