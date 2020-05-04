#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "rm_can.h"
#include "rm_uart.h"
#include "utility.h"
#include <string.h>

moto_measure_t motor_chassis[4] = { 0 };

void rm_can_init(void)
{
	can_device_init();
  //ע��CAN1�������ݻص�����
  can_recv_callback_register(USER_CAN1, can_receive_callback);
  //ע��CAN2�������ݻص�������û���豸ʹ�� CAN2������Ҫע��
  //can_recv_callback_register(USER_CAN2, can2_recv_callback); 
  //����CAN���������ж�
  can_receive_start();
	
	// memset(motor_chassis,0,sizeof(motor_chassis)*4);
}

void can_receive_callback(uint32_t recv_id, uint8_t data[])
{
	switch(recv_id){
		case MOTOR_ID_FR:
		{
			motor_chassis[0].msg_cnt <= 10 ? get_moto_offset(&motor_chassis[0], data) : encoder_data_handle(&motor_chassis[0], data);
			break;
		}
		case MOTOR_ID_FL:
		{
			motor_chassis[1].msg_cnt <= 10 ? get_moto_offset(&motor_chassis[1], data) : encoder_data_handle(&motor_chassis[1], data);
			break;
		}
		case MOTOR_ID_BL:
		{
			motor_chassis[2].msg_cnt <= 10 ? get_moto_offset(&motor_chassis[2], data) : encoder_data_handle(&motor_chassis[2], data);
			break;
		}
		case MOTOR_ID_BR:
		{
			motor_chassis[3].msg_cnt <= 10 ? get_moto_offset(&motor_chassis[3], data) : encoder_data_handle(&motor_chassis[3], data);
			break;
		}
	}
}

/**
  * @brief     ��õ����ʼƫ��
  * @param     ptr: ������� moto_measure_t �ṹ��ָ��
  * @param     data: ���յ��ĵ�� CAN ����ָ��
  */
static void get_moto_offset(moto_measure_t *ptr, uint8_t data[])
{
  ptr->ecd        = (uint16_t)(data[0] << 8 | data[1]);
  ptr->offset_ecd = ptr->ecd;
	ptr->msg_cnt ++;
}

/**
  * @brief     ��������ת��rmp Ȧ��round_cnt 
  *            �ܱ�������ֵtotal_ecd ����ת�ĽǶ�total_angle
  * @param     ptr: ������� moto_measure_t �ṹ��ָ��
  * @param     data: ���յ��ĵ�� CAN ����ָ��
  */
static void encoder_data_handle(moto_measure_t *ptr, uint8_t data[])
{
  int32_t temp_sum = 0;
  
  ptr->last_ecd      = ptr->ecd;
  ptr->ecd           = (uint16_t)(data[0] << 8 | data[1]);

  ptr->speed_rpm     = (int16_t)(data[2] << 8 | data[3]);

  if (ptr->ecd - ptr->last_ecd > 5000)
  {
    ptr->round_cnt--;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd - 8192;
  }
  else if (ptr->ecd - ptr->last_ecd < -5000)
  {
    ptr->round_cnt++;
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd + 8192;
  }
  else
  {
    ptr->ecd_raw_rate = ptr->ecd - ptr->last_ecd;
  }

  ptr->total_ecd = ptr->round_cnt * 8192 + ptr->ecd - ptr->offset_ecd;
  ptr->total_angle = ptr->total_ecd * 360 / 8192;
  
  
  ptr->rate_buf[ptr->buf_cut++] = ptr->ecd_raw_rate;
  if (ptr->buf_cut >= FILTER_BUF)
    ptr->buf_cut = 0;
  for (uint8_t i = 0; i < FILTER_BUF; i++)
  {
    temp_sum += ptr->rate_buf[i];
  }
  ptr->filter_rate = (int32_t)(temp_sum/FILTER_BUF);
}

void motor_set_current(uint16_t *speeds){
	uint8_t can_send_data[8];
	can_send_data[0] = speeds[0]>>8;
	can_send_data[1] = speeds[0];
	can_send_data[2] = speeds[1]>>8;
	can_send_data[3] = speeds[1];
	can_send_data[4] = speeds[2]>>8;
	can_send_data[5] = speeds[2];
	can_send_data[6] = speeds[3]>>8;
	can_send_data[7] = speeds[3];
	write_can(USER_CAN1, MOTOR_ID_SEND, can_send_data);
}
