//#include "main.h"
#include "stm32l4xx_hal.h"

#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

#include "task0.h"

static uint8 count = 0;
static uint8 is_continue = 1;
dos_task_tcb_t dos_task0_tcb;
#define ON_TIMES_MS    500


static struct timer timer0;
static dos_task_tcb_t *current_tcb = NULL;

static void inside_init(void *parg)
{
	current_tcb = parg;
	//MX_GPIO_Init();
	//dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_RED_ON, 4);
	dos_set_event(current_tcb->task_id, LED_EVT_RED_ON);
}

static void task0_process_fn(void *parg)
{
	if (current_tcb->event_set & LED_EVT_RED_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		++count;
		if ((count > 2) || (0 == is_continue))
		{
			//dos_set_event(current_tcb->task_id, LED_EVT_STOP);
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_STOP, 10);
		}
		else 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_GREEN_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_RED_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_GREEN_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_BLUE_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_GREEN_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_BLUE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_ORANGE_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_BLUE_ON; 
		return ;
	}
	
	if (current_tcb->event_set & LED_EVT_ORANGE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_CYAN_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_ORANGE_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_CYAN_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_PURPLE_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_CYAN_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_PURPLE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_WHITE_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_PURPLE_ON; 
		return ;
	}
	
	if (current_tcb->event_set & LED_EVT_WHITE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_BLACK_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_WHITE_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_BLACK_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_RED_ON, ON_TIMES_MS);//500ms
		}
		
		current_tcb->event_set ^= LED_EVT_BLACK_ON; 
		return ;
	}
	
	if (current_tcb->event_set & LED_EVT_RED_TOGGLE)
	{
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_7);//R	//PE7  
		
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);//G	//PE8  
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);//B	//PE9  
		
		current_tcb->event_set ^= LED_EVT_RED_TOGGLE; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_GREEN_TOGGLE)
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8);//G	//PE8  
		
		current_tcb->event_set ^= LED_EVT_GREEN_TOGGLE; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_BLUE_TOGGLE)
	{
		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_9);//B	//PE9  
		
		current_tcb->event_set ^= LED_EVT_BLUE_TOGGLE; 
		return ;
	}

	if (current_tcb->event_set & LED_EVT_STOP)
	{
		is_continue = 0;
		
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		current_tcb->event_set ^= LED_EVT_STOP; 
		return ;
	}

	return ;
}


dos_task_tcb_t dos_task0_tcb = 
{
	.process = task0_process_fn,
	.parameter = 0,
	
	.priority = 0,
	
	.init_tick = 3,
	
	.init = inside_init,
	.event_set = 0,//0x4,
};

void task0_init(struct list_node *head)
{
	//dos_set_event(dos_task0_tcb.task_id, 0x4); //now NOK
	//list_add(&(dos_task0_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task0_tcb);
	
}
