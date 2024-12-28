#ifndef __ST7789V2_H_
#define __ST7789V2_H_

#include "sys.h"	 
#include "gpio.h"
#include "stm32f10x.h"
#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				  //LCD ID
	u8  dir;			  //横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL  	 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

//TFTLCD部分外要调用的函数		   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色


//-----------------LCD端口移植---------------- 

#define RCC_LCD1    RCC_APB2Periph_GPIOA 
#define RCC_LCD2    RCC_APB2Periph_GPIOB 

//SCK
#define PORT_LCD_SCL    GPIOA
#define GPIO_LCD_SCL    GPIO_Pin_6

//MOSI
#define PORT_LCD_SDA    GPIOA
#define GPIO_LCD_SDA    GPIO_Pin_7

////MIS0
//#define PORT_LCD_MIS    GPIOB
//#define GPIO_LCD_MIS    GPIO_Pin_14

//NSS
#define PORT_LCD_CS     GPIOB
#define GPIO_LCD_CS     GPIO_Pin_10

//DC
#define PORT_LCD_DC     GPIOB
#define GPIO_LCD_DC     GPIO_Pin_0

//RES
#define PORT_LCD_RES    GPIOB
#define GPIO_LCD_RES    GPIO_Pin_1

//BLK/LED
#define PORT_LCD_BLK    GPIOA
#define GPIO_LCD_BLK    GPIO_Pin_5

//CS2/T_CS
#define PORT_LCD_CS2     GPIOA
#define GPIO_LCD_CS2     GPIO_Pin_3

 //PEN/T_IRQ
#define PORT_LCD_PEN     GPIOA
#define GPIO_LCD_PEN     GPIO_Pin_0

//T_CLK
#define PORT_LCD_TCLK     GPIOA
#define GPIO_LCD_TCLK     GPIO_Pin_4

//T_DIN
#define PORT_LCD_TDIN     GPIOA
#define GPIO_LCD_TDIN     GPIO_Pin_2

//T_DO
#define PORT_LCD_TD0      GPIOA
#define GPIO_LCD_TD0      GPIO_Pin_1



////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//液晶控制口置1操作语句宏定义
#define        LCD_CS_SET          GPIO_WriteBit(PORT_LCD_CS, GPIO_LCD_CS, Bit_SET)
#define        LCD_RS_SET          GPIO_WriteBit(PORT_LCD_DC, GPIO_LCD_DC, Bit_SET) 
#define        LCD_SDA_SET         GPIO_WriteBit(PORT_LCD_SDA, GPIO_LCD_SDA, Bit_SET)
#define        LCD_SCL_SET         GPIO_WriteBit(PORT_LCD_SCL, GPIO_LCD_SCL, Bit_SET)
#define        LCD_RST_SET         GPIO_WriteBit(PORT_LCD_RES, GPIO_LCD_RES, Bit_SET)
#define        LCD_LED_SET         GPIO_WriteBit(PORT_LCD_BLK, GPIO_LCD_BLK, Bit_SET)

//液晶控制口置0操作语句宏定义
#define        LCD_CS_CLR          GPIO_WriteBit(PORT_LCD_CS, GPIO_LCD_CS, Bit_RESET)//CS 
#define        LCD_RS_CLR          GPIO_WriteBit(PORT_LCD_DC, GPIO_LCD_DC, Bit_RESET)//DC
#define        LCD_SDA_CLR         GPIO_WriteBit(PORT_LCD_SDA, GPIO_LCD_SDA, Bit_RESET)//SDA=MOSI    
#define        LCD_SCL_CLR         GPIO_WriteBit(PORT_LCD_SCL, GPIO_LCD_SCL, Bit_RESET)//SCL=SCLK
#define        LCD_RST_CLR         GPIO_WriteBit(PORT_LCD_RES, GPIO_LCD_RES, Bit_RESET)//RES
#define        LCD_LED_CLR         GPIO_WriteBit(PORT_LCD_BLK, GPIO_LCD_BLK, Bit_RESET)






//画笔颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY     0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(u16 Color);	 
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void LCD_DrawPoint(u16 x,u16 y);//画点
u16  LCD_ReadPoint(u16 x,u16 y); //读点
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);

u16 LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
void LCD_WR_DATA(u8 data);
u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
u16 LCD_ReadRAM(void);		   
u16 LCD_BGR2RGB(u16 c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(u16 Data);
void LCD_direction(u8 direction );

//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(u16 data)函数定义哦
/*
#if LCD_USE8BIT_MODEL==1//使用8位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	DATAOUT(data<<8);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	}
	#else//使用16位并行数据总线模式
	#define LCD_WR_DATA(data){\
	LCD_RS_SET;\
	LCD_CS_CLR;\
	DATAOUT(data);\
	LCD_WR_CLR;\
	LCD_WR_SET;\
	LCD_CS_SET;\
	} 	
#endif
*/


#endif