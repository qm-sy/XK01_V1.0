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
                key1_press();
            }
            
            break;

        case 11:
            delay_ms(200);
            if(key_value == 11)
            {
                key2_press();
            }
        
            break;

        case 13:
            delay_ms(200);
            if(key_value == 13)
            {
                key3_press();
            }
            break;

        case 14:
            delay_ms(200);
            if(key_value == 14)
            {
                key4_press();
            }
            break;

        default:
            break;
    }
}

void key1_press()
{
    static uint8_t icon_select = 0;
    icon_select++;
    check_icon(icon_select);
    switch (icon_select)
    {
    case 1:
        gui.led_statu = 1;
        gui.beat_select = LED_ICON;
        gui.beat_switch = DIS_ON;
        break;
    
    case 2:
        gui.fan_statu = 1;
        gui.beat_select = FAN_ICON;
        gui.beat_switch = DIS_ON;
        break;

    default:
        break;
    }
    if( icon_select >= 2 )
    {
        icon_select = 0;
    }
}

void key2_press()
{
    gui.fan_statu = 1;
    gui.beat_select = FAN_ICON;
    gui.beat_switch = DIS_ON;
}

void key3_press()
{
    gui.beat_switch = DIS_OFF;
}

void key4_press()
{
    gui.led_statu = 0;
    gui.beat_select = LED_ICON;
    gui.beat_switch = DIS_ON;
}

