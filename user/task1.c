#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"

uint8 flag1 = 0;



void task1_process_fn(void *parg)
{
	flag1 = 1;
	dos_task1_tcb.event_set = 0;
	
	//tasks_ready_priority_group |= 1 << dos_task2_tcb.priority;
	//dos_task2_tcb.event_set = 0xffffffff;
	
	dos_set_event( dos_task0_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task0_tcb.priority;
	//dos_task0_tcb.event_set = 0xffffffff;
	flag1 = 0;
	return ;
}


dos_task_tcb_t dos_task1_tcb = {
	.process = task1_process_fn,
	.priority = 10,
	.init = 0,
	.event_set = 0x2,
	.parameter = 0,
};

void task1_init(struct list_node *head)
{
	//list_add(&(dos_task1_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task1_tcb);
}
