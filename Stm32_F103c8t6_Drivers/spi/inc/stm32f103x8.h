/*
 * stm32f103x8.h
 *
 *  Created on: Oct 5, 2023
 *      Author: mustafa_3ela
 */

#ifndef INC_STM32F103X8_H_
#define INC_STM32F103X8_H_
#include <stdint.h>

#define NULL  ((void*)0)


#define  FLASH_MEMORY_BASE                                    0x08000000UL
#define  SYSTEM_MEMORY_BASE                                   0x1FFFF000UL
#define  SRAM_MEMORY_BASE                                     0x20000000UL

#define  Peripherals_BASE                                     0x40000000UL

#define  Cortex_M3_Internal_Peripherals_BASE                  0x08000000UL

//NVIC register map
#define NVIC_Base					(0xE000E100UL)
#define NVIC_ISER0					*(volatile uint32_t *) (NVIC_Base + 0x0 )
#define NVIC_ISER1					*(volatile uint32_t *)(NVIC_Base + 0x4)
#define NVIC_ISER2					*(volatile uint32_t *)(NVIC_Base + 0x8)
#define NVIC_ICER0					*(volatile uint32_t *)(NVIC_Base + 0x80)
#define NVIC_ICER1					*(volatile uint32_t *)(NVIC_Base + 0x84)
#define NVIC_ICER2					*(volatile uint32_t *)(NVIC_Base + 0x88)
/***************BARE ADDRESS FOR AHB***********************/

/* RCC*/
#define  RCC_BASE                                             0x40021000UL

/***************BARE ADDRESS FOR APB2***********************/

/*GPIO*/
#define  GPIOA_BASE                                             0x40010800UL
#define  GPIOB_BASE                                             0x40010C00UL
#define  GPIOC_BASE                                             0x40011000UL
#define  GPIOD_BASE                                             0x40011400UL
#define  GPIOE_BASE                                             0x40011800UL

/*EXTI*/
#define  EXTI_BASE                                              0x40010400UL

/*AFIO*/
#define  AFIO_BASE                                              0x40010000UL

#define  SPI1_BASE                                              0x40013000UL


/***************BARE ADDRESS FOR APB1***********************/

#define  SPI2_BASE                                              0x40013800UL


/*========================================================================================*/

/*peripheral registers :GPIO*/

typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;

} GPIO_Typedef;
/*peripheral registers :RCC*/

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

} RCC_Typedef;

/*peripheral registers :EXTI*/

typedef struct {
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RSTR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;

} EXTI_Typedef;

/*peripheral registers :RCC*/

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

} RCC_TypeDef;
/*peripheral registers :AFIO*/

typedef struct {
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	uint32_t RESERVED;
	volatile uint32_t MAPR2;

} AFIO_Typedef;

/*peripheral registers :SPI*/

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;

} SPI_TypeDef;

/***********Peripherals instants*************/

#define GPIOA      ((GPIO_Typedef *)GPIOA_BASE)
#define GPIOB      ((GPIO_Typedef *)GPIOB_BASE)
#define GPIOC      ((GPIO_Typedef *)GPIOC_BASE)
#define GPIOD      ((GPIO_Typedef *)GPIOD_BASE)
#define GPIOE      ((GPIO_Typedef *)GPIOE_BASE)

#define RCC        ((RCC_TypeDef  *)  RCC_BASE)

#define AFIO       ((AFIO_Typedef *) AFIO_BASE)

#define EXTI       ((EXTI_Typedef *) EXTI_BASE)

#define SPI1      ((SPI_TypeDef  *) SPI1_BASE)
#define SPI2      ((SPI_TypeDef  *) SPI2_BASE)


/*=============================================================================================================*/
//IVT
////////////////
//EXTI
#define SPI1_IRQ                 35
#define SPI2_IRQ                 36
/******Clock enable macros*******/

#define  RCC_GPIOA_CLK_EN()     (RCC->APB2ENR |= 1<<2)
#define  RCC_GPIOB_CLK_EN()     (RCC->APB2ENR |= 1<<3)
#define  RCC_GPIOC_CLK_EN()     (RCC->APB2ENR |= 1<<4)
#define  RCC_GPIOD_CLK_EN()     (RCC->APB2ENR |= 1<<5)
#define  RCC_GPIOE_CLK_EN()     (RCC->APB2ENR |= 1<<6)

#define  RCC_AFIO_CLK_EN()    (RCC->APB2ENR |= 1<<0)
#define  RCC_SPI1_CLK_EN()    (RCC->APB2ENR |= 1<<12)
#define  RCC_SPI2_CLK_EN()    (RCC->APB1ENR |= 1<<14)

// RCC Reset SPI
#define RCC_SPI1_Reset()	( RCC->APB2RSTR |= (1<<12) )
#define RCC_SPI2_Reset()	( RCC->APB1RSTR |= (1<<14) )



//*_*_*_*__*_*_*_*_*_*__*_________________
//NVIC IRQ en/dis ableMACROS:
//*_*_*_*_*_*_*_*_*_*_*_*_________________

#define NVIC_IRQ35_SPI1_Enable      (NVIC_ISER1 |= 1<<(SPI1_IRQ-32))
#define NVIC_IRQ36_SPI2_Enable      (NVIC_ISER1 |= 1<<(SPI2_IRQ-32))
#define NVIC_IRQ35_SPI1_Disable     (NVIC_ICER1 |= 1<<(SPI1_IRQ-32))
#define NVIC_IRQ36_SPI2_Disable     (NVIC_ICER1 |= 1<<(SPI2_IRQ-32))




#endif /* INC_STM32F103X8_H_ */
