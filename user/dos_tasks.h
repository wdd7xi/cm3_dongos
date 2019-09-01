#ifndef __DOS_TASKS_H__
#define __DOS_TASKS_H__

#include "type_def.h"

#define TASKS_PRIOTITY_MAX  32 
#define TASKS_DRV_MAX_COUNT  32 


extern struct list_node tasks_priority_tab[TASKS_PRIOTITY_MAX];

extern void hw_interrupt_enable(dos_int_state_t level);
extern dos_int_state_t hw_interrupt_disable(void);

extern dos_task_tcb_t * tasks_tcb_array[];
extern const uint8 tasks_cnt;
//extern struct list_node *tasks_tcb_head;
extern uint32 tasks_ready_priority_group;

void dos_tasks_init(void);
int task_add(dos_task_tcb_t *tcb);


extern dos_task_tcb_t dos_task0_tcb;
extern dos_task_tcb_t dos_task1_tcb;
extern dos_task_tcb_t dos_task2_tcb;


void task0_init(struct list_node *head);
void task1_init(struct list_node *head);
void task2_init(struct list_node *head);

uint8 dos_set_event( uint8 task_id, uint32 event_flag );
uint8 dos_clear_event( uint8 task_id, uint32 event_flag );

#endif
