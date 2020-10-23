/*
 * logger.c
 *
 *  Created on: 11.08.2019
 *      Author: Agata
 */

#include "logger.h"

void loggerConfig(void){

	 __HAL_RCC_USART2_CLK_ENABLE();

	 GPIO_InitTypeDef gpio;
	 gpio.Mode = GPIO_MODE_AF_PP;
	 gpio.Pin = GPIO_PIN_2;
	 gpio.Pull = GPIO_NOPULL;
	 gpio.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOA, &gpio);

	 gpio.Mode = GPIO_MODE_AF_INPUT;
	 gpio.Pin = GPIO_PIN_3;
	 HAL_GPIO_Init(GPIOA, &gpio);

	 uart.Instance = USART2;
	 uart.Init.BaudRate = 115200;
	 uart.Init.WordLength = UART_WORDLENGTH_8B;
	 uart.Init.Parity = UART_PARITY_NONE;
	 uart.Init.StopBits = UART_STOPBITS_1;
	 uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	 uart.Init.OverSampling = UART_OVERSAMPLING_16;
	 uart.Init.Mode = UART_MODE_TX_RX;
	 HAL_UART_Init(&uart);

}

void sendData(uint8_t *buffer, int size){
	uint16_t a,b,c,d,e,f;
	char bufferforsprintf[50];


	a=buffer[1]<<8|buffer[2];
	b=buffer[3]<<8|buffer[4];
	c=buffer[5]<<8|buffer[6];
	d=buffer[7]<<8|buffer[8];
	e=buffer[9]<<8|buffer[10];
	f=buffer[11]<<8|buffer[12];

	int val = sprintf(bufferforsprintf, "%d %d %d %d %d %d\n\r", a, b, c, d, e, f);

	HAL_UART_Transmit(&uart, (uint8_t*) bufferforsprintf, val, 1000);

}
