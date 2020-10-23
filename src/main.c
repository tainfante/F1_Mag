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
#include "spi.h"
#include "lsm6ds33.h"
#include "logger.h"

extern volatile uint8_t acc_ready;
extern volatile uint8_t gyr_ready;
extern volatile uint8_t dma_transfer_complete;

int main(void)
{
	HAL_Init();
	HAL_Delay(1000);

	Initialize();
	Uart_Config();
	Tim_Config();
	Adc_Config();
	Dma_Config();
	loggerConfig();

	Spi_Config();

	Acc_Gyr_Config(); //setting acc to 26 Hz and +/-2g and gyr to  26 Hz and 245dps and enabling interrupts

	while(1){
		if(dma_transfer_complete){
			dma_transfer_complete = 0;
			sendADCdata();
		}
		if(acc_ready==1 && gyr_ready==1){
			acc_ready = 0;
			gyr_ready = 0;
			Read_Sensors_Data();
		}

	}
}
