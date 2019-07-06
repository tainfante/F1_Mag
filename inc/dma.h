/*
 * dma.h
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"

uint8_t dma_transfer_complete;
DMA_HandleTypeDef uart_dma;
DMA_HandleTypeDef i2c_dma;

void Dma_Config(void);
void sendADCdata(void);
void DMA2_Stream0_IRQHandler(void);

#endif /* DMA_H_ */
