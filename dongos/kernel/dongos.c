#include "dongos.h"
#include "dos_tasks.h"
#include "list.h"

static void dos_system_run(void)
{
	struct list_node *pos, *tasks_tcb_head;
	
	tasks_tcb_head = &tasks_priority_tab[1];
	if (NULL == tasks_tcb_head)
	{
		return ;
	}
	
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

