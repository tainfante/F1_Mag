/*
 * spi.h
 *
 *  Created on: 06.07.2019
 *      Author: Agata
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "uart.h"
#include "lsm6ds33.h"
#include "logger.h"

#define DATA_AMOUNT 12

void Spi_Config(void);
void Acc_Gyr_Config(void);
void SpiWriteData(uint8_t reg, uint8_t data);
uint8_t SpiReadData(uint8_t reg);
void PrepareBufferToSend(uint8_t reg, uint8_t data);
void Read_Sensors_Data(void);
int WhoAmI(void);
uint8_t CheckRegisters(uint8_t reg);

#endif /* SPI_H_ */
