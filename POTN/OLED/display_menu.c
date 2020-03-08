#include "display_menu.h"



PC_DATA PC1;
PC_DATA *PC_Data = &PC1;     //一个用于保存PC数据的 变量



u8 *week[8]={"Mon","Tue","Wed","Thu","Fri","Sat","Sun","war"};
u8 *month[13]={"-Jan-","-Feb-","-Mar-","-Apr-","-May-","-Jun-",
							"-Jul-","-Aug-","-Sep-","-Oct-","-Nov-","-Dec-","warn-"};
																	
void Init_PC_Data(void){
	PC_Data->MINUTE = 48;
	PC_Data->HOUR = 15;
	PC_Data->DAY = 10;
	PC_Data->WEEK_NUM = 1;
	PC_Data->MONTH_NUM = 2;
	PC_Data->YEAR = 2019;
	
	PC_Data->RAM_ALL = 16;
	PC_Data->RAM_USED_INTEGR = 10;
	PC_Data->RAM_USED_FRACTION = 0;
	
	PC_Data->VIDEO_MEMORY_ALL = 6;
	PC_Data->VIDEO_MEMORY_INTEGR = 3;
	PC_Data->VIDEO_MEMORY_FRACTION = 0;
	
	PC_Data->CPU_FREQUENCY = 2335;
	PC_Data->CPU_OCCUP_RATE = 35;
	PC_Data->CPU_TEMPERATURE = 46;
	
	PC_Data->GPU_FREQUENCY = 1548;
	PC_Data->GPU_OCCUP_RATE = 86;
	PC_Data->GPU_TEMPERATURE = 124;
	
	PC_Data->SPEED_UP = 15641;
	PC_Data->SPEED_DOWN = 231;
	
	PC_Data->RAM_RATE = (PC_Data->RAM_USED_INTEGR*100+PC_Data->RAM_USED_FRACTION*10)/PC_Data->RAM_ALL;
	if(PC_Data->RAM_RATE > 99){
		PC_Data->RAM_RATE = 99;
	}
	
	PC_Data->VIDEO_MEMORY_RATE = (PC_Data->VIDEO_MEMORY_INTEGR*100 + PC_Data->VIDEO_MEMORY_FRACTION*10)/PC_Data->VIDEO_MEMORY_ALL;
	if(PC_Data->VIDEO_MEMORY_RATE > 99){
		PC_Data->VIDEO_MEMORY_RATE = 99;
	}
	
}						

// 显示时间界面
void display_menu1(void){
	
		u32 Internet_up;
		u32 Internet_down;

		Internet_up =PC_Data->SPEED_UP;
		Internet_down = PC_Data->SPEED_DOWN;
	
		OLED_Only_Clear();
	
		OLED_ShowString_Space(0,0,week[PC_Data->WEEK_NUM-1],12);  //显示星期
		OLED_ShowNum_Space(51,0,PC_Data->DAY,2,12);	     // 显示号数
		OLED_ShowString_Space(51+7*2,0,month[PC_Data->MONTH_NUM-1],12);  //显示月份
		OLED_ShowNum_Space(51+7*2+7*5,0,PC_Data->YEAR,4,12);  //显示年份
		
		OLED_ShowNum(24,13,PC_Data->HOUR,2,32);  		 		//显示小时
		OLED_ShowChar(24+16*2,13,'9'+1,32); 	//显示 ：
		OLED_ShowNum(24+16*2+16,13,PC_Data->MINUTE,2,32);	// 显示分

		// 上传网速
		if(Internet_up < 1000){               				//网速小于1000KB
		OLED_ShowNum_Nozero(0,47,Internet_up,3,16);
		OLED_ShowString(3*8,47,"KB",16);
		OLED_ShowChar(3*8+2*8,47,'~'+1,16);
		}
		else if(Internet_up >= 1000){								
			if(Internet_up/1024 < 10){									//网速 小于10M
					OLED_ShowNum(0,47,Internet_up/1024,1,16);
					OLED_ShowChar(8,47,'.',16);
					OLED_ShowNum(8*2,47,(Internet_up%1024)*10/1024,1,16);
//				OLED_ShowString(8*3,47,"MB",16);
//				OLED_ShowChar(8*5,47,'~'+1,16);
			}
			else if(Internet_up/1024 >= 10 && Internet_up/1024 <= 999){					// 网速大于10M
				OLED_ShowNum_Nozero(0,47,Internet_up/1024,3,16);
//			OLED_ShowString(8*3,47,"MB",16);
//			OLED_ShowChar(8*5,47,'~'+1,16);
			}

			else if(Internet_up/1024 > 999){																					//网速 大于 999M
				OLED_ShowNum_Nozero(0,47,999,3,16);
//			OLED_ShowString(8*3,47,"MB",16);
//			OLED_ShowChar(8*5,47,'~'+1,16);
			}
			OLED_ShowString(8*3,47,"MB",16);
			OLED_ShowChar(8*5,47,'~'+1,16);			
		}
		
		
		//下载网速
		if(Internet_down < 1000){               				//网速小于1000KB
			OLED_ShowNum_Nozero(127-6*8,47,Internet_down,3,16);
			OLED_ShowString(127-3*8,47,"KB",16);
			OLED_ShowChar(127-8,47,'~'+2,16);
		}
		
		else if(Internet_down >= 1000){				
			if(Internet_down/1024 < 10){									//网速 小于10M
				OLED_ShowNum(127-8*6,47,Internet_down/1024,1,16);
				OLED_ShowChar(127-8*5,47,'.',16);
				OLED_ShowNum(127-8*4,47,(Internet_down%1024)*10/1024,1,16);
//				OLED_ShowString(127-3*8,47,"MB",16);
//				OLED_ShowChar(127-8,47,'~'+2,16);
			}
			else if(Internet_down/1024 >= 10 && Internet_down/1024 <= 999){					// 网速大于10M 小于 999M
				OLED_ShowNum_Nozero(127-6*8,47,Internet_down/1024,3,16);
//				OLED_ShowString(127-3*8,47,"MB",16);
//				OLED_ShowChar(127-8,47,'~'+1,16);
			}
			else if(Internet_down/1024 > 999){																					//网速 大于 999M
				OLED_ShowNum_Nozero(127-6*8,47,999,3,16);
//				OLED_ShowString(127-3*8,47,"MB",16);
//				OLED_ShowChar(127-8,47,'~'+1,16);
			}
			OLED_ShowString(127-3*8,47,"MB",16);
			OLED_ShowChar(127-8,47,'~'+1,16);
		
		}
		OLED_Refresh();

}
//显示PC资源
void display_menu2(void){

	OLED_Only_Clear();
	OLED_ShowString(0,0,"CPU:  %",16);
	OLED_ShowNum_Nozero(4*8,0,PC_Data->CPU_OCCUP_RATE,2,16);
	OLED_ShowNum_Nozero(7*8+4,0,PC_Data->CPU_TEMPERATURE,3,16);
	OLED_ShowChar(8*10+4,0,'~'+3,16);
	OLED_ShowNum_Nozero(97,2,PC_Data->CPU_FREQUENCY,4,12);
	OLED_ShowString(97+6*4,2,"M",12);
	
	OLED_ShowString(0,16,"GPU:  %",16);
	OLED_ShowNum_Nozero(4*8,16,PC_Data->GPU_OCCUP_RATE,2,16);
	OLED_ShowNum_Nozero(7*8+4,16,PC_Data->GPU_TEMPERATURE,3,16);
	OLED_ShowChar(8*10+4,16,'~'+3,16);
	OLED_ShowNum_Nozero(97,16+2,PC_Data->GPU_FREQUENCY,4,12);
	OLED_ShowString(97+6*4,16+2,"m",12);
	
	
	OLED_ShowString(0,32,"RAM:",16);
	OLED_ShowNum_Nozero(4*8,32,PC_Data->RAM_USED_INTEGR,2,16);
	OLED_ShowChar(48,32,'.',16);
	OLED_ShowNum(48+4,32,PC_Data->RAM_USED_FRACTION,1,16);
	OLED_ShowString(48+4+8,32,"/",16);
	OLED_ShowNum_Nozero(48+4+8+8,32,PC_Data->RAM_ALL,2,16);
	OLED_ShowString(48+4+32,32,"G",16);
	OLED_ShowNum_Nozero(127-8*3,32,PC_Data->RAM_RATE,2,16);
	OLED_ShowString(127-8,32,"%",16);
	
	OLED_ShowString(0,48,"V_M:",16);
	OLED_ShowNum_Nozero(4*8,48,PC_Data->VIDEO_MEMORY_INTEGR,2,16);
	OLED_ShowChar(48,48,'.',16);
	OLED_ShowNum(48+4,48,PC_Data->VIDEO_MEMORY_FRACTION,1,16);
	OLED_ShowString(48+4+8,48,"/",16);
	OLED_ShowNum_Nozero(48+4+8+8,48,PC_Data->VIDEO_MEMORY_ALL,2,16);
	OLED_ShowString(48+4+32,48,"G",16);
	OLED_ShowNum_Nozero(127-8*3,48,PC_Data->VIDEO_MEMORY_RATE,2,16);
	OLED_ShowString(127-8,48,"%",16);	
	OLED_Refresh();

}

//显示 CPU 和显卡 型号
void display_menu3(void){

	OLED_Only_Clear();
	OLED_ShowString(0,0,"CPU",16);
	OLED_ShowChinese(3*8,0,0,16);
	OLED_ShowChinese(5*8,0,1,16);
	OLED_ShowString(7*8,0,":",16);
	if(PC_Data->CPU_NMAE_LEN > 16){
	  OLED_ShowString(0,16,PC_Data->CPU_NAME,12);
	}
	else{
		OLED_ShowString(0,16,PC_Data->CPU_NAME,16);
	}

	
	OLED_ShowString(0,32,"GPU",16);
	OLED_ShowChinese(3*8,32,0,16);
	OLED_ShowChinese(5*8,32,1,16);
	OLED_ShowString(7*8,32,":",16);
	
	if(PC_Data->GPU_NAME_LEN > 16){
		OLED_ShowString(0,48,PC_Data->GPU_NAME,12);
	}
	else{
		OLED_ShowString(0,48,PC_Data->GPU_NAME,16);
	}

	
	OLED_Refresh();
}


//显示开始挂机
void display_menu4_1(void){

	OLED_Only_Clear();
	
	OLED_ShowChinese(0,0,0,32);
	OLED_ShowChinese(32,0,1,32);
	OLED_ShowChinese(64,0,2,32);
	OLED_ShowChinese(96,0,3,32);	
	
	OLED_ShowChinese(0,32,4,32);
	OLED_ShowChinese(32,32,5,32);
	OLED_ShowChinese(64,32,6,32);
	OLED_ShowChinese(96,32,7,32);
	OLED_Refresh();
}

//显示 挂机进行中
void display_menu4_2(void){
	OLED_Only_Clear();
		
	OLED_ShowChinese(16,0,4,32);
	OLED_ShowChinese(48,0,5,32);
	OLED_ShowChinese(78,0,8,32);	

	OLED_ShowChinese(0,32,0,32);	
	OLED_ShowChinese(32,32,1,32);
	OLED_ShowChinese(64,32,9,32);
	OLED_ShowChinese(96,32,10,32);	
	OLED_Refresh();
}

// 传输出错
void display_menu5(void){
	OLED_Only_Clear();
		
	OLED_ShowChinese(32,0,25,16);   //传输出错，
	OLED_ShowChinese(32+16,0,26,16);
	OLED_ShowChinese(32+16*2,0,27,16);
	OLED_ShowChinese(32+16*3,0,28,16);

	
	OLED_ShowChinese(24,16,30,16);  //请重新运行
	OLED_ShowChinese(24+16,16,31,16);	
	OLED_ShowChinese(24+16*2,16,32,16);	
	OLED_ShowChinese(24+16*3,16,33,16);	
	OLED_ShowChinese(24+16*4,16,34,16);		

	OLED_ShowChinese(32,32,35,16);	 //驱动程序
	OLED_ShowChinese(32+16,32,36,16);	
	OLED_ShowChinese(32+16*2,32,37,16);	
	OLED_ShowChinese(32+16*3,32,38,16);	

	
	OLED_ShowChinese(32,48,40,16);	 //插拔设备
	OLED_ShowChinese(32+16,48,41,16);	
	OLED_ShowChinese(32+16*2,48,42,16);	
	OLED_ShowChinese(32+16*3,48,43,16);	

	
	
	OLED_Refresh();
}
















