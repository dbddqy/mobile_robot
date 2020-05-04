//#include "stm32f4xx_hal.h"
#ifndef __STUDY_TASK_H__
#define __STUDY_TASK_H__

void study_task(const void* argu);

void call_back_motor_position(uint32_t recv_id, uint8_t data[]);

#endif
