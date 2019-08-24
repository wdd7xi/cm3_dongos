#ifndef __TYPE_DEF_H__
#define __TYPE_DEF_H__

#define  NULL  (0) 
typedef  unsigned char    uint8;
//typedef  unsigned int     uint32;
typedef  unsigned long     uint32;


struct list_node 
{
	struct list_node *next, *prev;
};

typedef  void (*task_handler_fn_t)(void * parg);

typedef struct dos_task_tcb {
	struct list_node dt_list; //inherit //Doubly Task Linked List
	
	task_handler_fn_t process; //encapsulation //
	void * parameter;
	
	void (*init)(void);
	
	//uint32 (*task_fn)(void * arg);
	uint32 event_set;
} dos_task_tcb_t;


#endif
