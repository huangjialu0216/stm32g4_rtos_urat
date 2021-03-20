/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 22:48:30
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 12:14:43
 */
#ifndef __APP_LOG_H
#define __APP_LOG_H

#include "main.h"

#define PRINTF_TASK_LOG 1

#if PRINTF_TASK_LOG
#define TASK_LOG_NUM    400

#endif


#define APP_LOG_DEBUG(...)  printf2(__VA_ARGS__)

#endif

