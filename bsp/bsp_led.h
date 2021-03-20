/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 21:36:52
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 00:02:49
 */
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "main.h"

#define LED1_PORT   LED_STATUS_GPIO_Port
#define LED1_PIN    LED_STATUS_Pin


#define SLOW_FLICKER_ON_TIME	100		//ms
#define SLOW_FLICKER_OFF_TIME	900		//ms
#define FLICKER_ON_TIME			500		//ms
#define FLICKER_OFF_TIME		500		//ms
#define QUICK_FLICKER_ON_TIME	900		//ms
#define	QUICK_FLICKER_OFF_TIME	100		//ms



typedef enum {kLightOff = 0,kLightOn,kSlowFlicker,kFlicker,kQuickFlicker,kLightOffFault,kLightControlAll} LightControl;
typedef struct 
{
    LightControl control;
	uint32_t time_cnt;
}LedHead;

typedef struct 
{
    LedHead led1_head;
}BspLedStr;


void LedTaskCycle(void);
void LedSetState(LightControl state);
LightControl LedGetState(void);
#endif
