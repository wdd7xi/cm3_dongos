#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#define  NULL  (0) 
typedef  unsigned char    uint8;
//typedef  unsigned int     uint32;
typedef  unsigned long     uint32;



#define wd(x)      do { x } while (__LINE__ == -1)

typedef long dos_int_state_t; /* 32bit CPU */ 
#define ENTER_CRITICAL_SECTION(x)   wd( x = hw_interrupt_disable(); )
#define EXIT_CRITICAL_SECTION(x)    wd( hw_interrupt_enable(x); )
#define CRITICAL_STATEMENT(x)       wd( dos_int_state_t _s; ENTER_CRITICAL_SECTION(_s); x; EXIT_CRITICAL_SECTION(_s); )




struct list_node 
{
	struct list_node *next, *prev;
};

typedef  void (*task_handler_fn_t)(void * parg);

typedef struct dos_task_tcb {
	struct list_node dt_list; //inherit //Doubly Task Linked List
	
	task_handler_fn_t process; //encapsulation //
	void * parameter;
	
	uint8 task_id;
	uint8 priority;
	uint32 priority_mask;
	
	void (*init)(void);
	
	//uint32 (*task_fn)(void * arg);
	uint32 event_set;
} dos_task_tcb_t;


#endif
