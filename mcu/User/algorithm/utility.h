#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdint.h>

char* itoa(int num, char *str, int radix);

float ByteToFloat(uint8_t* byteArray);

#endif
