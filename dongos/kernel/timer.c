#include "list.h"
#include "timer.h"
#include "dos_tasks.h"


static struct list_node gtimer_list_head;
static LIST_NODE(gtimer_list_head);
static struct list_node * pgtimer_list_head = &gtimer_list_head;

static uint32 tick = 0;

#define TICK_MAX    420000//7*60*1000ms //100 
uint32 random_nr;

uint32 tick_get(void)
{
	return tick;
}

static struct timer *dos_find_timer(uint8 task_id, uint32 event_flag)
{
  struct timer * srch_timer;

  // Head of the timer list
  srch_timer = (struct timer *)(pgtimer_list_head->next);

  // Stop when found or at the end
  while ((struct timer *)pgtimer_list_head != srch_timer)
  {
    if ( srch_timer->event_flag == event_flag &&
         srch_timer->task_id == task_id )
    {
      break;
    }

    // Not this one, check another
    srch_timer = (struct timer *)(srch_timer->tnode.next);
  }
  
  if (pgtimer_list_head == (struct list_node *)srch_timer)
	  return NULL;

  return srch_timer;
}

static int zj_insert_sort_to_timer_list(struct timer *new)
{
	//struct list_node * pos = NULL;
	struct timer * current = (struct timer *)&gtimer_list_head;//(pgtimer_list_head);
	uint32 diff_tick = 0;
	
	//if (list_empty(pgtimer_list_head))
	//{
	//	goto add_node;
	//}
	//for (;current != (struct timer *)(pgtimer_list_head->prev); current = (struct timer *)current->tnode.next)
	for (;current != (struct timer *)(gtimer_list_head.prev); current = (struct timer *)current->tnode.next)
	//list_for_each(pos, pgtimer_list_head)
	{
		struct timer * t = (struct timer *)current->tnode.next;
		//current = (struct timer *)pos;
		diff_tick = t->timeout_tick - new->timeout_tick;
		if (0 == diff_tick)
		{
			continue;
		}
		else if (diff_tick < TICK_MAX / 2)
		{
			//current = (struct timer *)current->tnode.prev;
			break;
		}
		else 
		{
			;
		}
	}

//add_node:
	random_nr++;
	
	list_add((struct list_node *)new, (struct list_node *)current);
	
	//new->timeout_tick = tick_get() + new->init_tick;
	return 0;
}

static uint8 dos_add_timer(struct timer * new_timer, uint8 taskID, uint32 event_id, uint32 timeout_value)
{
	struct timer *find_timer;
	
	find_timer = dos_find_timer(taskID, event_id);
	if (NULL != find_timer)
	{
		//find_timer->init_tick = timeout_value;
		find_timer->timeout_tick = tick_get() + timeout_value;
		//... todo del 
		list_del_init((struct list_node *)find_timer);
		
		zj_insert_sort_to_timer_list(find_timer);
	}
	else
	{
		new_timer->task_id = taskID;
		new_timer->event_flag = event_id;
		new_timer->timeout_tick = tick_get() + timeout_value;
		INIT_LIST_NODE((struct list_node *)new_timer);
		//... todo insert
		zj_insert_sort_to_timer_list(new_timer);
	}
	return 0;
}


uint8 dos_start_timer(struct timer * new_timer, uint8 taskID, uint32 event_id, uint32 timeout_value)
{
	dos_int_state_t int_state;
	//struct timer * new_timer;
	
	ENTER_CRITICAL_SECTION(int_state);
	
	dos_add_timer(new_timer, taskID, event_id, timeout_value);
	
	EXIT_CRITICAL_SECTION(int_state);

	return 0;
}
uint8 dos_start_reload_timer(struct timer * new_timer, uint8 taskID, uint32 event_id, uint32 timeout_value)
{
	dos_int_state_t int_state;
	//struct timer * new_timer;
	
	ENTER_CRITICAL_SECTION(int_state);
	
	dos_add_timer(new_timer, taskID, event_id, timeout_value);
    new_timer->reload_timeout = timeout_value;
	
	EXIT_CRITICAL_SECTION(int_state);

	return 0;
}



static void timer_check(void)
{
	dos_int_state_t int_state;
	struct timer * t;
	uint32 current_tick;

	current_tick = tick_get();
	ENTER_CRITICAL_SECTION(int_state);
	while (!list_empty(&gtimer_list_head))
	{
		t = (struct timer *)(gtimer_list_head.next);
		
		if ((current_tick - t->timeout_tick) < TICK_MAX / 2)
		{
			list_del_init((struct list_node *)t);
			
			dos_set_event(t->task_id, t->event_flag);
			
			
		}
		else
		{
			break;
		}
	}
	EXIT_CRITICAL_SECTION(int_state);
}

int task_yield(void)
{
	dos_int_state_t int_state;
	dos_task_tcb_t * tcb = task_tcb_self();
	
	ENTER_CRITICAL_SECTION(int_state);
	
	if (tcb->dt_node.next != tcb->dt_node.prev)
	{
		//list_del((struct list_node *)tcb);
		list_move_tail((struct list_node *)tcb, pgtimer_list_head);
	}
	
	EXIT_CRITICAL_SECTION(int_state);
	
	return 0;
}

void tick_increase(void)
{
	//dos_task_tcb_t * tcb;

	++tick;
#if 0
	tcb = task_tcb_self();
	if (NULL != tcb)
	{
		--tcb->remaining_tick;
		if (0 == tcb->remaining_tick)
		{
			tcb->remaining_tick = tcb->init_tick;
			task_yield();
		}
	}
#endif
	
	timer_check();//pro 0-OK    1-NOK
}

