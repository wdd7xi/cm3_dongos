#include "main.h"
#include "stm32l4xx_hal.h"

#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

uint8 flag0;
dos_task_tcb_t dos_task0_tcb;
#define R_LED_FLASH

struct timer timer0;

static void inside_init(void)
{

	dos_start_timer(&timer0, dos_task0_tcb.task_id, 0x4, 4);
}

void task0_process_fn(void *parg)
{
#if 0
	flag0 = 1;
	dos_task0_tcb.event_set = 0;
	
	dos_set_event( dos_task2_tcb.task_id, 0x1 );
	//tasks_ready_priority_group |= 1 << dos_task2_tcb.priority;
	//dos_task2_tcb.event_set = 0xffffffff;
	
	//tasks_ready_priority_group |= 1 << dos_task1_tcb.priority;
	//dos_task1_tcb.event_set = 0xffffffff;
	flag0 = 0;
#else
	
	if ( dos_task0_tcb.event_set & 0x4 )
	{
		flag0 = 1;
		
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
#if defined(R_LED_FLASH)//LED OFF
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//R 	//PE7 ??1
#elif defined(G_LED_FLASH)
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G 	//PE8 ??1
#elif defined(B_LED_FLASH)
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);//B 	//PE9 ??1
#endif
	//Delay_sw(0x7FFFFF);
		
		dos_start_timer(&timer0, dos_task0_tcb.task_id, 0x2, 500);//500ms
		//dos_set_event(dos_task0_tcb.task_id, 0x2);
		
		dos_task0_tcb.event_set ^= 0x4; //NOK
		//dos_task0_tcb.event_set = 0;      //OK
		return ;
	}
	if ( dos_task0_tcb.event_set & 0x2 )
	{
		flag0 = 0;

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);//G 	//PE8 ??1
#if defined(R_LED_FLASH)//LED ON
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);//R	//PE7 ??0
#elif defined(G_LED_FLASH)
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);//G	//PE8 ??0
#elif defined(B_LED_FLASH)
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);//B	//PE9 ??0
#endif
	//Delay_sw(0x7FFFFF);
		
		dos_start_timer(&timer0, dos_task0_tcb.task_id, 0x4, 500);//500ms
		//dos_set_event(dos_task0_tcb.task_id, 0x4);
		
		dos_task0_tcb.event_set ^= 0x2; //OK
		return ;
	}
#endif
	return ;
}


dos_task_tcb_t dos_task0_tcb = {
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
