#include "rtthread.h"

/*
*************************************************************
*		rt_thread_init() 线程初始化函数
*
*	thread		线程控制块指针
*	entry		线程函数名，表示线程的入口,
				指向函数的指针函数类型,类似 void a(void) 
				这种可以通过这个指针调用这个函数
*	parameter	线程形参，用于传递线程参数
*	stack_start	用于指向线程栈的起始地址
*	stack_size	线程栈的大小，单位为字节
*************************************************************
*/
rt_err_t rt_thread_init(struct rt_thread *thread,
						const char 		*name,
						void (*entry)(void *parameter),
						void 			 *parameter,
						void 			 *stack_start,
						rt_uint32_t 	 stack_size)
{
	/* 线程对象初始化 */
	/* 线程结构体开头部分的4个成员就是 rt_object_t 成员 */
	rt_object_init((rt_object_t)thread, RT_Object_Class_Thread, name);

	rt_list_init(&(thread->tlist));		/* 初始化线程链表节点 */
	
	thread->entry=(void *)entry;		/* 将线程入口保存到线程控制块的entry成员中 */
	thread->parameter=parameter;		/* 将线程入口形参保存到线程控制块的parameter成员中 */
	
	thread->stack_addr=stack_start;		/* 将线程栈起始地址保存到线程控制块的stack_start成员中 */
	thread->stack_size=stack_size;		/* 将线程栈起大小保存到线程控制块的stack_size成员中 */
	
	/* 初始化线程栈，并返回线程指针 */
	thread->sp=
	(void *)rt_hw_stack_init(thread->entry,
							 thread->parameter,
	(void *)((char *)thread->stack_addr+thread->stack_size-4));
	
	return RT_EOK;
}

/* 阻塞延时代码 */
void rt_thread_delay(rt_tick_t tick)
{
	struct rt_thread *thread;		

	/* 获取当前线程的线程控制块 */
	thread = rt_current_thread;

	/* 设置延时时间 */
	thread->remaining_tick = tick;

	/* 进行系统调度 */
	rt_schedule();
}
