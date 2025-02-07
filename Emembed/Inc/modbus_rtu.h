#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"

typedef struct  
{
    uint8_t *modbus_send_buf;
    uint8_t *modbus_rcv_buf;

    uint16_t NTC1_current_value;   //30001
    uint16_t NTC2_current_value; 
    uint16_t NTC3_current_value;    //30002
    uint16_t NTC4_current_value;    //30002
    uint16_t IR1_adc_value;          //30003
    uint16_t IR2_adc_value;          //30003
    uint16_t I_out12_value;         //30004
    uint16_t I_out3_value;          //30005

    uint16_t PWM_info;              //40001
    uint16_t LED_info;              //40002
    uint16_t AC220_info;            //40003
    uint16_t NTC12_alarm_value;     //40004
    uint16_t NTC3_alarm_value;      //40005
}MODBUS_INFO;

extern MODBUS_INFO modbus;

void Modbus_Fun3( void );

void Modbus_Fun4( void );

void Modbus_Fun6( void );

void Modbus_Fun16( void );

void Modbus_Event( void );

void modbus_send_data( uint8_t *buf , uint8_t len );

uint8_t modbus_wait_receive( void );

uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length);

void slave_statu_query_modify( uint8_t code,uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_data);

#endif
