/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 21:40:35
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-19 21:51:07
 */
#ifndef __BSP_TIME_DELAY_H
#define __BSP_TIME_DELAY_H

#include "main.h"




#define GET_TICK()			xTaskGetTickCount()
#define GET_TICK_IT()		xTaskGetTickCountFromISR()


unsigned char CheckTimeDelay(unsigned int delay_start, unsigned int ndelay);
unsigned char CheckTimeDelayIt(unsigned int delay_start, unsigned int ndelay);

#endif
