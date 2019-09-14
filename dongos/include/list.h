#ifndef __LIST_H__
#define __LIST_H__

#include "type_def.h"

#define LIST_POISON1  ((void *) 0x100)
#define LIST_POISON2  ((void *) 0x200)

struct list_node 
{
	struct list_node *next, *prev;
};

#define WRITE_ONCE(var, val) \
		(*((volatile typeof(val) *)(&(var))) = (val))


#define LIST_NODE_INIT(name)  {&(name), &(name)}
#define LIST_NODE(name)  \
		struct list_node name = LIST_NODE_INIT(name);

void __list_add(struct list_node *new, struct list_node *prev, struct list_node *next);

static inline void INIT_LIST_NODE(struct list_node *list)
{
	WRITE_ONCE(list->next, list);
	list->prev = list;
}

static inline void __list_add(struct list_node *new, struct list_node *prev, struct list_node *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	WRITE_ONCE(prev->next, new);
}

static inline void list_add(struct list_node *new, struct list_node *head)
{
	__list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_node *new, struct list_node *head)
{
	__list_add(new, head->prev, head);
}

static inline void __list_del(struct list_node * prev, struct list_node * next)
{
	next->prev = prev;
	WRITE_ONCE(prev->next, next);
}
static inline void __list_del_clearprev(struct list_node *entry)
{
	__list_del(entry->prev, entry->next);
	entry->prev = NULL;
}
static inline void __list_del_entry(struct list_node *entry)
{
	__list_del(entry->prev, entry->next);
}
static inline void list_del(struct list_node *entry)
{
	__list_del_entry(entry);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}
static inline void list_del_init(struct list_node *entry)
{
	__list_del_entry(entry);
	INIT_LIST_NODE(entry);
}

static inline int list_is_first(const struct list_node *list, const struct list_node *head)
{
	return list->prev == head;
}
static inline int list_is_last(const struct list_node *list, const struct list_node *head)
{
	return list->next == head;
}

static inline int list_empty(const struct list_node *head)
{
	return head->next == head;
}
static inline int list_empty_careful(const struct list_node *head)
{
	struct list_node *next = head->next;
	return (next == head) && (next == head->prev);
}


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)



#endif 
