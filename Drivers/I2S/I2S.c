/**
  ******************************************************************************
  * @file		I2S.c
  * @author		Neil Lab :: Left Radio
  * @version	v1.0.0
  * @date		2015-01-12-15.07
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "Audio_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const int16_t sinebuf2[96] = {0,0, 4276,4276, 8480,8480, 12539,12539, 16383,16383, 19947,19947, 23169,23169, 25995,25995,
                             28377,28377, 30272,30272, 31650,31650, 32486,32486, 32767,32767, 32486,32486, 31650,31650, 30272,30272,
                             28377,28377, 25995,25995, 23169,23169, 19947,19947, 16383,16383, 12539,12539, 8480,8480, 4276,4276,
                             0,0, -4276,-4276, -8480,-8480, -12539,-12539, -16383,-16383, -19947,-19947, -23169,-23169, -25995,-25995,
                             -28377,-28377, -30272,-30272, -31650,-31650, -32486,-32486, -32767,-32767, -32486,-32486, -31650,-31650, -30272,-30272,
                             -28377,-28377, -25995,-25995, -23169,-23169, -19947,-19947, -16383,-16383, -12539,-12539, -8480,-8480, -4276,-4276
                             };

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  I2S_Configuration
  * @param  None
  * @retval None
  */
void I2S_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	/* I2S pins configuration: SCK, SD and WS pins -----------------------------*/
	GPIO_InitStructure.GPIO_Pin = I2S_1_SCK_PIN | I2S_1_SD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(I2S_1_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = I2S_1_WS_PIN ;
	GPIO_Init(I2S_1_WS_GPIO, &GPIO_InitStructure);

	/* Connect pins to I2S peripheral -----------------------------*/
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SCK_PINSRC, I2S_1_GPIO_AF);
	GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SD_PINSRC, I2S_1_GPIO_AF);
	GPIO_PinAFConfig(I2S_1_WS_GPIO, I2S_1_WS_PINSRC, I2S_1_GPIO_AF);

	if(AudioInterface.I2S->I2S_MCLK_External == I2S_MCLKExternal_Enable)
	{
		/* I2S pins configuration: CLK IN pin */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		/* Connect pin to I2S peripheral  */
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SPI1);

		RCC_I2SCLKConfig(RCC_I2S2CLKSource_Ext);
	}

	/* Enable the I2S peripheral clock */
	RCC_APB1PeriphClockCmd(I2S_1_CLK, ENABLE);

	/* I2S peripheral deinit */
	SPI_I2S_DeInit(I2S_1);

	/* ReInitialize the I2S peripheral with the structure AudioInterface.AI_I2S */
	I2S_Init(I2S_1, (I2S_InitTypeDef*)AudioInterface.I2S);

	/* Enable the DMA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	/* Configure the I2S DMA Stream */
	DMA_Cmd(DMA1_Stream7, DISABLE);
	DMA_DeInit(DMA1_Stream7);

	/* Set the parameters to be configured */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2S_1_ADDRESS;
	//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;      /* This field will be configured in play function */
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AudioBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//	DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE;      /* This field will be configured in play function */
	DMA_InitStructure.DMA_BufferSize = (uint32_t)(AUDIO_BUFFER_SIZE_BYTES / 2);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_3QuartersFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream7, &DMA_InitStructure);

//	/* Enable the selected DMA interrupts */
//	DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE);
//
//	/* I2S DMA IRQ Channel configuration */
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	/* Enable the I2S DMA request */
	SPI_I2S_DMACmd(I2S_1, SPI_I2S_DMAReq_Tx, ENABLE);

	I2S_1->I2SCFGR |= SPI_I2SCFGR_I2SE;
	DMA1_Stream7->CR |= (uint32_t)DMA_SxCR_EN;

//	/* Enable the I2S DMA Stream*/
//	DMA_Cmd(DMA1_Stream7, ENABLE);
}


/**
  * @brief  I2S_Configuration
  * @param  None
  * @retval None
  */
void I2S_DeConfiguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Deinitialize the NVIC interrupt for the I2S DMA Stream */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Disable the DMA stream before the deinit */
	DMA_Cmd(DMA1_Stream7, DISABLE);

	/* Dinitialize the DMA Stream */
	DMA_DeInit(DMA1_Stream7);

	/* Disable the CODEC_I2S peripheral (in case it hasn't already been disabled) */
	I2S_Cmd(I2S_1, DISABLE);

	/* Deinitialize the CODEC_I2S peripheral */
	SPI_I2S_DeInit(I2S_1);

	/* Disable the CODEC_I2S peripheral clock */
	RCC_APB1PeriphClockCmd(I2S_1_CLK, DISABLE);
}




/**
  * @brief  AUDIO_I2S_DMA_IRQHandler
  * @param  None
  * @retval None
  */
void AUDIO_I2S_DMA_IRQHandler(void)
{
//	uint32_t WritePoint = (uint32_t)(IsocOutWrPtr - AudioBuffer);
//	static uint16_t i = 0, j = 0;
	static uint16_t cnt = 0;
//	uint16_t SOF_Cnt_tmp = TIM2->CNT;

//	int Wr = (int)WritePoint - (int)AUDIO_BUFFER_HALFSIZE;

	if (DMA_GetFlagStatus(DMA1_Stream7, DMA_FLAG_TCIF7) != RESET)
	{
//		while (DMA_GetCmdStatus(DMA1_Stream7) != DISABLE)
//		{}
//
////		DMA1_Stream7->CR &= ~(uint32_t)DMA_SxCR_EN;
//
//		/* Reset DMA memory pointer and transfer size */
//		DMA1_Stream7->M0AR = (uint32_t)AudioBuffer;
//		DMA1_Stream7->NDTR = (uint32_t)(AUDIO_BUFFER_SIZE_BYTES / 2);
//
//		/* Run the DMA Stream */
//		DMA1_Stream7->CR |= (uint32_t)DMA_SxCR_EN;

//		if(PlayFlag != 0)
//		{
			if(++cnt >= 10)
			{
				cnt = 0;
				T11_LED_Toggle(T11_LED_0);
			}
//		}

//		if(++i >= 10)		// 1 sec
//		{
//			if(j > 0)
//			{
//				Frequency_IN = ((float)USB_ReciveAudioPackedCnt / 4) + ((float)Wr);
//			}
//			else j = 1;
//
//			USB_ReciveAudioPackedCnt = 0;
//			i = 0;
//		}
//
//		if(Wr == 0)
//		{
////			STM_EVAL_LEDOn(LED3); STM_EVAL_LEDOn(LED6);
//		}
//		else if(Wr > 0)
//		{
////			STM_EVAL_LEDOn(LED3); STM_EVAL_LEDOff(LED6);
//		}
//		else
//		{
////			STM_EVAL_LEDOn(LED6); STM_EVAL_LEDOff(LED3);
//		}
//
//		if((Wr > AUDIO_BUFFER_HALFSIZE/2) || (Wr < -(AUDIO_BUFFER_HALFSIZE/2)))
//		{
////			STM_EVAL_LEDOn(LED4);
//		}
//		else
//		{
////			STM_EVAL_LEDOff(LED4);
//		}

		/* Clear the Interrupt flag */
		DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
	}
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
