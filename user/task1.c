#include "stm32l4xx_hal.h"

#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

#include "task1.h"

static uint8 count = 0;
static uint8 is_continue = 1;
dos_task_tcb_t dos_task1_tcb;
#define ON_TIMES_MS    500

static struct timer timer1;
static dos_task_tcb_t *current_tcb = NULL;


void beep_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//PB2
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}


static void inside_init(void *parg)
{
	current_tcb = parg;
	dos_start_timer(&timer1, current_tcb->task_id, BEEP_EVT_OFF, 2);
	
	beep_init();
}

static void task1_process_fn(void *parg)
{
	if (current_tcb->event_set & BEEP_EVT_ON)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		
		++count;
		if ((count > 3) || (0 == is_continue))
		{
			dos_start_timer(&timer1, current_tcb->task_id, BEEP_EVT_STOP, 10);
		}
		else 
		{
			dos_start_timer(&timer1, current_tcb->task_id, BEEP_EVT_OFF, ON_TIMES_MS);
		}

		current_tcb->event_set ^= BEEP_EVT_ON;
		return ;
	}
	if (current_tcb->event_set & BEEP_EVT_OFF)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		
		if (1 == is_continue) 
		{
			dos_start_timer(&timer1, current_tcb->task_id, BEEP_EVT_ON, ON_TIMES_MS);
		}

		current_tcb->event_set ^= BEEP_EVT_OFF;
		return ;
	}
	
	if (current_tcb->event_set & BEEP_EVT_TOGGLE)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);//PB2
		
		current_tcb->event_set ^= BEEP_EVT_TOGGLE; 
		return ;
	}
	
	if (current_tcb->event_set & BEEP_EVT_STOP)
	{
		is_continue = 0;
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);

		current_tcb->event_set ^= BEEP_EVT_STOP;
		return ;
	}

	return ;
}


dos_task_tcb_t dos_task1_tcb = 
{
	.process = task1_process_fn,
	.parameter = 0,
	
	.priority = 31,
	
	.init_tick = 2,
	
	.init = inside_init,
	.event_set = 0x2,
};

void task1_init(struct list_node *head)
{
	//list_add(&(dos_task1_tcb.dt_list), &(tasks_priority_tab[1]));
	task_add(&dos_task1_tcb);
	
	//dos_start_timer(&timer1, dos_task1_tcb.task_id, 0x4, 4);
}
