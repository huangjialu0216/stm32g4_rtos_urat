/*
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-20 00:04:39
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 12:22:43
 */
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "main.h"


#define UART2_RX_MAX_LEN 400
#define UART2_TX_MAX_LEN 400

typedef struct 
{
    uint8_t *rx_data;
    uint8_t *tx_data;
    uint16_t rx_data_len;
    uint32_t rx_time_difference;
    uint32_t rx_last_time;
}UartHead;


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
#endif
