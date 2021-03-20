/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-19 22:48:20
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 13:26:42
 */
#include "app_log.h"
#include "bsp_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "bsp_uart.h"
#include "tim.h"
#include "string.h"

LightControl state = kLightOff;
void StartLogTask(void *argument)
{
    uint16_t cnt_ms = 0;
    uint32_t test_cnt = 0;
    BspUartInit();
    TickType_t last_time = xTaskGetTickCount();
    for(;;)
    {
        vTaskDelayUntil(&last_time,10);
        cnt_ms++;
        if(cnt_ms >99)
            cnt_ms = 0;
      
		#if PRINTF_TASK_LOG
			if(cnt_ms == 0)
			{
				 uint8_t *cpu_run_info;
				 cpu_run_info = pvPortMalloc(TASK_LOG_NUM);
				if(cpu_run_info != NULL)
				{
					memset(cpu_run_info,0,TASK_LOG_NUM); //信息缓冲区清零
					vTaskList((char *)cpu_run_info); //获取任务运行时间信息
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					APP_LOG_DEBUG("任务名 \t\t任务状态   优先级   剩余栈   任务序号\r\n");
					APP_LOG_DEBUG("%s", cpu_run_info);
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					memset(cpu_run_info,0,TASK_LOG_NUM); //信息缓冲区清零
					vTaskGetRunTimeStats((char *)cpu_run_info);
					APP_LOG_DEBUG("任务名 \t\t运行计数         使用率\r\n");
					APP_LOG_DEBUG("%s", cpu_run_info);
					APP_LOG_DEBUG("---------------------------------------------\r\n");
					APP_LOG_DEBUG("当前未分配堆大小: %dByte\r\n",xPortGetFreeHeapSize());
					APP_LOG_DEBUG("未分配内存堆历史最小值 :%dByte\r\n\n",xPortGetMinimumEverFreeHeapSize());

					vPortFree(cpu_run_info);
				}
			}
		#endif
        LedSetState( state);
        LedTaskCycle();
    }
}



uint32_t RUN_Time=0;
extern TIM_HandleTypeDef htim6;
void configureTimerForRunTimeStats(void)
{
    RUN_Time=0;
    MX_TIM6_Init();
    HAL_TIM_Base_Start_IT(&htim6);

}
unsigned long getRunTimeCounterValue(void)
{ 
    return RUN_Time;
}

