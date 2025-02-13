#include "GUI.h"
#include "pic.h"

GUI_INFO gui_info;
GUI_BEAT gui_beat;

static void ac220_set_beat( void )
{   
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(250,56,282,87,WHITE);
    }else
    {
        LCD_Show_Image_Internal_Flash(250,56,32,32,gImage_temp_set,2048);
    }    
}

static void ntc_temp1_beat( void )
{  
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(31,65,67,89,WHITE);
    }else
    {
        LCD_ShowNum(31,65,gui_info.ntc1_temp,3,24,POINT_COLOR,BACK_COLOR);
    }    
}

static void ntc_temp2_beat( void )
{
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(105,65,141,89,WHITE);
    }else
    {
        LCD_ShowNum(105,65,gui_info.ntc2_temp,3,24,POINT_COLOR,BACK_COLOR);
    }  
}

static void ntc_temp3_beat( void )
{  
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(179,65,215,89,WHITE);
    }else
    {
        LCD_ShowNum(179,65,gui_info.ntc3_temp,3,24,POINT_COLOR,BACK_COLOR);
    }  
}

static void ac220_switch( void )
{
    if( gui_info.ac220_switch == DIS_ON )
    {
        if( gui_beat.beat_full == 1 )
        {
            LCD_Fill(242,100,308,139,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_on,5148);
        }
    }else
    {
        if( gui_beat.beat_full == 1 )
        {
            LCD_Fill(242,100,308,139,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_off,5148);
        }
    }   
}


static void led_beat( void )
{   
    if( gui_info.led_switch == DIS_ON )
    {
        if( gui_beat.beat_full == 1 )
        {
            LCD_Fill(32,166,64,198,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_on,2048);
        }
    }else
    {
        if( gui_beat.beat_full == 1 )
        {
            LCD_Fill(32,166,64,198,WHITE);
        }else
        {
            LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_off,2048);
        }
    }    
}

static void fan_beat( void )
{  
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(94,166,126,198,WHITE);
    }else
    {
        LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan_on,2048);
    }  
}

static void fan_level_beat( void )
{  
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(135,165,151,197,WHITE);
    }else
    {
        LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
    }   
}

static void bake_beat( void )
{ 
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(180,172,216,193,WHITE);
    }else
    {
        LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake_on,1512);
    }    
}

static void beat_power( void )
{    
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(272,162,308,192,WHITE);
    }else
    {
        LCD_ShowNum(272,162,gui_info.bake_power_percentage,3,16,POINT_COLOR,BACK_COLOR);
    }    
}

static void beat_wind( void )
{   
    if( gui_beat.beat_full == 1 )
    {
        LCD_Fill(280,190,292,220,WHITE);
    }else
    {
        LCD_ShowNum(280,190,gui_info.bake_wind_level,1,16,POINT_COLOR,BACK_COLOR);
    }
}

static void ac220_switch_dis( void)
{
    if( gui_info.ac220_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_on,5148);
    }else
    {
        LCD_Show_Image_Internal_Flash(242,100,66,39,gImage_ac220_off,5148);
    }
}

static void led_switch_dis( void)
{
    if( gui_info.led_switch == 1)
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_on,2048);
    }else
    {
        LCD_Show_Image_Internal_Flash(32,166,32,32,gImage_led_off,2048);
    }
}

void gui_init( void )
{
    gui_info.ac220_switch           = 1;
    gui_info.led_switch             = 1; 
    gui_info.fan_level              = 9;
    gui_info.bake_power_percentage  = 5;
    gui_info.bake_wind_level        = 4;     
    gui_info.ntc1_temp              = 55;
    gui_info.ntc2_temp              = 44;
    gui_info.ntc3_temp              = 33;

    gui_beat.beat_allow_flag        = 0;
    gui_beat.beat_start_flag        = 0;
    gui_beat.beat_select            = 6;
    gui_beat.beat_switch            = BEAT_OFF;
    gui_beat.beat_full              = 0;
}

void icon_beat(uint8_t pic_code , uint8_t on_off )
{
    if( on_off == 1 )
    {
        gui_beat.beat_allow_flag  = 1;
    }else
    {
        gui_beat.beat_allow_flag  = 0;
    }

    if( gui_beat.beat_start_flag == 1)
    {
        gui_beat.beat_full = 1 -  gui_beat.beat_full;
        switch (pic_code)
        {
            case AC220_SET_ICON:
                refresh_icon();
                ac220_set_beat();
                break;

            case NTC_TEMP1_STR:
                LCD_Show_Image_Internal_Flash(250,56,32,32,gImage_temp_set,2048);
                LCD_ShowNum(179,65,gui_info.ntc3_temp,3,24,POINT_COLOR,BACK_COLOR);
                ntc_temp1_beat();
                break;

            case NTC_TEMP2_STR:
                LCD_ShowNum(31,65,gui_info.ntc1_temp,3,24,POINT_COLOR,BACK_COLOR);
                ntc_temp2_beat();
                break;

            case NTC_TEMP3_STR:
                LCD_ShowNum(105,65,gui_info.ntc2_temp,3,24,POINT_COLOR,BACK_COLOR);
                ntc_temp3_beat();
                break;

            case AC220_SWITCH_ICON:
                refresh_icon();
                ac220_switch();
                break;

            case LED_ICON:
                refresh_icon();
                led_beat();
                break;

            case FAN_ICON:
                refresh_icon();
                fan_beat();
                break;

            case FAN_LEVEL_STR:
                LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan_on,2048);
                fan_level_beat();
                break;

            case BAKE_ICON:
                refresh_icon();       
                bake_beat();
                break;

            case BAKE_POWER_STR:
                LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake_on,1512);
                LCD_ShowNum(280,190,gui_info.bake_wind_level,1,16,POINT_COLOR,BACK_COLOR);
                beat_power();
                break;

            case BAKE_WIND_STR:
            LCD_ShowNum(272,162,gui_info.bake_power_percentage,3,16,POINT_COLOR,BACK_COLOR);
                beat_wind();
                break;

            case KONG:
                refresh_icon();
                //gui_beat.beat_switch = BEAT_OFF;   
                break;

            default:
                break;
        }
    }
    /* 				åœï¿½?ï¿½åˆ·æ–°ï¼Œç­‰å¾…500msåŽå†æ¬¡åˆ·ï¿?? 				*/
    gui_beat.beat_start_flag = 0; 
}

void refresh_icon( void )
{
    ac220_switch_dis();
    led_switch_dis();
    LCD_Show_Image_Internal_Flash(250,56,32,32,gImage_temp_set,2048);
    LCD_Show_Image_Internal_Flash(94,166,32,32,gImage_fan_on,2048);
    LCD_Show_Image_Internal_Flash(180,172,36,21,gImage_bake_on,1512);

    LCD_ShowNum(31,65,gui_info.ntc1_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(105,65,gui_info.ntc2_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(179,65,gui_info.ntc3_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(272,162,gui_info.bake_power_percentage,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(280,190,gui_info.bake_wind_level,1,16,POINT_COLOR,BACK_COLOR);
}

void sync_switch( void )
{
    static uint8_t switch_on_off = 1;
    if( switch_on_off == 1)
    {
        LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_sync,1800);
    }else
    {
        LCD_Fill(45,3,75,33,WHITE);
    }
    switch_on_off = 1 - switch_on_off;

    
}

void update_fun( void )
{
    get_slave_current_statu_multifunpower();
    delay_ms(5);
    LCD_ShowNum(31,125,modbus.NTC1_current_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(105,125,modbus.NTC2_current_value,3,24,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(179,125,modbus.NTC3_current_value,3,24,POINT_COLOR,BACK_COLOR);
    // LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
    // LCD_ShowNum(272,162,gui_info.bake_power_percentage,3,16,POINT_COLOR,BACK_COLOR);
    // LCD_ShowNum(280,190,gui_info.bake_wind_level,1,16,POINT_COLOR,BACK_COLOR);
    // // LCD_ShowNum(86,224,modbus.I_out1_value,2,12,POINT_COLOR,BACK_COLOR);
    // LCD_ShowNum(186,224,modbus.I_out2_value,2,12,POINT_COLOR,BACK_COLOR);
    // LCD_ShowNum(286,224,modbus.I_out3_value,2,12,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(262,222,modbus.I_out1_value,2,12,DARKBLUE,BACK_COLOR);
    LCD_ShowNum(162,222,modbus.I_out2_value,2,12,DARKBLUE,BACK_COLOR);
    LCD_ShowNum(62,222,modbus.I_out3_value,2,12,DARKBLUE,BACK_COLOR);
    if(modbus.NTC1_current_value >= gui_info.ntc1_temp)
    {
        LCD_Show_Image_Internal_Flash(46,92,30,30,gImage_temp_stop,1800);
    }else
    {
        LCD_Show_Image_Internal_Flash(46,92,30,30,gImage_temp_running,1800);
    }

    if(modbus.NTC2_current_value >= gui_info.ntc2_temp)
    {
        LCD_Show_Image_Internal_Flash(120,92,30,30,gImage_temp_stop,1800 );
    }else
    {
        LCD_Show_Image_Internal_Flash(120,92,30,30,gImage_temp_running,1800 );
    }

    if(modbus.NTC3_current_value >= gui_info.ntc3_temp)
    {
        LCD_Show_Image_Internal_Flash(194,92,30,30,gImage_temp_stop,1800);
    }else
    {
        LCD_Show_Image_Internal_Flash(194,92,30,30,gImage_temp_running,1800);
    }
    //ac220_switch_dis();
    //LCD_Show_Image_Internal_Flash(217,127,21,21,gImage_sheshidu_big,882);
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
    //LCD_Show_Image_Internal_Flash(45,3,30,30,gImage_sync,1800);
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
    LCD_DrawLine(10,36,310,36,GRAY);
    LCD_DrawLine(35,154,285,154,BROWN);
    LCD_DrawLine(10,210,310,210,GRAY);

    LCD_ShowChar(56,50,'F',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(130,50,'M',12,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(204,50,'R',12,POINT_COLOR,BACK_COLOR);

    LCD_ShowNum(31,65,gui_info.ntc1_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(69,67,21,21,gImage_sheshidu_big,882);
    LCD_ShowNum(105,65,gui_info.ntc2_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(143,67,21,21,gImage_sheshidu_big,882);
    LCD_ShowNum(179,65,gui_info.ntc3_temp,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(217,67,21,21,gImage_sheshidu_big,882);

    LCD_ShowNum(31,125,666,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(69,127,21,21,gImage_sheshidu_big,882);
    LCD_ShowNum(105,125,555,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(143,127,21,21,gImage_sheshidu_big,882);
    LCD_ShowNum(179,125,444,3,24,POINT_COLOR,BACK_COLOR);
    LCD_Show_Image_Internal_Flash(217,127,21,21,gImage_sheshidu_big,882);

    LCD_ShowNum(135,165,gui_info.fan_level,1,32,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(272,162,gui_info.bake_power_percentage,3,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(280,190,gui_info.bake_wind_level,1,16,POINT_COLOR,BACK_COLOR);
    PutChinese_strings(240,162,"¹¦ÂÊ",BACK_COLOR,POINT_COLOR);
    PutChinese_strings(240,190,"·çËÙ",BACK_COLOR,POINT_COLOR);

    LCD_ShowNum(20,220,24,2,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(40,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(50,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(62,222,15,2,12,DARKBLUE,BACK_COLOR);
    LCD_ShowChar(80,220,'A',16,POINT_COLOR,BACK_COLOR);

    LCD_ShowNum(120,220,36,2,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(140,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(150,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(162,222,15,2,12,DARKBLUE,BACK_COLOR);
    LCD_ShowChar(180,220,'A',16,POINT_COLOR,BACK_COLOR);

    LCD_ShowNum(220,220,42,2,16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(240,220,'V',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowChar(250,220,':',16,POINT_COLOR,BACK_COLOR);
    LCD_ShowNum(262,222,15,2,12,DARKBLUE,BACK_COLOR);
    LCD_ShowChar(280,220,'A',16,POINT_COLOR,BACK_COLOR);

}

