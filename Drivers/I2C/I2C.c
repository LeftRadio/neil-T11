/**
  ******************************************************************************
  * @file	 	I2C.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse for I2C driver
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "I2C.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define FLAG_TIMEOUT             ((long)0x1000)
#define LONG_TIMEOUT             ((long)(300 * FLAG_TIMEOUT))


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    :
* Input          : speed in Hz
* Return         : None
*******************************************************************************/
void I2C_Configuration(uint32_t speed)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	/* I2C SCL and SDA pins configuration -------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect pins to I2C peripheral */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	/* Enable the I2C peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	/* I2C1 peripheral configuration */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = speed;

	I2C_Init(I2C1, &I2C_InitStructure);

	/* Enable the I2C peripheral */
	I2C_Cmd(I2C1, ENABLE);
}



/*******************************************************************************
* Function Name  : I2C_DeConfiguration
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void I2C_DeConfiguration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Disable the I2C peripheral and I2C clock */
	I2C_Cmd(I2C1, DISABLE);
	I2C_DeInit(I2C1);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);

	/* I2C SCL and SDA pins deconfiguration -------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/**
  * @brief  Writes a Byte to a given register through the control interface (I2C)
  * @param  I2C_I2C_Addr
  * @param  RegisterAddr: The I2C_Address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
int I2C_Write(I2C_TypeDef* I2C, uint8_t I2C_Addr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
//	int32_t result = 0;
	__IO long Timeout = LONG_TIMEOUT;

	/*!< While the bus is busy */
	while(I2C_GetFlagStatus(I2C, I2C_FLAG_BUSY))
	{
		if((Timeout--) <= 0) return -1;
	}

	/* Start the config sequence */
	I2C_GenerateSTART(I2C, ENABLE);

	/* Test on EV5 and clear it */
	Timeout = FLAG_TIMEOUT;
	while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((Timeout--) <= 0) return -1;
	}

	/* Transmit the slave I2C_Address and enable writing operation */
	I2C_Send7bitAddress(I2C, I2C_Addr, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	Timeout = FLAG_TIMEOUT;
	while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((Timeout--) <= 0) return -1;
	}

	/* Transmit the first I2C_Address for write operation */
	I2C_SendData(I2C, RegisterAddr);

	/* Test on EV8 and clear it */
	Timeout = FLAG_TIMEOUT;
	while (!I2C_CheckEvent(I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((Timeout--) <= 0) return -1;
	}

	/* Prepare the register value to be sent */
	I2C_SendData(I2C, RegisterValue);

	/*!< Wait till all data have been physically transferred on the bus */
	Timeout = LONG_TIMEOUT;
	while(!I2C_GetFlagStatus(I2C, I2C_FLAG_BTF))
	{
		if((Timeout--) <= 0) return -1;
	}

	/* End the configuration sequence */
	I2C_GenerateSTOP(I2C, ENABLE);

#ifdef VERIFY_WRITTENDATA
	/* Verify that the data has been correctly written */
	result = (I2C_Read(I2C_Addr, RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

	/* Return the verifying value: 0 (Passed) */
	return 0;
}




/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
