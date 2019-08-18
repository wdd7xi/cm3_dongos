#include "type_def.h"
#include "dos_tasks.h"

extern uint8 flag;

void task2_process_fn(void *parg)
{
	flag = 0;
	tasks_tcb_array[1]->event_set = 0;
	tasks_tcb_array[0]->event_set = 0xffffffff;
	return ;
}


dos_task_tcb_t dos_task2_tcb = {
	.process = task2_process_fn,
	.init = 0,
	.event_set = 0xffffffff,
	.parameter = 0,
};
