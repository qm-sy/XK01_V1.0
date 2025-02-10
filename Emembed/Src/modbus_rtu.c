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
    uint16_t wait_time_cnt = 300;

    while((rs485.reflag!=1)&&(wait_time_cnt!=0))
    {
        wait_time_cnt--;
        delay_ms(1);
    }

    if( wait_time_cnt == 0 )
    {
        printf("receive error \r\n");
        return 0;
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
    if( rs485.reflag == 1 )
    {
        //printf("here \r\n");
        /*2.清空接收完毕标志位                              */
        rs485.reflag = 0;	
        
        /*3.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);

        /*4.清空接收计数                                    */
        rs485.recount = 0;
        if ( crc == rccrc)
        {
            if( rs485.rcvbuf[0] == SLAVE_ADDR )
            {
                switch ( rs485.rcvbuf[1] )
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
    uint8_t send_buf[5] = {0x00,0x01,0x02,0x03,0x04};
    //printf("success");
    HAL_UART_Transmit(&huart2,send_buf,5,1000);
	//printf("success");
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


void slave_statu_query_modify( uint8_t fun, uint16_t reg_addr,uint16_t reg_num,uint16_t reg_val )
{
    
    switch(fun)
    {
        case 0x03:  Modbus_fun03_Master(reg_addr, reg_num);             break;

        case 0x04:  Modbus_fun04_Master(reg_addr, reg_num);             break;

        case 0x06:  Modbus_fun06_Master(reg_addr, reg_num, reg_val);  break;

        case 0x0a:  Modbus_fun16_Master(reg_addr, reg_num, reg_val);  break;
    }        
}
void Modbus_fun03_Master( uint16_t reg_addr,uint16_t reg_num )
{
    uint16_t rccrc,crc;
    uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

    /*1. 主机 发送一帧查询报文后等待回复                    */
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
    modbus_wait_receive();

    /*2. 收到回复后校验crc                                  */
    if(rs485.reflag == 1)
    {
        rccrc = MODBUS_CRC16(rs485.rcvbuf,3 + reg_num * 2);
        if(rccrc == ((rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1])))
        {
            /*3. crc校验通过，对信息帧内数据进行处理        */
            for( uint16_t i = reg_addr; i < reg_addr + reg_num; i++)
            {
                switch (i)
                {
                case 0:
                    modbus.PWM_info = rs485.rcvbuf[start_addr_03 + 1];                       
                    printf("value1 is 0x%02x\r\n",modbus.PWM_info);
                    break;

                case 1:
                    modbus.LED_info = rs485.rcvbuf[start_addr_03 + 1];
                    printf("value3 is 0x%02x\r\n",modbus.LED_info);
                    break;

                case 2:
                    modbus.AC220_info = rs485.rcvbuf[start_addr_03 + 1];                            
                    printf("value5 is 0x%02x\r\n",modbus.AC220_info);                            
                    break; 

                case 3:
                    modbus.NTC1_alarm_value = rs485.rcvbuf[start_addr_03 + 1];
                    modbus.NTC2_alarm_value = rs485.rcvbuf[start_addr_03];
                    printf("value7 is 0x%02x\r\n",modbus.NTC1_alarm_value);
                    printf("value8 is 0x%02x\r\n",modbus.NTC2_alarm_value);
                    break;

                case 4:
                    modbus.NTC3_alarm_value = rs485.rcvbuf[start_addr_03 + 1];
                    printf("value9 is 0x%02x\r\n",modbus.NTC3_alarm_value);
                    break;   

                default:
                    break;
                }
                start_addr_03 += 2;
            }
        }
        /*4. RX允许继续接收，开启超时接收计时               */
        rs485.reflag = 0;
        rs485.recount = 0;
    }
}

void Modbus_fun04_Master( uint16_t reg_addr,uint16_t reg_num )
{
    uint16_t rccrc,crc;
    uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

    /*1. 主机 发送一帧查询报文后等待回复                    */
    modbus.modbus_send_buf[0] = SLAVE_ADDR;
    modbus.modbus_send_buf[1] = 0x04;
    modbus.modbus_send_buf[2] = reg_addr >> 8;
    modbus.modbus_send_buf[3] = reg_addr;
    modbus.modbus_send_buf[4] = reg_num >> 8;
    modbus.modbus_send_buf[5] = reg_num;
    crc = MODBUS_CRC16(modbus.modbus_send_buf,6);
    modbus.modbus_send_buf[6] = crc >> 8;
    modbus.modbus_send_buf[7] = crc;
    
    modbus_send_data(modbus.modbus_send_buf,8);
    modbus_wait_receive();

    /*2. 收到回复后校验crc                                  */
    if(rs485.reflag == 1)
    {
        rccrc = MODBUS_CRC16(rs485.rcvbuf,3 + reg_num * 2);
        if(rccrc == ((rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1])))
        {
            /*3. crc校验通过，对信息帧内数据进行处理        */
            for( uint16_t i = reg_addr; i < reg_addr + reg_num; i++)
            {
                switch (i)
                {
                    case 0:
                        modbus.NTC1_current_value = rs485.rcvbuf[start_addr_03 + 1];
                        modbus.NTC2_current_value = rs485.rcvbuf[start_addr_03];
                        printf("value1 is 0x%02x\r\n",modbus.NTC1_current_value);
                        printf("value2 is 0x%02x\r\n",modbus.NTC2_current_value);
                        break;

                    case 1:
                        modbus.NTC3_current_value = rs485.rcvbuf[start_addr_03 + 1];
                        modbus.NTC4_current_value = rs485.rcvbuf[start_addr_03];
                        printf("value3 is 0x%02x\r\n",modbus.NTC3_current_value);
                        printf("value4 is 0x%02x\r\n",modbus.NTC4_current_value);
                        break;

                    case 2:
                        modbus.IR1_adc_value = rs485.rcvbuf[start_addr_03 + 1];
                        modbus.IR2_adc_value = rs485.rcvbuf[start_addr_03];
                        printf("value5 is 0x%02x\r\n",modbus.IR1_adc_value);
                        printf("value6 is 0x%02x\r\n",modbus.IR2_adc_value);
                        break;    

                    case 3:
                        modbus.I_out1_value = rs485.rcvbuf[start_addr_03 + 1];
                        modbus.I_out2_value = rs485.rcvbuf[start_addr_03];
                        printf("value7 is 0x%02x\r\n",modbus.I_out1_value);
                        printf("value8 is 0x%02x\r\n",modbus.I_out2_value);
                        break;

                    case 4:
                        modbus.I_out3_value = rs485.rcvbuf[start_addr_03 + 1];
                        printf("value9 is 0x%02x\r\n",modbus.I_out3_value);
                        break;  

                    default:
                        break;
                }
                start_addr_03 += 2;
            }
        }
        /*4. RX允许继续接收，开启超时接收计时               */
        rs485.reflag = 0;
        rs485.recount = 0;
    }
}

void Modbus_fun06_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val )
{
    uint16_t rccrc,crc;

    /*1. 主机 发送一帧查询报文后等待回复                    */
    modbus.modbus_send_buf[0] = SLAVE_ADDR;
    modbus.modbus_send_buf[1] = 0x06;
    modbus.modbus_send_buf[2] = reg_addr >> 8;
    modbus.modbus_send_buf[3] = reg_addr;
    switch (reg_addr)
    {
        case 0:
            modbus.PWM_info = reg_val;
            modbus.modbus_send_buf[4] = 0x00;
            modbus.modbus_send_buf[5] = modbus.PWM_info;
            break;

        case 1:
            modbus.LED_info = reg_val;
            modbus.modbus_send_buf[4] = 0x00;
            modbus.modbus_send_buf[5] = modbus.LED_info;
            break;

        case 2:
            modbus.AC220_info = reg_val;
            modbus.modbus_send_buf[4] = 0x00;
            modbus.modbus_send_buf[5] = modbus.AC220_info;
            break;
            
        case 3:
            modbus.NTC1_alarm_value = reg_val;
            modbus.NTC2_alarm_value = reg_val>>8;
            modbus.modbus_send_buf[4] = modbus.NTC2_alarm_value;
            modbus.modbus_send_buf[5] = modbus.NTC1_alarm_value;
            break;

        case 4:
            modbus.NTC3_alarm_value = reg_val;
            modbus.modbus_send_buf[4] = 0x00;
            modbus.modbus_send_buf[5] = modbus.NTC3_alarm_value;
            break;

        default:
            break;
    }

    crc = MODBUS_CRC16(modbus.modbus_send_buf,6);
    modbus.modbus_send_buf[6] = crc >> 8;
    modbus.modbus_send_buf[7] = crc;
    
    modbus_send_data(modbus.modbus_send_buf,8);
    modbus_wait_receive();

    /*2. 收到回复后校验crc                                  */
    if(rs485.reflag == 1)
    {
        rccrc = MODBUS_CRC16(rs485.rcvbuf,3 + reg_num * 2);
        if(rccrc == ((rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1])))
        {
            /*3. crc校验通过，对信息帧内数据进行处理        */
            printf("reply success \r\n");
        }    
        /*4. RX允许继续接收，开启超时接收计时               */
        rs485.reflag = 0;
        rs485.recount = 0;
    }
}

void Modbus_fun16_Master( uint16_t reg_addr,uint16_t reg_num, uint16_t reg_val )
{

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

void test_hanshu(void)
{
    uint8_t send_buf[5] = {0x00,0x01,0x02,0x03,0x04};
    //printf("success");
    HAL_UART_Transmit(&huart2,send_buf,5,1000);
    if(rs485.reflag == 1)
    {
        rs485.reflag = 0;
        
        printf("hello \r\n");
    }
}
