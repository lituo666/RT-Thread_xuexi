#ifndef __RTTHREAD_H__
#define __RTTHREAD_H__

#include "rtdef.h"
#include "rtservice.h"
#include "rtconfig.h"
#include "rthw.h"

extern rt_list_t rt_thread_priority_table[RT_THREAD_PRIORITY_MAX];
extern struct rt_thread *rt_current_thread;

extern struct rt_thread idle;
extern struct rt_thread rt_flag1_thread;
extern struct rt_thread rt_flag2_thread;

void rt_system_scheduler_init(void);	/* 调度器初始化 */
void rt_system_scheduler_start(void);	/* 调度器启动 */

void rt_thread_idle_init(void);			/* 空闲线程初始化 */

void rt_schedule(void);

char *rt_strncpy(char *dst, const char *src, rt_ubase_t n);		/* 该函数将指定个数的字符串从一个地方拷贝到另外一个地方 */

/****************************************************************************
 * 				内核对象接口
 ****************************************************************************
*/
void rt_object_init(struct rt_object *object,
					enum rt_object_class_type type,
					const char *name);

/*
-------------------------------------------------------------------------
*                               线程接口
-------------------------------------------------------------------------
*/
rt_err_t rt_thread_init(struct rt_thread *thread,			/* 线程控制块指针 */
						const char 		*name,
						void (*entry)(void *parameter),		/* 线程控制块指针 */
						void 			 *parameter,		/* 线程控制块指针 */
						void 			 *stack_start,		/* 线程控制块指针 */
						rt_uint32_t 	 stack_size);		/* 线程控制块指针 */

rt_uint8_t *rt_hw_stack_init(void 		*tentry, 		/* 线程入口 */
							 void 		*parameter, 	/* 线程形参 */
							 rt_uint8_t *stack_addr);	/* 线程栈顶地址-4 */

void rt_thread_delay(rt_tick_t tick);

/*
-------------------------------------------------------------------------
*                             中断接口
-------------------------------------------------------------------------
*/                        
void rt_interrupt_enter(void);
void rt_interrupt_leave(void);

/*
-------------------------------------------------------------------------
*                             时钟相关接口
-------------------------------------------------------------------------
*/ 
void rt_tick_increase(void);

#endif
