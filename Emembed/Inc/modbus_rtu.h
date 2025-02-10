#ifndef __MODBUS_RTU_H_
#define __MODBUS_RTU_H_

#include "usart.h"
#include "main.h"
#include "PWM_CRL.h"
#include "communication.h"
#include "delay.h"

typedef struct  
{
    uint8_t modbus_send_buf[48];
    uint8_t modbus_rcv_buf[48];

    uint8_t NTC1_current_value;     //30001
    uint8_t NTC2_current_value; 
    uint8_t NTC3_current_value;     //30002
    uint8_t NTC4_current_value;    
    uint8_t IR1_adc_value;          //30003
    uint8_t IR2_adc_value;          
    uint8_t I_out1_value;           //30004
    uint8_t I_out2_value;           //30004
    uint8_t I_out3_value;           //30005

    uint8_t PWM_info;               //40001
    uint8_t LED_info;               //40002
    uint8_t AC220_info;             //40003
    uint8_t NTC1_alarm_value;       //40004
    uint8_t NTC2_alarm_value;       //40004
    uint8_t NTC3_alarm_value;       //40005
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

void slave_statu_query_modify( uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_val );
void test_hanshu(void);
void Modbus_fun03_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun04_Master( uint16_t reg_addr,uint16_t reg_num );
void Modbus_fun06_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val );
void Modbus_fun16_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val );

#endif
