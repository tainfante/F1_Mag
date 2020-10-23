/*
 * dma.c
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#include "dma.h"
#include "adc.h"
#include "uart.h"

#define ADC_DATA_BYTE 13
#define BUFFER_SIZE 4

DMA_HandleTypeDef dma;

UART_HandleTypeDef uart;

uint16_t adcValue[2];
uint8_t ADC1buffer[BUFFER_SIZE];
uint8_t adc_data_to_send[11];

volatile uint8_t dma_transfer_complete = 0;

void Dma_Config(void) {

	__HAL_RCC_DMA1_CLK_ENABLE();

	dma.Instance = DMA1_Channel1;
	dma.Init.Direction = DMA_PERIPH_TO_MEMORY;
	dma.Init.PeriphInc = DMA_PINC_DISABLE;
	dma.Init.MemInc = DMA_MINC_ENABLE;
	dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	dma.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	dma.Init.Mode = DMA_CIRCULAR;
	dma.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&dma);

	__HAL_LINKDMA(&adc, DMA_Handle, dma);
	__HAL_DMA_ENABLE_IT(&dma, DMA_IT_TC);

	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	HAL_ADC_Start_DMA(&adc, (uint32_t*) &adcValue, 2);


	uart_dma.Instance=DMA1_Channel2;
	uart_dma.Init.Direction=DMA_MEMORY_TO_PERIPH;
	uart_dma.Init.PeriphInc=DMA_PINC_DISABLE;
	uart_dma.Init.MemInc=DMA_MINC_ENABLE;
	uart_dma.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
	uart_dma.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
	uart_dma.Init.Mode=DMA_CIRCULAR;
	uart_dma.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&uart_dma);

	__HAL_DMA_ENABLE(&uart_dma);

	i2c_dma.Instance=DMA1_Channel7;
	i2c_dma.Init.Direction=DMA_PERIPH_TO_MEMORY;
	i2c_dma.Init.PeriphInc=DMA_PINC_DISABLE;
	i2c_dma.Init.MemInc=DMA_MINC_DISABLE;
	i2c_dma.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
	i2c_dma.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
	i2c_dma.Init.Mode=DMA_CIRCULAR;
	i2c_dma.Init.Priority = DMA_PRIORITY_HIGH;
	HAL_DMA_Init(&i2c_dma);

	__HAL_DMA_ENABLE(&i2c_dma);


}

uint8_t CodeAdcData(uint8_t *buffer){

	uint8_t size=7;
	adc_data_to_send[0]=START_BYTE;
	adc_data_to_send[1]=13;
	uint8_t k = 0;

	for(int i=0; i<4; i++){
		if((buffer[i]!=CHANGE_BYTE) && (buffer[i]!=STOP_BYTE) && (buffer[i]!=START_BYTE)){
			adc_data_to_send[2+i+k]=buffer[i];
		}
		else{
			adc_data_to_send[2+i+k]=CHANGE_BYTE;
			k++;
			size++;
			adc_data_to_send[2+i+k]= ~buffer[i];

		}
		if(i==3){
				adc_data_to_send[2+i+k+1]=STOP_BYTE;
			}
	}
	return size;
}

void sendADCdata(void) {

	uint8_t ADC1buffer[BUFFER_SIZE];
	uint8_t adc_frame_size;

	ADC1buffer[0] = (uint8_t) (adcValue[0] >> 8);
	ADC1buffer[1] = (uint8_t) (adcValue[0]);
	ADC1buffer[2] = (uint8_t) (adcValue[1] >> 8);
	ADC1buffer[3] = (uint8_t) (adcValue[1]);

	adc_frame_size=CodeAdcData(ADC1buffer);

	uart=UartInstance();

	HAL_UART_Transmit(&uart, adc_data_to_send, adc_frame_size, 1000);

}

void DMA1_Channel1_IRQHandler(void){
	HAL_DMA_IRQHandler(&dma);
	dma_transfer_complete=1;
}
