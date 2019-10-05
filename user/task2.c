#include "stm32l4xx_hal.h"

#include "type_def.h"
#include "dos_tasks.h"
#include "list.h"
#include "timer.h"

#include "task2.h"

#include "task0.h"
#include "task1.h"

static uint8 count = 0;
static dos_task_tcb_t dos_task2_tcb;

static struct timer timer2;
static dos_task_tcb_t *current_tcb = NULL;

/*
	KEY0 	- PD10
	KEY1 	- PD9
	KEY2 	- PD8
	WK_UP 	- PC13
*/
#define KEY0    HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10)
#define KEY1    HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9)
#define KEY2    HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)
#define WK_UP   HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)

#define KEY0_PRES    1
#define KEY1_PRES    2
#define KEY2_PRES    3
#define WK_UP_PRES   4

void key_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    __HAL_RCC_GPIOC_CLK_ENABLE(); 
    __HAL_RCC_GPIOD_CLK_ENABLE();  

    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 ;	//PD8.9.10
    GPIO_Initure.Mode = GPIO_MODE_INPUT; 
    GPIO_Initure.Pull = GPIO_PULLDOWN;  
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; 
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_13;     
    GPIO_Initure.Mode = GPIO_MODE_INPUT; 
    GPIO_Initure.Pull = GPIO_PULLUP;     
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; 
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);
}
static uint8 key_up = 0;
static uint8 key = 0;
uint8 key_scan(uint8 mode)
{
	if (mode == 1) 
	{
		key_up = 1;
	}
	
	if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1))
	{
		key_up = 7;//key first pres
		return key_up;
	}
    else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0)
	{
		key_up = 1;//key is up 
		return key_up;
	}
	
	return 0;//no key pres 
}
uint8 read_key_state(void)
{
	//if (key_up == 7) 
	{
        if(KEY0 == 0)       return KEY0_PRES;

        else if(KEY1 == 0)  return KEY1_PRES;

        else if(KEY2 == 0)  return KEY2_PRES;

        else if(WK_UP == 1) return WK_UP_PRES;
	}
}


static void inside_init(void *parg)
{
	current_tcb = parg;
	dos_start_timer(&timer2, current_tcb->task_id, KEY_EVT_PERIOD_SCAN, 3);
	
	key_init();
}

static void task2_process_fn(void *parg)
{
	if (current_tcb->event_set & KEY_EVT_PERIOD_SCAN)
	{
		if (7 == key_scan(0))
		{
			key_up = 0;
			++count;
		}
		if (0 == count%2)
			dos_start_timer(&timer2, current_tcb->task_id, KEY_EVT_READ_STATE, 10);
		else
			dos_start_timer(&timer2, current_tcb->task_id, KEY_EVT_PERIOD_SCAN, 10);
		
		current_tcb->event_set ^= KEY_EVT_PERIOD_SCAN;
		return ;
	}
	if (current_tcb->event_set & KEY_EVT_READ_STATE)
	{
		key = read_key_state();
		switch (key)
        {
            case WK_UP_PRES:		//¿ØÖÆ·äÃùÆ÷×´Ì¬·­×ª
                //BEEP_TogglePin;
                dos_set_event(dos_task1_tcb.task_id, BEEP_EVT_TOGGLE);
                break;

            case KEY2_PRES:		//¿ØÖÆLED_BºìµÆ×´Ì¬·­×ª
                //LED_B_TogglePin;
                dos_set_event(dos_task0_tcb.task_id, LED_EVT_GREEN_TOGGLE);
                break;

            case KEY1_PRES:		//¿ØÖÆLED_GÂÌµÆ×´Ì¬·­×ª
                //LED_G_TogglePin;
                dos_set_event(dos_task0_tcb.task_id, LED_EVT_RED_TOGGLE);
                break;

            case KEY0_PRES:		//¿ØÖÆLED_RÀ¶µÆ×´Ì¬·­×ª
                //LED_R_TogglePin;
                dos_set_event(dos_task0_tcb.task_id, LED_EVT_BLUE_TOGGLE);
                break;
			default:
				break;
        }
		
		dos_start_timer(&timer2, current_tcb->task_id, KEY_EVT_PERIOD_SCAN, 10);
		
		current_tcb->event_set ^= KEY_EVT_READ_STATE; 
		return ;
	}

	return ;
}


static dos_task_tcb_t dos_task2_tcb = 
{
	.process = task2_process_fn,
	.parameter = 0,
	
	.priority = 5,
	
	.init_tick = 1,
	
	.init = inside_init,
	.event_set = 0x3,
};

void task2_init(struct list_node *head)
{
	task_add(&dos_task2_tcb);
	
	//dos_start_timer(&timer2, dos_task2_tcb.task_id, 0x4, 3);
}
