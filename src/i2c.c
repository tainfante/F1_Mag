/*
 * i2c.c
 *
 *  Created on: 27.06.2019
 *      Author: Agata
 */

#include "i2c.h"

I2C_HandleTypeDef i2c;
UART_HandleTypeDef uart;
uint8_t acc_en = 1;
uint8_t gyr_en = 2;

void I2C_Config(void) {

	__HAL_RCC_I2C1_CLK_ENABLE()
	;

	i2c.Instance = I2C1;
	i2c.Init.ClockSpeed = 100000;
	i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&i2c);

}

void I2C_Clear_Busy_Flag(void) {
	i2c.Instance->CR1 &= ~(0x0001);

	//I2C SDA and SCL
	gpio_i2c.Mode = GPIO_MODE_OUTPUT_OD;
	gpio_i2c.Pin = GPIO_PIN_6 | GPIO_PIN_7; // SCL, SDA
	gpio_i2c.Pull = GPIO_PULLUP;
	gpio_i2c.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpio_i2c);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
		__NOP();
	}

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
		__NOP();
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

	while (GPIO_PIN_RESET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
		__NOP();
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

	while (GPIO_PIN_RESET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
		__NOP();
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)) {
		__NOP();
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	while (GPIO_PIN_SET != HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)) {
		__NOP();
	}
	gpio_i2c.Mode=GPIO_MODE_AF_OD;
	HAL_GPIO_Init(GPIOB, &gpio_i2c);
	i2c.Instance->CR1 |= 0x8000;
	i2c.Instance->CR1 &= ~0x8000;
	i2c.Instance->CR1 |= 0x0001;

	HAL_I2C_Init(&i2c);

}

uint8_t I2C_Read_Reg(uint8_t reg) {

	uint8_t value = 0;

	HAL_I2C_Mem_Read(&i2c, ACC_GYR_ADDR, reg, 1, &value, sizeof(value), 1000);

	return value;

}

void Acc_Gyr_Config(uint8_t acc_value, uint8_t gyr_value) {
	HAL_StatusTypeDef status;
	uint8_t set = 0;
	while (set == 0) {
		status = HAL_I2C_Mem_Write(&i2c, ACC_GYR_ADDR, CTRL1_XL, 1, &acc_value,
				sizeof(acc_value), 1000);
		if (HAL_BUSY == status) {
			I2C_Clear_Busy_Flag();
		} else if (HAL_OK == status) {
			set = 1;
		}
	}
	set = 0;
	while (set == 0) {
		status = HAL_I2C_Mem_Write(&i2c, ACC_GYR_ADDR, CTRL2_G, 1, &gyr_value,
				sizeof(gyr_value), 1000);
		if (HAL_BUSY == status) {
			I2C_Clear_Busy_Flag();
		} else if (HAL_OK == status) {
			set = 1;
		}
	}
	set = 0;
	while (set == 0) {
		status = HAL_I2C_Mem_Write(&i2c, ACC_GYR_ADDR, INT1_CTRL, 1, &acc_en,
				sizeof(acc_en), 1000);
		if (HAL_BUSY == status) {
			I2C_Clear_Busy_Flag();
		} else if (HAL_OK == status) {
			set = 1;
		}
	}
	set = 0;
	while (set == 0) {
		status = HAL_I2C_Mem_Write(&i2c, ACC_GYR_ADDR, INT2_CTRL, 1, &gyr_en,
				sizeof(gyr_en), 1000);
		if (HAL_BUSY == status) {
			I2C_Clear_Busy_Flag();
		} else if (HAL_OK == status) {
			set = 1;
		}
	}
}

void Read_Sensors_Data(void) {
	HAL_StatusTypeDef status;
	uint8_t sensors_data[DATA_AMOUNT];

	int i = 0;
	for (i = 0; i < DATA_AMOUNT; i++) {
		status = HAL_I2C_Mem_Read(&i2c, ACC_GYR_ADDR, (0x22+i), 1, &sensors_data[i], sizeof(sensors_data[i]), 1000);
		if (HAL_BUSY == status) {
			I2C_Clear_Busy_Flag();
			i--;
		}
	}
	uart = UartInstance();
	HAL_UART_Transmit(&uart, sensors_data, DATA_AMOUNT, 1000);
}

