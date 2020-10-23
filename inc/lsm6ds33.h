/*
 * lsm6ds33.h
 *
 *  Created on: 06.07.2019
 *      Author: Agata
 */

#ifndef LSM6DS33_H_
#define LSM6DS33_H_

#define ACC_GYR_ADDR 0xd6

#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define STATUS_REG 0x1e

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

#endif /* LSM6DS33_H_ */
