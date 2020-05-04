#ifndef __RM_UART_H__
#define __RM_UART_H__

extern uint8_t uart_recv[];

void rm_uart_init(void);
void uart3_receive_callback(void);
void LOGINFO(char *str, uint16_t len);
//int fputc(int ch, FILE* f);

#endif
