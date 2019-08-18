#include "type_def.h"
#include "dos_tasks.h"

uint8 flag = 0;

void task1_process_fn(void *parg)
{
	flag = 1;
	tasks_tcb_array[0]->event_set = 0;
	tasks_tcb_array[1]->event_set = 0xffffffff;
	return ;
}


dos_task_tcb_t dos_task1_tcb = {
	.process = task1_process_fn,
	.init = 0,
	.event_set = 0xffffffff,
	.parameter = 0,
};
