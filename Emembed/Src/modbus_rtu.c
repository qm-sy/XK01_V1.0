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
        /*2.CRC校验                                         */
        crc = MODBUS_CRC16(rs485.rcvbuf, rs485.recount-2);
        rccrc = (rs485.rcvbuf[rs485.recount-2]<<8) | (rs485.rcvbuf[rs485.recount-1]);

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
    uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

    for( uint16_t i = 0; i < 5; i++)
    {
        switch (i)
        {
        case 0:
            modbus.PWM_info = rs485.rcvbuf[start_addr_03 + 1];   
            gui_info.fan_level = modbus.PWM_info & 0x0F;
            gui_info.bake_wind_level = modbus.PWM_info >>4;                    
            break;

        case 1:
            modbus.LED_info = rs485.rcvbuf[start_addr_03 + 1];
            break;

        case 2:
            modbus.AC220_info = rs485.rcvbuf[start_addr_03 + 1];    
            gui_info.ac220_switch = (modbus.AC220_info & 0x01);   
            gui_info.bake_power_percentage = modbus.AC220_info>>1;                                    
            break; 

        case 3:
            modbus.NTC1_alarm_value = rs485.rcvbuf[start_addr_03 + 1];
            modbus.NTC2_alarm_value = rs485.rcvbuf[start_addr_03];
            gui_info.ntc1_temp = modbus.NTC1_alarm_value;
            gui_info.ntc2_temp = modbus.NTC2_alarm_value;
            break;

        case 4:
            modbus.NTC3_alarm_value = rs485.rcvbuf[start_addr_03 + 1];
            gui_info.ntc3_temp = modbus.NTC3_alarm_value;
            break;   

        default:
            break;
        }
        start_addr_03 += 2;
    }
    /* RX允许继续接收，开启超时接收计时               */
    rs485.reflag = 0;
    rs485.recount = 0;

}

void Modbus_Fun4()
{
    uint8_t start_addr_03 = 3;              //Slave reply  DATA1_H address

    for( uint16_t i = 0; i < 5; i++)
    {
        switch (i)
        {
        case 0:
            modbus.NTC1_current_value = rs485.rcvbuf[start_addr_03 + 1];
            modbus.NTC2_current_value = rs485.rcvbuf[start_addr_03];                       
            break;

        case 1:
            modbus.NTC3_current_value = rs485.rcvbuf[start_addr_03 + 1];
            break;

        case 2:
                                                         
            break; 

        case 3:
            modbus.I_out1_value = rs485.rcvbuf[start_addr_03 + 1];
            modbus.I_out2_value = rs485.rcvbuf[start_addr_03];
            break;

        case 4:
            modbus.I_out3_value = rs485.rcvbuf[start_addr_03 + 1];
            break;   

        default:
            break;
        }
        start_addr_03 += 2;
    }
    /* RX允许继续接收，开启超时接收计时               */
    rs485.reflag = 0;
    rs485.recount = 0;
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


void get_slave_init_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x35,0x03,0x00,0x00,0x00,0x05,0xBD,0x81};

    RS485_TX;
    delay_ms(5);

    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 

    RS485_RX;
}

void get_slave_current_statu_multifunpower( void )
{
    uint8_t send_buf[8] = {0x35,0x04,0x00,0x00,0x00,0x05,0x7D,0x34};

    RS485_TX;
    delay_ms(5);
    
    memcpy(modbus.modbus_send_buf,send_buf,8);

    modbus_send_data(modbus.modbus_send_buf,8); 
    RS485_RX;
}

void write_slave_reg( void )
{
    uint8_t send_buf[19];
    uint16_t crc;

    modbus.modbus_04_scan_allow = 0;

    RS485_TX;
    delay_ms(5);

    send_buf[0] = 0x35;
    send_buf[1] = 0x10;
    send_buf[2] = 0x00;
    send_buf[3] = 0x00;
    send_buf[4] = 0x00;
    send_buf[5] = 0x05;
    send_buf[6] = 0x0a;

    send_buf[7] = 0x00;
    send_buf[8] = (gui_info.bake_wind_level<<4) | (gui_info.fan_level);

    send_buf[9] = 0x00;
    send_buf[10] = 0x01;

    send_buf[11] = 0x00;
    send_buf[12] = gui_info.ac220_switch | gui_info.bake_power_percentage<<1;

    send_buf[13] = gui_info.ntc2_temp;
    send_buf[14] = gui_info.ntc1_temp;

    send_buf[15] = 0x00;
    send_buf[16] = gui_info.ntc3_temp;

    crc = MODBUS_CRC16(send_buf,17);

    send_buf[17] = crc>>8;
    send_buf[18] = crc;

    memcpy(modbus.modbus_send_buf,send_buf,19);

    modbus_send_data(modbus.modbus_send_buf,19);
    RS485_RX;

    delay_ms(5);
    modbus.modbus_04_scan_allow = 1;
}

