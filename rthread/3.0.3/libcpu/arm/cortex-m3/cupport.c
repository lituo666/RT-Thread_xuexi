#include "rtthread.h"

/*
************************************************************
* 		全局变量
************************************************************
*/
/* 用于存储上一个线程的栈的SP的指针 */
rt_uint32_t rt_interrupt_from_thread;

/* 用于存储下一个将要运行的线程的栈的SP的指针 */
rt_uint32_t rt_interrupt_to_thread;

/* PendSV 中断服务函数执行标志 */
rt_uint32_t rt_thread_switch_interrupt_flag;


/*
************************************************************
* 		结构体定义
************************************************************
*/
struct exception_stack_frame
{
	/* 发生异常时，自动加载到CPU寄存器的内容 */
	rt_uint32_t r0;
	rt_uint32_t r1;
	rt_uint32_t r2;
	rt_uint32_t r3;
	rt_uint32_t r12;
	rt_uint32_t lr;		/* 子程序链接寄存器LR(Link Register),当执行子程序调用指令(BL)时,R14可得到R15(程序计数器PC)的备份. */
	rt_uint32_t pc;		/* 程序计数器 */
	rt_uint32_t psr; 	/* xPSR 程序状态寄存器*/
};

struct stack_frame
{
	/* 发生异常时，需手动加载到CPU寄存器的内容 */
	rt_uint32_t r4;
	rt_uint32_t r5;
	rt_uint32_t r6;
	rt_uint32_t r7;
	rt_uint32_t r8;
	rt_uint32_t r9;
	rt_uint32_t r10;
	rt_uint32_t r11;
	
	struct exception_stack_frame exception_stack_frame;
};


rt_uint8_t *rt_hw_stack_init(void 		*tentry, 		/* 线程入口 */
							 void 		*parameter, 	/* 线程形参 */
							 rt_uint8_t *stack_addr) 	/* 线程栈顶地址-4. 该函数调用的时候传进来的是线程栈是栈顶地址-4*/
{
	struct stack_frame 	*stack_frame; 	/* 定义struct stack_frame类型的结构体指针stack_frame */
	rt_uint8_t 			*stk;
	unsigned long 		i;

	/* 获取栈顶指针
	rt_hw_stack_init 在调用的时候，传给stack_addr的是(栈顶指针-4) */
	stk = stack_addr + sizeof(rt_uint32_t);

	/* 让stk指针向下8字节对齐 
	#define RT_ALIGN_DOWN(size,align) 	((size) & ~((align) -1)) */
	stk = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stk, 8);

	/* stk 指针继续向下移动sizeof(struct stack_frame) 个偏移 */
	stk -= sizeof(struct stack_frame);

	/* 将stk指针强制转化为stack_frame类型后存放到stack_frame */
	stack_frame = (struct stack_frame *)stk;

	/* 以stack_frame 为起始地址，将栈空间里面的 sizeof(struct stack_frame)
	个内存初始化为 0xdeadbeef*/
	for (i = 0; i < sizeof(struct stack_frame) / sizeof(rt_uint32_t); i++)
	{
		((rt_uint32_t *)stack_frame)[i] = 0xdeadbeef;
	}
	
	/* 初始化异常发生时自动保存的寄存器 */
	stack_frame->exception_stack_frame.r0 = (unsigned long)parameter; /* r0 : argument */
	stack_frame->exception_stack_frame.r1 = 0; /* r1 */
	stack_frame->exception_stack_frame.r2 = 0; /* r2 */
	stack_frame->exception_stack_frame.r3 = 0; /* r3 */
	stack_frame->exception_stack_frame.r12 = 0; /* r12 */
	stack_frame->exception_stack_frame.lr = 0; /* lr: 暂时初始化为0 */
	stack_frame->exception_stack_frame.pc = (unsigned long)tentry; /* entry point, pc */
	stack_frame->exception_stack_frame.psr = 0x01000000L; /* PSR */
	
	/* 返回线程栈指针 */
	return stk;
}
