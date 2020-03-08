#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 

//#include "delay.h"

u8 OLED_GRAM[128][8];

//·´ÏÔº¯Êı
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//Õı³£ÏÔÊ¾
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//·´É«ÏÔÊ¾
		}
}

//ÆÁÄ»Ğı×ª180¶È
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//Õı³£ÏÔÊ¾
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//·´×ªÏÔÊ¾
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}


void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
//	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
//	OLED_CS_Set();
	OLED_DC_Set();   	  
}

//¿ªÆôOLEDÏÔÊ¾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µçºÉ±ÃÊ¹ÄÜ
	OLED_WR_Byte(0x14,OLED_CMD);//¿ªÆôµçºÉ±Ã
	OLED_WR_Byte(0xAF,OLED_CMD);//µãÁÁÆÁÄ»
}

//¹Ø±ÕOLEDÏÔÊ¾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µçºÉ±ÃÊ¹ÄÜ
	OLED_WR_Byte(0x10,OLED_CMD);//¹Ø±ÕµçºÉ±Ã
	OLED_WR_Byte(0xAF,OLED_CMD);//¹Ø±ÕÆÁÄ»
}

//¸üĞÂÏÔ´æµ½OLED	
void OLED_Refresh(void)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();	//½øÈëÁÙ½çÇø
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   OLED_WR_Byte(0xb0+i,OLED_CMD); //ÉèÖÃĞĞÆğÊ¼µØÖ·
	   OLED_WR_Byte(0x00,OLED_CMD);   //ÉèÖÃµÍÁĞÆğÊ¼µØÖ·
	   OLED_WR_Byte(0x10,OLED_CMD);   //ÉèÖÃ¸ßÁĞÆğÊ¼µØÖ·
	   for(n=0;n<128;n++)
		 OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
	OS_CRITICAL_EXIT();	//ÍË³öÁÙ½çÇø		
}
//ÇåÆÁº¯Êı
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//Çå³ıËùÓĞÊı¾İ
			}
  }
	OLED_Refresh();//¸üĞÂÏÔÊ¾
//	    u8 i,n;
//    for(i=0; i<8; i++)
//    {
//        OLED_WR_Byte (0xb0+i,OLED_CMD);    //?????(0~7)
//        OLED_WR_Byte (0x00,OLED_CMD);      //??????—????
//        OLED_WR_Byte (0x10,OLED_CMD);      //??????—????
//        for(n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA);
//    } //????
}

//ÇåÆÁº¯Êı
void OLED_Only_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//Çå³ıËùÓĞÊı¾İ
			}
  }
//	    u8 i,n;
//    for(i=0; i<8; i++)
//    {
//        OLED_WR_Byte (0xb0+i,OLED_CMD);    //?????(0~7)
//        OLED_WR_Byte (0x00,OLED_CMD);      //??????—????
//        OLED_WR_Byte (0x10,OLED_CMD);      //??????—????
//        for(n=0; n<128; n++)OLED_WR_Byte(0,OLED_DATA);
//    } //????
}


//»­µã 
//x:0~127
//y:0~63
//void OLED_DrawPoint(u8 x,u8 y)
//{
//	u8 i,m,n;
//	if(x>131||y>63)return;//³¬³ö·¶Î§ÁË.
//	i=y/8;
//	m=y%8;
//	n=1<<m;
//	OLED_GRAM[x][i]|=n;
//}

//»­µã 
//x:0~127
//y:0~63
//t:1 Ìî³ä 0,Çå¿Õ				   
void OLED_DrawPoint(uint8_t x,uint8_t y,TCOLOR t)
{
	u8 i,m,n;
	if(x>131||y>63)return;//³¬³ö·¶Î§ÁË.
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t)OLED_GRAM[x][i]|=n;
	else OLED_GRAM[x][i]&=~n;
//	if(t)OLED_GRAM[x][pos]|=temp;
//	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 Ìî³äÇøÓòµÄ¶Ô½Ç×ø±ê
//È·±£x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,Çå¿Õ;1,Ìî³ä	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,TCOLOR dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh();//¸üĞÂÏÔÊ¾
}

//Çå³ıÒ»¸öµã
//x:0~127
//y:0~63
void OLED_ClearPoint(u8 x,u8 y)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	OLED_GRAM[x][i]|=n;
	OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}


//»­Ïß
//x:0~128
//y:0~64
//dot:0,Çå¿Õ;1,Ìî³ä	 
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,TCOLOR dot)
{
	u8 i,k,k1,k2,y0;
	if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))return;
	if(x1==x2)    //»­ÊúÏß
	{
			for(i=0;i<(y2-y1);i++)
			{
				OLED_DrawPoint(x1,y1+i,dot);
			}
  }
	else if(y1==y2)   //»­ºáÏß
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1,dot);
			}
  }
	else      //»­Ğ±Ïß
	{
		k1=y2-y1;
		k2=x2-x1;
		k=k1*10/k2;
		for(i=0;i<(x2-x1);i++)
			{
			  OLED_DrawPoint(x1+i,y1+i*k/10,dot);
			}
	}
}
//x,y:Ô²ĞÄ×ø±ê
//r:Ô²µÄ°ë¾¶
void OLED_DrawCircle(u8 x,u8 y,u8 r,TCOLOR dot)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,dot);
        OLED_DrawPoint(x - a, y - b,dot);
        OLED_DrawPoint(x - a, y + b,dot);
        OLED_DrawPoint(x + a, y + b,dot);
 
        OLED_DrawPoint(x + b, y + a,dot);
        OLED_DrawPoint(x + b, y - a,dot);
        OLED_DrawPoint(x - b, y - a,dot);
        OLED_DrawPoint(x - b, y + a,dot);
        
        a++;
        num = (a * a + b * b) - r*r;//¼ÆËã»­µÄµãÀëÔ²ĞÄµÄ¾àÀë
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//size:Ñ¡Ôñ×ÖÌå 12/16/24
//È¡Ä£·½Ê½ ÖğÁĞÊ½
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1)
{
	u8 i,m,temp,size2,chr1;
	u8 y0=y;
	size2=(size1/8+((size1%8)?1:0))*(size1/2);  //µÃµ½×ÖÌåÒ»¸ö×Ö·û¶ÔÓ¦µãÕó¼¯ËùÕ¼µÄ×Ö½ÚÊı
	chr1=chr-' ';  //¼ÆËãÆ«ÒÆºóµÄÖµ
	for(i=0;i<size2;i++)
	{
		if(size1==12)
        {temp=asc2_1206[chr1][i];} //µ÷ÓÃ1206×ÖÌå
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //µ÷ÓÃ1608×ÖÌå
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //µ÷ÓÃ2412×ÖÌå
		else if(size1==32)
        {temp=asc2_3216[chr1-16][i];} //µ÷ÓÃ3618×ÖÌå
		else return;
				for(m=0;m<8;m++)           //Ğ´ÈëÊı¾İ
				{
					if(temp&0x80)OLED_DrawPoint(x,y,1);
					else OLED_ClearPoint(x,y);
					temp<<=1;
					y++;
					if((y-y0)==size1)
					{
						y=y0;
						x++;
						break;
          }
				}
  }
}


//ÏÔÊ¾×Ö·û´®
//x,y:Æğµã×ø±ê  
//size1:×ÖÌå´óĞ¡ 
//*chr:×Ö·û´®ÆğÊ¼µØÖ· 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1)
{
	while((*chr>=' ')&&(*chr<='~'))//ÅĞ¶ÏÊÇ²»ÊÇ·Ç·¨×Ö·û!
	{
		OLED_ShowChar(x,y,*chr,size1);
		x+=size1/2;
		if(x>128-size1)  //»»ĞĞ
		{
			x=0;
			y+=2;
    }
		chr++;
  }
}

//ÏÔÊ¾×Ö·û´®
//x,y:Æğµã×ø±ê  
//size1:×ÖÌå´óĞ¡ 
//*chr:×Ö·û´®ÆğÊ¼µØÖ· 
void OLED_ShowString_Space(u8 x,u8 y,u8 *chr,u8 size1)
{
	u8 i = 1;
	while((*chr>=' ')&&(*chr<='~'))//ÅĞ¶ÏÊÇ²»ÊÇ·Ç·¨×Ö·û!
	{
		OLED_ShowChar(x+i,y,*chr,size1);
		i++;
		x+=size1/2;
		if(x>128-size1)  //»»ĞĞ
		{
			x=0;
			y+=size1;
    }
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

////ÏÔÊ¾2¸öÊı×Ö
////x,y :Æğµã×ø±ê	 
////len :Êı×ÖµÄÎ»Êı
////size:×ÖÌå´óĞ¡
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size1/2)*t,y,' ',size1);
 //     }
//			else 
//			{
			  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
//			}
  }
}

////ÏÔÊ¾2¸öÊı×Ö
////x,y :Æğµã×ø±ê	 
////len :Êı×ÖµÄÎ»Êı
////size:×ÖÌå´óĞ¡
//¿ªÍ·µÄ0 ²»ÏÔÊ¾
void OLED_ShowNum_Nozero(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp,flag;
	flag = 1;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0 && flag == 1 && t != (len -1))
			{
				OLED_ShowChar(x+(size1/2)*t,y,' ',size1);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
				flag = 0;
			}
  }
}



////ÏÔÊ¾2¸öÊı×Ö
////x,y :Æğµã×ø±ê	 
////len :Êı×ÖµÄÎ»Êı
////size:×ÖÌå´óĞ¡
void OLED_ShowNum_Space(u8 x,u8 y,u32 num,u8 len,u8 size1)
{
	u8 t,temp;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size1/2)*t,y,' ',size1);
 //     }
//			else 
//			{
			  OLED_ShowChar(x+(size1/2)*t+t+1,y,temp+'0',size1);
//			}
  }
}

//ÏÔÊ¾ºº×Ö
//x,y:Æğµã×ø±ê
//num:ºº×Ö¶ÔÓ¦µÄĞòºÅ
//È¡Ä£·½Ê½ ÁĞĞĞÊ½
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1)
{
	u8 i,m,n=0,temp,chr1;
	u8 x0=x,y0=y;
	u8 size3=size1/8;
	while(size3--)
	{
		chr1=num*size1/8+n;
		n++;
			for(i=0;i<size1;i++)
			{
				if(size1==16)
						{temp=Hzk1[chr1][i];}//µ÷ÓÃ16*16×ÖÌå
				else if(size1==24)
						{temp=Hzk2[chr1][i];}//µ÷ÓÃ24*24×ÖÌå
				else if(size1==32)       
						{temp=Hzk3[chr1][i];}//µ÷ÓÃ32*32×ÖÌå
//				else if(size1==64)
//						{temp=Hzk4[chr1][i];}//µ÷ÓÃ64*64×ÖÌå
				else return;
							
						for(m=0;m<8;m++)
							{
								if(temp&0x01)OLED_DrawPoint(x,y,1);
								else OLED_ClearPoint(x,y);
								temp>>=1;
								y++;
							}
							x++;
							if((x-x0)==size1)
							{x=x0;y0=y0+8;}
							y=y0;
			 }
	}
}

//num ÏÔÊ¾ºº×ÖµÄ¸öÊı
//space Ã¿Ò»±éÏÔÊ¾µÄ¼ä¸ô
void OLED_ScrollDisplay(u8 num,u8 space)
{
	OS_ERR err;
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,12,t+2,16); //Ğ´ÈëÒ»¸öºº×Ö±£´æÔÚOLED_GRAM[][]Êı×éÖĞ
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //ÏÔÊ¾¼ä¸ô
				 {
					for(i=0;i<144;i++)
						{
							for(n=0;n<4;n++)
							{
								OLED_GRAM[i-1][n]=0;
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=0;i<144;i++)   //ÊµÏÖ×óÒÆ
		{
			for(n=0;n<4;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_HMSM_STRICT,&err); //ÑÓÊ±10ms
	}
}

//ÅäÖÃĞ´ÈëÊı¾İµÄÆğÊ¼Î»ÖÃ
void OLED_WR_BP(u8 x,u8 y)
{
	OLED_WR_Byte(0xb0+y,OLED_CMD);//ÉèÖÃĞĞÆğÊ¼µØÖ·
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0£ºÆğµã×ø±ê
//x1,y1£ºÖÕµã×ø±ê
//BMP[]£ºÒªĞ´ÈëµÄÍ¼Æ¬Êı×é
void OLED_ShowPicture(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	u32 j=0;
	u8 x=0,y=0;
	if(y%8==0)y=0;
	else y+=1;
	for(y=y0;y<y1;y++)
	 {
		 OLED_WR_BP(x0,y);
		 for(x=x0;x<x1;x++)
		 {
			 OLED_WR_Byte(BMP[j],OLED_DATA);
			 j++;
     }
	 }
}
//OLEDµÄ³õÊ¼»¯
void OLED_Init(void)
{
	OS_ERR err;	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //??B????
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	 
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//??50MHz
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //???GPIOD1,12,13,15
// 	GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15);	
//	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
		__HAL_RCC_GPIOB_CLK_ENABLE();
			/*Configure GPIO pin : PB 1 12 13 15 */
		GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	OLED_RST_Set();
	OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //ÑÓÊ±100ms
	OLED_RST_Clr();//¸´Î»
	OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //ÑÓÊ±200ms
	OLED_RST_Set();

	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0×óÓÒ·´ÖÃ 0xa1Õı³£
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0ÉÏÏÂ·´ÖÃ 0xc8Õı³£
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);
	OLED_Clear();
}




/****************************************************************************
* Ãû³Æ£ºGUI_RLine()
* ¹¦ÄÜ£º»­´¹Ö±Ïß¡£
* Èë¿Ú²ÎÊı£º x0     ´¹Ö±ÏßÆğµãËùÔÚÁĞµÄÎ»ÖÃ
*           y0      ´¹Ö±ÏßÆğµãËùÔÚĞĞµÄÎ»ÖÃ
*           y1      ´¹Ö±ÏßÖÕµãËùÔÚĞĞµÄÎ»ÖÃ
*           color   ÏÔÊ¾ÑÕÉ«
* ³ö¿Ú²ÎÊı£ºÎŞ
* ËµÃ÷£º¶ÔÓÚµ¥É«¡¢4¼¶»Ò¶ÈµÄÒº¾§£¬¿ÉÍ¨¹ıĞŞ¸Ä´Ëº¯Êı×÷Í¼Ìá¸ßËÙ¶È£¬Èçµ¥É«LCM£¬¿ÉÒÔÒ»´Î¸ü
*      ĞÂ8¸öµã£¬¶ø²»ĞèÒªÒ»¸öµãÒ»¸öµãµÄĞ´µ½LCMÖĞ¡£
****************************************************************************/
void  GUI_RLine(uint16 x0, uint8 y0, uint8 y1, TCOLOR color)
{
    uint8  temp;
    if(y0>y1)       // ¶Ôy0¡¢y1´óĞ¡½øĞĞÅÅÁĞ£¬ÒÔ±ã»­Í¼
    {
        temp = y1;
        y1 = y0;
        y0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, color);   // ÖğµãÏÔÊ¾£¬Ãè³ö´¹Ö±Ïß
        y0++;
    }
    while(y1>=y0);
}


/****************************************************************************
* Ãû³Æ£ºGUI_HLine()
* ¹¦ÄÜ£º»­Ë®Æ½Ïß¡£
* Èë¿Ú²ÎÊı£º x0     Ë®Æ½ÏßÆğµãËùÔÚÁĞµÄÎ»ÖÃ
*           y0      Ë®Æ½ÏßÆğµãËùÔÚĞĞµÄÎ»ÖÃ
*           x1      Ë®Æ½ÏßÖÕµãËùÔÚÁĞµÄÎ»ÖÃ
*           color   ÏÔÊ¾ÑÕÉ«(¶ÔÓÚºÚ°×É«LCM£¬Îª0Ê±Ãğ£¬Îª1Ê±ÏÔÊ¾)
* ³ö¿Ú²ÎÊı£ºÎŞ
* ËµÃ÷£º¶ÔÓÚµ¥É«¡¢4¼¶»Ò¶ÈµÄÒº¾§£¬¿ÉÍ¨¹ıĞŞ¸Ä´Ëº¯Êı×÷Í¼Ìá¸ßËÙ¶È£¬Èçµ¥É«LCM£¬¿ÉÒÔÒ»´Î¸ü
*      ĞÂ8¸öµã£¬¶ø²»ĞèÒªÒ»¸öµãÒ»¸öµãµÄĞ´µ½LCMÖĞ¡£
****************************************************************************/
void  GUI_HLine(uint16 x0, uint8 y0, uint16 x1, TCOLOR color)
{
    uint8  temp;
    if(x0>x1)               // ¶Ôx0¡¢x1´óĞ¡½øĞĞÅÅÁĞ£¬ÒÔ±ã»­Í¼
    {
        temp = x1;
        x1 = x0;
        x0 = temp;
    }
    do
    {
        OLED_DrawPoint(x0, y0, color);   // ÖğµãÏÔÊ¾£¬Ãè³ö´¹Ö±Ïß
        x0++;
    }
    while(x1>=x0);
}




/****************************************************************************
* Ãû³Æ£ºGUI_LineWith()
* ¹¦ÄÜ£º»­ÈÎÒâÁ½µãÖ®¼äµÄÖ±Ïß£¬²¢ÇÒ¿ÉÉèÖÃÏßµÄ¿í¶È¡£
* Èë¿Ú²ÎÊı£º x0		Ö±ÏßÆğµãµÄx×ø±êÖµ
*           y0		Ö±ÏßÆğµãµÄy×ø±êÖµ
*           x1      Ö±ÏßÖÕµãµÄx×ø±êÖµ
*           y1      Ö±ÏßÖÕµãµÄy×ø±êÖµ
*           with    Ïß¿í(0-50)
*           color	ÏÔÊ¾ÑÕÉ«
* ³ö¿Ú²ÎÊı£ºÎŞ
* ËµÃ÷£º²Ù×÷Ê§°ÜÔ­ÒòÊÇÖ¸¶¨µØÖ·³¬³öÓĞĞ§·¶Î§¡£
****************************************************************************/
void  GUI_LineWith(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint8 with, TCOLOR color)
{  int32   dx;						// Ö±ÏßxÖá²îÖµ±äÁ¿
   int32   dy;          			// Ö±ÏßyÖá²îÖµ±äÁ¿
   int8    dx_sym;					// xÖáÔö³¤·½Ïò£¬Îª-1Ê±¼õÖµ·½Ïò£¬Îª1Ê±ÔöÖµ·½Ïò
   int8    dy_sym;					// yÖáÔö³¤·½Ïò£¬Îª-1Ê±¼õÖµ·½Ïò£¬Îª1Ê±ÔöÖµ·½Ïò
   int32   dx_x2;					// dx*2Öµ±äÁ¿£¬ÓÃÓÚ¼Ó¿ìÔËËãËÙ¶È
   int32   dy_x2;					// dy*2Öµ±äÁ¿£¬ÓÃÓÚ¼Ó¿ìÔËËãËÙ¶È
   int32   di;						// ¾ö²ß±äÁ¿
   
   int32   wx, wy;					// Ïß¿í±äÁ¿
   int32   draw_a, draw_b;
   
   /* ²ÎÊı¹ıÂË */
   if(with==0) return;
   if(with>50) with = 50;
   
   dx = x1-x0;						// ÇóÈ¡Á½µãÖ®¼äµÄ²îÖµ
   dy = y1-y0;
   
   wx = with/2;
   wy = with-wx-1;
   
   /* ÅĞ¶ÏÔö³¤·½Ïò£¬»òÊÇ·ñÎªË®Æ½Ïß¡¢´¹Ö±Ïß¡¢µã */
   if(dx>0)							// ÅĞ¶ÏxÖá·½Ïò
   {  dx_sym = 1;					// dx>0£¬ÉèÖÃdx_sym=1
   }
   else
   {  if(dx<0)
      {  dx_sym = -1;				// dx<0£¬ÉèÖÃdx_sym=-1
      }
      else
      {  /* dx==0£¬»­´¹Ö±Ïß£¬»òÒ»µã */
         wx = x0-wx;
         if(wx<0) wx = 0;
         wy = x0+wy;
         
         while(1)
         {  x0 = wx;
            GUI_RLine(x0, y0, y1, color);
            if(wx>=wy) break;
            wx++;
         }
         
      	 return;
      }
   }
   
   if(dy>0)							// ÅĞ¶ÏyÖá·½Ïò
   {  dy_sym = 1;					// dy>0£¬ÉèÖÃdy_sym=1
   }
   else
   {  if(dy<0)
      {  dy_sym = -1;				// dy<0£¬ÉèÖÃdy_sym=-1
      }
      else
      {  /* dy==0£¬»­Ë®Æ½Ïß£¬»òÒ»µã */
         wx = y0-wx;
         if(wx<0) wx = 0;
         wy = y0+wy;
         
         while(1)
         {  y0 = wx;
            GUI_HLine(x0, y0, x1, color);
            if(wx>=wy) break;
            wx++;
         }
      	 return;
      }
   }
    
   /* ½«dx¡¢dyÈ¡¾ø¶ÔÖµ */
   dx = dx_sym * dx;
   dy = dy_sym * dy;
 
   /* ¼ÆËã2±¶µÄdx¼°dyÖµ */
   dx_x2 = dx*2;
   dy_x2 = dy*2;
   
   /* Ê¹ÓÃBresenham·¨½øĞĞ»­Ö±Ïß */
   if(dx>=dy)						// ¶ÔÓÚdx>=dy£¬ÔòÊ¹ÓÃxÖáÎª»ù×¼
   {  di = dy_x2 - dx;
      while(x0!=x1)
      {  /* xÖáÏòÔö³¤£¬Ôò¿í¶ÈÔÚy·½Ïò£¬¼´»­´¹Ö±Ïß */
         draw_a = y0-wx;
         if(draw_a<0) draw_a = 0;
         draw_b = y0+wy;
         GUI_RLine(x0, draw_a, draw_b, color);
         
         x0 += dx_sym;				
         if(di<0)
         {  di += dy_x2;			// ¼ÆËã³öÏÂÒ»²½µÄ¾ö²ßÖµ
         }
         else
         {  di += dy_x2 - dx_x2;
            y0 += dy_sym;
         }
      }
      draw_a = y0-wx;
      if(draw_a<0) draw_a = 0;
      draw_b = y0+wy;
      GUI_RLine(x0, draw_a, draw_b, color);
   }
   else								// ¶ÔÓÚdx<dy£¬ÔòÊ¹ÓÃyÖáÎª»ù×¼
   {  di = dx_x2 - dy;
      while(y0!=y1)
      {  /* yÖáÏòÔö³¤£¬Ôò¿í¶ÈÔÚx·½Ïò£¬¼´»­Ë®Æ½Ïß */
         draw_a = x0-wx;
         if(draw_a<0) draw_a = 0;
         draw_b = x0+wy;
         GUI_HLine(draw_a, y0, draw_b, color);
         
         y0 += dy_sym;
         if(di<0)
         {  di += dx_x2;
         }
         else
         {  di += dx_x2 - dy_x2;
            x0 += dx_sym;
         }
      }
      draw_a = x0-wx;
      if(draw_a<0) draw_a = 0;
      draw_b = x0+wy;
      GUI_HLine(draw_a, y0, draw_b, color);
   } 
  
}





