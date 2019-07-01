/*
 * adc.h
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"

ADC_HandleTypeDef adc;

void Adc_Config(void);
ADC_HandleTypeDef ADC_getInstance(void);

#endif /* ADC_H_ */
