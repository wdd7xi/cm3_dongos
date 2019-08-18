
#include "main.h"
#include "dos_tasks.h"


dos_task_tcb_t * tasks_tcb_array[] = 
{
	&dos_task1_tcb,
	&dos_task2_tcb

};
const uint8 tasks_cnt = sizeof(tasks_tcb_array)/ sizeof(tasks_tcb_array[0]);

void dos_tasks_init(void)
{
	uint8 i;

	for (i = 0; i < tasks_cnt; i++)
	{
		if (NULL != tasks_tcb_array[i]->init)
		{
			tasks_tcb_array[i]->init();
		}
	}


}

