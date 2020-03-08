#ifndef __OLED_H
#define __OLED_H 

//#include "sys.h"
#include "stdlib.h"	
#include "stm32f1xx_hal.h"
#include "includes.h"	//UCOS相关
////-----------------测试LED端口定义---------------- 

//#define LED_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)
//#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)

////-----------------OLED端口定义---------------- 

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//DIN
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

//#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
//#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_0)

//#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_1)//DC
//#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_1)
// 		     
//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)  //时钟 D0
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)//DIN  数据 D1
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET)//DC
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET)

//#define OLED_CS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
//#define OLED_CS_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define UP (95 + ' ')
#define DOWN (96 + ' ')

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int
	


/* 定义颜色数据类型(可以是数据结构) */
#define  TCOLOR	            uint8_t     // 自行修改

typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */




void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
//void OLED_DrawPoint(u8 x,u8 y);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
//void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,TCOLOR dot);
//void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_DrawCircle(u8 x,u8 y,u8 r,TCOLOR dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1);
void OLED_ScrollDisplay(u8 num,u8 space);
void OLED_WR_BP(u8 x,u8 y);
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[]);
void OLED_Init(void);

void OLED_ShowString_Space(u8 x,u8 y,u8 *chr,u8 size1);
void OLED_ShowNum_Space(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_ShowNum_Nozero(u8 x,u8 y,u32 num,u8 len,u8 size1);
void OLED_Only_Clear(void);


void  GUI_RLine(uint16 x0, uint8 y0, uint8 y1, TCOLOR color);
void  GUI_LineWith(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint8 with, TCOLOR color);
void  GUI_HLine(uint16 x0, uint8 y0, uint16 x1, TCOLOR color);
#endif
