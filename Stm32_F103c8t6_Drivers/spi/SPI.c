/*
 * SPI.c
 *
 *  Created on: Oct 10, 2023
 *      Author: MOSTAFA MOHAMED SHABAN
 */

#include "SPI.h"

/*==========================================================================
 *                     Generic Variables
 ===========================================================================*/
SPI_Config* Global_spi_config[2] ={NULL,NULL};


/*==========================================================================
 *                     Generic Macros
 ===========================================================================*/
#define  SPI1_INDEX          0
#define  SPI2_INDEX          1

/*==========================================================================
 *                     APIs
 ===========================================================================*/

void MCAL_SPI_Init(SPI_TypeDef *SPIx,SPI_Config* SPI_Config)
{

	uint16_t temp_reg_CR1 = 0 ;
	uint16_t temp_reg_CR2 = 0 ;


	if (SPIx ==SPI1)
	{
		Global_spi_config[SPI1_INDEX] =  SPI_Config ;
		RCC_SPI1_CLK_EN();


	}else if(SPIx ==SPI2)
	{
		Global_spi_config[SPI2_INDEX] =  SPI_Config ;
		RCC_SPI2_CLK_EN();

	}
	temp_reg_CR1  = (0x1U<<6) ;
	temp_reg_CR1 |= SPI_Config-> Device_Mode ;
	temp_reg_CR1 |= SPI_Config-> CLKPhase ;
	temp_reg_CR1 |= SPI_Config-> CLKPolarity ;
	temp_reg_CR1 |= SPI_Config-> Communication_Mode ;
	temp_reg_CR1 |= SPI_Config-> DataSize ;
	temp_reg_CR1 |= SPI_Config-> Frame_Format ;

	if(SPI_Config ->NSS == SPI_NSS_Hard_Master_SS_OUTPUT_Enable)
	{
		temp_reg_CR2 |= SPI_Config->NSS ;
	}else if(SPI_Config ->NSS == SPI_NSS_Hard_Master_SS_OUTPUT_Disable)
	{
		temp_reg_CR2 &= SPI_Config->NSS ;

	}else
	{
		temp_reg_CR1 |=  SPI_Config->NSS ;
	}
	temp_reg_CR1 |= SPI_Config ->SPI_BAUDRATEPRESCALER;
	if (SPI_Config->IRQ_Enable !=SPI_IRQ_Enable_N0NE)
	{
		temp_reg_CR2 |=SPI_Config->IRQ_Enable;
		if (SPIx == SPI1)
		{
			NVIC_IRQ35_SPI1_Enable;
		}else if (SPIx == SPI2)
		{
			NVIC_IRQ36_SPI2_Enable;
		}
	}
	SPIx ->CR1 = temp_reg_CR1 ;
	SPIx ->CR2 = temp_reg_CR2 ;

}
void MCAL_SPI_DeInit(SPI_TypeDef *SPIx)
{

	if (SPIx == SPI1)
	{
		NVIC_IRQ35_SPI1_Disable ;
		RCC_SPI1_Reset();
	}else if (SPIx == SPI2)
	{
		NVIC_IRQ36_SPI2_Disable ;
		RCC_SPI2_Reset();
	}
}
void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx)
{
	GPIO_PIN_Config_t PinCfg;

	if(SPIx == SPI1)
	{

		//PA4 - NSS
		//PA5 - CLK
		//PA6 - MISO
		//PA7 - MOSI

		if(Global_spi_config[SPI1_INDEX]->Device_Mode == SPI_Device_Mode_Master)
		{
			switch(Global_spi_config[SPI1_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_SS_OUTPUT_Disable:
				//hardware master input floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
				MCAL_GPIO_Init(GPIOA , &PinCfg);
				break;

			case SPI_NSS_Hard_Master_SS_OUTPUT_Enable:
				//hardware master output floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
				PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
				MCAL_GPIO_Init(GPIOA , &PinCfg);
				break;
			}

			//CLK for master
			PinCfg.GPIO_pinNumber = GPIO_PIN_5;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOA , &PinCfg );


			//MISO for master (assuming full duplex)
			PinCfg.GPIO_pinNumber = GPIO_PIN_6;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOA , &PinCfg );

			//MOSI for master
			PinCfg.GPIO_pinNumber = GPIO_PIN_7;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOA , &PinCfg );
		}
		else    //slave
		{
			if(Global_spi_config[SPI1_INDEX]->NSS == SPI_NSS_Hard_slave)
			{
				//hardware slave input floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_4;
				PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
				MCAL_GPIO_Init(GPIOA , &PinCfg );
			}

			//CLK for slave
			PinCfg.GPIO_pinNumber = GPIO_PIN_5;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOA , &PinCfg );

			//MISO for slave (assuming full duplex & point to point)
			PinCfg.GPIO_pinNumber = GPIO_PIN_6;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOA , &PinCfg );

			//MOSI for slave
			PinCfg.GPIO_pinNumber = GPIO_PIN_7;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOA , &PinCfg );

		}
	}
	else if(SPIx == SPI2)
	{
		//PB12 - NSS

		//PB13 - CLK

		//PB14 - MISO

		//PB15 - MOSI


		if(Global_spi_config[SPI2_INDEX]->Device_Mode == SPI_Device_Mode_Master)
		{
			switch(Global_spi_config[SPI1_INDEX]->NSS)
			{
			case SPI_NSS_Hard_Master_SS_OUTPUT_Disable:
				//hardware master input floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
				MCAL_GPIO_Init(GPIOB , &PinCfg );
				break;

			case SPI_NSS_Hard_Master_SS_OUTPUT_Enable:
				//hardware master output floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
				PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
				MCAL_GPIO_Init(GPIOB , &PinCfg );
				break;
			}

			//CLK for master
			PinCfg.GPIO_pinNumber = GPIO_PIN_13;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg );


			//MISO for master (assuming full duplex)
			PinCfg.GPIO_pinNumber = GPIO_PIN_14;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOB , &PinCfg );

			//MOSI for master
			PinCfg.GPIO_pinNumber = GPIO_PIN_15;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg );
		}
		else    //slave
		{
			if(Global_spi_config[SPI2_INDEX]->NSS == SPI_NSS_Hard_slave)
			{
				//hardware slave input floating
				PinCfg.GPIO_pinNumber = GPIO_PIN_12;
				PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
				MCAL_GPIO_Init(GPIOB , &PinCfg );
			}

			//CLK for slave
			PinCfg.GPIO_pinNumber = GPIO_PIN_13;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOB , &PinCfg );

			//MISO for slave (assuming full duplex & point to point)
			PinCfg.GPIO_pinNumber = GPIO_PIN_14;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_PUSHPULL;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg );

			//MOSI for slave
			PinCfg.GPIO_pinNumber = GPIO_PIN_15;
			PinCfg.GPIO_MODE = GPIO_Mode_FLOATING;
			MCAL_GPIO_Init(GPIOB , &PinCfg );

		}

	}
}

void MCAL_SPI_SendData(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn)
{
	if (PollingEn == pollingenable)
		while(((SPIx->SR >>1)&1) == 0);           //Transmit buffer is empty
	SPIx->DR = *PtxBuffer;
}
void MCAL_SPI_RecieveData(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn)
{
	if(PollingEn == pollingenable)
		while(((SPIx->SR >>0)&1) == 0);    //receive buffer is full

	*PtxBuffer = SPIx->DR;
}
void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn)
{
	if (PollingEn == pollingenable)
		while(((SPIx->SR >>1)&1) == 0);           //Transmit buffer is empty
	SPIx->DR = *PtxBuffer;
	if(PollingEn == pollingenable)
		while(((SPIx->SR >>0)&1) == 0);    //receive buffer is full

	*PtxBuffer = SPIx->DR;

}

void SPI1_IRQHandler(void)
{
	struct S_IRQ_SRC irq_src ;
	irq_src.TXE =  ((SPI1->SR>>1)&1);
	irq_src.RXNE = ((SPI1->SR>>0)&1);
	irq_src.ERRI = ((SPI1->SR>>4)&1);

	Global_spi_config[SPI1_INDEX]->P_IRQ_Callback(irq_src);
}

void SPI2_IRQHandler(void)
{
	struct S_IRQ_SRC irq_src ;
	irq_src.TXE = ((SPI2->SR>>1)&1);
	irq_src.RXNE = ((SPI2->SR>>0)&1);
	irq_src.ERRI = ((SPI2->SR>>4)&1);

	Global_spi_config[SPI2_INDEX]->P_IRQ_Callback(irq_src);
}

