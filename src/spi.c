/*
 * spi.c
 *
 *  Created on: 06.07.2019
 *      Author: Agata
 */

#include "spi.h"

SPI_HandleTypeDef spi;
UART_HandleTypeDef uart;
uint8_t acc_en = 1;
uint8_t gyr_en = 2;
uint8_t acc_value = 0x58;
uint8_t gyr_value = 0x58;

uint8_t buffer_to_send[2];

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

	SpiWriteData(INT1_CTRL, acc_en);
	SpiWriteData(INT2_CTRL, gyr_en);
	SpiWriteData(CTRL1_XL, acc_value);
	SpiWriteData(CTRL2_G, gyr_value);

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

void Read_Sensors_Data(void) {

	uint8_t sensors_data[DATA_AMOUNT];

	int i = 0;
	for (i = 0; i < DATA_AMOUNT; i++) {
		sensors_data[i] = SpiReadData(0x22+i);
	}

	uart = UartInstance();
	HAL_UART_Transmit(&uart, sensors_data, DATA_AMOUNT,1000);
}

