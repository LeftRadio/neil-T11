/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides all exceptions handler and peripherals interrupt
  *          service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usbd_core.h"
#include "usbd_audio_core.h"
#include "touchscreen.h"
#include "platform.h"
#include "Global_Init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CURSOR_STEP     7

extern const int16_t sinebuf2[96];
extern uint8_t Buffer[6];

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
//	if (STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET) {STM_EVAL_LEDOn(LED6);Audio_MAL_Play((uint32_t)(&sinebuf2[0]),40);} else {STM_EVAL_LEDOff(LED6);}
//	Audio_MAL_Play((uint32_t)(&sinebuf2[0]),40);
}

/******************************************************************************/
/*                 STM32Fxxx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32fxxx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @brief  This function handles EXTI0_IRQ Handler.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
//	UserButtonPressed = 0x01;
	/* Clear the EXTI line pending bit */
//	EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
}

/**
  * @brief  This function handles EXTI15_10_IRQ Handler.
  * @param  None
  * @retval None
  */
void OTG_FS_WKUP_IRQHandler(void)
{
//  if(USB_OTG_dev.cfg.low_power)
//  {
//	/* Reset SLEEPDEEP and SLEEPONEXIT bits */
//	SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
//
//	/* After wake-up from sleep mode, reconfigure the system clock */
//	SystemInit();
//    USB_OTG_UngateClock(&USB_OTG_dev);
//  }
//  EXTI_ClearITPendingBit(EXTI_Line18);
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */

#ifndef USE_USB_OTG_HS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}


#ifdef USB_OTG_HS_DEDICATED_EP1_ENABLED
/**
  * @brief  This function handles EP1_IN Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_IN_IRQHandler(void)
{
  USBD_OTG_EP1IN_ISR_Handler (&USB_OTG_dev);
}

/**
  * @brief  This function handles EP1_OUT Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_EP1_OUT_IRQHandler(void)
{
  USBD_OTG_EP1OUT_ISR_Handler (&USB_OTG_dev);
}
#endif


volatile long timer = 0;


/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval : None
  */
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		/* Clear the  EXTI line 2 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line2);

		if(TouchScreen_Done == RESET)
		{
			NVIC_DisableIRQ(EXTI2_IRQn);
			TouchScreen_SetState(Touch_AxisX_State);
			TIM_Cmd(TIM3, ENABLE);
		}
//		else
//		{
//			while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != (uint8_t)Bit_RESET));
//			TouchScreen_SetState(Touch_Interrupt);
//			TouchScreen_Done = RESET;
//		}
	}
}


/**
  * @brief  This function handles DMA1_Channel1_IRQHandler
  * @param  None
  * @retval : None
  */
void DMA2_Stream0_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) != RESET)
	{
//		T11_LED_Toggle(T11_LED_0);

//		DMA2_Stream0->CR &= ~(uint32_t)DMA_SxCR_EN;
//		ADC_Cmd(ADC3, DISABLE);
//		SCB_DEMCR |= 0x01000000;
//		DWT_CYCCNT  = 0;
//		DWT_CONTROL|= 1; // enable counter

		TIM_Cmd(TIM3, DISABLE);
		TouchScreen_UpdateState();




//		timer = DWT_CYCCNT;
//		DWT_CONTROL &= ~1; // disable counter

//		/* Run the DMA Stream */
//		DMA2_Stream0->CR |= (uint32_t)DMA_SxCR_EN;
//		ADC_Cmd(ADC3, ENABLE);
//		ADC_SoftwareStartConv(ADC3);
	}

	/* Clear DMA2 stream 0 interrupt pending bits */
	DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_FEIF0 | DMA_IT_DMEIF0 | DMA_IT_TEIF0 | DMA_IT_HTIF0 | DMA_IT_TCIF0);
}




/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
