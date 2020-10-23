/*
 * spi.c
 *
 *  Created on: 06.07.2019
 *      Author: Agata
 */

#include "spi.h"
#include "logger.h"

SPI_HandleTypeDef spi;
UART_HandleTypeDef uart;
uint8_t acc_en = 1;
uint8_t gyr_en = 2;
uint8_t acc_value = 0x22;
uint8_t gyr_value = 0x20;

uint8_t buffer_to_send[2];
uint8_t acc_data_to_send[27];

void Spi_Config(void) {

	__HAL_RCC_SPI1_CLK_ENABLE()
	;

	spi.Instance = SPI1;
	spi.Init.Mode = SPI_MODE_MASTER;
	spi.Init.NSS = SPI_NSS_SOFT;
	spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64; // APB2 f=64MHz
	spi.Init.Direction = SPI_DIRECTION_2LINES;
	spi.Init.CLKPhase = SPI_PHASE_2EDGE;
	spi.Init.CLKPolarity = SPI_POLARITY_HIGH;
	spi.Init.DataSize = SPI_DATASIZE_8BIT;
	spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi.Init.TIMode = SPI_TIMODE_DISABLE;
	spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	HAL_SPI_Init(&spi);

}

int WhoAmI(void) {

	int res;

	uint8_t txBuf[2] = {0x8f, 0x00};
	uint8_t rxBuf[2] = {0x00, 0x00};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	res = HAL_SPI_TransmitReceive(&spi, txBuf, rxBuf, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	return res;
}

uint8_t CheckRegisters(uint8_t reg){

	uint8_t txBuf[2] = {(0x80|reg), 0x00};
	uint8_t rxBuf[2] = {0x00, 0x00};
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&spi, txBuf, rxBuf, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	return rxBuf[1];

}

void Acc_Gyr_Config(void) {
	SpiWriteData(CTRL1_XL, acc_value);
	SpiWriteData(CTRL2_G, gyr_value);
	SpiWriteData(INT1_CTRL, acc_en);
	SpiWriteData(INT2_CTRL, gyr_en);
}

void SpiWriteData(uint8_t reg, uint8_t data){

	PrepareBufferToSend(reg, data);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&spi, buffer_to_send, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

}

uint8_t SpiReadData(uint8_t reg){

	uint8_t received_data[2];
	PrepareBufferToSend((reg|0x80), 0x00);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&spi, buffer_to_send, received_data, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);

	return received_data[1];

}

void PrepareBufferToSend(uint8_t reg, uint8_t data) {

	buffer_to_send[0] = reg;
	buffer_to_send[1] = data;

}

/*uint8_t IsDataReady(void){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&spi, txBuf, rxBuf, 2, 1000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	return 0;
}*/

uint8_t CodeData(uint8_t *buffer){

	uint8_t size=15;
	acc_data_to_send[0]=START_BYTE;
	acc_data_to_send[1]=15;
	uint8_t k = 0;

	for(int i=0; i<12; i++){
		if((buffer[i]!=CHANGE_BYTE) && (buffer[i]!=STOP_BYTE) && (buffer[i]!=START_BYTE)){
			acc_data_to_send[2+i+k]=buffer[i];
		}
		else{
			acc_data_to_send[2+i+k]=CHANGE_BYTE;
			k++;
			size++;
			acc_data_to_send[2+i+k]= ~buffer[i];

		}
		if(i==11){
				acc_data_to_send[2+i+k+1]=STOP_BYTE;
			}
	}
	return size;
}

void Read_Sensors_Data(void) {

	uint8_t sensors_data[DATA_AMOUNT];
	uint16_t sensors_data_16[6];
	uint8_t size_of_data_to_send;

	sensors_data[0] = SpiReadData(0x29);
	sensors_data[1] = SpiReadData(0x28);
	sensors_data[2] = SpiReadData(0x2b);
	sensors_data[3] = SpiReadData(0x2a);
	sensors_data[4] = SpiReadData(0x2d);
	sensors_data[5] = SpiReadData(0x2c);
	sensors_data[6] = SpiReadData(0x23);
	sensors_data[7] = SpiReadData(0x22);
	sensors_data[8] = SpiReadData(0x25);
	sensors_data[9] = SpiReadData(0x24);
	sensors_data[10] = SpiReadData(0x27);
	sensors_data[11] = SpiReadData(0x26);

	size_of_data_to_send=CodeData(sensors_data);

	uart = UartInstance();
	HAL_UART_Transmit(&uart, acc_data_to_send, size_of_data_to_send,1000);

}

