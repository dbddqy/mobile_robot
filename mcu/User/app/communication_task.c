#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "communication_task.h"
#include "rm_can.h"
#include "rm_gpio.h"
#include "rm_uart.h"

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


uint8_t motorPositionBytes[16];

extern moto_measure_t motor_chassis[4];

void communication_task(const void* argu)
{
	uint32_t ticks;
	while(1) {
		ticks = osKernelSysTick();
		reportMotorPosition();
		// LOGINFO("hello hello asd\n", 16);
		osDelayUntil(&ticks, 40);
	}
}

static void reportMotorPosition(void)
{
	motorPositionBytes[0] = (uint8_t) (motor_chassis[0].total_angle >> 24);
	motorPositionBytes[1] = (uint8_t) (motor_chassis[0].total_angle >> 16);
	motorPositionBytes[2] = (uint8_t) (motor_chassis[0].total_angle >> 8);
	motorPositionBytes[3] = (uint8_t) (motor_chassis[0].total_angle);
	
	motorPositionBytes[4] = (uint8_t) (motor_chassis[1].total_angle >> 24);
	motorPositionBytes[5] = (uint8_t) (motor_chassis[1].total_angle >> 16);
	motorPositionBytes[6] = (uint8_t) (motor_chassis[1].total_angle >> 8);
	motorPositionBytes[7] = (uint8_t) (motor_chassis[1].total_angle);
	
	motorPositionBytes[8] = (uint8_t) (motor_chassis[2].total_angle >> 24);
	motorPositionBytes[9] = (uint8_t) (motor_chassis[2].total_angle >> 16);
	motorPositionBytes[10] = (uint8_t) (motor_chassis[2].total_angle >> 8);
	motorPositionBytes[11] = (uint8_t) (motor_chassis[2].total_angle);
	
	motorPositionBytes[12] = (uint8_t) (motor_chassis[3].total_angle >> 24);
	motorPositionBytes[13] = (uint8_t) (motor_chassis[3].total_angle >> 16);
	motorPositionBytes[14] = (uint8_t) (motor_chassis[3].total_angle >> 8);
	motorPositionBytes[15] = (uint8_t) (motor_chassis[3].total_angle);
	
	write_uart(USER_UART3, motorPositionBytes, 16);
}
