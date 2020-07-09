#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "communication_task.h"
#include "rm_can.h"
#include "rm_gpio.h"
#include "rm_uart.h"
#include "motor_task.h"

#include <stdio.h>

/* protocal for sending the msh
 * [00] -> motor0 position [0]
 * [01] -> motor0 position [1]
 * [02] -> motor0 position [2]
 * [03] -> motor0 position [3]
 
 * [04] -> motor1 position [0]
 * [05] -> motor1 position [1]
 * [06] -> motor1 position [2]
 * [07] -> motor1 position [3]
 
 * [08] -> motor2 position [0]
 * [09] -> motor2 position [1]
 * [10] -> motor2 position [2]
 * [11] -> motor2 position [3]
 
 * [12] -> motor3 position [0]
 * [13] -> motor3 position [1]
 * [14] -> motor3 position [2]
 * [15] -> motor3 position [3]
 
 */


uint8_t motorSpeedBytes[8];

extern moto_measure_t motor_chassis[4];

void communication_task(const void* argu)
{
	uint32_t ticks;
	while(1) {
		ticks = osKernelSysTick();
		reportMotorPosition();
		// LOGINFO("hello hello asd\n", 16);
		osDelayUntil(&ticks, 2);
	}
}

static void reportMotorPosition(void)
{
	motorSpeedBytes[0] = (uint8_t) (motor_chassis[0].speed_rpm >> 8);
	motorSpeedBytes[1] = (uint8_t) (motor_chassis[0].speed_rpm);
	
	motorSpeedBytes[2] = (uint8_t) (motor_chassis[1].speed_rpm >> 8);
	motorSpeedBytes[3] = (uint8_t) (motor_chassis[1].speed_rpm);
	
	motorSpeedBytes[4] = (uint8_t) (motor_chassis[2].speed_rpm >> 8);
	motorSpeedBytes[5] = (uint8_t) (motor_chassis[2].speed_rpm);
	
	motorSpeedBytes[6] = (uint8_t) (motor_chassis[3].speed_rpm >> 8);
	motorSpeedBytes[7] = (uint8_t) (motor_chassis[3].speed_rpm);
	
	write_uart(USER_UART3, motorSpeedBytes, 8);
}
