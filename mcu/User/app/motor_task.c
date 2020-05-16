#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "motor_task.h"
#include "rm_can.h"
#include "rm_gpio.h"
#include "rm_uart.h"

#include <stdio.h>

pid_t pid_wheel_spd[4]  = { 0 };
pid_t pid_wheel_position[4]  = { 0 };

extern moto_measure_t motor_chassis[4];
extern float chassis_speed[3];
extern int iterationsToContinue;

char data[106];
int16_t m_current[4] = { 0, 0, 0, 0 };
float m_speed[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
float m_position[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

void motor_task(const void* argu)
{
	
	for (int i=0; i < 4; i++) {
		pid_init(&pid_wheel_spd[i], 7000, 1000, 3.0f, 0.005f, 0.00f);
		pid_init(&pid_wheel_position[i], 3000, 500, 3.0f, 0.00f, 0.00f);
	}		
	uint32_t ticks;
	uint32_t count = 0;
	uint32_t flag = 0;
	while(1)
	{
		ticks = osKernelSysTick();
		if (iterationsToContinue-- > 0)
			chassis_apply_speed(); // speed from uart communication
		else
			chassis_halt();
		// reportMotorState();
//		if ( buttonPressed() ) {
//			osDelay(3000);
//			flag += 1;
//			count += (2500); // ms
//		} 
//		if (count > 0) {
//			if ( flag % 3 == 0 )
//				chassis_set_speed(0.0f, 0.0f, 0.31416f * 2);
//			else if ( flag % 3 == 1 )
//				chassis_set_speed(200.0f, 0.0f, 0.0f);
//			else
//				chassis_set_speed(0.0f, 200.0f, 0.0f);
//			count --;
//		} else {
//			chassis_set_speed(0.0f, 0.0f, 0.0f);
//		}
		motor_set_speed(pid_wheel_spd); // apply the current to all the motors
		osDelayUntil(&ticks, 1);
	}
}

void motor_set_speed(pid_t *pid) //speed in rpm
{
	for (int i=0; i < 4; i++)
		m_current[i] = (int16_t)pid_calc(pid, motor_chassis[i].speed_rpm, m_speed[i]);
	motor_set_current(m_current);
}

void motor_set_position(pid_t *pid_p, pid_t *pid_s) //position in degree
{
	
	for (int i=0; i < 4; i++)
		m_speed[i] = (int16_t)pid_calc(pid_p, motor_chassis[i].total_angle, m_position[i]);
	motor_set_speed(pid_s);
}

void reportMotorState(void)
{
	if (motor_chassis[0].msg_cnt <= 10) {
		LOGINFO("under calibration\n", 18);
		return;
	}
//	sprintf(data, "%8d\n", motor_chassis[2].total_angle);
//	LOGINFO(data, 9);
	sprintf(data, "m0: %8d m1: %8d m2: %8d m3: %8d \ns0: %8d s1: %8d s2: %8d s3: %8d \n", motor_chassis[0].total_angle, motor_chassis[1].total_angle, motor_chassis[2].total_angle, motor_chassis[3].total_angle, motor_chassis[0].speed_rpm, motor_chassis[1].speed_rpm, motor_chassis[2].speed_rpm, motor_chassis[3].speed_rpm);
	LOGINFO(data, 106);
}

/* vx and vy in (mm/s), w in rad/s */
void chassis_set_speed(float vx, float vy, float w)
{
	// speed in rpm
	m_speed[0] = -(vy - vx + w * 0.5f * (CHASSIS_LENGTH + CHASSIS_WIDTH)) * CHASSIS_REDUCTION * 30.0f / M_PI / CHASSIS_WHEEL_RADIUS;
	m_speed[1] = +(vy + vx - w * 0.5f * (CHASSIS_LENGTH + CHASSIS_WIDTH)) * CHASSIS_REDUCTION * 30.0f / M_PI / CHASSIS_WHEEL_RADIUS;
	m_speed[2] = +(vy - vx - w * 0.5f * (CHASSIS_LENGTH + CHASSIS_WIDTH)) * CHASSIS_REDUCTION * 30.0f / M_PI / CHASSIS_WHEEL_RADIUS;
	m_speed[3] = -(vy + vx + w * 0.5f * (CHASSIS_LENGTH + CHASSIS_WIDTH)) * CHASSIS_REDUCTION * 30.0f / M_PI / CHASSIS_WHEEL_RADIUS;
}

void chassis_apply_speed(void)
{
	if (chassis_speed[0] > -CHASSIS_VEL_LIMIT_X && chassis_speed[0] < CHASSIS_VEL_LIMIT_X
		&& chassis_speed[1] > -CHASSIS_VEL_LIMIT_Y && chassis_speed[1] < CHASSIS_VEL_LIMIT_Y
		&& chassis_speed[2] > -CHASSIS_VEL_LIMIT_W && chassis_speed[2] < CHASSIS_VEL_LIMIT_W)
		chassis_set_speed(chassis_speed[0], chassis_speed[1], chassis_speed[2]);
	else
		chassis_set_speed(0.0f, 0.0f, 0.0f);
}

void chassis_halt(void)
{
	chassis_set_speed(0.0f, 0.0f, 0.0f);
}
