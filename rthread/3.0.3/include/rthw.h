#ifndef __RT_HW_H__
#define __RT_HW_H__

#include "rtthread.h"



/*
*************************************************************************
*                               上下文切换相关函数声明
*************************************************************************
*/
void rt_hw_context_switch_to(rt_uint32_t to);
void rt_hw_context_switch(rt_uint32_t from, rt_uint32_t to);
void rt_hw_context_switch_interrupt(rt_uint32_t from, rt_uint32_t to);

/*
*************************************************************************
*                               中断相关函数声明
*************************************************************************
*/
/* 开关中断函数，在 context.S 汇编中实现 */
rt_base_t rt_hw_interrupt_disable(void);    			/* 关中断 */
void rt_hw_interrupt_enable(rt_base_t level);          /* 开中断 */

#endif
