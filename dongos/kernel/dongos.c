#include "dongos.h"
#include "dos_tasks.h"
#include "list.h"

static void dos_system_run(void)
{
	uint8 idx;
	struct list_node *pos;
	
	list_for_each(pos, tasks_tcb_head) {
		if (0 != ((dos_task_tcb_t *)pos)->event_set)
		{
			break;
		}
	}
	
	if (pos != tasks_tcb_head)
	{
		((dos_task_tcb_t *)pos)->process(0);
	}
	
	return ;
	
	for (idx = 0; idx < tasks_cnt; idx++)
	{
		if (0 != tasks_tcb_array[idx]->event_set)
		{
			break;
		}
	}
	
	if (idx < tasks_cnt)
	{
		tasks_tcb_array[idx]->process(0);
	}
}

void dos_system_init(void)
{
	dos_tasks_init();
}

void dos_system_start(void)
{
	while (1)
	{
		dos_system_run();
	}
}

