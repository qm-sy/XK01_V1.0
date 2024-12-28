#include "PWM_CRL.h"

void pwm_crl( uint16_t pwm1_val, uint16_t pwm2_val, uint16_t pwm3_val)
{
    htim4.Instance->CCR2 = pwm1_val;
    htim4.Instance->CCR3 = pwm2_val;
    htim4.Instance->CCR4 = pwm3_val;
}

