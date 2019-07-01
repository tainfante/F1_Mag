/*
 * adc.c
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#include "adc.h"



void Adc_Config(void) {

	__HAL_RCC_ADC1_CLK_ENABLE();

	RCC_PeriphCLKInitTypeDef adc_clk;
	adc_clk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	adc_clk.AdcClockSelection = RCC_ADCPCLK2_DIV8;
	HAL_RCCEx_PeriphCLKConfig(&adc_clk);

	adc.Instance = ADC1;
	adc.Init.ContinuousConvMode = DISABLE;
	adc.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T3_TRGO;
	//adc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	//adc.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	adc.Init.ScanConvMode = ADC_SCAN_ENABLE;
	adc.Init.NbrOfConversion = 2;
	adc.Init.DiscontinuousConvMode = DISABLE;
	//adc.Init.DMAContinuousRequests = ENABLE;

	HAL_ADC_Init(&adc);

	ADC_ChannelConfTypeDef adc_ch;
	adc_ch.Channel = ADC_CHANNEL_0;
	adc_ch.Rank = 1;
	adc_ch.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	adc_ch.Channel = ADC_CHANNEL_1;
	adc_ch.Rank = 2;
	adc_ch.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	HAL_ADC_ConfigChannel(&adc, &adc_ch);

	HAL_ADCEx_Calibration_Start(&adc);

}

ADC_HandleTypeDef ADC_getInstance(void){
	return adc;
}


