/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 21:36:45
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 00:00:18
 */
#include "bsp_led.h"
#include "bsp_time_delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


BspLedStr bsp_led_str_head = {.led1_head.control = kLightOff,.led1_head.time_cnt = 0};
BspLedStr *bsp_led_str = &bsp_led_str_head;


/**
 * @name: 
 * @description: LED处理函数,不能在中断调用
 * @msg: 
 * @param {*}
 * @return {*}
 */
void LedTaskCycle(void)
{
    if(bsp_led_str == NULL)
        return;
    BspLedStr *led_str = bsp_led_str;
    
    if(HAL_GPIO_ReadPin(LED1_PORT,LED1_PIN))
    {
        switch (led_str->led1_head.control)
        {
            case kSlowFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,SLOW_FLICKER_ON_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,FLICKER_ON_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kQuickFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,QUICK_FLICKER_ON_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kLightOff:
                HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                led_str->led1_head.time_cnt = xTaskGetTickCount();
                break;
        }
    }
    else 
    {
        switch (led_str->led1_head.control)
        {
            case kSlowFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,SLOW_FLICKER_OFF_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,FLICKER_OFF_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kQuickFlicker:
                if(CheckTimeDelay(led_str->led1_head.time_cnt,QUICK_FLICKER_OFF_TIME))
                {
                    HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                    led_str->led1_head.time_cnt = xTaskGetTickCount();
                }
                break;
            case kLightOn:
                HAL_GPIO_TogglePin(LED1_PORT,LED1_PIN);
                led_str->led1_head.time_cnt = xTaskGetTickCount();
                break;
        }
    }
}

/**
 * @name: 
 * @description: 设置led状态
 * @msg: 
 * @param {LightControl} state
 * @return {*}
 */
void LedSetState(LightControl state)
{
    if(bsp_led_str == NULL)
        return;
    BspLedStr *led_str = bsp_led_str;
    led_str->led1_head.control = state;
}
/**
 * @name: 
 * @description: 获取当前LED状态
 * @msg: 
 * @param {*}
* @return {*}	0xff: 异常
 */
LightControl LedGetState(void)
{
     if(bsp_led_str == NULL)
        return kLightOffFault;
    BspLedStr *led_str = bsp_led_str;
    return led_str->led1_head.control;
}
