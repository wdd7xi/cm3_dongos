#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

uint8 flag1 = 0;
dos_task_tcb_t dos_task1_tcb;

struct timer timer1;
static void inside_init(void)
{

	dos_start_timer(&timer1, dos_task1_tcb.task_id, 0x4, 2);
}

void task1_process_fn(void *parg)
{
#if 0
	flag1 = 1;
	dos_task1_tcb.event_set = 0;
	
	//tasks_ready_priority_group |= 1 << dos_task2_tcb.priority;
	//dos_task2_tcb.event_set = 0xffffffff;
	
	dos_set_event( dos_task0_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task0_tcb.priority;
	//dos_task0_tcb.event_set = 0xffffffff;
	flag1 = 0;
#else
	
	if ( dos_task1_tcb.event_set & 0x4 )
	{
		flag1 = 1;
		
		dos_start_timer(&timer1, dos_task1_tcb.task_id, 0x2, 2);
		//delay(10);
		//dos_set_event(dos_task1_tcb.task_id, 0x2);
		
		dos_task1_tcb.event_set ^= 0x4; //NOK
		//dos_task1_tcb.event_set = 0;      //OK
		return ;
	}
	if ( dos_task1_tcb.event_set & 0x2 )
	{
		flag1 = 0;
		
		dos_start_timer(&timer1, dos_task1_tcb.task_id, 0x4, 2);
		//delay(10);
		//dos_set_event(dos_task1_tcb.task_id, 0x4);
		
		dos_task1_tcb.event_set ^= 0x2; //OK
		return ;
	}
#endif
	return ;
}


dos_task_tcb_t dos_task1_tcb = {
	.process = task1_process_fn,
	.parameter = 0,
	
	.priority = 31,
	
	.init_tick = 2,
	
	.init = inside_init,
	.event_set = 0x2,
};

void task1_init(struct list_node *head)
{
	//list_add(&(dos_task1_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task1_tcb);
	
	//dos_start_timer(&timer1, dos_task1_tcb.task_id, 0x4, 4);
}
