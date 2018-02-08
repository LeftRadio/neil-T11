

/* Includes ------------------------------------------------------------------*/
#include "DACs_Codecs_Layer.h"
#include "stm32f4_discovery.h"
#include "CS43L22_audio_codec.h"


DAC_Codec_HAL_pFunc CS43L22_Codec =
{
		CS43L22_Codec_Init,
		CS43L22_Codec_DeInit,
		CS43L22_Codec_Play,
		CS43L22_Codec_PauseResume,
		CS43L22_Codec_Stop,
		CS43L22_Codec_VolumeCtrl,
		CS43L22_Codec_Mute,
};

volatile uint8_t OutputDev = 0;
__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;

static void  Delay(__IO uint32_t nCount);


/** @addtogroup  CS43L22_AUDIO_CODEC
  * @brief       This file includes the low layer driver for CS43L22 Audio Codec
  * @{
  */

/*========================

                CS43L22 Audio Codec Control Functions
                                                ==============================*/
/**
  * @brief  Initializes the audio codec and all related interfaces (control
  *         interface: I2C and audio interface: I2S)
  * @param  OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_Init(uint32_t AudioFreq, uint8_t Volume, uint16_t OutputDevice)
{
	uint32_t counter = 0;

	/* Configure the Codec related IOs */
	CS43L22_Codec_GPIO_Init();

	/* Reset the Codec Registers */
	CS43L22_Codec_Reset();

	/* Initialize the Control interface of the Audio Codec */
	CS43L22_Codec_CtrlInterface_Init();

	/* Keep Codec powered OFF */
	counter += CS43L22_Codec_WriteRegister(0x02, 0x01);

	counter += CS43L22_Codec_WriteRegister(0x04, 0xAF); /* SPK always OFF & HP always ON */
	OutputDev = 0xAF;

	/* Clock configuration: Auto detection */
	counter += CS43L22_Codec_WriteRegister(0x05, 0x81);

	/* Set the Slave Mode and the audio Standard */
	counter += CS43L22_Codec_WriteRegister(0x06, CODEC_STANDARD);		//CODEC_STANDARD

	/* Set the Master volume */
	CS43L22_Codec_VolumeCtrl(Volume);

	/* Power on the Codec */
	counter += CS43L22_Codec_WriteRegister(0x02, 0x9E);

	/* Additional configuration for the CODEC. These configurations are done to reduce
      the time needed for the Codec to power off. If these configurations are removed,
      then a long delay should be added between powering off the Codec and switching
      off the I2S peripheral MCLK clock (which is the operating clock for Codec).
      If this delay is not inserted, then the codec will not shut down properly and
      it results in high noise after shut down. */

	/* Disable the analog soft ramp */
	counter += CS43L22_Codec_WriteRegister(0x0A, 0x00);

	/* Disable the limiter attack level */
	counter += CS43L22_Codec_WriteRegister(0x27, 0x00);
	/* Adjust Bass and Treble levels */
	counter += CS43L22_Codec_WriteRegister(0x1F, 0x0F);
	/* Adjust PCM volume level */
	counter += CS43L22_Codec_WriteRegister(0x1A, 0x0A);
	counter += CS43L22_Codec_WriteRegister(0x1B, 0x0A);

//	counter += CS43L22_Codec_WriteRegister(0x1E, 0xC0);
//	counter += CS43L22_Codec_WriteRegister(0x1C, 0x08);

	/* Return communication control value */
	return counter;
}

/**
  * @brief  Restore the audio codec state to default state and free all used
  *         resources.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_DeInit(void)
{
	uint32_t counter = 0;

	/* Reset the Codec Registers */
	CS43L22_Codec_Reset();

	/* Keep Codec powered OFF */
	counter += CS43L22_Codec_WriteRegister(0x02, 0x01);

	/* Deinitialize all use GPIOs */
	CS43L22_Codec_GPIO_DeInit();

	/* Disable the Codec control interface */
	CS43L22_Codec_CtrlInterface_DeInit();

	/* Return communication control value */
	return counter;
}

/**
  * @brief  Start the audio Codec play feature.
  * @note   For this codec no Play options are required.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_Play(void)
{
  /*
     No actions required on Codec level for play command
     */

  /* Return communication control value */
  return 0;
}

/**
  * @brief  Pauses and resumes playing on the audio codec.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_PauseResume(uint32_t Cmd)
{
	uint32_t counter = 0;

	/* Pause the audio file playing */
	if (Cmd == AUDIO_PAUSE)
	{
		/* Mute the output first */
		counter += CS43L22_Codec_Mute(AUDIO_MUTE_ON);

		/* Put the Codec in Power save mode */
		counter += CS43L22_Codec_WriteRegister(0x02, 0x01);
	}
	else /* AUDIO_RESUME */
	{
		/* Unmute the output first */
		counter += CS43L22_Codec_Mute(AUDIO_MUTE_OFF);

		counter += CS43L22_Codec_WriteRegister(0x04, OutputDev);

		/* Exit the Power save mode */
		counter += CS43L22_Codec_WriteRegister(0x02, 0x9E);
	}

	return counter;
}

/**
  * @brief  Stops audio Codec playing. It powers down the codec.
  * @param  CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_SW: only mutes the audio codec. When resuming from this
  *                           mode the codec keeps the previous initialization
  *                           (no need to re-Initialize the codec registers).
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration
  *                           (user should re-Initialize the codec in order to
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_Stop(uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;

  /* Mute the output first */
  CS43L22_Codec_Mute(AUDIO_MUTE_ON);

  if (CodecPdwnMode == CODEC_PDWN_SW)
  {
    /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
    counter += CS43L22_Codec_WriteRegister(0x02, 0x9F);
  }
  else /* CODEC_PDWN_HW */
  {
    /* Power down the DAC components */
    counter += CS43L22_Codec_WriteRegister(0x02, 0x9F);

    /* Wait at least 100us */
    Delay(0xFFF);

    /* Reset The pin */
    GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_RESET);
  }

  return counter;
}

/**
  * @brief  Sets higher or lower the codec volume level.
  * @param  Volume: a byte value from 0 to 255 (refer to codec registers
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_VolumeCtrl(uint8_t Volume)
{
	uint32_t counter = 0;

	if (Volume > 0xE6)
	{
		/* Set the Master volume */
		counter += CS43L22_Codec_WriteRegister(0x20, Volume - 0xE7);
		counter += CS43L22_Codec_WriteRegister(0x21, Volume - 0xE7);
	}
	else
	{
		/* Set the Master volume */
		counter += CS43L22_Codec_WriteRegister(0x20, Volume + 0x19);
		counter += CS43L22_Codec_WriteRegister(0x21, Volume + 0x19);
	}

	return counter;
}

/**
  * @brief  Enables or disables the mute feature on the audio codec.
  * @param  Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_Mute(uint32_t Cmd)
{
	uint32_t counter = 0;

	/* Set the Mute mode */
	if (Cmd == AUDIO_MUTE_ON)
	{
		counter += CS43L22_Codec_WriteRegister(0x04, 0xFF);
	}
	else /* AUDIO_MUTE_OFF Disable the Mute */
	{
		counter += CS43L22_Codec_WriteRegister(0x04, OutputDev);
	}

	return counter;
}

/**
  * @brief  Resets the audio codec. It restores the default configuration of the
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
void CS43L22_Codec_Reset(void)
{
	/* Power Down the codec */
	GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_RESET);

	/* wait for a delay to insure registers erasing */
	Delay(CODEC_RESET_DELAY);

	/* Power on the codec */
	GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_SET);
}

/**
  * @brief  Writes a Byte to a given register into the audio codec through the
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t CS43L22_Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
	uint32_t result = 0;

	/*!< While the bus is busy */
	CODECTimeout = CODEC_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Start the config sequence */
	I2C_GenerateSTART(CODEC_I2C, ENABLE);

	/* Test on EV5 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Transmit the slave address and enable writing operation */
	I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Transmit the first address for write operation */
	I2C_SendData(CODEC_I2C, RegisterAddr);

	/* Test on EV8 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Prepare the register value to be sent */
	I2C_SendData(CODEC_I2C, RegisterValue);

	/*!< Wait till all data have been physically transferred on the bus */
	CODECTimeout = CODEC_LONG_TIMEOUT;
	while(!I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF))
	{
		if((CODECTimeout--) == 0) Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* End the configuration sequence */
	I2C_GenerateSTOP(CODEC_I2C, ENABLE);

#ifdef VERIFY_WRITTENDATA
	/* Verify that the data has been correctly written */
	result = (CS43L22_Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

	/* Return the verifying value: 0 (Passed) or 1 (Failed) */
	return result;
}

/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
uint32_t CS43L22_Codec_ReadRegister(uint8_t RegisterAddr)
{
	uint32_t result = 0;

	/*!< While the bus is busy */
	CODECTimeout = CODEC_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Start the config sequence */
	I2C_GenerateSTART(CODEC_I2C, ENABLE);

	/* Test on EV5 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Transmit the slave address and enable writing operation */
	I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

	/* Test on EV6 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/* Transmit the register address to be read */
	I2C_SendData(CODEC_I2C, RegisterAddr);

	/* Test on EV8 and clear it */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/*!< Send START condition a second time */
	I2C_GenerateSTART(CODEC_I2C, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/*!< Send Codec address for read */
	I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Receiver);

	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_ADDR) == RESET)
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(CODEC_I2C, DISABLE);

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)CODEC_I2C->SR2;

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(CODEC_I2C, ENABLE);

	/* Wait for the byte to be received */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_RXNE) == RESET)
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/*!< Read the byte received from the Codec */
	result = I2C_ReceiveData(CODEC_I2C);

	/* Wait to make sure that STOP flag has been cleared */
	CODECTimeout = CODEC_FLAG_TIMEOUT;
	while(CODEC_I2C->CR1 & I2C_CR1_STOP)
	{
		if((CODECTimeout--) == 0) return Dac_Codec_ResponseTIMEOUT_UserCallback();
	}

	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(CODEC_I2C, ENABLE);

	/* Clear AF flag for next communication */
	I2C_ClearFlag(CODEC_I2C, I2C_FLAG_AF);

	/* Return the byte read from Codec */
	return result;
}

/**
  * @brief  Initializes the Audio Codec control interface (I2C).
  * @param  None
  * @retval None
  */
void CS43L22_Codec_CtrlInterface_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure;

	/* Enable the CODEC_I2C peripheral clock */
	RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);

	/* CODEC_I2C peripheral configuration */
	I2C_DeInit(CODEC_I2C);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
	/* Enable the I2C peripheral */
	I2C_Cmd(CODEC_I2C, ENABLE);
	I2C_Init(CODEC_I2C, &I2C_InitStructure);
}

/**
  * @brief  Restore the Audio Codec control interface to its default state.
  *         This function doesn't de-initialize the I2C because the I2C peripheral
  *         may be used by other modules.
  * @param  None
  * @retval None
  */
void CS43L22_Codec_CtrlInterface_DeInit(void)
{
  /* Disable the I2C peripheral */ /* This step is not done here because
     the I2C interface can be used by other modules */
  /* I2C_DeInit(CODEC_I2C); */
}




/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio
  *        interfaces).
  * @param  None
  * @retval None
  */
void CS43L22_Codec_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable Reset GPIO Clock */
	RCC_AHB1PeriphClockCmd(AUDIO_RESET_GPIO_CLK,ENABLE);

	/* Audio reset pin configuration -------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = AUDIO_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStructure);

	/* Enable I2S and I2C GPIO clocks */
	RCC_AHB1PeriphClockCmd(CODEC_I2C_GPIO_CLOCK | I2S_1_GPIO_CLOCK, ENABLE);

	/* CODEC_I2C SCL and SDA pins configuration -------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN | CODEC_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(CODEC_I2C_GPIO, &GPIO_InitStructure);
	/* Connect pins to I2C peripheral */
	GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SCL_PINSRC, CODEC_I2C_GPIO_AF);
	GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SDA_PINSRC, CODEC_I2C_GPIO_AF);

	/* CODEC_I2S pins configuration: WS, SCK and SD pins -----------------------------*/
	GPIO_InitStructure.GPIO_Pin = I2S_1_SCK_PIN | I2S_1_SD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2S_1_GPIO, &GPIO_InitStructure);

	/* Connect pins to I2S peripheral  */
	GPIO_PinAFConfig(I2S_1_WS_GPIO, I2S_1_WS_PINSRC, I2S_1_GPIO_AF);
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SCK_PINSRC, I2S_1_GPIO_AF);

	GPIO_InitStructure.GPIO_Pin = I2S_1_WS_PIN ;
	GPIO_Init(I2S_1_WS_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SD_PINSRC, I2S_1_GPIO_AF);

//		/* GPIOA clock enable (to be used with DAC) */
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//
//		/* DAC channel 1 & 2 (DAC_OUT1 = PA.4) configuration */
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);


#ifdef CODEC_MCLK_ENABLED
	/* CODEC_I2S pins configuration: MCK pin */
	GPIO_InitStructure.GPIO_Pin = I2S_1_MCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2S_1_MCK_GPIO, &GPIO_InitStructure);
	/* Connect pins to I2S peripheral  */
	GPIO_PinAFConfig(I2S_1_MCK_GPIO, I2S_1_MCK_PINSRC, I2S_1_GPIO_AF);
#endif /* CODEC_MCLK_ENABLED */
}


/**
  * @brief  Restores the IOs used by the Audio Codec interface to their default state.
  * @param  None
  * @retval None
  */
void CS43L22_Codec_GPIO_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Deinitialize all the GPIOs used by the driver */
	GPIO_InitStructure.GPIO_Pin =  I2S_1_SCK_PIN | I2S_1_SD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(I2S_1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2S_1_WS_PIN ;
	GPIO_Init(I2S_1_WS_GPIO, &GPIO_InitStructure);

	/* Disconnect pins from I2S peripheral  */
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_WS_PINSRC, 0x00);
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SCK_PINSRC, 0x00);
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SD_PINSRC, 0x00);

#ifdef CODEC_MCLK_ENABLED
	/* CODEC_I2S pins deinitialization: MCK pin */
	GPIO_InitStructure.GPIO_Pin = I2S_1_MCK_PIN;
	GPIO_Init(I2S_1_MCK_GPIO, &GPIO_InitStructure);
	/* Disconnect pins from I2S peripheral  */
	GPIO_PinAFConfig(I2S_1_MCK_GPIO, I2S_1_MCK_PINSRC, I2S_1_GPIO_AF);
#endif /* CODEC_MCLK_ENABLED */
}


/**
  * @brief  Inserts a delay time (not accurate timing).
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void Delay( __IO uint32_t nCount)
{
	for (; nCount != 0; nCount--);
}


