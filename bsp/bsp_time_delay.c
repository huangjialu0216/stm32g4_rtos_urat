/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 21:40:21
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-19 21:50:29
 */
#include "bsp_time_delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"


/**
 * @name: 
 * @description: 非阻塞检测时间到达的功能函数
 * @msg: 
 * @param {unsignedint} delay_start :延时初始时间
 * @param {unsignedint} ndelay :延时时间
 * @return {*} 1:延时到达,0:延迟未到达
 */ 
unsigned char CheckTimeDelay(unsigned int delay_start, unsigned int ndelay)
{
	unsigned long long tmpTime;
	tmpTime = GET_TICK(); //当前时间

	if (delay_start > tmpTime) //开始时间大于当前时间,计时器溢出
		tmpTime |= 0x100000000;

	tmpTime = tmpTime - delay_start; //当前时间减去开始时间

	if ((unsigned int)tmpTime >= ndelay) //当前时间减去开始时间,如果时间大于延时时间,延时时间到
		return 1;
	else
		return 0;
}


/**
 * @name: 
 * @description: 非阻塞中断检测时间到达的功能函数
 * @msg: 
 * @param {unsignedint} delay_start :延时初始时间
 * @param {unsignedint} ndelay :延时时间
 * @return {*} 1:延时到达,0:延迟未到达
 */ 
unsigned char CheckTimeDelayIt(unsigned int delay_start, unsigned int ndelay)
{
	unsigned long long tmpTime;
	tmpTime = GET_TICK_IT(); //当前时间

	if (delay_start > tmpTime) //开始时间大于当前时间,计时器溢出
		tmpTime |= 0x100000000;

	tmpTime = tmpTime - delay_start; //当前时间减去开始时间

	if ((unsigned int)tmpTime >= ndelay) //当前时间减去开始时间,如果时间大于延时时间,延时时间到
		return 1;
	else
		return 0;
}
