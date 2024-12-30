#ifndef __ST7789V2_H_
#define __ST7789V2_H_

#include "sys.h"	 
#include "gpio.h"
#include "main.h"

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;		//LCD 高度
	uint16_t id;			//LCD ID
	uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t wramcmd;	//开始写gram指令
	uint16_t setxcmd;		//设置x坐标指令
	uint16_t setycmd;		//设置y坐标指令	 

}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
/////////////////////////////////////用户配置区///////////////////////////////////	 
#define USE_HORIZONTAL 0//定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 240
#define LCD_H 320

//TFTLCD部分外要调用的函数		   
extern uint16_t  POINT_COLOR;//默认红色    
extern uint16_t  BACK_COLOR; //背景颜色.默认为白色


//-----------------LCD端口移植---------------- 

// //SCK
// #define TFT_SCK_GPIO_Port    GPIOA
// #define TFT_SCK_Pin    GPIO_Pin_6

// //MOSI
// #define TFT_SCK_GPIO_Port    GPIOA
// #define TFTSDI_Pin    GPIO_Pin_7

// //CS
// #define TFT_CS_GPIO_Port     GPIOB
// #define TFT_CS_Pin     GPIO_Pin_10

// //DC
// #define TFT_DC_GPIO_Port     GPIOB
// #define TFT_DC_Pin     GPIO_Pin_0

// //RES
// #define TFT_RESET_GPIO_Port    GPIOB
// #define TFT_RESET_Pin    GPIO_Pin_1


////////////////////////////////////////////////////////////////////
//-----------------LCD端口定义---------------- 
//液晶控制口置1操作语句宏定义
#define        LCD_CS_SET          HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET)
#define        LCD_RS_SET          HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET) 
#define        LCD_SDA_SET         HAL_GPIO_WritePin(TFT_SDI_GPIO_Port, TFT_SDI_Pin, GPIO_PIN_SET)
#define        LCD_SCL_SET         HAL_GPIO_WritePin(TFT_SCK_GPIO_Port, TFT_SCK_Pin, GPIO_PIN_SET)
#define        LCD_RST_SET         HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_SET)

//液晶控制口置0操作语句宏定义
#define        LCD_CS_CLR          HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET)//CS 
#define        LCD_RS_CLR          HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_RESET)//DC
#define        LCD_SDA_CLR         HAL_GPIO_WritePin(TFT_SDI_GPIO_Port, TFT_SDI_Pin, GPIO_PIN_RESET)//SDA=MOSI    
#define        LCD_SCL_CLR         HAL_GPIO_WritePin(TFT_SCK_GPIO_Port, TFT_SCK_Pin, GPIO_PIN_RESET)//SCL=SCLK
#define        LCD_RST_CLR         HAL_GPIO_WritePin(TFT_RESET_GPIO_Port, TFT_RESET_Pin, GPIO_PIN_RESET)//RES

//画笔颜色
#define WHITE			0xFFFF
#define BLACK			0x0000	  
#define BLUE			0x001F  
#define BRED			0XF81F
#define GRED			0XFFE0
#define GBLUE			0X07FF
#define RED				0xF800
#define MAGENTA			0xF81F
#define GREEN			0x07E0
#define CYAN			0x7FFF
#define YELLOW			0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色

#define DARKBLUE		0X01CF	//深蓝色
#define LIGHTBLUE		0X7D7C	//浅蓝色  
#define GRAYBLUE		0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LIGHTGRAY       0XEF5B //浅灰色(PANNEL)
#define LGRAY			0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);	 
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);

uint16_t LCD_RD_DATA(void);//读取LCD数据									    
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t LCD_ReadRAM(void);		   
uint16_t LCD_BGR2RGB(uint16_t c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction );
void SPIv_WriteData(uint8_t Data);
//如果仍然觉得速度不够快，可以使用下面的宏定义,提高速度.
//注意要去掉lcd.c中void LCD_WR_DATA(uint16_t data)函数定义哦
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
