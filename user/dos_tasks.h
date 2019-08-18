#ifndef __DOS_TASKS_H__
#define __DOS_TASKS_H__

#include "type_def.h"

extern dos_task_tcb_t * tasks_tcb_array[];
extern const uint8 tasks_cnt;



void dos_tasks_init(void);


extern dos_task_tcb_t dos_task1_tcb;
extern dos_task_tcb_t dos_task2_tcb;


#endif
