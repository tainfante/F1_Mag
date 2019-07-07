/*
 * uart.c
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#include "uart.h"
#include "dma.h"

volatile uint8_t acc_ready = 0;
volatile uint8_t gyr_ready = 0;

void Uart_Config(void) {

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();

	GPIO_InitTypeDef gpio;
	GPIO_InitTypeDef gpio_adc;

	//External interrupts from I2C - INT1
	gpio.Mode = GPIO_MODE_IT_RISING;
	gpio.Pin = GPIO_PIN_8;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);

	//External interrupts from I2C - INT2
	gpio.Mode = GPIO_MODE_IT_RISING;
	gpio.Pin = GPIO_PIN_10;
	gpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpio);

	//SPI data lines
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7; // SCK, MOSI
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_AF_INPUT;
	gpio.Pin = GPIO_PIN_6; // MISO
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOA, &gpio);

	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = GPIO_PIN_0; // CS
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOC, &gpio);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	//Usart TX pin
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_10;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &gpio);

	//Usart Rx pin
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pin = GPIO_PIN_3;
	gpio.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpio);

	//Data sending frequency check
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOA, &gpio);

	//ADC Channel 1
	gpio_adc.Mode = GPIO_MODE_ANALOG;
	gpio_adc.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &gpio_adc);

	//ADC Channel 2
	gpio_adc.Mode = GPIO_MODE_ANALOG;
	gpio_adc.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOA, &gpio_adc);

	UartHandle.Instance = USART3;
	UartHandle.hdmatx = &uart_dma;
	UartHandle.Init.BaudRate = 460800;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	HAL_UART_Init(&UartHandle);

	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

UART_HandleTypeDef UartInstance(void) {
	return UartHandle;
}

void EXTI15_10_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void EXTI9_5_IRQHandler(void) {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {


	if (GPIO_PIN_8 == GPIO_Pin) {
		gyr_ready = 1;
	}

	if (GPIO_PIN_10 == GPIO_Pin) {
		acc_ready = 1;
	}
}
