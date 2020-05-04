#ifndef __RM_CAN_H__
#define __RM_CAN_H__

#define FILTER_BUF 5

typedef enum
{
  //接收ID
  MOTOR_ID_FR       = 0x201,
  MOTOR_ID_FL       = 0x202,
  MOTOR_ID_BL       = 0x203,
  MOTOR_ID_BR       = 0x204,

  //发送ID
  MOTOR_ID_SEND       = 0x200,

} motor_id;

typedef struct
{
  /* 以下是电机电调直接回传的数据 */
  
  uint16_t ecd;         //电机的编码器数值
  uint16_t last_ecd;    //上一次电机的编码器数值
  int16_t  speed_rpm;   //电机的转速值

  /* 以下是计算出来的电机相关数据 */
  int32_t  round_cnt;   //电机旋转的总圈数
  int32_t  total_ecd;   //电机旋转的总编码器数值
  int32_t  total_angle; //电机旋转的总角度
  
  /* 以下电机计算相关数据时的中间变量，可以忽略 */
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
