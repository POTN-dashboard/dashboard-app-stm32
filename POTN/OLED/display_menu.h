#ifndef __DISPLAY_MENU_H
#define __DISPLAY_MENU_H

#include "stm32f1xx_hal.h"
#include "oled.h"

typedef struct STRUCT_PC_DATA{
	u8 CPU_NAME[20];								//CPU型号
	u8 CPU_NMAE_LEN;								//CPU型号名称有多长
	u8 GPU_NAME[20];								//GPU型号
	u8 GPU_NAME_LEN;								//GPU型号名称有多长
	
	u8 MINUTE;											//当前几分钟 0~59
	u8 HOUR;                        //当前几点  0~23
	u8 DAY;													//当前几号   1~31
	u8 WEEK_NUM;										//星期几      1~7
	u8 MONTH_NUM;										//月份				1~12
	u32 YEAR;												//年份  0~9999
	
	
	u32 RAM_ALL;										//总内存   0~99
	u32 RAM_USED_INTEGR;						//已经使用内存的整数部分  0~99
	u32 RAM_USED_FRACTION;					//已经使用内存的小数部分  0~9
	u32 RAM_RATE;
	
	u32 VIDEO_MEMORY_ALL;						//总显存    0~99
	u32 VIDEO_MEMORY_INTEGR;				//已经使用显存的整数部分	0~99
	u32 VIDEO_MEMORY_FRACTION;			//已经使用显存的小数部分	0~9
	u32 VIDEO_MEMORY_RATE;
	
	u32 CPU_FREQUENCY;							//CPU频率					0~9999
	u32 CPU_OCCUP_RATE;             //CPU占用率				0~99
	u32 CPU_TEMPERATURE;						//CPU温度					0~255
	
	u32 GPU_FREQUENCY;							//GPU频率				0~9999
	u32 GPU_OCCUP_RATE;             //GPU占用率			0~99
	u32 GPU_TEMPERATURE;						//GPU温度				0~255
	
	u32 SPEED_UP;           				//上传网速			0~~999M
	u32 SPEED_DOWN; 								//下载网速			0~~999M
	
		
}PC_DATA;



void display_menu1(void);
void display_menu2(void);
void display_menu3(void);
void display_menu4_1(void);
void display_menu4_2(void);
void display_menu5(void);
void Init_PC_Data(void);
#endif

