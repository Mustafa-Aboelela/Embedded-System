/*
 * gpio.h
 *
 *  Created on: Oct 5, 2023
 *      Author: mustafa_3ela
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f103x8.h"

/*========================================================================================================*/

typedef struct {
	uint16_t GPIO_pinNumber;
	uint8_t GPIO_MODE;
	uint8_t GPIO_OUTPUT_SPEED; /*for output mode only */

} GPIO_PIN_Config_t;

/*========================================================================================================*/
/******macros configrations********/
/*pin state*/

#define  GPIO_PIN_SET                     1
#define  GPIO_PIN_RESET                   0
/*GPIO RETURN LOCK*/
#define  GPIO_RETURN_LOCK_ENABLED         1
#define  GPIO_RETURN_LOCK_ERROR           0

/*pin number */
#define GPIO_PIN_0                       ((uint16_t)0x0001)   /*pin 0 selected 01*/
#define GPIO_PIN_1                       ((uint16_t)0x0002)   /*pin 1 selected  10*/
#define GPIO_PIN_2                       ((uint16_t)0x0004)   /*pin 2 selected  100*/
#define GPIO_PIN_3                       ((uint16_t)0x0008)   /*pin 3 selected  1000*/
#define GPIO_PIN_4                       ((uint16_t)0x0010)   /*pin 4 selected*/
#define GPIO_PIN_5                       ((uint16_t)0x0020)   /*pin 5 selected*/
#define GPIO_PIN_6                       ((uint16_t)0x0040)   /*pin 6 selected*/
#define GPIO_PIN_7                       ((uint16_t)0x0080)   /*pin 7 selected*/
#define GPIO_PIN_8                       ((uint16_t)0x0100)   /*pin 8 selected*/
#define GPIO_PIN_9                       ((uint16_t)0x0200)   /*pin 9 selected*/
#define GPIO_PIN_10                      ((uint16_t)0x0400)   /*pin 10 selected*/
#define GPIO_PIN_11                      ((uint16_t)0x0800)   /*pin 11 selected*/
#define GPIO_PIN_12                      ((uint16_t)0x1000)   /*pin 12 selected*/
#define GPIO_PIN_13                      ((uint16_t)0x2000)   /*pin 13 selected*/
#define GPIO_PIN_14                      ((uint16_t)0x4000)   /*pin 14 selected*/
#define GPIO_PIN_15                      ((uint16_t)0x8000)   /*pin 15 selected*/
#define GPIO_PIN_ALL                     ((uint16_t)0xFFFF)   /*all pins selected*/

#define GPIO_PIN_MASK                    0x0000FFFFu          /*pin mask for assert test*/

/*pin mode */
#define GPIO_Mode_ANALOG                     0x00000000u
#define GPIO_Mode_FLOATING                   0x00000001u
#define GPIO_Mode_INPUT_PULLUP               0x00000002u
#define GPIO_Mode_INPUT_PULLDOWN             0x00000003u
#define GPIO_Mode_OUTPUT_PUSHPULL            0x00000004u
#define GPIO_Mode_OUTPUT_OPENDRAIN           0x00000005u
#define GPIO_Mode_OUTPUT_AF_PUSHPULL         0x00000006u
#define GPIO_Mode_OUTPUT_AF_OPENDRAIN        0x00000007u
#define GPIO_Mode_INPUT_AF                   0x00000008u

/*output speed */

#define  GPIO_SPEED_10_MHZ                   0x00000001u      /*Output mode, max speed 10 MHz*/
#define  GPIO_SPEED_2_MHZ                    0x00000002u      /*Output mode, max speed 2  MHz*/
#define  GPIO_SPEED_50_MHZ                   0x00000003u      /*Output mode, max speed 50 MHz*/

/*================================================================================================================
                                    APIs supported by MCAL GPIO DRIVER
/*================================================================================================================*/

void MCAL_GPIO_Init(GPIO_Typedef *GPIOx, GPIO_PIN_Config_t *Pinconfig);
void MCAL_GPIO_DeInit(GPIO_Typedef *GPIOx);
uint8_t MCAL_GPIO_ReadPin(GPIO_Typedef *GPIOx, uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort(GPIO_Typedef *GPIOx);
void MCAL_GPIO_WritePin(GPIO_Typedef *GPIOx, uint16_t PinNumber, uint8_t Value);
void MCAL_GPIO_WritePort(GPIO_Typedef *GPIOx, uint32_t Value);
void MCAL_GPIO_TogglePin(GPIO_Typedef *GPIOx, uint16_t PinNumber);

#endif /* INC_GPIO_H_ */
