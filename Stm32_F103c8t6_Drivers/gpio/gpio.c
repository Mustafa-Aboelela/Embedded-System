/*
 * gpio.c
 *
 *  Created on: Oct 5, 2023
 *      Author: mustafa_3ela
 */

#include  "gpio.h"

uint8_t Get_CRLH_Position(uint16_t PinNumber)
{
	switch (PinNumber)
	{
	case GPIO_PIN_0:
	case GPIO_PIN_8:
		return 0;     break ;
	case GPIO_PIN_1:
	case GPIO_PIN_9:
		return 4;     break ;
	case GPIO_PIN_2 :
	case GPIO_PIN_10:
		return 8;     break ;
	case GPIO_PIN_3:
	case GPIO_PIN_11:
		return 12;    break ;
	case GPIO_PIN_4:
	case GPIO_PIN_12:
		return 16;    break ;
	case GPIO_PIN_5:
	case GPIO_PIN_13:
		return 20;    break ;
	case GPIO_PIN_6:
	case GPIO_PIN_14:
		return 24;    break ;
	case GPIO_PIN_7:
	case GPIO_PIN_15:
		return 28;    break ;
	}
}

void MCAL_GPIO_Init(GPIO_Typedef *GPIOx,GPIO_PIN_Config_t* Pinconfig)
{
  volatile uint32_t* ConfigReg = NULL ;
  uint8_t PIN_CONFIG = 0;

  ConfigReg = (Pinconfig->GPIO_pinNumber < GPIO_PIN_8)?  &GPIOx->CRL : &GPIOx->CRH ;
  /*if mode is output*/
   if ((Pinconfig->GPIO_MODE == GPIO_Mode_OUTPUT_AF_OPENDRAIN) ||  (Pinconfig->GPIO_MODE == GPIO_Mode_OUTPUT_AF_PUSHPULL) ||  (Pinconfig->GPIO_MODE == GPIO_Mode_OUTPUT_OPENDRAIN) || (Pinconfig->GPIO_MODE == GPIO_Mode_OUTPUT_PUSHPULL))
   {
	   PIN_CONFIG =((((Pinconfig->GPIO_MODE -4)<<2)|(Pinconfig->GPIO_OUTPUT_SPEED))& 0x0f);
   }
   else      /*if input*/
   {
	   if ((Pinconfig->GPIO_MODE == GPIO_Mode_ANALOG) ||(Pinconfig->GPIO_MODE == GPIO_Mode_FLOATING))
	   {
		   PIN_CONFIG =((((Pinconfig->GPIO_MODE )<<2)|0x0)&0x0f);
	   }
	   else if (Pinconfig->GPIO_MODE == GPIO_Mode_INPUT_AF)
	   {
		   PIN_CONFIG =((((GPIO_Mode_FLOATING )<<2)|0x0)&0x0f);
	   }
	   else /*pu or pd*/
	   {
		   PIN_CONFIG =((((GPIO_Mode_INPUT_PULLUP )<<2)|0x0)&0x0f);
           if(Pinconfig->GPIO_MODE == GPIO_Mode_INPUT_PULLUP)
           {
        	   GPIOx->ODR |= Pinconfig->GPIO_pinNumber;
           }
           else if(Pinconfig->GPIO_MODE == GPIO_Mode_INPUT_PULLDOWN)
           {
                   	   GPIOx->ODR &= ~ (Pinconfig->GPIO_pinNumber);
           }
	   }
   }
   (*ConfigReg) |=((PIN_CONFIG) << Get_CRLH_Position(Pinconfig->GPIO_pinNumber));


}
void MCAL_GPIO_DeInit(GPIO_Typedef *GPIOx)
{
	if (GPIOx ==GPIOA)
	{
		RCC->APB2RSTR |=  (1<<2);
		RCC->APB2RSTR &= ~(1<<2);
	}
	else if (GPIOx ==GPIOB)
	{
		RCC->APB2RSTR |=  (1<<3);
		RCC->APB2RSTR &= ~(1<<3);
	}
	else if (GPIOx ==GPIOC)
	{
		RCC->APB2RSTR |=  (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
	}
	else if (GPIOx ==GPIOD)
	{
		RCC->APB2RSTR |=  (1<<5);
		RCC->APB2RSTR &= ~(1<<5);
	}
	else if (GPIOx ==GPIOE)
	{
		RCC->APB2RSTR |=  (1<<6);
		RCC->APB2RSTR &= ~(1<<6);
	}


}
uint8_t MCAL_GPIO_ReadPin(GPIO_Typedef *GPIOx,uint16_t PinNumber)
{
uint8_t bitstatus ;
if (((GPIOx->IDR) & PinNumber) != GPIO_PIN_RESET)
{
	bitstatus = GPIO_PIN_SET;

}
else
{
	bitstatus = GPIO_PIN_RESET;

}
return bitstatus ;
}
uint16_t MCAL_GPIO_ReadPort(GPIO_Typedef *GPIOx)
{
	uint16_t port_value ;
	port_value = (uint16_t)(GPIOx->IDR) ;
	return port_value ;
}

void MCAL_GPIO_WritePin(GPIO_Typedef *GPIOx,uint16_t PinNumber,uint8_t Value)
{
	if(Value != GPIO_PIN_RESET)
	{
		(GPIOx->BSRR) =(uint16_t)PinNumber ;
	}
	else
	{
		(GPIOx->BRR) =(uint16_t)PinNumber ;

	}
}
void MCAL_GPIO_WritePort(GPIO_Typedef *GPIOx,uint32_t Value)
{
 GPIOx->ODR = Value ;
}
void MCAL_GPIO_TogglePin(GPIO_Typedef *GPIOx,uint16_t PinNumber)
{
GPIOx->ODR ^= PinNumber ;
}



