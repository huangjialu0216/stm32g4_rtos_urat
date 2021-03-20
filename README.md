## 1. 简介  
适用于一清BCM_V3.0程序,包括但不限于夸父,夸父plus,扫地车,雪狮,车型.

## 2. 功能  
- 1. 由硬件开关决定的遥控/电脑控制模式选择.
- 2. 解析SBUS数据并通过CAN2发送至总线
- 3. 解析EPS,EPB,IBC,BMS,MCU数据并对故障进行处理.
- 4. 里程计功能,钥匙拨至"OFF"档时保存总里程至FLASH.
- 5. 不同车型支持，通过vehicle_config.h.

## 3. 软件结构
- 1.目录[bsp](./MDK-ARM/bsp):包含外设驱动.
- 2.目录[app](./MDK-ARM/app):包含逻辑功能,实时任务.
- 3.函数`control_task()`: 处理can1总线数据发送.
- 4.函数`deal_ipc_task()`: 解析IPC数据.
- 4.函数`sbus_parse_task()`: 解析SBUS数据及can2数据发送.
- 5.函数`process_cmd()`: 解析can1总线数据.
- 6.函数`detection_task()`:车身状态监控,统一错误处理.
- 6.函数`io_task()`:输入输出监控.
- 7.结构body_super_block:包含车身状态.

