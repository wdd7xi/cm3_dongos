#ifndef __TIMER_H__
#define __TIMER_H__



struct timer
{
	struct list_node tnode;
	
	uint8 task_id;
	uint32 event_flag;
	
	uint32 reload_timeout;//init_tick;
	uint32 timeout_tick;
};



uint8 dos_start_timer(struct timer * new_timer, uint8 taskID, uint32 event_id, uint32 timeout_value);
uint8 dos_start_reload_timer(struct timer * new_timer, uint8 taskID, uint32 event_id, uint32 timeout_value);
void tick_increase(void);


#endif
