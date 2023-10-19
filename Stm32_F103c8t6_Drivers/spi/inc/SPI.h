/*
 * SPI.h
 *
 *  Created on: Oct 10, 2023
 *      Author: MOSTAFA MOHAMED SHABAN
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "stm32f103x8.h"
#include  "gpio.h"

struct S_IRQ_SRC
{
	uint8_t TXE :1;
	uint8_t RXNE :1;
	uint8_t ERRI :1;
	uint8_t RESERVED :5;

};

typedef struct
{
	uint16_t Device_Mode        ;
	uint16_t Communication_Mode ;
	uint16_t Frame_Format       ;
	uint16_t DataSize ;
	uint16_t CLKPolarity ;
	uint16_t CLKPhase ;
	uint16_t NSS ;
	uint16_t IRQ_Enable;
	uint16_t SPI_BAUDRATEPRESCALER;
	void(* P_IRQ_Callback)(struct S_IRQ_SRC  irq_src);



}SPI_Config;

/*================================================================
Reference Macros
/*================================================================*/

//Device_Mode
#define SPI_Device_Mode_Slave                     (0x00000000U)
#define SPI_Device_Mode_Master                    (0x1U<<2)

//communication mode
#define SPI_DIRECTION_2LINES                      (0x00000000U)
#define SPI_DIRECTION_2LINES_RXONLY               (0x1U<<10)
#define SPI_DIRECTION_1LINE_recieve_only          (0x1U<<15)
#define SPI_DIRECTION_1LINE_transmit_only         (0x1U<<15)|(0x1U<<14)

//Data size
#define SPI_DataSize_8bit                         (0x00000000U)
#define SPI_DataSize_16bit                        (0x1U<<11)

//frame format
#define SPI_FrameFormat_MSB_transmitted_first     (0x00000000U)
#define SPI_FrameFormat_LSB_transmitted_first     (0x1U<<7)

//Clock polarity
#define SPI_CLK_Polarity_LOW_when_idle            (0x00000000U)
#define SPI_CLK_Polarity_HIGH_when_idle           (0x1U<<1)


//CLKPhase
#define SPI_CLKPhase_1st_Edge_Data_capture        (0x00000000U)
#define SPI_CLKPhase_second_Edge_Data_capture     (0x1U<<0)

//NSS :slave select
//Hardware
#define  SPI_NSS_Hard_slave                       (0x00000000U)
#define  SPI_NSS_Hard_Master_SS_OUTPUT_Enable     (0x1U<<2)
#define  SPI_NSS_Hard_Master_SS_OUTPUT_Disable    ~(0x1U<<2)

//Software
#define  SPI_NSS_Soft_Reset                       (0x1U<<9)
#define  SPI_NSS_Soft_set                         (0x1U<<9) |(0x1U<<8)

//Baud rate prescaler
#define  SPI_BAUDRATEPRESCALER_2                  (0x00000000U)
#define  SPI_BAUDRATEPRESCALER_4                  (0b001U<<3)
#define  SPI_BAUDRATEPRESCALER_8                  (0b010U<<3)
#define  SPI_BAUDRATEPRESCALER_16                 (0b011U<<3)
#define  SPI_BAUDRATEPRESCALER_32                 (0b100U<<3)
#define  SPI_BAUDRATEPRESCALER_64                 (0b101U<<3)
#define  SPI_BAUDRATEPRESCALER_128                (0b110U<<3)
#define  SPI_BAUDRATEPRESCALER_256                (0b111U<<3)

//SPI_IRQ_Enable
#define SPI_IRQ_Enable_N0NE                       (uint32_t)(0)
#define SPI_IRQ_Enable_TXEIE                      (uint32_t)(1<<7)
#define SPI_IRQ_Enable_RXNEIE                     (uint32_t)(1<<6)
#define SPI_IRQ_Enable_ERRIE                      (uint32_t)(1<<5)


enum PollingMechanism
{
pollingenable,
pollingdisable
};


/*================================================================================================================*/
/*APIs supported by MCAL SPI DRIVER*/
/*================================================================================================================*/
void MCAL_SPI_Init(SPI_TypeDef *SPIx,SPI_Config* SPI_Config);
void MCAL_SPI_DeInit(SPI_TypeDef *SPIx);
void MCAL_SPI_GPIO_SetPins(SPI_TypeDef *SPIx);
void MCAL_SPI_SendData(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn);
void MCAL_SPI_RecieveData(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn);
void MCAL_SPI_TX_RX(SPI_TypeDef *SPIx,uint16_t *PtxBuffer,enum PollingMechanism PollingEn);














#endif /* INC_SPI_H_ */
