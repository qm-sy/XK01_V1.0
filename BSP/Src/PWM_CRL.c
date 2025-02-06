#include "PWM_CRL.h"

SLAVE_PWM slave_pwm;

/**
 * @brief	控制pwm输出的占空比及频率
 *
 * @remark	Fpwm =Tclk / ((arr+1)*(psc+1)) = 1000000/(arr+1) (单位：Hz)
 *          Tclk = 72M Hz    psc=71    arr = 1000000/frequency - 1
 *
 * @param   pwm1_dutycycle		PWM1占空比
 * @param   pwm2_dutycycle	    PWM2占空比
 * @param   pwm3_dutycycle	    PWM3占空比
 * @param   frequency           频率
 *
 * @return  void
 */
void pwm_crl( uint8_t pwm1_dutycycle, uint8_t pwm2_dutycycle, uint8_t pwm3_dutycycle,uint32_t frequency)
{
    uint32_t Fpwm = 1000000 / frequency - 1;

    /* 1, 设置频率                                            */
    htim4.Instance->ARR  = Fpwm;

    /* 2, 设置PWM1(CCR2)、PWM2(CCR3)、PWM3(CCR4)的占空比      */
    htim4.Instance->CCR2 = ((uint32_t)pwm1_dutycycle * Fpwm) / 100 ;
    htim4.Instance->CCR3 = ((uint32_t)pwm2_dutycycle * Fpwm) / 100 ;
    htim4.Instance->CCR4 = ((uint32_t)pwm3_dutycycle * Fpwm) / 100 ;
}

void slave_statu_query_modify( uint8_t code, uint16_t reg_addr,uint16_t reg_data )
{
    uint16_t crc;
    uint8_t crc_buf_03[6];
    switch(code)
    {
        case 3:
            slave_pwm.slave_send_buf[0] = 0x35;
            slave_pwm.slave_send_buf[1] = code;
            slave_pwm.slave_send_buf[2] = reg_addr>>8;
            slave_pwm.slave_send_buf[3] = reg_addr;
            slave_pwm.slave_send_buf[4] = 0;
            slave_pwm.slave_send_buf[5] = 1;
            memcpy(crc_buf_03,slave_pwm.slave_send_buf,6);
            crc = MODBUS_CRC16(crc_buf_03,6);
            slave_pwm.slave_send_buf[6] = crc>>8;
            slave_pwm.slave_send_buf[7] = crc;
            modbus_send_data(slave_pwm.slave_send_buf,8);
            
            modbus_wait_receive();

            switch (reg_addr)
            {
                case 0:
                    slave_fan_statu_query();
                    break;
                
                default:
                    break;
            }
            break;
        case 4:
        default:
            break;
    }
}

void slave_fan_statu_query(void)
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

//     slave_pwm.modbus_06[6] = crc>>8;
//     slave_pwm.modbus_06[7]  = crc;       

//     modbus_send_data(slave_pwm.modbus_06,8);
// }

