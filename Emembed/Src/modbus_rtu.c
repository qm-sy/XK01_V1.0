#include "modbus_rtu.h"

MODBUS_INFO modbus;

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

    while(~rs485.reflag)
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
        rs485.reflag = 0;	
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
    // uint16_t crc,rccrc;

    // /*1.接收完毕                                           */
    // if( rs485.reflag == 1 )
    // {
    //     /*2.清空接收完毕标志位                              */
    //     rs485.reflag = 0;	
        
    //     /*3.CRC校验                                         */
    //     crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
    //     rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);

    //     /*4.清空接收计数                                    */
    //     rs485.recount = 0;
    //     if ( crc == rccrc)
    //     {
    //         if( rs485.multifunpower == rs485.rcvbuf[0] )
    //         {
    //             switch ( rs485.rcvbuf[1] )
    //             {         
    //                 case 0x03:		Modbus_Fun3();		break;
    //                 case 0x04:		Modbus_Fun4();      break;            
    //                 case 0x06:		Modbus_Fun6();		break;        
    //                 case 0x10:	    Modbus_Fun16();		break;  

    //                 default:						    break;
    //             }
    //         }
    //     }
    // }
}

void Modbus_Fun3()
{
	slave_pwm.fan_info = rs485.rcvbuf[4];     
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


void slave_statu_query_modify( uint8_t code,uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_data)
{
    uint16_t rccrc,crc,err;
    uint8_t crc_buf_03[6];
    uint8_t start_addr_03 = 3;

    switch(fun)
    {
        case 0x03:
            modbus.modbus_send_buf[0] = SLAVE_ADDR;
            modbus.modbus_send_buf[1] = 0x03;
            modbus.modbus_send_buf[2] = reg_addr >> 8;
            modbus.modbus_send_buf[3] = reg_addr;
            modbus.modbus_send_buf[4] = reg_num >> 8;
            modbus.modbus_send_buf[5] = reg_num;
            crc = MODBUS_CRC16(modbus.modbus_send_buf,6);
            modbus.modbus_send_buf[6] = crc >> 8;
            modbus.modbus_send_buf[7] = crc;

            modbus_send_data(modbus.modbus_send_buf,8);
            err = modbus_wait_receive();
            if( err == 1)
            {
                rccrc = MODBUS_CRC16(rs485.rcvbuf,3 + reg_num * 2);
                if(rccrc == (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]))
                {
                    for( uint16_t i = reg_addr; i < reg_addr + reg_num; i++)
                    {
                        switch (i)
                        {
                        case 0:
                            modbus.NTC1_current_value = rs485.rcvbuf[start_addr_03+1];
                            modbus.NTC2_current_value = rs485.rcvbuf[start_addr_03];
                            break;
                        case 1:
                            modbus.NTC3_current_value = rs485.rcvbuf[start_addr_03+1];
                            modbus.NTC4_current_value = rs485.rcvbuf[start_addr_03];
                            break;
                        case 2:
                            /* code */
                            break;    
                        case 3:
                            /* code */
                            break;
                        case 4:
                            /* code */
                            break;                                                    
                        default:
                            break;
                        }
                        start_addr_03 += 2;
                    }
                }
            }
        case 0x04:
            break;
        case 0x06:
            break;    
        case 0x0a:
            break;
        default:
            break;
    }
}

void slave_fan_statu_query(void)
{
    slave_pwm.fan_info = rs485.rcvbuf[4];

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

    slave_pwm.fan_pwm7_level = slave_pwm.fan_info>>2 & 0x07;
    slave_pwm.fan_pwm8_level = slave_pwm.fan_info>>5;
}

// void slave_fan_statu_modify( void )
// {
//     uint16_t crc;
//     uint8_t crc_buf[6] = {0x35,0x06,0x00,0x00,0X00,0x00};

//     slave_pwm.fan_info = slave_pwm.fan_pwm7_level<<2                \
//                        | slave_pwm.fan_pwm8_level<<5                \
//                        | slave_pwm.fan_pwm7_statu                   \
//                        | slave_pwm.fan_pwm8_statu;                  

     
//     crc = MODBUS_CRC16(crc_buf, 6);  

//     slave_pwm.rs485[6] = crc>>8;
//     slave_pwm.rs485[7]  = crc;       

//     modbus_send_data(slave_pwm.modbus_06,8);
// }

