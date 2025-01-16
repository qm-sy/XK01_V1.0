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
		modbus.rcbuf[modbus.recount++] = (uint8_t)(huart2.Instance->DR & 0x00FF);
		modbus.timout = 0;
		if( modbus.recount == 1 )
		{
			modbus.timrun = 1;
		}

		HAL_UART_Receive_IT(&huart2,&modbus.rcbuf[modbus.recount],1);
    }
}


/**
 * @brief	发送1byte数据
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

    if( modbus.reflag == 0 )
    {
        return ;
    }

    crc = MODBUS_CRC16(modbus.rcbuf, modbus.recount-2);
    rccrc = (modbus.rcbuf[modbus.recount-2]<<8) | (modbus.rcbuf[modbus.recount-1]);

    if ( crc == rccrc)
    {
        if( modbus.myaddr == modbus.rcbuf[0] )
        {
            printf("GOOD \r\n");
            switch ( modbus.rcbuf[1] )
            {
				case 0:							break;            
                case 1:		Modbus_Fun1();		break;
				case 2:							break;            
                case 3:		Modbus_Fun3();		break;
				case 4:							break;            
                case 5:		Modbus_Fun5();		break;
				case 15:	Modbus_Fun15();		break;            
                case 16:	Modbus_Fun16();		break;  

				default:						break;
            }
        }
    }
    modbus.recount = 0;
    modbus.reflag = 0;	//接收标志清零
}

void Modbus_Fun1()
{
    uint16_t crc;
	
	//发送回应数据包
	modbus.sendbuf[0] = modbus.myaddr;     
	modbus.sendbuf[1] = 0x01;  //功能码  01          
    modbus.sendbuf[2] = 0x01; //1个字节数
    modbus.sendbuf[3] = 0xFF; //

	crc = MODBUS_CRC16(modbus.sendbuf,4);    
    
	modbus.sendbuf[4] = crc >> 8;
	modbus.sendbuf[5] = crc & 0x00FF;
	//数据包打包完成
	RS485_TX;

    modbus_send_data(modbus.sendbuf,5);	

	RS485_RX;
}

void Modbus_Fun3()
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}

void Modbus_Fun5()
{
	
}

void Modbus_Fun15()
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
