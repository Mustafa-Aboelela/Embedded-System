/*
 * SPI.h
 *
 *  Created on: Oct  10, 2023
 *      Author: MOSTAFA MOHAMED SHABAN
 */

#ifndef INC_USART_H_
#define INC_USART_H_
#include "gpio.h"
#include "RCC.h"

typedef struct{

	uint8_t       USART_MODE;       //specify TX/RX enable/disable
								    //this parameter must be set based on @ref USART MODE DEFINE

	uint32_t      Baud_rate;         //specify the baud rate
								    //this parameter must be set based on @ref USART BAUD_RATE DEFINE

	uint8_t       Payload_length;    //specify the number of data bits transmitted or received in a frame
    								//this parameter must be set based on @ref USART PAYLOAD_LENGTH DEFINE

	uint8_t       Parity;           //specify parity mode
									//@ref USART PARITY DEFINE

	uint8_t      Stop_bits;         //@ref USART STOP BITS DEFINE


	uint8_t      HW_Flow_Control;   //specify where the flow control is enabled or disabled
									//@ref USART HW FLOW CONTROL DEFINE

	uint8_t      IRQ_Enable;       //@ref USART IRQ ENABLE DEFINE


	void(*P_IRQ_Callback)(void);    //set the function which will be called when the IRQ happen

}USART_config;



//=================================================================================
//Macros Configuration References
//=================================================================================

//@ref USART MODE DEFINE

#define USART_MODE_TX       (uint32_t)(1<<3)
#define USART_MODE_RX       (uint32_t)(1<<2)
#define USART_MODE_TX_RX    ((uint32_t)(1<<3 | 1<<2))

//@ref USART BAUD_RATE DEFINE

#define  USART_BaudRate_2400           2400
#define  USART_BaudRate_9600           9600
#define  USART_BaudRate_19200          19200
#define  USART_BaudRate_57600          57600
#define  USART_BaudRate_115200         115200
#define  USART_BaudRate_230400         230400
#define  USART_BaudRate_460800         460800
#define  USART_BaudRate_921600         921600
#define  USART_BaudRate_2250000        2250000
#define  USART_BaudRate_4500           4500000

//@ref USART PAYLOAD_LENGTH DEFINE

#define USART_Payload_Length_8                  (uint32_t)(0)
#define USART_Payload_Length_9                  (uint32_t)(1<<12)


//@ref USART PARITY DEFINE
#define USART_Parity_NONE                       (uint32_t)(0)
#define USART_Parity_EVEN                       (uint32_t)(1<<10)
#define USART_Parity_ODD                        ((uint32_t)(1<<10 | 1<<9))


//@ref USART STOP BITS DEFINE
#define USART_Stop_bit_1                        (uint32_t)(0)
#define USART_Stop_bit_half                     (uint32_t)(1<<12)
#define USART_Stop_bit_one_and_half             (uint32_t)(3<<12)
#define USART_Stop_bit_2                        (uint32_t)(2<<12)

//@ref USART HW FLOW CONTROL DEFINE
#define USART_Flow_Control_CTS_ENABLE           (uint32_t)(1<<9)
#define USART_Flow_Control_RTS_ENABLE           (uint32_t)(1<<8)
#define USART_Flow_Control_RTS_CTS_ENABLE       ((uint32_t)(1<<8|1<<9))
#define USART_Flow_Control_NONE                 (uint32_t)(0)


//@ref USART IRQ ENABLE DEFINE

#define USART_IRQ_ENABLE_NONE                   (uint32_t)(0)
#define USART_IRQ_ENABLE_TXE                    (uint32_t)(1<<7)  //Transmit data register empty
#define USART_IRQ_ENABLE_TC                     (uint32_t)(1<<6)  //Transmission complete
#define USART_IRQ_ENABLE_RXNEIE                 (uint32_t)(1<<5)  //Received data ready to be read (or overrun error detected)
#define USART_IRQ_ENABLE_PE                     (uint32_t)(1<<8)  //Parity error


enum Polling_Mechanism{
	enable,
	disable
};



//@ref Baud rate calculation
#define USARTDIV(_PCLK_, _BAUD_)							(uint32_t) (_PCLK_/(16 * _BAUD_ ))
#define USARTDIV_MUL100(_PCLK_, _BAUD_)						(uint32_t) ( (25 * _PCLK_ ) / (4  * _BAUD_ ))
#define Mantissa_MUL100(_PCLK_, _BAUD_)						(uint32_t) (USARTDIV(_PCLK_, _BAUD_) * 100)
#define Mantissa(_PCLK_, _BAUD_)							(uint32_t) (USARTDIV(_PCLK_, _BAUD_) )
#define DIV_Fraction(_PCLK_, _BAUD_)						(uint32_t) (((USARTDIV_MUL100(_PCLK_, _BAUD_) -  Mantissa_MUL100(_PCLK_, _BAUD_) ) * 16 ) / 100 )
#define USART_BRR_Register(_PCLK_, _BAUD_)					(( Mantissa (_PCLK_, _BAUD_) ) <<4 )|( (DIV_Fraction(_PCLK_, _BAUD_)) & 0xF )
//-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-
//                            APIS Supported by "MCAL USART DRIVER"
//-*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*-*-

void MCAL_USART_Init(USART_TypeDef* USARTx, USART_config* USART_cfg);
void MCAL_USART_DeInit(USART_TypeDef* USARTx);
void MCAL_USART_GPIO_Set_Pins(USART_TypeDef* USARTx);

void MCAL_USART_Send_Data(USART_TypeDef* USARTx, uint16_t* Data, enum Polling_Mechanism PollingEN );
void MCAL_USART_Receive_Data(USART_TypeDef* USARTx, uint16_t* Data, enum Polling_Mechanism PollingEN );

void MCAL_USART_Wait_TC(USART_TypeDef* USARTx);


#endif
