/**
 ******************************************************************************
 * @file           : main.c
 * @author         : MUSTAFA MOHAMED SHABAN
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "stm32f103x8.h"
#include "gpio.h"

void clock_init()
{
	RCC_GPIOA_CLK_EN() ;
	RCC_GPIOB_CLK_EN() ;
}

void GPIO_Init()
{

	GPIO_PIN_Config_t pinCfg ;
	pinCfg.GPIO_pinNumber = GPIO_PIN_1 ;
	pinCfg.GPIO_MODE = GPIO_Mode_FLOATING ;
	MCAL_GPIO_Init(GPIOA, &pinCfg);

	pinCfg.GPIO_pinNumber = GPIO_PIN_1 ;
	pinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_PUSHPULL ;
	pinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ ;
	MCAL_GPIO_Init(GPIOB, &pinCfg);

	pinCfg.GPIO_pinNumber = GPIO_PIN_13 ;
	pinCfg.GPIO_MODE = GPIO_Mode_FLOATING ;
	MCAL_GPIO_Init(GPIOA, &pinCfg);

	pinCfg.GPIO_pinNumber = GPIO_PIN_13 ;
	pinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_PUSHPULL ;
	pinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ ;
	MCAL_GPIO_Init(GPIOB, &pinCfg);

}

void wait_ms(uint32_t time)
{
	uint32_t i,j;
	for(i=0;i<time;i++)
	{
		for (j=0;j<255;j++);
	}
}


int main(void)
{
	clock_init();
	GPIO_Init();
	while(1)
	{
		//pa1 pu
		if (MCAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==0)  //PRESS
		{
			MCAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
			while(MCAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)==0); //single press
		}
		//pa13 is pd
		if (MCAL_GPIO_ReadPin(GPIOA,GPIO_PIN_13)==1) //multi press
		{
			MCAL_GPIO_TogglePin(GPIOB,GPIO_PIN_13);

		}
		wait_ms(1);
	}

return 0;
}











