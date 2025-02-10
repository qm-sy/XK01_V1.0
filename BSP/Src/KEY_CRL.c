#include "KEY_CRL.h"

volatile uint8_t key_value_flag = 0;

void key_scan( void )
{
    uint8_t key_value;

    if(key_value_flag == 1)
	{
		key_value = (B0_VAL) | (B1_VAL<<1) | (B2_VAL<<2) | (B3_VAL<<3);
	}
    switch (key_value)
    {
        case 7:
            delay_ms(200);
            if(key_value == 7)
            {
                key1_press( key_value );
            }
            
            break;

        case 11:
            delay_ms(200);
            if(key_value == 11)
            {
                key2_press( key_value );
            }
        
            break;

        case 13:
            delay_ms(200);
            if(key_value == 13)
            {
                key3_press( key_value );
            }
            break;

        case 14:
            delay_ms(200);
            if(key_value == 14)
            {
                key4_press( key_value );
            }
            break;

        default:
            break;
    }
}

void key1_press( uint8_t key_value )
{

    printf("The value of key is : %d \r\n",key_value);
}

void key2_press( uint8_t key_value )
{

    printf("The value of key is : %d \r\n",key_value);
}

void key3_press( uint8_t key_value )
{

    printf("The value of key is : %d \r\n",key_value);
}

void key4_press( uint8_t key_value )
{

    printf("The value of key is : %d \r\n",key_value);
}
