#include "Modbus_RTU.h"

MODBUS modbus;

uint16_t USART_RX_STA = 0;

/**
 * @brief	uart2回调函数 用于接收数据
 * 
 * @param   huart：标识触发回调函数的UART外设                     
 * 
  @return  void
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( huart->Instance == USART2)
    {
        if ( modbus.reflag == 1 )
        {
            return;
        }
		modbus.rcvbuf[modbus.recount++] = (uint8_t)(huart2.Instance->DR & 0x00FF);
		modbus.timout = 0;
		if( modbus.recount == 1 )
		{
			modbus.timrun = 1;
		}

		HAL_UART_Receive_IT(&huart2,&modbus.rcvbuf[modbus.recount],1);
    }
}


/**
 * @brief	发送1帧数据
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void modbus_send_data( uint8_t *buf , uint8_t len )
{
    HAL_UART_Transmit(&huart2,(uint8_t*)buf,len,1000);
    
    while (__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC) != SET);
}

uint8_t modbus_wait_receive( void )
{
    uint8_t wait_time_cnt = 100;

    while(~modbus.reflag)
    {
        wait_time_cnt--;
        HAL_Delay(1);
    }
    if( wait_time_cnt == 0 )
    {
        printf("receive error \r\n");
        return 0;
    }else
    {
        modbus.reflag = 0;	
    }
    return 1;
}
/**
 * @brief	modbus接收函数，接收并判断Function后转到相应Function函数进行处理
 * 
 * @param   buf：待发送数组首地址           
 * @param   len：数组长度           
 * 
  @return  void
 */
void Modbus_Event( void )
{
    uint16_t crc,rccrc;

    /*1.接收完毕                                           */
    if( modbus.reflag == 1 )
    {
        /*2.清空接收完毕标志位                              */
        modbus.reflag = 0;	
        
        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(modbus.rcvbuf, modbus.recount-2);
        rccrc = (modbus.rcvbuf[modbus.recount-2]<<8) | (modbus.rcvbuf[modbus.recount-1]);

        /*4.清空接收计数                                    */
        modbus.recount = 0;
        if ( crc == rccrc)
        {
            if( modbus.multifunpower == modbus.rcvbuf[0] )
            {
                switch ( modbus.rcvbuf[1] )
                {         
                    case 0x03:		Modbus_Fun3();		break;
                    case 0x04:		Modbus_Fun4();      break;            
                    case 0x06:		Modbus_Fun6();		break;        
                    case 0x10:	    Modbus_Fun16();		break;  

                    default:						    break;
                }
            }
        }
    }
}

void Modbus_Fun3()
{
	slave_pwm.fan_info = modbus.rcvbuf[4];     
    if((slave_pwm.fan_info & 0xFE) & 0X01)
    {
        slave_pwm.fan_pwm7_statu = 1;
    }else
    {
        slave_pwm.fan_pwm7_statu = 0;
    }

    if((slave_pwm.fan_info & 0xFD) & 0X02)
    {
        slave_pwm.fan_pwm8_statu = 1;
    }else
    {
        slave_pwm.fan_pwm8_statu = 0;
    }

    slave_pwm.fan_pwm7_level = ((slave_pwm.fan_info>>2) & 0x07) / 184;
    slave_pwm.fan_pwm8_level = (slave_pwm.fan_info>>5) / 184;
}

void Modbus_Fun4()
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void Modbus_Fun6()
{
	
}

void Modbus_Fun16()
{
	
}

/**
 * @brief	crc校验函数
 * 
 * @param   buf：  Address(1 byte) +Funtion(1 byte) ）+Data(n byte)   
 * @param   length:数据长度           
 * 
  @return  crc16:crc校验的值 2byte
 */
//buf内的值为 
uint16_t MODBUS_CRC16(uint8_t *buf, uint8_t length)
{
	uint8_t	i;
	uint16_t	crc16;

    /* 1, 预置16位CRC寄存器为0xffff（即全为1）                          */
	crc16 = 0xffff;	

	do
	{
        /* 2, 把8位数据与16位CRC寄存器的低位相异或，把结果放于CRC寄存器     */        
		crc16 ^= (uint16_t)*buf;		//
		for(i=0; i<8; i++)		
		{
            /* 3, 如果最低位为1，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位 再异或0xA001    */
			if(crc16 & 1)
            {
                crc16 = (crc16 >> 1) ^ 0xA001;
            }
            /* 4, 如果最低位为0，把CRC寄存器的内容右移一位(朝低位)，用0填补最高位                */
            else
            {
                crc16 >>= 1;
            }		
		}
		buf++;
	}while(--length != 0);

	return	(crc16);
}
