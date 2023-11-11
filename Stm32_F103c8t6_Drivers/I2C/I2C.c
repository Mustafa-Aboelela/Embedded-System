/*
 * I2C.c
 *
 *  Created on: Nov 3, 2023
 *      Author: mustafa_mohamed_shaban
 */


#include"I2C.h"

/*==================================
 * Generic MACROS
 * =================================
 * */
#define I2C1_index     0
#define I2C2_index     1


/*==================================
 * Generic variables
 * =================================
 * */
I2C_InitTypedef Global_I2C_Config[2] = {NULL,NULL} ;


void MCAL_I2C_init(I2C_TypeDef* I2Cx , I2C_InitTypedef* I2C_Init_Struct)
{
	uint16_t temp_reg = 0 , freq_range =0;
	uint32_t P_clk = 8000000;
	uint16_t result =0;
	if(I2Cx == I2C1)
	{
		Global_I2C_Config[I2C1_index] = *I2C_Init_Struct;
		RCC_I2C1_CLK_EN;
	}
	else
	{
		Global_I2C_Config[I2C2_index] = *I2C_Init_Struct;
		RCC_I2C2_CLK_EN;
	}

	if(I2C_Init_Struct->I2C_Mode == I2C_Mode_I2C)
	{
		//init timing
		temp_reg = I2Cx->CR2 ;

		//clear freq bits
		temp_reg &= ~(I2C_CR2_FREQ);

		//Get the clk freqency
		P_clk = MCAL_RCC_GetPCLKFreq();

		freq_range = P_clk/1000000;

		temp_reg |= freq_range;

		//write on the CR2 register
		I2Cx->CR2 = temp_reg;

		//configure CCR register

		//first disable the i2c
		I2Cx->CR1 &= ~(I2C_CR1_PE);
		temp_reg = 0;


		if(I2C_Init_Struct->clk_speed == I2C_Sclk_sm_50kH || I2C_Init_Struct->clk_speed == I2C_Sclk_sm_100kH)
		{
			//calculate CCR
			result = (uint16_t)(P_clk / (2*I2C_Init_Struct->clk_speed));
			temp_reg = result;

			I2Cx->CCR = temp_reg;

			//for TRISE register , add 1 to the input p_clk
			I2Cx->TRISE = (freq_range + 1);
		}
		else
		{
			//fast mode not supported
		}

		//=== I2C CR1 configuration=====
		temp_reg = I2Cx->CR1;

		temp_reg = (uint16_t)(I2C_Init_Struct->I2C_ACK_CONTROL | I2C_Init_Struct->I2C_General_call_address | I2C_Init_Struct->I2C_Mode | I2C_Init_Struct->Stretch_mode);

		//write on the CR1 register
		I2Cx->CR1 = temp_reg;

		//I2C OAR1 & OAR2 addresses
		temp_reg = 0;
		if(I2C_Init_Struct->I2C_Device_address.Enable_dual_address == 1)
		{
			temp_reg |= I2C_OAR2_ENDUAL;
			temp_reg |= I2C_Init_Struct->I2C_Device_address.Secondary_slave_address << I2C_OAR2_ADD2_Pos;
			I2Cx->OAR2 = temp_reg;
		}
		//for primary address
		temp_reg = 0;
		temp_reg |= I2C_Init_Struct->I2C_Device_address.Primary_Slave_address << 1;
		temp_reg |= I2C_Init_Struct->I2C_Device_address.I2C_Addressing_slave_mode ;
		I2Cx->OAR1 = temp_reg;
	}
	else
	{
		//not supported
	}


	//Interrupt mode for slave only
	if(I2C_Init_Struct->P_Slave_event_callback != NULL)
	{
		//enable IRQ
		I2Cx->CR1 |= (I2C_CR2_ITBUFEN);
		I2Cx->CR1 |= (I2C_CR2_ITEVTEN);
		I2Cx->CR1 |= (I2C_CR2_ITERREN);
		if(I2Cx == I2C1)
		{
			NVIC_IRQ31_I2C1_EV_EN;
			NVIC_IRQ32_I2C1_ER_EN;
		}
		else
		{
			NVIC_IRQ33_I2C2_EV_EN;
			NVIC_IRQ34_I2C2_ER_EN;
		}
	}

	//clear status register
	I2Cx->SR1 = 0;
	I2Cx->SR2 = 0;

	//enable i2c
	I2Cx->CR1 |= (I2C_CR1_PE);
}

void MCAL_I2C_Deinit(I2C_TypeDef* I2Cx)
{
	if(I2Cx == I2C1)
	{
		NVIC_IRQ31_I2C1_EV_DIS;
		NVIC_IRQ32_I2C1_ER_DIS;
		RCC_I2C1_CLK_DIS;  //(RCC->APB1RSTR |=(1<<21));

	}
	else
	{
		NVIC_IRQ33_I2C2_EV_DIS;
		NVIC_IRQ34_I2C2_ER_DIS;
		RCC_I2C2_CLK_DIS;  //(RCC->APB1RSTR |=(1<<22));
	}
}


void MCAL_I2C_GPIO_SetPins(I2C_TypeDef* I2Cx)
{
	GPIO_PIN_Config_t PinCfg;
		if(I2Cx == I2C1)
		{
			//PB6 - SCL
			//PB7 - SDA

			PinCfg.GPIO_pinNumber = GPIO_PIN_6;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_OPENDRAIN;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg);

			PinCfg.GPIO_pinNumber = GPIO_PIN_7;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_OPENDRAIN;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg);
		}
		else if(I2Cx == I2C2)
		{
			//PB10 - SCL
			//PB11 - SDA

			PinCfg.GPIO_pinNumber = GPIO_PIN_10;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_OPENDRAIN;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg);

			PinCfg.GPIO_pinNumber = GPIO_PIN_11;
			PinCfg.GPIO_MODE = GPIO_Mode_OUTPUT_AF_OPENDRAIN;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10_MHZ;
			MCAL_GPIO_Init(GPIOB , &PinCfg);
		}
}

void MCAL_I2C_MASTER_Transmit(I2C_TypeDef* I2Cx , uint16_t Device_address , uint8_t* data , uint32_t data_len , stop_condtion stop , Start_condtion start)
{
	int i;
		//generate start bit
		i2c_GenerateSTART(I2Cx , State_ENABLE , Start);

		//EV5 .. check if start bit is done and the device become a master
		while(!Get_flag_status(I2Cx , EV5));

		//send address of slave
		I2C_send_address(I2Cx , Device_address , I2C_Transmit_direction);

		//EV6
		while(!Get_flag_status(I2Cx , EV6));

		//check TRA ,busy , MSL , TXE flages
		while(!Get_flag_status(I2Cx , MASTER_BY_TRASMITTING));

		//data
		for(i=0 ; i<data_len ; i++)
		{
			I2Cx->DR = data[i];
			while(!Get_flag_status(I2Cx , EV8));
		}

		if(stop == with_stop)
		{
			i2c_GenerateSTOP(I2Cx , State_ENABLE);
		}
}

MCAL_I2C_MASTER_Receive(I2C_TypeDef* I2Cx , uint16_t Device_address , uint8_t* data , uint32_t data_len , stop_condtion stop , Start_condtion start)
{
	uint8_t index = (I2Cx == I2C1)? I2C1_index : I2C2_index ;

	int i;
	//generate start bit
	i2c_GenerateSTART(I2Cx , State_ENABLE , Start);

	//EV5 .. check if start bit is done and the device become a master
	while(!Get_flag_status(I2Cx , EV5));

	//send address of slave
	I2C_send_address(I2Cx , Device_address , I2C_Reciver_direction);

	//EV6
	while(!Get_flag_status(I2Cx , EV6));

	I2C_AcknowledgeConfig(I2Cx, State_ENABLE);

	if(data_len)
	{
		for(i = data_len ; i>1 ; i--)
		{
			while(!Get_flag_status(I2Cx , EV7));

			//Read data
			*data = I2Cx->DR ;

			data++;
		}
	}

	I2C_AcknowledgeConfig(I2Cx, State_DISABLE);

	if(stop == with_stop)
	{
		i2c_GenerateSTOP(I2Cx , State_ENABLE);
	}

	if((Global_I2C_Config[index]).I2C_ACK_CONTROL = I2C_ACK_Enable)
	{
		I2C_AcknowledgeConfig(I2Cx, State_ENABLE);
	}

}

void MCAL_I2C_SLAVE_send(I2C_TypeDef* I2Cx , uint8_t data)
{
	I2Cx->DR = data ;
}
uint8_t  MCAL_I2C_SLAVE_recieve(I2C_TypeDef* I2Cx)
{
	return (uint8_t)I2Cx->DR;
}

void i2c_GenerateSTART(I2C_TypeDef* I2Cx , Functional_state new_state , Start_condtion start)
{
	if(start != repeated_start)
	{
		//check the bus
		while(Get_flag_status(I2Cx , I2C_FLAG_BUSY));

	}

	if(new_state != State_DISABLE)
	{
		I2Cx->CR1 |= I2C_CR1_START;
	}
	else
	{
		I2Cx->CR1 &= ~(I2C_CR1_START);
	}
}


void i2c_GenerateSTOP(I2C_TypeDef* I2Cx, Functional_state new_state)
{
	if(new_state != State_DISABLE)
	{
		I2Cx->CR1 |= I2C_CR1_STOP;
	}
	else
	{
		I2Cx->CR1 &= ~(I2C_CR1_STOP);
	}
}

FlagStatus Get_flag_status(I2C_TypeDef* I2Cx , status flag)
{
	volatile uint32_t dummy_Read;

	uint32_t flag1 , flag2 , lastevent;
	FlagStatus bit_status= Flag_reset;

	switch(flag)
	{
	case I2C_FLAG_BUSY:
	{
		if(I2Cx->SR2 & I2C_SR2_BUSY)
			bit_status = Flag_set;
		else
			bit_status = Flag_reset;
	}
	break;

	case EV5:
	{
		if(I2Cx->SR1 & I2C_SR1_SB)
			bit_status = Flag_set;
		else
			bit_status = Flag_reset;
	}
	break;

	case EV6:
	{
		if(I2Cx->SR1 & I2C_SR1_ADDR)
			bit_status = Flag_set;
		else
			bit_status = Flag_reset;

		dummy_Read = I2Cx->SR2;
	}
	break;

	case MASTER_BY_TRASMITTING:
	{
		flag1 = I2Cx->SR1;
		flag2 = I2Cx->SR2;
		flag2 = flag2<<16;

		lastevent = ((flag1 | flag2) & (uint32_t)0x00FFFFFF);

		if((lastevent & MASTER_BY_TRASMITTING) == MASTER_BY_TRASMITTING)
		{
			bit_status = Flag_set;
		}
		else
		{
			bit_status = Flag_reset;
		}
	}
	break;

	case EV8_1:
	case EV8:
	{
		if(I2Cx->SR1 & I2C_SR1_TXE)
			bit_status = Flag_set;
		else
			bit_status = Flag_reset;
	}
	break;

	case EV7:
	{
		if(I2Cx->SR1 & I2C_SR1_RXNE)
			bit_status = Flag_set;
		else
			bit_status = Flag_reset;
	}
	break;

	}

	return bit_status;
}


void I2C_send_address(I2C_TypeDef* I2Cx , uint16_t address ,I2C_direction direction)
{
	address = (address<<1);

	if(direction == I2C_Transmit_direction)
	{
		address &= ~(1<<0);
	}
	else
	{
		address |=(1<<0);
	}

	I2Cx->DR = address;

}

void I2C_AcknowledgeConfig(I2C_TypeDef *I2Cx, Functional_state NewState)
{
	if(NewState == State_ENABLE)
	{
		/* Enable the Acknowledgment */
		I2Cx->CR1 |= I2C_CR1_ACK;
	}
	else
	{
		/* Disable the Acknowledgment */
		I2Cx->CR1 &= ~(I2C_CR1_ACK);
	}
}


void I2C1_EV_IRQHandler(void)
{
	/* I2C1 event interrupt*/
	volatile uint32_t dummy_read =0;
	I2C_TypeDef* I2Cx = I2C1;

	uint32_t temp1 , temp2 , temp3;

	temp1= I2Cx->CR2 & (I2C_CR2_ITEVTEN);
	temp2= I2Cx->CR2 & (I2C_CR2_ITBUFEN);

	temp2= I2Cx->SR1 & (I2C_SR1_STOPF);
}


void I2C1_ER_IRQHandler(void)
{
	/* I2C1 error interrupt  */
}


void I2C2_EV_IRQHandler(void)
{
	/* I2C1 event interrupt*/
}


void I2C2_ER_IRQHandler(void)
{
	/* I2C1 error interrupt  */
}



















