#ifndef __RT_DEF_H__
#define __RT_DEF_H__

#include "rtthread.h"
#include "rtconfig.h"

/*
*********************************************************
*				数据类型
*********************************************************
*/
/* RT-Thread 基础数据类型 */
typedef signed char 	rt_int8_t;
typedef signed short 	rt_int16_t;
typedef signed long 	rt_int32_t;
typedef unsigned char 	rt_uint8_t;
typedef unsigned short 	rt_uint16_t;
typedef unsigned long 	rt_uint32_t;
typedef int 			rt_bool_t;

/*  32bit CPU */
typedef long 			rt_base_t;
typedef unsigned long 	rt_ubase_t;

typedef rt_base_t 		rt_err_t;
typedef rt_uint32_t 	rt_timer_t;
typedef rt_uint32_t 	rt_tick_t;
typedef rt_base_t 		rt_flag_t;
typedef rt_ubase_t 		rt_size_t;
typedef rt_ubase_t 		rt_dev_t;
typedef rt_base_t 		rt_off_t;

/* 布尔数据类型重定义 */
#define RT_TRUE 		1
#define RT_FLASE 		0

#ifdef __CC_ARM
#define rt_inline 		static __inline
#define ALIGN(n) 		__attribute__((aligned(n))) 	/* 字节对齐 */

#elif defined (__IAR_SYSTEMS_ICC__)
#define rt_inline 		static inline
#define ALIGN(n) 		PRAGMA(data_alignment=n)

#elif defined (__GNUC__)
#define rt_inline 		static __inline
#define ALIGN(n) 		__attribute__((aligned(n)))
#else
#error not supported tool chain
#endif

/*
*********************************************************************************
* 		宏定义
*********************************************************************************
*/
/* 地址对齐，以align为整数倍对齐*/
/*
RT_ALIGN(size,align) 		向上对齐，即上进
RT_ALIGN_DOWN(size,align) 	向下对齐，即丢弃
详细介绍
https://blog.csdn.net/heybeaman/article/details/80491871
*/
#define RT_ALIGN(size,align) 		(((size)+(align)- 1) &~((align) - 1))
#define RT_ALIGN_DOWN(size,align) 	((size) & ~((align) -1))

#define RT_NULL 		(0)

/*
*********************************************************************************
* 		枚举类型定义
*********************************************************************************
*/
/* 对象类型枚举定义 */
enum rt_object_class_type
{
	RT_Object_Class_Thread = 0,         /* 对象是线程 */
    RT_Object_Class_Semaphore,			/* 对象是信号量 */
	RT_Object_Class_Mutex,				/* 对象是互斥量 */
	RT_Object_Class_Event,				/* 对象是事件 */
	RT_Object_Class_MailBox,			/* 对象是邮箱 */
	RT_Object_Class_MessageQueue,		/* 对象是消息队列 */
	RT_Object_Class_MemHeap,			/* 对象是内存堆 */
	RT_Object_Class_MemPool,			/* 对象是内存池 */
	RT_Object_Class_Device,				/* 对象是设备 */
	RT_Object_Class_Timer,				/* 对象是定时器 */
	RT_Object_Class_Module,				/* 对象是模块 */
	RT_Object_Class_Unknown,			/* 对象是未知 */
	RT_Object_Class_Static = 0x80,		/* 对象是静态对象 */
};

/* 对象容器数组的下标定义 */
enum rt_object_info_type
{
	RT_Object_Info_Thread = 0,			/* 对象是线程 */
#ifdef RT_USING_SEMAPHORE
	RT_Object_Info_Semaphore,			/* 对象是信号量 */
#endif
#ifdef RT_USING_MUTEX
	RT_Object_Info_Mutex,				/* 对象是互斥量 */
#endif 
#ifdef RT_USING_EVENT
	RT_Object_Info_Event,				/* 对象是事件 */
#endif
#ifdef RT_USING_MAILBOX
	RT_Object_Info_MailBox,				/* 对象是邮箱 */
#endif
#ifdef RT_USING_MESSAGEQUEUE
	RT_Object_Info_MessageQueue,		/* 对象是消息队列 */
#endif
#ifdef RT_USING_MEMHEAP
	RT_Object_Info_MemHeap,				/* 对象是内存堆 */
#endif
#ifdef RT_USING_MEMPOOL
	RT_Object_Info_MemPool,				/* 对象是内存池 */
#endif
#ifdef RT_USING_DEVICE
	RT_Object_Info_Device,				/* 对象是设备 */
#endif
	RT_Object_Info_Timer,				/* 对象是定时器 */
#ifdef RT_USING_MODULE
	RT_Object_Info_Module,				/* 对象是模块 */
#endif
	RT_Object_Info_Unknown				/* 对象未知 */
};

/*
*********************************************************************************
* 		错误代码定义
*********************************************************************************
*/
/* RT-Thread 错误代码重定义 */
#define RT_EOK 				0 		/* There is no error 没有错误 */
#define RT_ERROR 			1 		/* A generic error happens 发生一般错误*/
#define RT_ETIMEOUT			2 		/* Timed out 超时*/
#define RT_EFULL			3 		/* The resource is full 资源已满*/
#define RT_EEMPTY			4 		/* The resource id empty 资源标识为空*/
#define RT_ENOMEM			5 		/* No memory 无内存*/
#define RT_ENOSYS			6 		/* No system 无系统*/
#define RT_EBUSY			7 		/* Busy 忙碌*/
#define RT_EIO				8 		/* IO error  */
#define RT_EINTR			9 		/* Interrupted system call 中断的系统调用*/
#define RT_EINVAL			10 		/* Invalid argument */





/*
*********************************************************************************
* 		结构体定义
*********************************************************************************
*/
/* 双向链表节点数据类型 */
struct rt_list_node
{
	struct rt_list_node *next;			/* 指向后一个节点 */
	struct rt_list_node *prev;			/* 指向前一个节点 */
};
typedef struct rt_list_node rt_list_t;


/* 线程控制块类型声明 */
struct rt_thread
{
	/* rt 对象 */
	char name[RT_NAME_MAX];		/* 对象的名字 */
	rt_uint8_t type;			/* 对象类型 */
	rt_uint8_t flags;			/* 对象的状态 */
	rt_list_t list;			/* 对象的列表节点 */

	void 			*sp;				/* 线程栈指针 */
	void 			*entry;				/* 线程入口地址 */
	void 			*parameter;			/* 线程形参 */
	void 			*stack_addr;		/* 线程栈起始大小 */
	rt_uint32_t 	stack_size;			/* 线程栈大小，单位为字节 */
	
	rt_list_t 		tlist;				/* 线程链表节点 */

	rt_ubase_t remaining_tick;			/*用于实现阻塞延时 */
};
typedef struct rt_thread *rt_thread_t;

/* 对象数据类型定义 */
struct rt_object
{
	char name[RT_NAME_MAX];		/* 内核对象的名字 */
	rt_uint8_t type;			/* 内核对象的类型 */
	rt_uint8_t flag;			/* 内核对象的状态 */

	rt_list_t list;				/* 内核对象的列表节点 */
};
typedef struct rt_object *rt_object_t;		/* 内核对象数据类型重定义 */

/* 内核对象信息结构体定义 */
struct rt_object_information
{
	enum rt_object_class_type type;	/* 对象类型 */
	rt_list_t object_list;			/* 对象列表节点头 */
	rt_size_t object_size;			/* 对象大小 */ 
};

#endif		/* __RT_DEF_H__ */
