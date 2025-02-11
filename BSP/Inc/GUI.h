#ifndef __GUI_H_
#define __GUI_H_

#include "sys.h"
#include "ST7789V2.h"



#define DIS_ON 			1
#define DIS_OFF 		0

#define BEAT_ON         1
#define BEAT_OFF        0

/*      需要跳动显示的内容   */
#define AC220_SET_ICON      1
#define NTC_TEMP1_STR       2
#define NTC_TEMP2_STR       3
#define NTC_TEMP3_STR       4
#define AC220_SWITCH_ICON   5
#define LED_ICON            6
#define FAN_ICON            7
#define FAN_LEVEL_STR       8
#define BAKE_ICON           9
#define BAKE_POWER_STR      10
#define BAKE_WIND_STR       11
#define KONG                12


typedef struct  
{
    uint8_t  ac220_switch;   
    uint8_t  led_switch;           
    uint8_t  fan_level;
    uint8_t  bake_power_percentage;       
    uint8_t  bake_wind_level;  
    uint8_t  ntc1_temp;
    uint8_t  ntc2_temp;
    uint8_t  ntc3_temp;
}GUI_INFO;

typedef struct 
{
    uint8_t  beat_allow_flag;
    uint8_t  beat_start_flag;
    uint8_t  beat_select;
    uint8_t  beat_switch; 
    uint8_t  beat_full;
}GUI_BEAT;

extern GUI_INFO gui_info;
extern GUI_BEAT gui_beat;

void icon_beat(uint8_t pic_code , uint8_t on_off );
void dis_ac220_switch( uint8_t on_off );
void dis_ac220_set( uint8_t on_off );
void dis_led( uint8_t on_off );
void dis_fan( uint8_t on_off );
void dis_bake_power( uint8_t on_off );
void dis_bake_windspeed( uint8_t on_off );
void screen_test( void );
void check_icon( uint8_t icon_select );
void gui_init( void );

#endif

