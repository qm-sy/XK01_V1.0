#include "POWER_CRL.h"

volatile uint8_t zero_flag = 0;
volatile uint8_t power_ch1_statu = 0;
volatile uint8_t power_ch2_statu = 0;
volatile uint8_t power_ch3_statu = 0;
volatile uint8_t power_ch4_statu = 0;
volatile uint16_t power_phase_delay = 0;

void power_crl(uint16_t power_num)
{
    power_phase_delay = (100 - power_num) * 83 / 10;
}
