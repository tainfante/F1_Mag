/*
 * uart.c
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#include "uart.h"

void Uart_Config(void){

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_USART3_CLK_ENABLE();


		GPIO_InitTypeDef gpio;
		GPIO_InitTypeDef gpio_adc;

		//External interrupts from I2C - INT1
		gpio.Mode=GPIO_MODE_IT_RISING;
		gpio.Pin=GPIO_PIN_8;
		gpio.Pull=GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &gpio);

		//External interrupts from I2C - INT2
		gpio.Mode=GPIO_MODE_IT_RISING;
		gpio.Pin=GPIO_PIN_10;
		gpio.Pull=GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &gpio);


		//Usart TX pin
		gpio.Mode = GPIO_MODE_AF_PP;
		gpio.Pin = GPIO_PIN_10;
		gpio.Pull = GPIO_NOPULL;
		gpio.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOB, &gpio);

		//Usart Rx pin
		gpio.Mode = GPIO_MODE_AF_PP;
		gpio.Pin = GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA, &gpio);

		//I2C SDA and SCL
		gpio_i2c.Mode = GPIO_MODE_AF_OD;
		gpio_i2c.Pin = GPIO_PIN_6 | GPIO_PIN_7; // SCL, SDA
		gpio_i2c.Pull = GPIO_PULLUP;
		gpio_i2c.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &gpio_i2c);

		//Data sending frequency check
		gpio.Mode=GPIO_MODE_OUTPUT_PP;
		gpio.Pin=GPIO_PIN_9;
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
		//UartHandle.hdmatx= &uart_dma;
		UartHandle.Init.BaudRate = 460800;
		UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
		UartHandle.Init.StopBits = UART_STOPBITS_1;
		UartHandle.Init.Parity = UART_PARITY_NONE;
		UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
		UartHandle.Init.Mode = UART_MODE_TX_RX;
		HAL_UART_Init(&UartHandle);

		HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

		HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

UART_HandleTypeDef UartInstance(void){
	return UartHandle;
}

void EXTI15_10_IRQHandler(void){
	gyr_ready=1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void EXTI9_5_IRQHandler(void){
	acc_ready=1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}




