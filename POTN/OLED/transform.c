#include "transform.h"

extern unsigned char USB_Recive_Buffer[64]; //USB接收缓存
extern PC_DATA *PC_Data;     //一个用于保存PC数据的 变量


void Buffer_Set(u8 *buffer,u8 num){
	int i;
	
	for(i = 0; i < 64; i++){
		buffer[i] = num;
	}

}	

void transform_CPUGPU(void){
	
	PC_Data->RAM_ALL = (USB_Recive_Buffer[1]*512+512)/1024;   //总内存
	if(PC_Data->RAM_ALL > 99) PC_Data->RAM_ALL = 99;
	PC_Data->VIDEO_MEMORY_ALL = (USB_Recive_Buffer[2]*128+128)/1024;  //总显存
	
	
	
	PC_Data->CPU_NMAE_LEN = USB_Recive_Buffer[3];								//CPU型号名称的长度
	if(PC_Data->CPU_NMAE_LEN > 20)	PC_Data->CPU_NMAE_LEN = 20;	
	PC_Data->GPU_NAME_LEN = USB_Recive_Buffer[24];							//GPU型号名称的长度
	if(PC_Data->GPU_NAME_LEN > 20)	PC_Data->GPU_NAME_LEN = 20;
	
	
	for(int i = 0; i < 20; i++){										//CPU型号名称 复位
		PC_Data->CPU_NAME[i] = 0xff;
	}
	for(int i = 0;i < PC_Data->CPU_NMAE_LEN;i++){							//CPU 型号
		PC_Data->CPU_NAME[i] = USB_Recive_Buffer[4+i];	
	}
	
	for(int i = 0; i < 20;i++){											//GPU型号名称  复位
		PC_Data->GPU_NAME[i] = 0xff;
	}
	for(int i = 0; i < PC_Data->GPU_NAME_LEN;i++){
		PC_Data->GPU_NAME[i] = USB_Recive_Buffer[i+25];
	}
	Buffer_Set(USB_Recive_Buffer,0);
}








void transform_DATA(void){
	
	PC_Data->CPU_FREQUENCY = USB_Recive_Buffer[1];  		//CPU 频率 
	PC_Data->CPU_FREQUENCY <<= 8;
	PC_Data->CPU_FREQUENCY += USB_Recive_Buffer[2];
	PC_Data->CPU_FREQUENCY <<= 8;
	PC_Data->CPU_FREQUENCY += USB_Recive_Buffer[3];
	if(PC_Data->CPU_FREQUENCY > 9999) PC_Data->CPU_FREQUENCY = 9999;
	
	PC_Data->CPU_OCCUP_RATE = USB_Recive_Buffer[4];											//CPU占用率
	if(PC_Data->CPU_OCCUP_RATE >= 100) PC_Data->CPU_OCCUP_RATE =99;
	
	PC_Data->CPU_TEMPERATURE = USB_Recive_Buffer[5];									//CPU温度
	
	
	
	PC_Data->GPU_FREQUENCY = USB_Recive_Buffer[6];     //GPU频率
	PC_Data->GPU_FREQUENCY <<= 8;
	PC_Data->GPU_FREQUENCY += USB_Recive_Buffer[7];
	PC_Data->GPU_FREQUENCY <<= 8;
	PC_Data->GPU_FREQUENCY += USB_Recive_Buffer[8];	
	if(PC_Data->GPU_FREQUENCY > 9999) PC_Data->GPU_FREQUENCY = 9999;
	
	PC_Data->GPU_OCCUP_RATE = USB_Recive_Buffer[9];    	//GPU 占用率 
	if(PC_Data->GPU_OCCUP_RATE >= 100) PC_Data->GPU_OCCUP_RATE  =99;
	
	PC_Data->GPU_TEMPERATURE = USB_Recive_Buffer[10];  //GPU温度
	
	
	PC_Data->RAM_USED_INTEGR = USB_Recive_Buffer[11];													//当前占用内存整数部分
	if(PC_Data->RAM_USED_INTEGR > 99) PC_Data->RAM_USED_INTEGR = 99;	
	PC_Data->RAM_USED_FRACTION = USB_Recive_Buffer[12];												//当前占用内存小数部分 
	if(PC_Data->RAM_USED_FRACTION > 9) PC_Data->RAM_USED_FRACTION = 9;
	
	PC_Data->RAM_RATE = (PC_Data->RAM_USED_INTEGR*100+PC_Data->RAM_USED_FRACTION*10)/PC_Data->RAM_ALL; //内存占用率
	if(PC_Data->RAM_RATE > 99){
		PC_Data->RAM_RATE = 99;
	}
	
	PC_Data->VIDEO_MEMORY_INTEGR = USB_Recive_Buffer[13];    									//当前显存占用整数部分
	if(PC_Data->VIDEO_MEMORY_INTEGR > 99) PC_Data->VIDEO_MEMORY_INTEGR = 99;
	PC_Data->VIDEO_MEMORY_FRACTION = USB_Recive_Buffer[14];
	if(PC_Data->VIDEO_MEMORY_FRACTION > 9) PC_Data->VIDEO_MEMORY_FRACTION = 9; //当前显存占用小数部分
	
	PC_Data->VIDEO_MEMORY_RATE = (PC_Data->VIDEO_MEMORY_INTEGR*100 + PC_Data->VIDEO_MEMORY_FRACTION*10)/PC_Data->VIDEO_MEMORY_ALL;
	if(PC_Data->VIDEO_MEMORY_RATE > 99){																				// 显存占用率
		PC_Data->VIDEO_MEMORY_RATE = 99;
	}
	
	PC_Data->SPEED_UP = USB_Recive_Buffer[15];					//上传网速
	PC_Data->SPEED_UP <<= 8;
	PC_Data->SPEED_UP += USB_Recive_Buffer[16];
	PC_Data->SPEED_UP <<= 8;
	PC_Data->SPEED_UP += USB_Recive_Buffer[17];	
	PC_Data->SPEED_UP <<= 8;
	PC_Data->SPEED_UP += USB_Recive_Buffer[18];		
	
	PC_Data->SPEED_DOWN = USB_Recive_Buffer[19];     //下载网速
	PC_Data->SPEED_DOWN <<= 8;
	PC_Data->SPEED_DOWN += USB_Recive_Buffer[20];
	PC_Data->SPEED_DOWN <<= 8;
	PC_Data->SPEED_DOWN += USB_Recive_Buffer[21];	
	PC_Data->SPEED_DOWN <<= 8;
	PC_Data->SPEED_DOWN += USB_Recive_Buffer[22];		
	
	
	PC_Data->YEAR = USB_Recive_Buffer[23];  						//当前年份
	PC_Data->YEAR <<= 8;
	PC_Data->YEAR += USB_Recive_Buffer[24];
	if(PC_Data->YEAR > 9999) PC_Data->YEAR = 9999;
	
	PC_Data->MONTH_NUM = USB_Recive_Buffer[25];  					//月份
	if(PC_Data->MONTH_NUM > 12 || PC_Data->MONTH_NUM == 0) PC_Data->MONTH_NUM = 13;
	
	PC_Data->WEEK_NUM = USB_Recive_Buffer[26];
	if(PC_Data->WEEK_NUM > 7 || PC_Data->WEEK_NUM == 0) PC_Data->WEEK_NUM = 8;
	
	
	PC_Data->DAY = USB_Recive_Buffer[27];								//号数
	if(PC_Data->DAY > 31 || PC_Data->DAY == 0) PC_Data->DAY = 99;
	
	PC_Data->HOUR = USB_Recive_Buffer[28];							//小时
	if(PC_Data->HOUR > 24) PC_Data->HOUR = 99;
	
	PC_Data->MINUTE = USB_Recive_Buffer[29];						// 分
	if(PC_Data->MINUTE > 59) PC_Data->MINUTE = 99;
	
	Buffer_Set(USB_Recive_Buffer,0);
}




