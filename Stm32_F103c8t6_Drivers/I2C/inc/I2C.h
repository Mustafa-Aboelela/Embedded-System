/*
 * I2C.h
 *
 *  Created on: Nov 3, 2023
 *      Author: mustafa_mohamed
 */

#ifndef INC_I2C_C_
#define INC_I2C_C_

#include "stm32f103x8.h"
#include "GPIO.h"
#include "RCC.h"


struct S_I2C_Device_Address
{
	uint16_t Enable_dual_address;
	uint16_t Primary_Slave_address;
	uint16_t Secondary_slave_address;
	uint32_t I2C_Addressing_slave_mode;   //can be a value of @ref I2C_Addressing_Slave
};

typedef enum
{
	I2C_EV_STOP,
	I2C_ERORR_AF,
	I2C_EV_ADDR_Matched,
	I2C_EV_DATA_REQ,
	I2C_EV_DATA_RCV
}Slave_state;

typedef enum
{
	with_stop,
	without_stop
}stop_condtion;

typedef enum
{
	Start,
	repeated_start
}Start_condtion;

typedef struct
{
	uint32_t clk_speed ;   //specify clk speed
	//this parameter can be a value of @ref I2C_SClk
	uint32_t Stretch_mode ;  //specify stretch mode
	//this parameter can be a value of @ref I2C_StretchMode

	uint32_t I2C_Mode ;  //specify I2C mode
	                    //this parameter can be a value of @ref I2C_Mode

	struct S_I2C_Device_Address I2C_Device_address; 			//I2C_Device_Address


	uint32_t I2C_ACK_CONTROL;      //specify I2C_ACK Enable or disable
								 //this parameter can be a value of @ref I2C_ACK_Mode

	uint32_t I2C_General_call_address;   //this parameter can be a value of @ref I2C_GeneralAddress_Mode

	void(* P_Slave_event_callback)(Slave_state State);

}I2C_InitTypedef;


//Standard Speed (up to 100 kHz)
//Fast Speed (up to 400 kHz)

#define I2C_Sclk_sm_50kH              (50000UL )
#define I2C_Sclk_sm_100kH             (100000UL)
#define I2C_Sclk_sm_200kH             (200000UL)   //Fas mode not supported
#define I2C_Sclk_sm_400kH             (400000UL)


//@ref I2C_StretchMode
/*
This bit is used to disable clock stretching in slave mode when ADDR or BTF flag is set, until
it is reset by software.
0: Clock stretching enabled
1: Clock stretching disabled
*/

#define I2C_Stretch_enable        0
#define I2C_Stretch_disable        I2C_CR1_NOSTRETCH


//@ref I2C_Mode
/*
 0: I2C mode
 1: SMBus mode
*/
#define I2C_Mode_I2C              0
#define I2C_MODE_SMBUS            I2C_CR1_SMBUS


//@ref I2C_Addressing_Slave
#define I2C_AddressingMode_7bit   0
#define I2C_AddressingMode_10bit  (uint16_t)(1<<15)


//@ref I2C_ACK_Mode
#define I2C_ACK_Enable           I2C_CR1_ACK
#define I2C_ACK_Disable		     0

//======================================
typedef enum
{
	Polling_DISABLE ,
	Polling_ENABLE
}I2C_Polling_mechanism;

typedef enum
{
	State_DISABLE ,
	State_ENABLE
}Functional_state;

typedef enum
{
	Flag_reset ,
	Flag_set
}FlagStatus;

#define I2C_EVENT_MASTER_BY_TRASMITTING       (uint32_t)(0x00070080);

typedef enum
{
	I2C_FLAG_BUSY=0 ,
	EV5,
	EV6,
	EV7,
	EV8,
	EV8_1,
	MASTER_BY_TRASMITTING = (uint32_t)(0x00070080)
}status;

typedef enum
{
	I2C_Transmit_direction=0,I2C_Reciver_direction
}I2C_direction;


//===============================
void i2c_GenerateSTART(I2C_TypeDef* I2Cx , Functional_state new_state , Start_condtion start);
void i2c_GenerateSTOP(I2C_TypeDef* I2Cx, Functional_state new_state);
/*
* ===============================================
* APIs Supported by "MCAL I2C DRIVER"
* ===============================================
*/
void MCAL_I2C_init(I2C_TypeDef* I2Cx , I2C_InitTypedef* I2C_Init_Struct);
void MCAL_I2C_Deinit(I2C_TypeDef* I2Cx);

void MCAL_I2C_GPIO_SetPins(I2C_TypeDef* I2Cx);

//Master polling mechanism

void MCAL_I2C_MASTER_Transmit(I2C_TypeDef* I2Cx , uint16_t Device_address , uint8_t* data , uint32_t data_len , stop_condtion stop , Start_condtion start);
void MCAL_I2C_MASTER_Receive(I2C_TypeDef* I2Cx , uint16_t Device_address , uint8_t* data , uint32_t data_len , stop_condtion stop , Start_condtion start);
FlagStatus Get_flag_status(I2C_TypeDef* I2Cx , status flag);
void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, Functional_state NewState);
void I2C_send_address(I2C_TypeDef* I2Cx , uint16_t address ,I2C_direction direction);

//slave interrupt mechanism
void MCAL_I2C_SLAVE_send(I2C_TypeDef* I2Cx , uint8_t data);
uint8_t  MCAL_I2C_SLAVE_recieve(I2C_TypeDef* I2Cx);





#endif /* INC_I2C_C_ */
