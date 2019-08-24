#ifndef __DOS_TASKS_H__
#define __DOS_TASKS_H__

#include "type_def.h"

extern dos_task_tcb_t * tasks_tcb_array[];
extern const uint8 tasks_cnt;
extern struct list_node *tasks_tcb_head;


void dos_tasks_init(void);


extern dos_task_tcb_t dos_task1_tcb;
extern dos_task_tcb_t dos_task2_tcb;

void task1_init(struct list_node *head);
void task2_init(struct list_node *head);

#endif
