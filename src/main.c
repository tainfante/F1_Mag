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

extern volatile uint8_t acc_ready;
extern volatile uint8_t gyr_ready;
extern volatile uint8_t dma_transfer_complete;

int main(void)
{
	int res;

	HAL_Init();

	Initialize();
	Uart_Config();
	Tim_Config();
	Adc_Config();
	Dma_Config();

	Spi_Config();

	res = WhoAmI();

	Acc_Gyr_Config(); //setting acc to 208 Hz and +/-4g and gyr to 208 Hz and 1000dps and enabling interrupts

	if(CheckRegisters(INT1_CTRL)!=1){}
	if(CheckRegisters(INT2_CTRL)!=2){}
	if(CheckRegisters(CTRL1_XL)!=88){}
	if(CheckRegisters(CTRL2_G)!=88){}

	while(1){
		if(dma_transfer_complete){
			dma_transfer_complete = 0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			sendADCdata();
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
		}
		if(acc_ready==1 && gyr_ready==1){
			acc_ready = 0;
			gyr_ready = 0;
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
			Read_Sensors_Data();
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
		}
	}
}
