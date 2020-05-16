/****************************************************************************
 *  Copyright (C) 2018 RoboMaster.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

/**
  *********************** (C) COPYRIGHT 2018 DJI **********************
  * @update
  * @history
  * Version     Date              Author           Modification
  * V1.0.0      January-15-2018   ric.luo
  * @verbatim
  *********************** (C) COPYRIGHT 2018 DJI **********************
  */ 
 


#include "startup.h"
#include "rm_uart.h"
#include "rm_can.h"
#include "sys.h"
#include "rm_gpio.h"


/**
  * @brief     运行在定义的任务函数执行前，可以用来初始化任务中用到的 IO 端口，
  *            配置、开启外界硬件设备，注册硬件设备的接收回调函数
  */
void init_setup(void)
{
  //关闭 LED 状态指示灯
  write_led_io(LED_G, LED_OFF);
  write_led_io(LED_R, LED_OFF);
  
  //关闭所有 LED IO
  write_led_io(LED_IO1, LED_OFF);
  write_led_io(LED_IO2, LED_OFF);
  write_led_io(LED_IO3, LED_OFF);
  write_led_io(LED_IO4, LED_OFF);
  write_led_io(LED_IO5, LED_OFF);
  write_led_io(LED_IO6, LED_OFF);
  write_led_io(LED_IO7, LED_OFF);
  write_led_io(LED_IO8, LED_OFF);
	
  //初始化串口
	rm_uart_init();
  
  //初始化 CAN 设备
  rm_can_init();
	
	//打开四路电源
	rm_power_init();
	
}








/**
* @brief     开启相关任务函数，不需要用户改动
  */
extern TaskHandle_t task1_t;
extern TaskHandle_t task2_t;
extern TaskHandle_t task3_t;
extern TaskHandle_t task4_t;
extern TaskHandle_t task5_t;

void sys_start_task(void)
{
#ifdef USER_TASK1
    osThreadDef(ostask1, USER_TASK1, osPriorityAboveNormal, 0, 128);
    task1_t = osThreadCreate(osThread(ostask1), NULL);
#endif

#ifdef USER_TASK2
    osThreadDef(ostask2, USER_TASK2, osPriorityAboveNormal, 0, 128);
    task2_t = osThreadCreate(osThread(ostask2), NULL);
#endif

#ifdef USER_TASK3
    osThreadDef(ostask3, USER_TASK3, osPriorityHigh, 0, 128);
    task3_t = osThreadCreate(osThread(ostask3), NULL);
#endif

#ifdef USER_TASK4
    osThreadDef(ostask4, USER_TASK4, osPriorityNormal, 0, 128);
    task4_t = osThreadCreate(osThread(ostask4), NULL);
#endif

#ifdef USER_TASK5
    osThreadDef(ostask5, USER_TASK5, osPriorityNormal, 0, 128);
    task5_t = osThreadCreate(osThread(ostask5), NULL);
#endif
}



