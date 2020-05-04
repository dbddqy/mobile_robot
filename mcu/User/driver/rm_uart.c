#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "rm_uart.h"
#include <string.h>

uint8_t uart_recv[255];

// USER_UART3 : uart6

void rm_uart_init(void)
{
	uart_init(USER_UART3, 115200, WORD_LEN_8B, STOP_BITS_1, PARITY_NONE);
  //uart_recv_callback_register(USER_UART3, uart3_receive_callback);
  //uart_receive_start(USER_UART3, uart_recv, 1);
}

void uart3_receive_callback(void)
{
	char* data = "data received";
	LOGINFO(data, strlen(data));
}

void LOGINFO(char *str, uint16_t len)
{
	write_uart(USER_UART3, (uint8_t *)str, len);
	// write_uart(USER_UART3, (uint8_t *)"\n", 1);
}

//int fputc(int ch, FILE* f)
//{
//	uint8_t temp[1] = {ch};
//	write_uart(USER_UART3, temp, 1);
//	return ch;
//}
