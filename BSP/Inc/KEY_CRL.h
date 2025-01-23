#ifndef __KEY_CRL_H_
#define __KEY_CRL_H_

#include "main.h"

extern volatile uint8_t key_value;

#define B0_VAL HAL_GPIO_ReadPin(D0_B0_GPIO_Port,D0_B0_Pin)
#define B1_VAL HAL_GPIO_ReadPin(D1_B1_GPIO_Port,D1_B1_Pin)
#define B2_VAL HAL_GPIO_ReadPin(D2_B2_GPIO_Port,D2_B2_Pin)
#define B3_VAL HAL_GPIO_ReadPin(D3_B3_GPIO_Port,D3_B3_Pin)

#endif
