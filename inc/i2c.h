/*
 * i2c.h
 *
 *  Created on: 27.06.2019
 *      Author: Agata
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "uart.h"

#define ACC_GYR_ADDR 0xd6

#define CTRL1_XL 0x10
#define CTRL2_G 0x11

//gyroscope registers addresses
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27

//accelerometer registers addresses
#define OUTX_L_XL 0x28
#define OUTX_H_XL 0x29
#define OUTY_L_XL 0x30
#define OUTY_H_XL 0x31
#define OUTZ_L_XL 0x32
#define OUTZ_H_XL 0x33

#define INT1_CTRL 0x0d
#define INT2_CTRL 0x0e

#define DATA_AMOUNT 12


void I2C_Config(void);
void Acc_Gyr_Config(uint8_t acc_value, uint8_t gyr_value);
void Read_Sensors_Data(void);
void I2C_Clear_Busy_Flag(void);

#endif /* I2C_H_ */
