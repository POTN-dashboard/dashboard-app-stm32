#ifndef OS_CFG_APP_H
#define OS_CFG_APP_H

#define OS_CFG_MSG_POOL_SIZE 				100u 				//消息数量
#define OS_CFG_ISR_STK_SIZE 				128u 				//中断任务堆栈大小
//任务堆栈深度，比如定义为10时，表示当任务堆栈剩余10%时候，就说明堆栈为空
#define OS_CFG_TASK_STK_LIMIT_PCT_EMPTY 10u

//*********************空闲任务*************************
#define OS_CFG_IDLE_TASK_STK_SIZE 		128u 			//空闲任务堆栈大小

//********************中断服务管理任务*****************
#define OS_CFG_INT_Q_SIZE 						10u 			//中断队列大小
#define OS_CFG_INT_Q_TASK_STK_SIZE 		128u 			//中断服务管理任务堆栈大小

//********************统计任务**************************
//统计任务的优先级为倒数第二个，最后一个优先级是空闲任务的。
#define OS_CFG_STAT_TASK_PRIO (OS_CFG_PRIO_MAX-2u)
// OS_CFG_STAT_TASK_RATE_HZ 用于统计任务计算CPU的使用率，统计任务通过统计在
//(1/ OS_CFG_STAT_TASK_RATE_HZ)秒的时间内OSStatTaskCtr能够达到的最大值来得到
//CPU的使用率，OS_CFG_STAT_TASK_RATE_HZ 值应该在 1~10Hz
#define OS_CFG_STAT_TASK_RATE_HZ 				10u
#define OS_CFG_STAT_TASK_STK_SIZE 			128u 		//统计任务堆栈大小   

//******************时钟节拍任务***********************
#define OS_CFG_TICK_RATE_HZ 						1000u 		//系统时钟节拍频率
//时钟节拍任务，一般设置一个相对较高的优先级
#define OS_CFG_TICK_TASK_PRIO 					1u
#define OS_CFG_TICK_TASK_STK_SIZE 			128u 		//时钟节拍任务堆栈大小
#define OS_CFG_TICK_WHEEL_SIZE 					17u 		//时钟节拍列表大小

//************************定时任务************************
#define OS_CFG_TMR_TASK_PRIO 						2u 			//定时器任务优先级
#define OS_CFG_TMR_TASK_RATE_HZ 				100u 		//定时器频率，一般为100Hz
#define OS_CFG_TMR_TASK_STK_SIZE 				128u 		//定时器任务堆栈大小
#define OS_CFG_TMR_WHEEL_SIZE 					17u 		//定时器列表数量

#endif







