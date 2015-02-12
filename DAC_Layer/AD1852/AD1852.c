/**
  ******************************************************************************
  * @file	 	AD1852.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "Audio_Interface.h"
#include "SPI.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*-----------------------------------
                    Hardware Configuration defines parameters
                                     -----------------------------------------*/
/* Audio Reset Pin definition */
#define AUDIO_RESET_PIN                	GPIO_Pin_7
#define AUDIO_RESET_GPIO               	GPIOB
#define AD1852_REG_VALUE				(DAC_AD1852.UsrData[0])

/* Masks for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 SPI_I2SCFGR_I2SE
#define I2S_DISABLE_MASK				((uint16_t)~((uint16_t)SPI_I2SCFGR_I2SE))

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void AD1852_Init(void);
static void AD1852_DeInit(void);
static void AD1852_Play(void);
static void AD1852_PauseResume(AUDIO_CMD_TypeDef Cmd);
static void AD1852_Stop(void);
static void AD1852_VolumeCtrl(uint8_t Volume);
static void AD1852_MuteSetState(AUDIO_MUTE_TypeDef State);

static void AD1852_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue);
static uint32_t AD1852_ReadRegister(uint8_t RegisterAddr);

/* Private variables ---------------------------------------------------------*/
volatile DAC_HAL_TypeDef DAC_AD1852 =
{
		DeEmphasisOFF,
		I2S_RightJustified,
		I2S_DataFormat_16bextended,
		INT_NONE,
		AD1852_Init,
		AD1852_DeInit,
		AD1852_Play,
		AD1852_PauseResume,
		AD1852_Stop,
		AD1852_VolumeCtrl,
		AD1852_MuteSetState,
		{0},
};

FlagStatus AD1852REG_INIT = RESET;

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  AD1852_Play
  * @param  None
  * @retval None
  */
static void AD1852_Play(void)
{
//	static uint16_t cnt = 0;


//	if(++cnt < 10)
//	{
//		AD1852_WriteRegister(0x01, 1<<4);
//	}
//
//	/* If DMA Stream is stopped */
//	if((DMA1_Stream7->CR & (uint32_t)DMA_SxCR_EN) == 0)
//	{
//		/* If the I2S peripheral is still not enabled, enable it */
//		I2S_1->I2SCFGR |= I2S_ENABLE_MASK;
//
//		/* Reset DMA memory pointer and transfer size */
//		DMA1_Stream7->M0AR = (uint32_t)AudioBuffer;
//		DMA1_Stream7->NDTR = (uint32_t)(AUDIO_BUFFER_SIZE_BYTES / 2);
//
//		/* Run the DMA Stream */
//		DMA1_Stream7->CR |= (uint32_t)DMA_SxCR_EN;
//	}
}


/**
  * @brief  AD1852_PauseResume
  * @param  None
  * @retval None
  */
static void AD1852_PauseResume(AUDIO_CMD_TypeDef Cmd)
{
	/* Pause the audio file playing */
	if (Cmd == AUDIO_CMD_PAUSE)
	{
		AD1852_MuteSetState(AUDIO_MUTE);
	}
	else /* AUDIO_RESUME */
	{
		AD1852_MuteSetState(AUDIO_UNMUTE);
	}
}


/**
  * @brief  AD1852_Stop
  * @param  None
  * @retval None
  */
static void AD1852_Stop(void)
{
	/* Previosly mute audio */
	AD1852_MuteSetState(AUDIO_MUTE);

	/* Stop and disable the DMA stream */
	DMA1_Stream7->CR &= ~(uint32_t)DMA_SxCR_EN;

	/* Clear all the DMA flags for the next transfer */
	DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7 |DMA_FLAG_HTIF7 | \
			DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);

	/* Disable the I2S peripheral */
	I2S_1->I2SCFGR &= I2S_DISABLE_MASK;
}


/**
  * @brief  AD1852_VolumeCtrl
  * @param  None
  * @retval None
  */
static void AD1852_VolumeCtrl(uint8_t Volume)
{
	/* not used for DAC, volume sets external */
}


/**
  * @brief  AD1852_Mute
  * @param  None
  * @retval None
  */
static void AD1852_MuteSetState(AUDIO_MUTE_TypeDef State)
{
	uint16_t tmp = 0;

	/* Set the Mute mode */
	if (State == AUDIO_MUTE)
	{
		tmp = AD1852_ReadRegister(0x01) | (1 << 6);
		AD1852_WriteRegister(0x01, tmp);
	}
	else /* AUDIO_MUTE_OFF Disable the Mute */
	{
		tmp = AD1852_ReadRegister(0x01) & (~(1 << 6));
		AD1852_WriteRegister(0x01, tmp);
	}
}



/**
  * @brief  Initializes specefic function(IOs etc) used by the Audio DAC
  * @param  None
  * @retval None
  */
static void AD1852_Init(void)
{
	/* Configure the Codec related IOs */
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef AD_SPI_InitStruct;

	/* Initialization parameters for AD1852 control SPI interface */
	AD_SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	AD_SPI_InitStruct.SPI_Mode = SPI_Mode_Master;							// transmit in master mode, NSS pin has to be always high
	AD_SPI_InitStruct.SPI_DataSize = SPI_DataSize_16b;
	AD_SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;								// clock is high when idle
	AD_SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;							// data sampled at first edge
	AD_SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;								// set the NSS management to internal
	AD_SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	// SPI frequency is APB2 frequency / 4
	AD_SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;						// data is transmitted MSB first

	/* Configure parameters */
	SPI_Configuration(&AD_SPI_InitStruct);

	/* Configure the I2S/DMA peripherals */
	I2S_Configuration();

	/* AD1852 reset pin configuration -------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = AUDIO_RESET_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStructure);

//	AUDIO_RESET_GPIO->BSRRL |= AUDIO_RESET_PIN; // set RESET high

	/* Reset the Codec Registers */
	AUDIO_RESET_GPIO->BSRRH = AUDIO_RESET_PIN;
	delay_ms(1);
	AUDIO_RESET_GPIO->BSRRL = AUDIO_RESET_PIN;

	/* Initialize registers the AD185x */
	delay_ms(1);
	AD1852_WriteRegister(0x01, 1<<4);
}


/**
  * @brief  Restores specefic function(IOs etc) used by the Audio DAC
  * @param  None
  * @retval None
  */
static void AD1852_DeInit(void)
{
//    GPIO_InitTypeDef GPIO_InitStructure;

	/* Power Down the codec */
	AUDIO_RESET_GPIO->BSRRH = AUDIO_RESET_PIN;

//	/* AD1852 reset pin -------------------------------------------------*/
//	GPIO_InitStructure.GPIO_Pin = AUDIO_RESET_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStructure);


	I2S_DeConfiguration();

	AD1852REG_INIT = RESET;
}


/**
  * @brief  Writes a Byte to a given register into the audio codec through the
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
static void AD1852_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue)
{
	AD1852_REG_VALUE = (RegisterValue & 0xFFFC) | RegisterAddr;
	SPI_WriteWord(AD1852_REG_VALUE);
}


/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
static uint32_t AD1852_ReadRegister(uint8_t RegisterAddr)
{
	return AD1852_REG_VALUE;
}



