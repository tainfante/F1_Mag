/*
 * logger.h
 *
 *  Created on: 11.08.2019
 *      Author: Agata
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "uart.h"

UART_HandleTypeDef uart;
UART_HandleTypeDef uartBluetooth;
UART_HandleTypeDef UartLoggerInstance(void);
void sendData(uint8_t *buffer, int size);

void loggerConfig(void);

#endif /* LOGGER_H_ */
