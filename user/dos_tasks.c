
#include "main.h"
#include "dos_tasks.h"

#include "list.h"

LIST_NODE(tasks_tcb);
struct list_node *tasks_tcb_head = &tasks_tcb;

dos_task_tcb_t * tasks_tcb_array[] = 
{
	&dos_task1_tcb,
	&dos_task2_tcb

};
const uint8 tasks_cnt = sizeof(tasks_tcb_array)/ sizeof(tasks_tcb_array[0]);

void dos_tasks_init(void)
{
	uint8 i;
	struct list_node *pos;

	task1_init(tasks_tcb_head);
	task2_init(tasks_tcb_head);
	
	list_for_each(pos, tasks_tcb_head) {
		if (NULL != ((dos_task_tcb_t *)pos)->init)
		{
			((dos_task_tcb_t *)pos)->init();
		}
	}
	
	return ;
	for (i = 0; i < tasks_cnt; i++)
	{
		if (NULL != tasks_tcb_array[i]->init)
		{
			tasks_tcb_array[i]->init();
		}
	}


}

