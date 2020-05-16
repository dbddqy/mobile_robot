#ifndef __MOTOR_TASK_H__
#define __MOTOR_TASK_H__

#include "pid.h"
// #include <math.h>

#define CHASSIS_LENGTH 431.0f // mm
#define CHASSIS_WIDTH 303.5f
#define CHASSIS_WHEEL_DIAMETER 152.0f // mm
#define CHASSIS_WHEEL_RADIUS (CHASSIS_WHEEL_DIAMETER/2.0f)
#define CHASSIS_REDUCTION 36

#define M_PI 3.1415926536f

#define CHASSIS_VEL_LIMIT_X 600 //mm/s
#define CHASSIS_VEL_LIMIT_Y 600 //mm/s
#define CHASSIS_VEL_LIMIT_W (M_PI*0.6f) //rad/s

void motor_task(const void* argu);

void motor_set_speed(pid_t *pid);

void motor_set_position(pid_t *pid, pid_t *pid_s);

void reportMotorState(void);

void chassis_set_speed(float vx, float vy, float w);

void chassis_apply_speed(void);

void chassis_halt(void);

#endif
