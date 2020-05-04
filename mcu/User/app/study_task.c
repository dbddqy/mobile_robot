#include <stdlib.h>
#include "rm_hal_lib.h"
#include "cmsis_os.h"
//#include "oled.h"
#include "stm32f4xx_hal.h"
#include "startup.h"
#include "rm_can.h"
#include "rm_uart.h"

// PWM

//void study_task(const void* argu)
//{
//	int16_t angle=0, pulse=0, t=0, x=80, targetAngle=-50, angularVelocity=10;
//	set_pwm_group_param(PWM_GROUP1, 200000);
//	start_pwm_output(PWM_IO2);

//	while(1){
//		if (angle < targetAngle)
//		{
//			angle += 1;
//			t = 1000 / angularVelocity;
//			pulse = (1500 + (600/x) * angle);
//			set_pwm_param(PWM_IO2, pulse);
//			osDelay(t);
//		}
//		if (angle > targetAngle)
//		{
//			angle -= 1;
//			t = 1000 / angularVelocity;
//			pulse = (1500 + (600/x) * angle);
//			set_pwm_param(PWM_IO2, pulse);
//			osDelay(t);
//		}
//	}
//}


// ·äÃùÆ÷

//void study_task(const void* argu)
//{
//	while(1){
//		set_beep_param(BEEP1_IO, BEEP_FREQ*10, BEEP_ON);
//		osDelay(2000);
//		set_beep_param(BEEP1_IO, BEEP_FREQ*10, BEEP_OFF);
//		osDelay(2000);
//	}
//}


// GPIO Input

//uint8_t sensor;
//void study_task(const void* argu)
//{
//	set_digital_io_dir(DIGI_IO1, IO_INPUT);
//	while(1){
//		read_digital_io(DIGI_IO1, &sensor);
//		if( sensor == 1)
//			write_led_io(LED_IO5, LED_ON);
//		else
//			write_led_io(LED_IO5, LED_OFF);
//	}
//}


// IMU

//void study_task(const void* argu)
//{
//	imu_t imu;
//	while(1){
//		get_imu_data(&imu);
//		if(imu.angle_z>45)
//			write_led_io(LED_IO5, LED_ON);
//		else
//			write_led_io(LED_IO5, LED_OFF);
//	}
//}


// OLED

//void study_task(const void* argu)
//{
//	oled_init();
//	while(1){
//		oled_clear(Pen_Clear);
//    oled_LOGO();
//    oled_refresh_gram();   
//    HAL_Delay(2000);
//		
//		oled_clear(Pen_Clear);
//		char a[] = "hello, world!";
//		oled_printf(2, 3, a);
//		oled_refresh_gram();
//		HAL_Delay(2000);
//	}
//}


// UART

void study_task(const void* argu)
{
//	uart_init(USER_UART3, 115200, WORD_LEN_8B, STOP_BITS_1, PARITY_NONE);
//	can_device_init();
//	can_recv_callback_register(USER_CAN1, call_back_motor_position);
//	can_receive_start();
	rm_uart_init();
	rm_can_init();
	
	// uint16_t speeds[4] = { 300, 0, 0, 0};
	while(1)
	{
		// write_uart(USER_UART3, (uint8_t*)"hello\n", 6);
		osDelay(50);
	}
}

void call_back_motor_position(uint32_t recv_id, uint8_t data[])
{
	if(recv_id == 0x201)
	{
		write_uart(USER_UART3, (uint8_t*)"data received", 14);
		write_uart(USER_UART3, data, 8);
		write_uart(USER_UART3, (uint8_t*)"\n", 2);
	}
}


// Motor

//uint8_t can1_send_data[8];

//void study_task(const void* argu)
//{
//	can_device_init();
//	can1_send_data[0] = 0>>8;
//	can1_send_data[1] = 0;
//	can1_send_data[2] = 0>>8;
//	can1_send_data[3] = 0;
//	can1_send_data[4] = 0>>8;
//	can1_send_data[5] = 0;
//	can1_send_data[6] = 0>>8;
//	can1_send_data[7] = 0;
//	while(1){
//		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == 0)
//			continue;
//		for (int i=0; i<2000; i+=10)
//		{
//			can1_send_data[0] = i>>8;
//			can1_send_data[1] = i;
//			write_can(USER_CAN1, 0X200, can1_send_data);
//			HAL_Delay(10);
//		}
//		for (int i=1000; i>-2000; i-=10)
//		{
//			can1_send_data[0] = i>>8;
//			can1_send_data[1] = i;
//			write_can(USER_CAN1, 0X200, can1_send_data);
//			HAL_Delay(10);
//		}
//		for (int i=-2000; i<0; i+=10)
//		{
//			can1_send_data[0] = i>>8;
//			can1_send_data[1] = i;
//			write_can(USER_CAN1, 0X200, can1_send_data);
//			HAL_Delay(10);
//		}
//		HAL_Delay(500);
//	}
//}























