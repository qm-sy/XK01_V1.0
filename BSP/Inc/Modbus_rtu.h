#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"


typedef struct  
{
	//作为从机时使用
    uint8_t  myaddr;         //本设备从机地址
    uint8_t  timout;       //modbus数据持续时间
    uint8_t  recount;      //modbus端口接收到的数据个数
    uint8_t  timrun;       //modbus定时器是否计时标志
    uint8_t  reflag;       //modbus一帧数据接受完成标志位
    uint8_t  rcbuf[100];   //modbus接收缓冲区
    uint8_t  sendbuf[100]; //modbus接发送缓冲区
}MODBUS ;

extern MODBUS modbus;
void Modbus_Fun1( void );

void Modbus_Fun3( void );

void Modbus_Fun5( void );

void Modbus_Fun15( void );

void Modbus_Fun16( void );

void Modbus_Event( void );

void modbus_send_data( uint8_t *buf , uint8_t len );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);
#endif
