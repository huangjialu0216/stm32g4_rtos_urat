/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-20 00:04:23
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-04-10 16:36:13
 */
#include "bsp_uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "usart.h"
#include "semphr.h"
#include <stdarg.h>

extern UART_HandleTypeDef huart2;
extern osSemaphoreId_t uart2TxIdleBinarySemHandle;
extern osSemaphoreId_t uart2RxIdleBinarySemHandle;

BspUartStr* bsp_uart_str = NULL;

/**
 * @name: 
 * @description: 串口初始化，其中包含内存申请
 * @msg: 
 * @param {*}
 * @return {*} 0: 初始成功  bit 0 : 1结构体内存申请失败
 *                          bit 1 : 1发送数据缓存内存申请失败
 *                          bit 2 : 1接收数据缓存内存申请失败
 */
uint8_t BspUartInit(void)
{
    if(bsp_uart_str != NULL)
        BspUartDeInit();
    bsp_uart_str = pvPortMalloc(sizeof(BspUartStr));
    if(bsp_uart_str == NULL)
        return 1;
    bsp_uart_str->uart2_head.rx_data = NULL;

    
	return BspUart2Init();
}
/**
 * @name: 
 * @description: 串口反初始化，内存释放
 * @msg: 
 * @param {*}
 * @return {*}
 */
void BspUartDeInit(void)
{
    if(bsp_uart_str == NULL)
        return;
    BspUart2DeInit();
    vPortFree(bsp_uart_str);
    bsp_uart_str = NULL;
}

/**
 * @name: 
 * @description: 串口2初始化，其中包含内存申请
 * @msg: 
 * @param {*}
 * @return {*} 0: 初始成功  bit 0 : 1结构体内存申请失败
 *                          bit 1 : 1发送数据缓存内存申请失败
 *                          bit 2 : 1接收数据缓存内存申请失败
 */
uint8_t BspUart2Init(void)
{
    if(bsp_uart_str == NULL)
        return 1;
    UartHead* uart2_head_str;
    uart2_head_str = &bsp_uart_str->uart2_head;
    if((uart2_head_str->rx_data != NULL)|(uart2_head_str->tx_data != NULL))
        BspUart2DeInit();
    uart2_head_str->rx_data = pvPortMalloc(UART2_RX_MAX_LEN * UART2_RX_MAX_FPS);
    uart2_head_str->tx_data = pvPortMalloc(UART2_TX_MAX_LEN);
    uart2_head_str->rx_data_fps = 0;
    if(uart2_head_str->tx_data == NULL)
        return 2;
    if(uart2_head_str->rx_data == NULL)
        return 4;
    MX_USART2_UART_Init();  //初始化串口

    HAL_UART_Receive_DMA(&huart2,uart2_head_str->rx_data,UART2_RX_MAX_LEN);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    SET_BIT(huart2.Instance->ISR, USART_ISR_IDLE);
    xSemaphoreGive(uart2TxIdleBinarySemHandle);	//释放信号量
	return 0;
}
/**
 * @name: 
 * @description: 串口2反初始化，内存释放
 * @msg: 
 * @param {*}
 * @return {*}
 */
void BspUart2DeInit(void)
{
     if(bsp_uart_str == NULL)
        return;
    if(bsp_uart_str->uart2_head.rx_data != NULL)
    {
        vPortFree(bsp_uart_str->uart2_head.rx_data);
        bsp_uart_str->uart2_head.rx_data = NULL;
    }
    if(bsp_uart_str->uart2_head.tx_data != NULL)
    {
        vPortFree(bsp_uart_str->uart2_head.tx_data);
        bsp_uart_str->uart2_head.tx_data = NULL;
    }
}

/**
 * @name: 
 * @description: 串口打印函数重定义
 * @msg: 
 * @param {*}
 * @return {*}
 */
void printf2(char* fmt,...)
{
    uint16_t i;
    va_list ap;
    if(xSemaphoreTake(uart2TxIdleBinarySemHandle,1000) == pdTRUE)
    { 
        va_start(ap,fmt);
         if((bsp_uart_str == NULL)||(bsp_uart_str->uart2_head.tx_data == NULL))
		 {
			 xSemaphoreGive(uart2TxIdleBinarySemHandle);	//释放信号量
			 return ;
		 }
        UartHead* uart_head_str;
        uart_head_str = &bsp_uart_str->uart2_head;
        
        i=vsnprintf((char*)uart_head_str->tx_data,UART2_TX_MAX_LEN,fmt,ap);
        va_end(ap);
        if((i == 0)||(i > UART2_TX_MAX_LEN))
		{
			xSemaphoreGive(uart2TxIdleBinarySemHandle);	//释放信号量
			return;
		}
        if(HAL_UART_Transmit_DMA(&huart2,uart_head_str->tx_data,i)!=HAL_OK)
            xSemaphoreGive(uart2TxIdleBinarySemHandle);	//释放信号量

    }
}

/**
 * @name: 
 * @description: 串口2接收数据获取
 * @msg: 
 * @param {UartRxData} *uart_rx ：接收数据指针
 * @return {*} 0：未接收到数据    1：接收到数据
 */
uint8_t BspGetUart2RxData(UartRxData *uart_rx)
{
    if(xSemaphoreTake(uart2RxIdleBinarySemHandle,0) == pdFALSE)
        return 0;
    UartHead* uart_head_str;
    uart_head_str = &bsp_uart_str->uart2_head;
    taskENTER_CRITICAL();
    if(uart_head_str->rx_data_fps == 0)
        uart_rx->rx_data = uart_head_str->rx_data + (UART2_RX_MAX_FPS-1) * UART2_RX_MAX_LEN;
    else
        uart_rx->rx_data = uart_head_str->rx_data + (uart_head_str->rx_data_fps-1) * UART2_RX_MAX_LEN;
    uart_rx->rx_data_len = uart_head_str->rx_data_len;
    uart_rx->rx_time_difference = uart_head_str->rx_time_difference;
    uart_rx->rx_last_time = uart_head_str->rx_last_time;
    taskEXIT_CRITICAL();
    return 1;
}

/**
 * @name: 
 * @description: 串口中断回调函数，一般只处理IDLE
 * @msg: 
 * @param {UART_HandleTypeDef} *huart 串口句柄
 * @return {*}
 */
void BspUsrtIrqHandler(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
    {
        if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);
            HAL_DMA_Abort(huart->hdmarx);
            CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE));
            CLEAR_BIT(huart->Instance->CR3, (USART_CR3_EIE | USART_CR3_RXFTIE));
            huart->RxState = HAL_UART_STATE_READY;
            huart->RxISR = NULL;

            if((bsp_uart_str == NULL)||(bsp_uart_str->uart2_head.tx_data == NULL))
                return ;
            UartHead* uart_head_str;
            uart_head_str = &bsp_uart_str->uart2_head;
            uart_head_str->rx_data_fps +=1;
            if(uart_head_str->rx_data_fps >= UART2_RX_MAX_FPS)
                uart_head_str->rx_data_fps = 0;
            uart_head_str->rx_time_difference = HAL_GetTick() - uart_head_str->rx_last_time;
            uart_head_str->rx_last_time = HAL_GetTick();
            uart_head_str->rx_data_len = UART2_RX_MAX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);

            BaseType_t  pxHigherPriorityTaskWoken;
            xSemaphoreGiveFromISR(uart2RxIdleBinarySemHandle,&pxHigherPriorityTaskWoken);	//释放信号量

            HAL_UART_Receive_DMA(&huart2,uart_head_str->rx_data + uart_head_str->rx_data_fps * UART2_RX_MAX_LEN,UART2_RX_MAX_LEN);
            __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
            SET_BIT(huart2.Instance->ISR, USART_ISR_IDLE);
            //如果需要的话进行一次任务切换
            portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
        }
    }
}

/**
 * @name: 
 * @description: 串口发送完成回调函数
 * @msg: 
 * @param {*}
 * @return {*}
 */
//TX回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		BaseType_t  pxHigherPriorityTaskWoken;
		xSemaphoreGiveFromISR(uart2TxIdleBinarySemHandle,&pxHigherPriorityTaskWoken);	//释放信号量
		//如果需要的话进行一次任务切换
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}

}

