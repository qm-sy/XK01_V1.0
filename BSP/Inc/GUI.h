#ifndef __GUI_H_
#define __GUI_H_

#include "sys.h"
#include "ST7789V2.h"



#define DIS_ON 			1
#define DIS_OFF 		0

#define AC220_SWITCH_ICON   1
#define AC220_SET_ICON      2
#define LED_ICON            3
#define FAN_ICON            4
#define BAKE_POWER_ICON     5
#define BAKE_WIND_ICON      6

#define NTC_TEMP1_STR       1
#define NTC_TEMP2_STR       2
#define NTC_TEMP3_STR       3


typedef struct  
{
    uint8_t  ac220_switch_statu;
    uint8_t  ac220_set_statu;    
    uint8_t  led_statu;       
    uint8_t  fan_statu;      
    uint8_t  bake_power_statu;       
    uint8_t  bake_wind_statu;  

    uint8_t  beat_allow_flag;
    uint8_t  beat_start_flag;
    uint8_t  beat_select;
    uint8_t  beat_switch;
}GUI;

extern GUI gui;

void icon_beat(uint8_t pic_code , uint8_t on_off );
void dis_ac220_switch( uint8_t on_off );
void dis_ac220_set( uint8_t on_off );
void dis_led( uint8_t on_off );
void dis_fan( uint8_t on_off );
void dis_bake_power( uint8_t on_off );
void dis_bake_windspeed( uint8_t on_off );
void screen_test( void );
void check_icon( uint8_t icon_select );

#endif

