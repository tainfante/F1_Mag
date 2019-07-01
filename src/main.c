/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f1xx.h"
#include "stm32f1xx_nucleo.h"
#include "init.h"
#include "dma.h"
#include "adc.h"
#include "tim.h"
#include "uart.h"
#include "i2c.h"

int main(void)
{
	HAL_Init();

	Initialize();
	Tim_Config();
	Adc_Config();
	Dma_Config();
	I2C_Config();
	Uart_Config();


	Acc_Gyr_Config(0x58, 0x58); //setting acc to 208 Hz and +/-4g and gyr to 208 Hz and 1000dps and enabling interrupts

	while(1){
		/*if(dma_transfer_complete){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			sendADCdata();
			dma_transfer_complete=0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
		}*/
//		if(acc_ready==1 && gyr_ready==1){
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			Read_Sensors_Data();
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			acc_ready=0;
			gyr_ready=0;
			HAL_Delay(100);
//		}
	}
}
