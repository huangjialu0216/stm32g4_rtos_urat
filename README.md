<!--
 * @Descripttion: 
 * @version: 
 * @Author: JiaLu
 * @Date: 2021-03-20 13:51:47
 * @LastEditors: JiaLu
 * @LastEditTime: 2021-03-20 16:10:41
-->
## 1. 简介  
工程适用于STM32G431C8U6,可移植内部串口、LED等代码

## 2. 功能  
- 1. LED驱动可实现常灭，常亮，短闪，闪烁，长闪
- 2. 串口发送函数，采用DMA串口发送，DMA+空闲中断接收串口数据
- 3. 实现freeRTOS的CPU占用率，任务堆栈占用率，剩余堆大小和最小剩余堆大小
## 3. 软件结构
- 1.目录[bsp](./bsp):包含外设驱动.
- 2.目录[app](./app):包含逻辑功能,实时任务.
- 3.目录[task](./task):作为bsp和app中间层

## 4. app任务说明
- 1. void StartLogTask(void *argument); 
 - 此任务主要用来开发调试使用，用来打印任务信息。
 - 调试驱动时可打印驱动获取数据等

