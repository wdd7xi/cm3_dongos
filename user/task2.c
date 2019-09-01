#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"

uint8 flag2;

void task2_process_fn(void *parg)
{
	flag2 = 1;
	dos_task2_tcb.event_set = 0;
	
	dos_set_event( dos_task0_tcb.task_id, 0x1 );
	dos_set_event( dos_task1_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task1_tcb.priority;
	//dos_task1_tcb.event_set = 0xffffffff;
	
	//tasks_ready_priority_group |= 1 << dos_task0_tcb.priority;
	//dos_task0_tcb.event_set = 0xffffffff;
	flag2 = 0;
	return ;
}


dos_task_tcb_t dos_task2_tcb = {
	.process = task2_process_fn,
	.priority = 20,
	.init = 0,
	.event_set = 0x3,
	.parameter = 0,
};

void task2_init(struct list_node *head)
{
	//list_add(&(dos_task2_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task2_tcb);
}
