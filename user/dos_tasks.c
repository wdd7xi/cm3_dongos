
#include "main.h"
#include "dos_tasks.h"

#include "list.h"

//LIST_NODE(tasks_tcb);
//struct list_node *tasks_tcb_head = &tasks_tcb;

uint32 tasks_ready_priority_group = 0x1;
struct list_node tasks_priority_tab[TASKS_PRIOTITY_MAX];


dos_task_tcb_t * tasks_tcb_array[TASKS_DRV_MAX_COUNT] = {0};



uint8 dos_set_event(uint8 task_id, uint32 event_flag)
{
  if (task_id < TASKS_DRV_MAX_COUNT)
  {
	dos_int_state_t int_state;
    ENTER_CRITICAL_SECTION(int_state);    // Hold off interrupts
    tasks_tcb_array[task_id]->event_set |= event_flag;  // Stuff the event bit(s)
	tasks_ready_priority_group |= tasks_tcb_array[task_id]->priority_mask;
    EXIT_CRITICAL_SECTION(int_state);     // Release interrupts
    return 0;//( SUCCESS );
  }
   else
  {
    return 0xff;//( INVALID_TASK );
  }
}

uint8 dos_clear_event(uint8 task_id, uint32 event_flag)
{
  if ( task_id < TASKS_DRV_MAX_COUNT )
  {
	dos_int_state_t int_state;
    ENTER_CRITICAL_SECTION(int_state);    // Hold off interrupts
    tasks_tcb_array[task_id]->event_set &= ~(event_flag);   // Clear the event bit(s)
    EXIT_CRITICAL_SECTION(int_state);     // Release interrupts
    return 0;//( SUCCESS );
  }
   else
  {
    return 0xff;//( INVALID_TASK );
  }
}


int task_add(dos_task_tcb_t *tcb)
{
	int i = 0;
	
	for (i = 0; i < TASKS_DRV_MAX_COUNT; i++) 
	{
		if (NULL == tasks_tcb_array[i]) 
		{
			tasks_tcb_array[i] = tcb;
			tcb->task_id = i;
			tcb->priority_mask = 1L << tcb->priority;
			list_add_tail((struct list_node *)tcb, &(tasks_priority_tab[tcb->priority]));
			break;
		}
	}
	if (TASKS_DRV_MAX_COUNT == i)
	{
		return -1;
	}
	return 0;
}

const uint8 tasks_cnt = TASKS_DRV_MAX_COUNT;//sizeof(tasks_tcb_array)/ sizeof(tasks_tcb_array[0]);

void dos_tasks_init(void)
{
	//struct list_node *pos;
	uint8 i = 0;
	
	for (i = 0; i < TASKS_DRV_MAX_COUNT; i++) 
	{
		INIT_LIST_NODE(&tasks_priority_tab[i]);
	}

	task0_init(0);
	task1_init(0);
	task2_init(0);
	
	#if 0
	list_for_each(pos, (struct list_node *)tasks_tcb_array[0]) {
		if (NULL != ((dos_task_tcb_t *)pos)->init)
		{
			((dos_task_tcb_t *)pos)->init();
		}
	}
	#else
	for (i = 0; i < TASKS_DRV_MAX_COUNT; i++) 
	{
		if ((NULL != tasks_tcb_array[i]) && (NULL != tasks_tcb_array[i]->init))
		{
			tasks_tcb_array[i]->init();
		}
	}
	#endif
	
	return ;
}

