#define BSP_MODULE
#include <bsp.h>

#define BSP_REG_DEM_CR 							(*(CPU_REG32 *)0xE000EDFC) //DEMCR 寄存器
#define BSP_REG_DWT_CR 							(*(CPU_REG32 *)0xE0001000) //DWT 控制寄存器
#define BSP_REG_DWT_CYCCNT 					(*(CPU_REG32 *)0xE0001004) //DWT 时钟计数寄存器
#define BSP_REG_DBGMCU_CR 					(*(CPU_REG32 *)0xE0042004)

//DEMCR寄存器的第24位，如果要使用DWT ETM ITM和TPIU的话DEMCR寄存器的第24位置1
#define BSP_BIT_DEM_CR_TRCENA 			DEF_BIT_24

//DWTCR 寄存器的第0位，当为1的时候使能CYCCNT计数器，使用CYCCCNT之前应当先初始化
#define BSP_BIT_DWT_CR_CYCCNTENA 		DEF_BIT_00

//获取系统时钟
//返回值：系统时钟HCLK
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
	//我们这里是使用的HAL库，所以可以直接调用函数 获取系统时钟
	return HAL_RCC_GetHCLKFreq();	//返回HCLK时钟
}

//此函数用来开启和初始化CPU的时间戳定时器，其实就是使能DWT和CYYCNT，并且
//初始化CYCCNT为0
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;//使能DWT
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;		//初始化CYCCNT寄存器
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;//开启CYCCNT

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif

//此函数用于获取CYCCNT中的值
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif

//下面这两个函数CPU_TS32_to_uSec()和CPU_TS64_to_uSec()是用来
//将读取到的时钟周期数转换为us的。
#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif
