#include "rm_gpio.h"
#include "rm_hal_lib.h"

void rm_power_init(void)
{
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_5, GPIO_PIN_SET);
}

uint8_t buttonPressed(void)
{
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_SET)
		return 1;
	else
		return 0;
}
