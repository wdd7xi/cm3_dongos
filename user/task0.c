#include "main.h"
#include "stm32l4xx_hal.h"

#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

#include "task0.h"

uint8 flag0;
static dos_task_tcb_t dos_task0_tcb;
#define ON_TIMES_MS    1000


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
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_GREEN_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_RED_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_GREEN_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_BLUE_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_GREEN_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_BLUE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_ORANGE_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_BLUE_ON; 
		return ;
	}
	
	if (current_tcb->event_set & LED_EVT_ORANGE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_CYAN_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_ORANGE_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_CYAN_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_PURPLE_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_CYAN_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_PURPLE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_WHITE_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_PURPLE_ON; 
		return ;
	}
	
	if (current_tcb->event_set & LED_EVT_WHITE_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_BLACK_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_WHITE_ON; 
		return ;
	}
	if (current_tcb->event_set & LED_EVT_BLACK_ON)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R	//PE7 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G	//PE8 ??0
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B	//PE9 ??0
		
		dos_start_timer(&timer0, current_tcb->task_id, LED_EVT_RED_ON, ON_TIMES_MS);//500ms
		
		current_tcb->event_set ^= LED_EVT_BLACK_ON; 
		return ;
	}

	return ;
}


static dos_task_tcb_t dos_task0_tcb = {
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
