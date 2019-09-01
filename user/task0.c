#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"

uint8 flag0;



void task0_process_fn(void *parg)
{
	flag0 = 1;
	dos_task0_tcb.event_set = 0;
	
	dos_set_event( dos_task2_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task2_tcb.priority;
	//dos_task2_tcb.event_set = 0xffffffff;
	
	//tasks_ready_priority_group |= 1 << dos_task1_tcb.priority;
	//dos_task1_tcb.event_set = 0xffffffff;
	flag0 = 0;
	return ;
}


dos_task_tcb_t dos_task0_tcb = {
	.process = task0_process_fn,
	.priority = 0,
	.init = 0,
	.event_set = 0x1,
	.parameter = 0,
};

void task0_init(struct list_node *head)
{
	//list_add(&(dos_task0_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task0_tcb);
}
