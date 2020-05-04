#ifndef __RM_CAN_H__
#define __RM_CAN_H__

#define FILTER_BUF 5

typedef enum
{
  //����ID
  MOTOR_ID_FR       = 0x201,
  MOTOR_ID_FL       = 0x202,
  MOTOR_ID_BL       = 0x203,
  MOTOR_ID_BR       = 0x204,

  //����ID
  MOTOR_ID_SEND       = 0x200,

} motor_id;

typedef struct
{
  /* �����ǵ�����ֱ�ӻش������� */
  
  uint16_t ecd;         //����ı�������ֵ
  uint16_t last_ecd;    //��һ�ε���ı�������ֵ
  int16_t  speed_rpm;   //�����ת��ֵ

  /* �����Ǽ�������ĵ��������� */
  int32_t  round_cnt;   //�����ת����Ȧ��
  int32_t  total_ecd;   //�����ת���ܱ�������ֵ
  int32_t  total_angle; //�����ת���ܽǶ�
  
  /* ���µ�������������ʱ���м���������Ժ��� */
  uint16_t offset_ecd;
  uint32_t msg_cnt;
  int32_t  ecd_raw_rate;
  int32_t  rate_buf[FILTER_BUF];
  uint8_t  buf_cut;
  int32_t  filter_rate;
} moto_measure_t;

void rm_can_init(void);
void can_receive_callback(uint32_t recv_id, uint8_t data[]);

static void get_moto_offset(moto_measure_t *ptr, uint8_t data[]);
static void encoder_data_handle(moto_measure_t *ptr, uint8_t data[]);

void motor_set_current(uint16_t *speeds);

#endif
