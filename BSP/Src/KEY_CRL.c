#include "KEY_CRL.h"

KEY key;

void key_init( void )
{
    key.key_value_flag = 1;
    key.key1_cycle_flag = 1;
    key.gui_key2_allow_flag = 1;
    key.gui_key3_allow_flag = 1;
    key.gui_key4_allow_flag = 0;  

    key.key1_press_cnt = 0;
    key.key4_press_cnt = 0;
}

void key_scan( void )
{
    uint8_t key_value;

    if(key.key_value_flag == 1)
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
    printf("key1 press \r\n");
    if( key.key1_cycle_flag == 1)
    {
        key.key1_press_cnt++;
        gui_beat.beat_select = key.key1_press_cnt;
        gui_beat.beat_switch = BEAT_ON;
        if( key.key1_press_cnt == KONG ) 
        {
            key.key1_press_cnt = 0;
        }
    }
    key.gui_key4_allow_flag = 1;
}

void key2_press()
{
    if( key.gui_key2_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case NTC_TEMP1_STR:
                gui_info.ntc1_temp += 1;
                break;

            case NTC_TEMP2_STR:
                gui_info.ntc2_temp += 1;
                break;

            case NTC_TEMP3_STR:
                gui_info.ntc3_temp += 1;
                break;

            case FAN_LEVEL_STR:
                gui_info.fan_level += 1;
                break;

            case BAKE_POWER_STR:
                gui_info.bake_power_percentage += 1;
                break;

            case BAKE_WIND_STR:
                gui_info.bake_wind_level += 1;
                break;

            default:
                break;
        }
    }
}

void key3_press()
{
    if( key.gui_key3_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case NTC_TEMP1_STR:
                gui_info.ntc1_temp -= 1;
                break;

            case NTC_TEMP2_STR:
                gui_info.ntc2_temp -= 1;
                break;

            case NTC_TEMP3_STR:
                gui_info.ntc3_temp -= 1;
                break;

            case FAN_LEVEL_STR:
                gui_info.fan_level -= 1;
                break;

            case BAKE_POWER_STR:
                gui_info.bake_power_percentage -= 1;
                break;

            case BAKE_WIND_STR:
                gui_info.bake_wind_level -= 1;
                break;

            default:
                break;
        }
    }
}

void key4_press()
{
    if(key.gui_key4_allow_flag == 1)
    {
        switch (gui_beat.beat_select)
        {
            case AC220_SET_ICON:
            
                break;

            case NTC_TEMP1_STR:
                key.gui_key2_allow_flag = 1;
                key.gui_key3_allow_flag = 1;
                break;

            case NTC_TEMP2_STR:

                break;

            case NTC_TEMP3_STR:
    
                break;

            case AC220_SWITCH_ICON:

                break;

            case LED_ICON:

                break;

            case FAN_ICON:

                break;

            case FAN_LEVEL_STR:
    
                break;

            case BAKE_ICON:
                
                break;

            case BAKE_POWER_STR:
                
                break;

            case BAKE_WIND_STR:
                
                break;

            case KONG:
                
                break;

            default:
                break;
        }
    }
    if( key.key4_press_cnt == 1)
    {
        key.key4_press_cnt = 0;
        key.key1_press_cnt = 0;
        gui_beat.beat_switch = 0;
    }
    key.key4_press_cnt += 1;
}

