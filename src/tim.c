/*
 * tim.c
 *
 *  Created on: 26.06.2019
 *      Author: Agata
 */

#include "tim.h"

void Tim_Config(void) {

	__HAL_RCC_TIM3_CLK_ENABLE();

	TIM_MasterConfigTypeDef sMasterConfig;
	//TIM_ClockConfigTypeDef sClockSourceConfig;

	//sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	//HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig);

	htim.Instance = TIM3;
	htim.Init.Period = 21333;
	htim.Init.Prescaler = 1;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.RepetitionCounter = 0x0;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	HAL_TIM_Base_Init(&htim);

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	HAL_TIM_Base_Start(&htim);

}
