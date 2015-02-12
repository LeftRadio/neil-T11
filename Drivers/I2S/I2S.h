/**
  ******************************************************************************
  * @file		temp.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-01-12-15.09
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2S_H
#define __I2S_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
#define I2S_1								SPI3
#define I2S_1_CLK							RCC_APB1Periph_SPI3
#define I2S_1_ADDRESS						0x40003C0C
#define I2S_1_GPIO_AF						GPIO_AF_SPI3

#define I2S_1_WS_PIN						GPIO_Pin_15
#define I2S_1_WS_PINSRC						GPIO_PinSource15
#define I2S_1_WS_GPIO						GPIOA

#define I2S_1_SCK_PIN						GPIO_Pin_10
#define I2S_1_SD_PIN						GPIO_Pin_12
#define I2S_1_SCK_PINSRC					GPIO_PinSource10
#define I2S_1_SD_PINSRC						GPIO_PinSource12
#define I2S_1_GPIO							GPIOC

#define I2S_1_IRQ							SPI3_IRQn
#define AUDIO_I2S_IRQHandler				SPI3_IRQHandler
#define AUDIO_I2S_DMA_IRQHandler       		DMA1_Stream7_IRQHandler


/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void I2S_Configuration(void);
void I2S_DeConfiguration(void);





#endif /* __I2S_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
