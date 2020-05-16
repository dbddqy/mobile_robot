#include "rm_hal_lib.h"
#include "cmsis_os.h"
#include "rm_uart.h"
#include "utility.h"

#include <string.h>
#include <stdio.h>


uint8_t uart_recv[12];

float chassis_speed[3] = { 0 };
int iterationsToContinue = 0;

char data_send[6];

// USER_UART3 : uart6

void rm_uart_init(void)
{
	uart_init(USER_UART3, 74880, WORD_LEN_8B, STOP_BITS_1, PARITY_NONE);
  uart_recv_callback_register(USER_UART3, uart3_receive_callback);
  uart_receive_start(USER_UART3, uart_recv, 12);
}

void uart3_receive_callback(void)
{
	iterationsToContinue = UART_ITERATIONS_TO_CONTINUE;
	uint8_t *temp_data = uart_recv;
	if ( ByteToFloat(temp_data) > -1000.0f && ByteToFloat(temp_data) < 1000.0f )
		chassis_speed[0] = ByteToFloat(temp_data);
	else
		chassis_speed[0] = 0.0f;
	temp_data += 4;
	
	if ( ByteToFloat(temp_data) > -1000.0f && ByteToFloat(temp_data) < 1000.0f )
		chassis_speed[1] = ByteToFloat(temp_data);
	else
		chassis_speed[1] = 0.0f;
	temp_data += 4;
	
	if ( ByteToFloat(temp_data) > -3.14f && ByteToFloat(temp_data) < 3.14f )
		chassis_speed[2] = ByteToFloat(temp_data);
	else
		chassis_speed[2] = 0.0f;
//	sprintf(data_send, "%5.1f\n", chassis_speed[1]);
//	LOGINFO(data_send, 6);
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
