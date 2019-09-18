/*
**********************************************************************
*					包含的头文件
**********************************************************************
*/
#include "rtthread.h"
#include "ARMCM3.h"
#include "core_cm3.h"


ALIGN(RT_ALIGN_SIZE)

/*
**********************************************************************
*					全局变量
**********************************************************************
*/
rt_uint8_t flag1;
rt_uint8_t flag2;

/*
**********************************************************************
*					线程控制块 & STACK & 线程声明
**********************************************************************
*/
/* 定义线程控制块 */
struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;

/* 定义线程 */
rt_uint8_t rt_flag1_thread_stack[512];
rt_uint8_t rt_flag2_thread_stack[512];

/* 线程声明 */
void flag1_thread_entry(void *p_arg);
void flag2_thread_entry(void *p_arg);

/*
**********************************************************************
*					函数声明
**********************************************************************
*/
void delay(rt_uint32_t count);



/*
**********************************************************************
*					main函数
**********************************************************************
*/
int main(void)
{
	/* 硬件初始化 
	将硬件相关的初始化放在这里，如果是软件仿真则没有相关初始化代码*/
	/* 关中断 */
	rt_hw_interrupt_disable();
	
	/* SysTick 中断设置 */
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

	/* 调度器初始化 */
	rt_system_scheduler_init();
	
	/* 初始化空闲线程 */
	rt_thread_idle_init();

	/* 初始化线程 */
	rt_thread_init( &rt_flag1_thread, 				/* 线程控制块 */
					"rt_flag1_thread",
					flag1_thread_entry, 			/* 线程入口地址 */
					RT_NULL,						/* 线程形参 */
					&rt_flag1_thread_stack[0],		/* 线程栈起始地址 */
					sizeof(rt_flag1_thread_stack));	/* 线程栈大小，单位为字节 */
	/* 将线程插入到就绪列表 */
	rt_list_insert_bfter(&(rt_thread_priority_table[0]),&(rt_flag1_thread.tlist));
	
	/* 初始化线程 */
	rt_thread_init( &rt_flag2_thread, 				/* 线程控制块 */
					"rt_flag2_thread",
					flag2_thread_entry, 			/* 线程入口地址 */
					RT_NULL,						/* 线程形参 */
					&rt_flag2_thread_stack[0],		/* 线程栈起始地址 */
					sizeof(rt_flag2_thread_stack));	/* 线程栈大小，单位为字节 */
	/* 将线程插入到就绪列表 */
	rt_list_insert_bfter(&(rt_thread_priority_table[1]),&(rt_flag2_thread.tlist));
	
	/* 启动调度器 */
	rt_system_scheduler_start();
}

/* 软件延时 */
void delay(rt_uint32_t count)
{
	for(;count!=0;count--);
}

/* 线程1 */
void flag1_thread_entry(void *p_arg)
{
	for(;;)
	{
#if 0
		flag1=1;
		delay(100);
		flag1=0;
		delay(100);
		rt_schedule();
#else
		flag1 = 1;
		rt_thread_delay(2);
		flag1 = 0;
		rt_thread_delay(2);
#endif
	}
	
}


/* 线程2 */
void flag2_thread_entry(void *p_arg)
{
	for(;;)
	{
#if 0
		flag2=1;
		delay(100);
		flag2=0;
		delay(100);
		
		rt_schedule();
#else
		flag2 = 1;
		rt_thread_delay(2);
		flag2 = 0;
		rt_thread_delay(2);
#endif 
	}
}

void SysTick_Handler(void)
{
	/* 进入中断 */
	rt_interrupt_enter();

	/* 时基更新 */
	rt_tick_increase();
	
	/* 离开中断 */
	rt_interrupt_leave();
}
