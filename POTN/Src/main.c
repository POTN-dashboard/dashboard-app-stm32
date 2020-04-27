/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"
#include "usbd_customhid.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "includes.h"	//UCOS相关
#include "oled.h"   //OLED
#include "display_menu.h"
#include "display_fft.h"
#include "transform.h"
#include "flash.h"

#include "stm32_dsp.h"  
#include "table_fft.h"
//#include "bmp.h"		//图片

#define YES 1 
#define NO  0

int MENU_NUM = 1; //当前界面编号
int MENU_ALL =6; //总共多少个界面
int MENU4_FLAG = NO; //当前 界面四的 时候 是否开始挂机 

extern uint32_t adc_buf[NPT];  
extern uint8_t fall_pot[128];	//记录下落点的坐标
extern long lBufInArray[NPT];
extern long lBufOutArray[NPT/2];
extern long lBufMagArray[NPT/2];
extern uint8 fft_num;
extern uint8 fft_auto_flag;

extern USBD_HandleTypeDef hUsbDeviceFS;
unsigned char USB_Recive_Buffer[64]; //USB接收缓存
unsigned char USB_Received_Count = 0;//USB接收计数
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define START_TASK_PRIO  3    //任务优先级   开始任务
#define START_STK_SIZE 128   //任务堆栈大小
OS_TCB StartTaskTCB;        //任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];  //任务堆栈
void start_task(void *p_arg);   //任务函数


#define KEY_TASK_PRIO		4  //任务优先级						 按键任务
#define KEY_STK_SIZE 		128		//任务堆栈大小	
OS_TCB Key_TaskTCB;			//任务控制块
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];			//任务堆栈	
void key_task(void *p_arg);				//任务函数


#define USB_TASK_PRIO  5    //任务优先级					USB任务
#define USB_STK_SIZE 128   //任务堆栈大小
OS_TCB UsbTaskTCB;        //任务控制块
CPU_STK USB_TASK_STK[USB_STK_SIZE];  //任务堆栈
void usb_task(void *p_arg);   //任务函数


#define DISPLAY_TASK_PRIO  6    //任务优先级			显示任务
#define DISPLAY_STK_SIZE 128   //任务堆栈大小
OS_TCB DisplayTaskTCB;        //任务控制块
CPU_STK DISPLAY_TASK_STK[USB_STK_SIZE];  //任务堆栈
void display_task(void *p_arg);   //任务函数


#define KEYBOARD_TASK_PRIO  7    //任务优先级			键盘任务
#define KEYBOARD_STK_SIZE 128   //任务堆栈大小
OS_TCB KeyboardTaskTCB;        //任务控制块
CPU_STK KEYBOARD_TASK_STK[USB_STK_SIZE];  //任务堆栈
void keyboard_task(void *p_arg);   //任务函数



/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#define RegBase  (0x40005C00L)  /* USB_IP Peripheral Registers base address */
/* Control register */
#define CNTR    ((__IO unsigned *)(RegBase + 0x40))
#define _SetCNTR(wRegValue)  (*CNTR   = (uint16_t)wRegValue)
/* GetCNTR */
#define _GetCNTR()   ((uint16_t) *CNTR)

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 

void usb_port_set(u8 enable) 
{ 
		__HAL_RCC_GPIOA_CLK_ENABLE();           	//开启GPIOA时钟	 
		if(enable){ 
			GPIOA->CRH&=0XFFF00FFF; 
			GPIOA->CRH|=0X00044000; 
			_SetCNTR(_GetCNTR()&(~(1<<1)));
		}
		else { 
			_SetCNTR(_GetCNTR()|(1<<1)); 
			GPIOA->CRH&=0XFFF00FFF; 
			GPIOA->CRH|=0X00033000; 
			PAout(12)=0; 
		} 
}
typedef  void (*pFunction)(void);
#define BootloaderAddress    (0x8000000)
pFunction Jump_To_Bootloader;
uint32_t JumpAddress;
void BootloaderStart(void)
{
//		__set_PRIMASK(1);																	
	  if (((*(__IO uint32_t*)BootloaderAddress) & 0x2FFE0000 ) == 0x20000000)
    { 
      /* Jump to user application */

      JumpAddress = *(__IO uint32_t*) (BootloaderAddress + 4);

      Jump_To_Bootloader = (pFunction) JumpAddress;

      /* Initialize user application's Stack Pointer */
			__set_CONTROL(0);
      __set_MSP(*(__IO uint32_t*) BootloaderAddress);

		for(int i = 0; i < 8; i++)
		{			
			NVIC->ICER[i] = 0xFFFFFFFF;	/* 关闭中断*/
			NVIC->ICPR[i] = 0xFFFFFFFF;	/* 清楚中断标志位 */
		}
      Jump_To_Bootloader();
    }

}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	OS_ERR err;
	CPU_SR_ALLOC();
	uint8_t flash_flag = 0; //判断当前 是否进入app模式的标志位 从flash第15页最后一个字中读取出来
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	usb_port_set(1);   								//USB控制寄存器 关闭USB			
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USB_DEVICE_Init();	
	
//	Flash_ReadData(ADDR_FLASH_PAGE_16-1,&flash_flag,1);
//	if(flash_flag == 0xFF){
//		while(1){
//		usb_port_set(0);		//USB控制寄存器 关闭USB			
////		BootloaderStart();
//		__set_FAULTMASK(1); 
//		NVIC_SystemReset();				//系统软件复位			
//		};
//	}
//	
	
  MX_ADC1_Init();
  MX_TIM3_Init();
	
	//初始化下落点 把下落的点 初始化为最底部显示
	for(int i=0;i<128;i++)
		fall_pot[i] = 63;	
	/*启动ADC的DMA传输 配合下面定时器来触发ADC转换*/
	HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);	
  /* USER CODE BEGIN 2 */

	OSInit(&err);		    //初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区	
	//创建开始任务		
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
  /* USER CODE END 2 */
 
 

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}




//开始任务任务函数
void start_task(void *p_arg)
{
	OS_ERR err;

	CPU_SR_ALLOC();		
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	OLED_Init();	
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	Init_PC_Data();
	OLED_Refresh();
				
	OS_CRITICAL_ENTER();	//进入临界区
	//创建USB任务
	OSTaskCreate((OS_TCB 	* )&UsbTaskTCB,		
				 (CPU_CHAR	* )"USB task", 		
                 (OS_TASK_PTR )usb_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )USB_TASK_PRIO,     
                 (CPU_STK   * )&USB_TASK_STK[0],	
                 (CPU_STK_SIZE)USB_STK_SIZE/10,	
                 (CPU_STK_SIZE)USB_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);
	
//创建KEY任务
	OSTaskCreate((OS_TCB 	* )&Key_TaskTCB,		
				 (CPU_CHAR	* )"key task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     	
                 (CPU_STK   * )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		

//创建DISPLAY任务
	OSTaskCreate((OS_TCB 	* )&DisplayTaskTCB,		
				 (CPU_CHAR	* )"display task", 		
                 (OS_TASK_PTR )display_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )DISPLAY_TASK_PRIO,     	
                 (CPU_STK   * )&DISPLAY_TASK_STK[0],	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE/10,	
                 (CPU_STK_SIZE)DISPLAY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		

								 
//创建KEYBOARD任务
	OSTaskCreate((OS_TCB 	* )&KeyboardTaskTCB,		
				 (CPU_CHAR	* )"keyboard task", 		
                 (OS_TASK_PTR )keyboard_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )KEYBOARD_TASK_PRIO,     	
                 (CPU_STK   * )&KEYBOARD_TASK_STK[0],	
                 (CPU_STK_SIZE)KEYBOARD_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEYBOARD_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		
	OSTaskSuspend((OS_TCB*)&KeyboardTaskTCB,&err);  //挂起 键盘任务
								 
	OS_CRITICAL_EXIT();	//退出临界区								 
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}

//usb任务函数
void usb_task(void *p_arg)
{
	static u8 status = 0;
	static u32 flag = 0;    // 类似看门狗flag
	uint8_t HID_Buffer[64];
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	Buffer_Set(HID_Buffer,READY_PACK);
	Buffer_Set(USB_Recive_Buffer,0);
	
	while(1)
	{
		if(USB_Recive_Buffer[0] == UPGRED_PACK){
			
			Flash_WriteWord(ADDR_FLASH_PAGE_15,0xFFFFFFFF,255);
			HAL_Delay(100);
			usb_port_set(0);   								//USB控制寄存器 关闭USB			
			
//			OS_CRITICAL_ENTER();	//进入临界区
//			OSTaskDel((OS_TCB*)&KeyboardTaskTCB,&err);
//			OSTaskDel((OS_TCB*)&DisplayTaskTCB,&err);	
//			OSTaskDel((OS_TCB*)&Key_TaskTCB,&err);	
//			OSTaskDel((OS_TCB*)&DisplayTaskTCB,&err);
//			OSTaskDel((OS_TCB*)&UsbTaskTCB,&err);				
//			OS_CRITICAL_EXIT();	//退出临界区	
//			__set_FAULTMASK(1); 
//			NVIC_SystemReset();				//系统软件复位		
			BootloaderStart();
			while(1);
		}
		
		switch (status){
			
			case 0:						
				
				if(USB_Recive_Buffer[0] == CPU_GPU_PACK){						// 是否接受到了CPU+GPU型号包 PC->STM32
					flag = 0;
					status = 1;
					transform_CPUGPU();						
					break;	
				}
				USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_Buffer,64);			//发送 就绪包 STM32->PC
				OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
				flag++;				
				if(flag == 16)			//如果16s内未能接收到 
					status = 3;
			break;
				
			case 1:
				Buffer_Set(HID_Buffer,AKC_PACK);
				USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_Buffer,64);			//发送 应答包 STM32->PC
				Buffer_Set(HID_Buffer,0);
				status = 2;
			
			break;
			
			case 2: 																			//接受数据包
								
				if(USB_Recive_Buffer[0] == DATA_PACK){
					transform_DATA();
					flag = 0;
				}
				if(MENU_NUM == MENU_ALL -1){   //当前为显示 FFT音乐频谱界面
					flag = 0;
				}
					
				
				flag++;
				if(flag > 10*15 ){
					status = 3;
				}

			break;
				
			case 3:								// 传输错误
				MENU_NUM = MENU_ALL;	  //最后一个页面
			break;
		
		}
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时100ms
		
	}
}

void display_task(void *p_arg){

	
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1){
		switch(MENU_NUM){
		
			case 1:
				display_menu1();
				break;
			
			case 2:
				display_menu2();
				break;
			
			case 3:
				display_menu3();
				break;
			
			case 4:
				if(MENU4_FLAG == 0)
					display_menu4_1();
				else if(MENU4_FLAG == 1)
					display_menu4_2();
				break;
			
			case 5:  // FFT 音乐频谱 在中断中显示
				OLED_Clear();
				break;
				
			case 6:                    // 最后一个页面
				display_menu5();
			break;
	
		}
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms	
	
	}


}





//key任务函数
void key_task(void *p_arg)
{
	static u8 STATUS = 0; 
	
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1){
		
		switch (STATUS){
			case 0:										//检测到按键
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){
					STATUS = 1;
				}
				break;

			case 1:										//消抖
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){
					STATUS = 2;
				}
				else STATUS = 0;
				break;
	
			case 2:
				for(int i = 0; i < 100;i++){
					if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 1){			// 不是长按
						STATUS = 3;		
						break;
					}
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
				}
				
				if(STATUS != 3){
					STATUS = 4;         //长按
					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != 1){
						OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
					}
				}

			break;


				
				
			case 3:
				STATUS = 6;    //确定不是双击
				for(int i = 0; i < 20; i++){
					if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){
						OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
						if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){
							
							STATUS = 5;     //    确定双击
							break;
						}
					}
				 OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
				}

			break;
				
					
			case 4:                 //长按
			STATUS = 0;	
			if(MENU_NUM == 4){
				if(MENU4_FLAG == NO){
					MENU4_FLAG = YES;
					OSTaskResume((OS_TCB*)&KeyboardTaskTCB,&err);
				}					
				else {
					MENU4_FLAG = NO;
					OSTaskSuspend((OS_TCB*)&KeyboardTaskTCB,&err);  //挂起 键盘任务
				}
			}
			if(MENU_NUM == MENU_ALL-1){ //FFT音乐频谱界面
				
				if(fft_auto_flag == 1)	
					fft_auto_flag = 0;
				else 
					fft_auto_flag =1;			
			}
			
			
			
				
			break;
			
			case 5:								//	双击
			STATUS = 0;
			if(MENU_NUM == MENU_ALL - 1){  //FFT音乐频谱界面
				fft_num++;
				if(fft_num > 4)
					fft_num = 1;					
			}
			
			while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != 1){
				OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
			}
			break;
			
			case 6:								// 单击
			STATUS = 0;

			if(MENU_NUM != 4 || MENU4_FLAG == NO){   //不在挂机界面才有效
				MENU_NUM +=1 ;
				if(MENU_NUM > MENU_ALL - 1){     		//从音乐频谱的界面切回到第一个界面
					OLED_Clear();
					MENU_NUM = 1;				
					HAL_TIM_Base_Stop(&htim3);        //关闭定时器3 
					HAL_ADC_Stop_DMA(&hadc1);
					OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms
					OLED_Clear();
					OSTaskResume((OS_TCB*)&DisplayTaskTCB,&err);  //恢复显示任务				
					
				}
				else if(MENU_NUM == MENU_ALL - 1){  //如果是 显示FFT音乐频谱的界面 					
					OSTaskSuspend((OS_TCB*)&DisplayTaskTCB,&err);  						//挂起 显示任务  		
					OLED_Clear();
					OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms
					HAL_TIM_Base_Start(&htim3);		/*开启定时器 用溢出事件来触发ADC转换*/
					HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);
				}
					
			}
		
			break;
				
		
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms			
		
		
	
//			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){
//			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
//		
//			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0){

////					OS_CRITICAL_ENTER();		
////					USBD_KEYBOADR_HID_SendReport(&hUsbDeviceFS, KEYBOARD_Buffer,9);
////					OS_CRITICAL_EXIT();
////					
////					KEYBOARD_Buffer[2] = 0;	
////					OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); //延时300ms

////					OS_CRITICAL_ENTER();			
////					USBD_KEYBOADR_HID_SendReport(&hUsbDeviceFS, KEYBOARD_Buffer,9);							
////					OS_CRITICAL_EXIT();
////					
////					KEYBOARD_Buffer[2] = 4;
//					HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);				
//					while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) != 1){
//					OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //延时10ms
//					}
//				
//				}
//			}
	};
}




void keyboard_task(void *p_arg){

	uint8_t KEYBOARD_Buffer[8]={0x00,0x00,44,0x00,0x00,0x00,0x00,0x00};  //空格
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1){
		OSTimeDlyHMSM(0,0,3,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时3s
		if(MENU4_FLAG == 1&&	MENU_NUM == 4){
					OS_CRITICAL_ENTER();		
					USBD_KEYBOADR_HID_SendReport(&hUsbDeviceFS, KEYBOARD_Buffer,9);
					OS_CRITICAL_EXIT();
					
					KEYBOARD_Buffer[2] = 0;	
					OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_HMSM_STRICT,&err); //延时300ms

					OS_CRITICAL_ENTER();			
					USBD_KEYBOADR_HID_SendReport(&hUsbDeviceFS, KEYBOARD_Buffer,9);							
					OS_CRITICAL_EXIT();
					
					KEYBOARD_Buffer[2] = 44;	
		}

	
	
	}



}






//ADC DMA传输中断
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//进入临界区
	uint16_t i = 0;
	static uint16_t num = 0;	
	
	HAL_ADC_Stop_DMA(&hadc1);							//完成一次测量 关闭DMA传输
	
	
	
	//填充数组
	for(i=0;i<NPT;i++)
		lBufInArray[i] = ((signed short)(adc_buf[i]-2048)) << 16;		//这里因为单片机的ADC只能测正的电压 所以需要前级加直流偏执
																																	//加入直流偏执后 软件上减去2048即一半 达到负半周期测量的目的
						 
	cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);							//FFT变换
	GetPowerMag();																									//取直流分量对应的AD值

	if(fft_auto_flag == 1){
		num++;
		if(num > 150){
		num = 0;
			
		fft_num++;
		if(fft_num > 4)	fft_num = 1;
		}
	}
	switch (fft_num){
	
		case 1:
			display_fft_1();	
			break;
		
		case 2:
			display_fft_3();	

			break;
		
		case 3:
			display_fft_2();				
			break;
		case 4:
			display_fft_4();	
			break;
		
	}
	OS_CRITICAL_EXIT();	//退出临界区		
	HAL_ADC_Start_DMA(&hadc1, adc_buf, NPT);
}






/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
