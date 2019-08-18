#include "dongos.h"
#include "dos_tasks.h"

static void dos_system_run(void)
{
	uint8 idx;
	
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

