/**
  ******************************************************************************
  * @file	 	Global_Init.c
  * @author  	Left Radio
  * @version
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"
#include "I2C.h"
#include "SPI.h"
#include "Audio_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define SI5351_I2C_ADDR				0xC0


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

__IO uint16_t SOF_TimerUpdate;

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void DMA_memcpy_init(void);
//static void TIM2_Configuration(void);
//static void TIM4_Configuration(void);



/* Private functions --------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void Global_peripheal_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
			RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(T11_LEDs_PORT, &GPIO_InitStructure);

	Delay_Configuration();

//	DMA_memcpy_init();
}



/**
  * @brief  DMA_memcpy_init
  * @param  None
  * @retval None
  */
static void DMA_memcpy_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_Cmd(DMA2_Stream0, DISABLE);
	DMA_DeInit(DMA2_Stream0);

	/* Set the parameters to be configured */
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)IsocOutBuff;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AudioBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = (uint32_t)AUDIO_OUT_PACKET;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
}


/**
  * @brief  DMA_memcpy8
  * @param  None
  * @retval None
  */
void DMA_memcpy8(void* pDstAddr, void* pSrcAddr, unsigned int uSize)
{
    /* As per page 233 this is how to configure a stream */
	if((DMA2_Stream0->CR & (uint32_t)DMA_SxCR_EN) != 0)
	{
		DMA2_Stream0->CR &= ~(uint32_t)DMA_SxCR_EN;
		while((DMA2_Stream0->CR & (uint32_t)DMA_SxCR_EN) != 0)
		{
		}
	}

	DMA2_Stream0->M0AR = (uint32_t)pDstAddr;		// Destination
	DMA2_Stream0->PAR = (uint32_t)pSrcAddr;		// Sourse
	DMA2_Stream0->NDTR = (uint32_t)uSize;

    /* Run the DMA Stream */
	DMA2_Stream0->CR |= (uint32_t)DMA_SxCR_EN;
}


///**
//  * @brief  TIM2_Configuration
//  * @param  None
//  * @retval None
//  */
//static void TIM2_Configuration(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//
//	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR1);
////	TIM_ITRxExternalClockConfig(TIM2, TIM_TS_ITR2);
//
//	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
//	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);
//
//	TIM2->OR |= 1 << 11;
//
////	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);
//
//	/* Enable the CC2 Interrupt Request */
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//
//	/* TIM enable counter */
//	TIM_Cmd(TIM2, ENABLE);
//
//	NVIC_EnableIRQ(TIM2_IRQn);
//
//}
//
//
///**
//  * @brief  TIM4_Configuration
//  * @param  None
//  * @retval None
//  */
//static void TIM4_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_ICInitTypeDef  TIM_ICInitStructure;
//
//	/* TIM1 clock enable */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//
//	/* GPIOA clock enable */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//
//	/* TIM1 channel 2 pin (PE.11) configuration */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	/* Connect TIM pins to AF2 */
//	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
//
//	/* Enable the TIM1 global Interrupt */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 15;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
//	TIM_ICInitStructure.TIM_ICFilter = 0x0;
//
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);
//
//	/* TIM enable counter */
//	TIM_Cmd(TIM1, ENABLE);
//
//	/* Enable the CC2 Interrupt Request */
//	TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
//
////	/* TIM1 clock enable */
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
////
////	/* GPIOA clock enable */
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
////
////	/* TIM1 channel 2 pin (PE.11) configuration */
////	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//////	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
////	GPIO_Init(GPIOD, &GPIO_InitStructure);
////
////	/* Connect TIM pins to AF2 */
////	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
////
////	/* Enable the TIM1 global Interrupt */
////	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
////
////
////	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
////	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
////	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
////	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
////	TIM_ICInitStructure.TIM_ICFilter = 0x0;
////
////	TIM_ICInit(TIM4, &TIM_ICInitStructure);
////
////	/* TIM enable counter */
////	TIM_Cmd(TIM4, ENABLE);
////
////	/* Enable the CC2 Interrupt Request */
////	TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
//}
//




void TIM1_CC_IRQHandler(void)
{
	static uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
	static uint16_t CaptureNumber = 0, i = 0;
	uint32_t Capture = 0;
	static uint32_t TIM1Freq = 0;
	static uint32_t TIM1FreqSum = 0;

	if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET)
	{
		/* Clear TIM1 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
		if(CaptureNumber == 0)
		{
			/* Get the Input Capture value */
			IC3ReadValue1 = TIM_GetCapture2(TIM1);
			CaptureNumber = 1;
		}
		else if(CaptureNumber == 1)
		{
			/* Get the Input Capture value */
			IC3ReadValue2 = TIM_GetCapture2(TIM1);

			/* Capture computation */
			if (IC3ReadValue2 > IC3ReadValue1)
			{
				Capture = (IC3ReadValue2 - IC3ReadValue1);
			}
			else if (IC3ReadValue2 < IC3ReadValue1)
			{
				Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2);
			}
			else
			{
				Capture = 0;
			}
			/* Frequency computation */
			TIM1Freq = (uint32_t) SystemCoreClock / Capture;
			TIM1FreqSum += TIM1Freq;
			CaptureNumber = 0;

			i++;
			if(i >= 100)
			{
				i = 0;
				TIM1FreqSum /= 100;
				TIM1Freq = TIM1FreqSum;
				TIM1FreqSum = 0;
			}
		}
	}
}



void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		SOF_TimerUpdate++;
	}
}






