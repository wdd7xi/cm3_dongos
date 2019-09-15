#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

uint8 flag2;
dos_task_tcb_t dos_task2_tcb;

struct timer timer2;
static void inside_init(void)
{

	dos_start_timer(&timer2, dos_task2_tcb.task_id, 0x4, 3);
}

void task2_process_fn(void *parg)
{
#if 0
	flag2 = 1;
	dos_task2_tcb.event_set = 0;
	
	//dos_set_event( dos_task0_tcb.task_id, 0x1 );
	dos_set_event( dos_task1_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task1_tcb.priority;
	//dos_task1_tcb.event_set = 0xffffffff;
	
	//tasks_ready_priority_group |= 1 << dos_task0_tcb.priority;
	//dos_task0_tcb.event_set = 0xffffffff;
	flag2 = 0;
#else
	
	if ( dos_task2_tcb.event_set & 0x4 )
	{
		flag2 = 1;
		
		dos_start_timer(&timer2, dos_task2_tcb.task_id, 0x2, 3);
		//delay(200);
		//dos_set_event(dos_task2_tcb.task_id, 0x2);
		
		dos_task2_tcb.event_set ^= 0x4; //NOK
		//dos_task2_tcb.event_set = 0;      //OK
		return ;
	}
	if ( dos_task2_tcb.event_set & 0x2 )
	{
		flag2 = 0;
		
		dos_start_timer(&timer2, dos_task2_tcb.task_id, 0x4, 3);
		//delay(200);
		//dos_set_event(dos_task2_tcb.task_id, 0x4);
		
		dos_task2_tcb.event_set ^= 0x2; //OK
		return ;
	}
#endif
	return ;
}


dos_task_tcb_t dos_task2_tcb = {
	.process = task2_process_fn,
	.parameter = 0,
	
	.priority = 5,
	
	.init_tick = 1,
	
	.init = inside_init,
	.event_set = 0x3,
};

void task2_init(struct list_node *head)
{
	task_add(&dos_task2_tcb);
	
	//dos_start_timer(&timer2, dos_task2_tcb.task_id, 0x4, 3);
}
