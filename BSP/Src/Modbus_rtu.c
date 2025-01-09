#include "Modbus_rtu.h"

uint16_t USART_RX_STA = 0;

void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart2,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) != SET);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( huart->Instance == USART2)
    {
        if (USART_RX_STA)
        {
            /* code */
        }
        
    }
}