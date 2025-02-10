#ifndef __KEY_CRL_H_
#define __KEY_CRL_H_

#include "main.h"
#include "delay.h"

extern volatile uint8_t key_value_flag;

#define B0_VAL HAL_GPIO_ReadPin(D0_B0_GPIO_Port,D0_B0_Pin)
#define B1_VAL HAL_GPIO_ReadPin(D1_B1_GPIO_Port,D1_B1_Pin)
#define B2_VAL HAL_GPIO_ReadPin(D2_B2_GPIO_Port,D2_B2_Pin)
#define B3_VAL HAL_GPIO_ReadPin(D3_B3_GPIO_Port,D3_B3_Pin)

void key_scan( void );

void key1_press( uint8_t key_value );

void key2_press( uint8_t key_value );

void key3_press( uint8_t key_value );

void key4_press( uint8_t key_value );


#endif
