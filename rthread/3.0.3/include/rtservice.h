#ifndef __RT_SERVICE_H__
#define __RT_SERVICE_H__

#include "rtdef.h"

/* 已知一个结构体里面的成员的地址，反推出改结构体的首地址 */
#define rt_container_of(ptr,type,member) \
		((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#define rt_list_entry(node,type,member) rt_container_of(node,type,member)


/*
* @brief 初始化rt_list类型的链表节点
*		 将节点里面的next和prev这两个节点指针指向节点本身
*
* @param l 要初始化的列表
*/
rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

/*
* @brief 在双向链表表头后面插入一个节点
*		 
* @param l 表头节点
* @param n 要在后面插入的新节点
*/
rt_inline void rt_list_insert_after(rt_list_t *l,rt_list_t *n)
{
	l->next->prev=n;		/* l节点的下一个节点中的prev指向新节点n */
	n->next=l->next;		/* 新节点n.next指向节点l的下一个节点 */
	
	l->next=n;				/* l节点的next指向新节点n */
	n->prev=l;				/* 新节点n.prev指向节点l */
}

/*
* @brief 在双向链表表头前面插入一个节点
*		 
* @param l 表头节点
* @param n 要在前面插入的新节点
*/
rt_inline void rt_list_insert_bfter(rt_list_t *l,rt_list_t *n)
{
	l->prev->next=n;		/* 头节点的上一个节点中的next指向新节点n */
	n->prev=l->prev;		/* 新节点n.prev指向头节点的上一个节点 */
	
	l->prev=n;				/* l节点的next指向新节点n */
	n->next=l;				/* 新节点n.prev指向节点l */
}

/*
* @brief 从双向链表删除一个节点
*		 
* @param n 要删除节点
*/
rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev=n->prev;		/* n.next的prev指向n.prev */
	n->prev->next=n->next;		/* n.prev的next指向n.next */
	
	n->next=n->prev=n;			/* n.next和prev都指向自己 */
}
#endif
