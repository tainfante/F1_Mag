/*
 * uart.h
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"

UART_HandleTypeDef UartHandle;
GPIO_InitTypeDef gpio_i2c;

#define CHANGE_BYTE 10
#define STOP_BYTE 11
#define START_BYTE 12

void Uart_Config(void);
UART_HandleTypeDef UartInstance(void);

#endif /* UART_H_ */
