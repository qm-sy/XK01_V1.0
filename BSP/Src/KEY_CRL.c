#include "KEY_CRL.h"

KEY key;

void key_init( void )
{
    key.key_init_flag = 1;
    key.key1_cycle_flag = 1;
    key.gui_key2_allow_flag = 1;
    key.gui_key3_allow_flag = 1;
    key.gui_key4_allow_flag = 0;  
    key.sync_allow_flag = 0;
    key.sycn_keep_cnt = 0;
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
    RS485_TX;
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    buzzer_flag = 1;
    if( key.key1_cycle_flag == 1)
    {
        if( key.key1_press_cnt == KONG ) 
        {
            key.key1_press_cnt = 0;
        }

        if( gui_beat.beat_select == NTC_TEMP1_STR )
        {
            gui_beat.beat_select = NTC_TEMP2_STR;
        }
        else if( gui_beat.beat_select == NTC_TEMP2_STR )
        {
            gui_beat.beat_select = NTC_TEMP3_STR;
        }
        else if( gui_beat.beat_select == NTC_TEMP3_STR )
        {
            gui_beat.beat_select = NTC_TEMP1_STR;
        }
        else if ( gui_beat.beat_select == BAKE_POWER_STR )
        {
            gui_beat.beat_select = BAKE_WIND_STR;
        }
        else if ( gui_beat.beat_select == BAKE_WIND_STR )
        {
            gui_beat.beat_select = BAKE_POWER_STR;
        }
        else if( gui_beat.beat_select == FAN_LEVEL_STR)
        {
            jump_to_init();
        }
        else
        {
            key.key1_press_cnt++;
            gui_beat.beat_select = key.key1_press_cnt;
            gui_beat.beat_switch = BEAT_ON;
            key.gui_key2_allow_flag = 0;
            key.gui_key3_allow_flag = 0;
            modbus.modbus_04_scan_allow = 0;
        }

    }
    key.gui_key4_allow_flag = 1;
    delay_ms(150);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}

void key2_press()
{

    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    if( gui_beat.beat_switch == 0 )
    {
        gui_info.fan_level += 1;
        if( gui_info.fan_level >= 6 )
        {
            gui_info.fan_level = 6;
        }
        LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
        write_slave_reg();
    }
    if( key.gui_key2_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case NTC_TEMP1_STR:
                gui_info.ntc1_temp += 1;
                if( gui_info.ntc1_temp >= 120 )
                {
                    gui_info.ntc1_temp = 120;
                }
                break;

            case NTC_TEMP2_STR:
                gui_info.ntc2_temp += 1;
                if( gui_info.ntc2_temp >= 120 )
                {
                    gui_info.ntc2_temp = 120;
                }
                break;

            case NTC_TEMP3_STR:
                gui_info.ntc3_temp += 1;
                if( gui_info.ntc3_temp >= 120 )
                {
                    gui_info.ntc3_temp = 120;
                }
                break;

            case FAN_LEVEL_STR:
                gui_info.fan_level += 1;
                if( gui_info.fan_level >= 6 )
                {
                    gui_info.fan_level = 6;
                }
                break;

            case BAKE_POWER_STR:
                gui_info.bake_power_percentage += 5;
                if( gui_info.bake_power_percentage >= 100 )
                {
                    gui_info.bake_power_percentage = 100;
                }
                break;

            case BAKE_WIND_STR:
                gui_info.bake_wind_level += 1;
                if( gui_info.bake_wind_level >= 6 )
                {
                    gui_info.bake_wind_level = 6;
                }
                break;

            default:
                break;
        }
    }
    delay_ms(150);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}

void key3_press()
{

    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    buzzer_flag = 1;
    if( gui_beat.beat_switch == 0 )
    {
        gui_info.fan_level -= 1;
        if( gui_info.fan_level <= 0 )
        {
            gui_info.fan_level = 0;
        }
        LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
        write_slave_reg();
    }
    if( key.gui_key3_allow_flag == 1 )
    {
        switch(gui_beat.beat_select)
        {
            case NTC_TEMP1_STR:
                gui_info.ntc1_temp -= 1;
                if( gui_info.ntc1_temp <= 10 )
                {
                    gui_info.ntc1_temp = 10;
                }
                break;

            case NTC_TEMP2_STR:
                gui_info.ntc2_temp -= 1;
                if( gui_info.ntc2_temp <= 10 )
                {
                    gui_info.ntc2_temp = 10;
                }
                break;

            case NTC_TEMP3_STR:
                gui_info.ntc3_temp -= 1;
                if( gui_info.ntc3_temp <= 10 )
                {
                    gui_info.ntc3_temp = 10;
                }
                break;

            case FAN_LEVEL_STR:
                gui_info.fan_level -= 1;
                if( gui_info.fan_level <= 0 )
                {
                    gui_info.fan_level = 0;
                }
                break;

            case BAKE_POWER_STR:
                gui_info.bake_power_percentage -= 5;
                if( gui_info.bake_power_percentage <= 30 )
                {
                    gui_info.bake_power_percentage = 30;
                }
                break;

            case BAKE_WIND_STR:
                gui_info.bake_wind_level -= 1;
                if( gui_info.bake_wind_level <= 0 )
                {
                    gui_info.bake_wind_level = 0;
                }
                break;

            default:
                break;
        }
    }
    delay_ms(150);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}

void key4_press()
{

    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_SET);
    buzzer_flag = 1;
    if( gui_beat.beat_switch == 0 )
    {
        key.sync_allow_flag = 1;
    }else
    {
        key.key4_press_cnt += 1;
        if( key.key4_press_cnt == 2)
        {
            key.key4_press_cnt = 0;
            key.key1_press_cnt = 0;
            gui_beat.beat_switch = 0;
            modbus.modbus_04_scan_allow = 1;
            gui_beat.beat_select = KONG;
            key.key_init_flag = 1;
            
            write_slave_reg();
        }
        if(key.gui_key4_allow_flag == 1)
        {
            switch (gui_beat.beat_select)
            {
                case AC220_SET_ICON:
                    gui_beat.beat_select = NTC_TEMP1_STR;
                    key.gui_key2_allow_flag = 1;
                    key.gui_key3_allow_flag = 1;
                    break;
    
                case AC220_SWITCH_ICON:
                    gui_info.ac220_switch = 1 - gui_info.ac220_switch;
                    write_slave_reg();
                    jump_to_init();
                    break;
    
                case LED_ICON:
                    gui_info.led_switch = 1 - gui_info.led_switch;
                    write_slave_reg();
                    if(gui_info.led_switch == 0)
                    {
                        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
                    }else
                    {
                        HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
                    }
                    
                    jump_to_init();
                    break;
    
                case FAN_ICON:
                    gui_beat.beat_select = FAN_LEVEL_STR;
                    key.gui_key2_allow_flag = 1;
                    key.gui_key3_allow_flag = 1;
                    break;
    
                case BAKE_ICON:
                    gui_beat.beat_select = BAKE_POWER_STR;
                    key.gui_key2_allow_flag = 1;
                    key.gui_key3_allow_flag = 1;
                    break;
    
                case KONG:
                    
                    break;
    
                default:
                    break;
            }
        }
    }
    delay_ms(150);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,BUZZER_Pin,GPIO_PIN_RESET);
}

void jump_to_init( void )
{
    key.gui_key2_allow_flag = 1;
    key.gui_key3_allow_flag = 1;

    key.key1_press_cnt = 0;
    key.key4_press_cnt = 0;
    
    gui_beat.beat_switch = 0;
    modbus.modbus_04_scan_allow = 1;
    gui_beat.beat_select = KONG;

    key.key_init_flag = 1;
}


