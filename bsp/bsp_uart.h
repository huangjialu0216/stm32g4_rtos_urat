/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-20 00:04:39
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-04-10 15:50:12
 */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"

#define UART2_RX_MAX_FPS 2      //接收缓存帧数
#define UART2_RX_MAX_LEN 400    //接收总缓存会*UART2_RX_MAX_FPS
#define UART2_TX_MAX_LEN 400    //发送最大长度

typedef struct 
{
    uint8_t rx_data_fps;            //当前正在接收缓存帧
    uint16_t rx_data_len;           //接收数据长度
    uint8_t *rx_data;               //接收缓存区首地址
    uint8_t *tx_data;               //发送缓存区首地址
    uint32_t rx_time_difference;    //于上次接收数据时间差
    uint32_t rx_last_time;          //当前接收时间
}UartHead;

typedef struct 
{
    uint16_t rx_data_len;           //接收数据长度
    uint8_t *rx_data;               //接收缓存区首地址
    uint32_t rx_time_difference;    //于上次接收数据时间差
    uint32_t rx_last_time;          //当前接收时间
}UartRxData;


typedef struct 
{
    UartHead uart2_head;
}BspUartStr;

uint8_t BspUartInit(void);
void BspUartDeInit(void);
uint8_t BspUart2Init(void);
void BspUart2DeInit(void);
void printf2(char* fmt,...);
void BspUsrtIrqHandler(UART_HandleTypeDef *huart);
uint8_t BspGetUart2RxData(UartRxData *uart_rx);
#endif
