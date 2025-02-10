#include "GUI.h"
#include "pic.h"

GUI gui;

void gui_init( void )
{
    gui.ac220_switch_statu = 1;
    gui.ac220_set_statu    = 1; 
    gui.led_statu          = 1;
    gui.fan_statu          = 1;
    gui.bake_power_statu   = 1;     
    gui.bake_wind_statu    = 1;

    gui.beat_allow_flag    = 0;
    gui.beat_start_flag    = 0;
    gui.beat_select        = 0;
    gui.beat_switch        = 0;
}
void icon_beat(uint8_t pic_code , uint8_t on_off )
{
    if( on_off == 1 )
    {
        gui.beat_allow_flag  = 1;
    }else
    {
        gui.beat_allow_flag  = 0;
    }

    if( gui.beat_start_flag == 1)
    {
        printf("get press \r\n");
        switch (pic_code)
        {
            case AC220_SWITCH_ICON:
                dis_ac220_switch(gui.ac220_switch_statu);
                break;

            case AC220_SET_ICON:
                dis_ac220_set(gui.ac220_set_statu);
                break;

            case LED_ICON:
                dis_led(gui.led_statu);
                break;

            case FAN_ICON:
                dis_fan(gui.fan_statu);
                break;

            case BAKE_POWER_ICON:
                dis_bake_power(gui.bake_power_statu);
                break;

            case BAKE_WIND_ICON:
                dis_bake_windspeed(gui.bake_wind_statu);
                break;

            default:
                break;
        }
    }
    gui.beat_start_flag = 0; 
}

void dis_ac220_switch( uint8_t on_off )
{
    // if( on_off == DIS_ON )
    // {
    //     LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_on,1922);
    // }else
    // {
    //     LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_off,1922);
    // }
}

void dis_ac220_set( uint8_t on_off )
{
    // if( on_off == DIS_ON )
    // {
    //     LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_on,1922);
    // }else
    // {
    //     LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_off,1922);
    // }
}

void dis_led( uint8_t on_off )
{
    static uint8_t now_value = 0;
    if( on_off == DIS_ON )
    {
        if( now_value == 1)
        {
            LCD_Fill(40,180,71,211,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_on,1922);
        }
    }else
    {
        if( now_value == 1)
        {
            LCD_Fill(40,180,71,211,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_off,1922);
        }
    }
    now_value = 1 - now_value;
}

void dis_fan( uint8_t on_off )
{
    static uint8_t now_value = 0;
    if( on_off == DIS_ON )
    {
        if( now_value == 1)
        {
            LCD_Fill(110,179,141,210,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(110,179,31,31,gImage_fan_on,1922);
        }
    }else
    {
        if( now_value == 1)
        {
            LCD_Fill(110,179,141,210,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_off,1922);
        }
    } 
    now_value = 1 - now_value;
}

void dis_bake_power( uint8_t on_off )
{
    if( on_off == DIS_ON )
    {
        LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_quan_on,1922);
    }else
    {
        LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_quan_on,1922);
    }  
}

void dis_bake_windspeed( uint8_t on_off )
{
    if( on_off == DIS_ON )
    {
        LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_hot_on,1922);
    }else
    {
        LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_hot_on,1922);
    }  
}

void check_icon( uint8_t icon_select )
{
    if( icon_select == 1)
    {
        LCD_Show_Image_Internal_Flash(110,179,31,31,gImage_fan_on,1922);
    }else
    {
        
        LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_on,1922);
    }
}

void screen_test( void )
{
    //LCD_Clear(BLACK);
    // LCD_DrawLine(0,33,200,33,RED);
    // LCD_DrawLine(0,66,200,66,RED);
	// LCD_DrawLine(0,99,200,99,RED);
    // LCD_DrawLine(0,0,200,33,RED);
	// LCD_DrawLine(0,0,200,66,RED);
    // LCD_DrawLine(0,0,200,99,RED);
	// LCD_DrawRectangle(0,0,200,200,RED);
    LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_off,1922);

    LCD_Show_Image_Internal_Flash(110,179,31,31,gImage_fan_off,1922);

    LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_quan_on,1922);

    LCD_Show_Image_Internal_Flash(240,80,66,39,gImage_hot_on,5148);

    LCD_Show_Image_Internal_Flash(250,180,31,31,gImage_temp_on,1922);
   
    LCD_Show_Image_Internal_Flash(130,210,29,29,gImage_connect_on,1682);
 

    //LCD_ShowString(160,215,56,16,16,"connect",BLACK,WHITE);
    // HAL_Delay(100);
}
