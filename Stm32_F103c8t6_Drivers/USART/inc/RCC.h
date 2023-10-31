/*
 * EXTI.h
 *
 *  Created on: Oct 28, 2023
 *      Author: mustafa_3ela
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_


#include "stm32f103x8.h"
#include "gpio.h"

#define HSI_CLK (uint32_t)8000000
#define HSE_CLK (uint32_t)16000000


uint32_t MCAL_RCC_GetSYSCLKFreq(void);
uint32_t MCAL_RCC_GetHCLKFreq(void);
uint32_t MCAL_RCC_GetPCLK1Freq(void);
uint32_t MCAL_RCC_GetPCLK2Freq(void);


#endif
