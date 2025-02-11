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
    // if( on_off == DIS_ON )
    // {
    //     LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_quan_on,1922);
    // }else
    // {
    //     LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_quan_on,1922);
    // }  
}

void dis_bake_windspeed( uint8_t on_off )
{
    // if( on_off == DIS_ON )
    // {
    //     LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_hot_on,1922);
    // }else
    // {
    //     LCD_Show_Image_Internal_Flash(180,180,31,31,gImage_hot_on,1922);
    // }  
}

void check_icon( uint8_t icon_select )
{
    // if( icon_select == 1)
    // {
    //     LCD_Show_Image_Internal_Flash(110,179,31,31,gImage_fan_on,1922);
    // }else
    // {
        
    //     LCD_Show_Image_Internal_Flash(40,180,31,31,gImage_led_on,1922);
    // }
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
    /*              connect icon                */
    LCD_Show_Image_Internal_Flash(15,3,29,30,gImage_connect_on,1740);
    //LCD_Show_Image_Internal_Flash(5,3,30,30,gImage_connect_on,1800);
    /*              sync icon                */
    LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_sync,1800);
    /*              temp icon                */
    LCD_Show_Image_Internal_Flash(46,92,30,30,gImage_temp_stop,1800);
    LCD_Show_Image_Internal_Flash(120,92,30,30,gImage_temp_stop,1800 );
    LCD_Show_Image_Internal_Flash(194,92,30,30,gImage_temp_running,1800);
    LCD_Show_Image_Internal_Flash(250,56,32,32,gImage_temp_set,2048);
    /*              ac 220 icon                */
    LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_off,5148);
    //LCD_Show_Image_Internal_Flash(240,85,66,39,gImage_ac220_off,5148);
    /*              led  icon                */
    LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_on,2048);
    //LCD_Show_Image_Internal_Flash(20,160,32,32,gImage_led_off,2048);
   
    /*              fan  icon                */
    LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan_on,2048);
    //LCD_Show_Image_Internal_Flash(80,160,32,32,gImage_fan_off,2048);
 
    /*              bake  icon                */
    LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake_on,1512);
    //LCD_Show_Image_Internal_Flash(140,160,36,21,gImage_bake_off,1512);
    //LCD_Show_Image_Internal_Flash(80,160,32,32,gImage_fan_off,2048);
    //LCD_ShowString(160,215,56,16,16,"connect",BLACK,WHITE);
    // HAL_Delay(100);
}
