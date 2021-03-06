#ifndef __RM_UART_H__
#define __RM_UART_H__

/* <when there is no new message, continue the curent speed with 200 control cycles*/
#define UART_ITERATIONS_TO_CONTINUE 200

extern uint8_t uart_recv[];

void rm_uart_init(void);
void uart3_receive_callback(void);
void LOGINFO(char *str, uint16_t len);
//int fputc(int ch, FILE* f);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif
