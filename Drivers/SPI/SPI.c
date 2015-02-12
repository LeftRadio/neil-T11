/**
  ******************************************************************************
  * @file	 	SPI.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "systick.h"
#include "SPI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_NSS_PORT							GPIOA
#define SPI_NSS_PIN								GPIO_Pin_4
#define SPI_NSS_SET_HIGHT()						SPI_NSS_PORT->BSRRL = SPI_NSS_PIN
#define SPI_NSS_SET_LOW()						SPI_NSS_PORT->BSRRH = SPI_NSS_PIN

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SPI_Configuration
* Description    :
* Input          : speed in Hz
* Return         : None
*******************************************************************************/
void SPI_Configuration(SPI_InitTypeDef* SPI_InitStruct)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable SPI peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* Configure pins used by SPI1 */
	SPI_NSS_SET_HIGHT();
	GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(SPI_NSS_PORT, &GPIO_InitStruct);
	SPI_NSS_SET_HIGHT();

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	/* configure SPI1 */
	SPI_Init(SPI1, SPI_InitStruct);

	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);

	SPI_Cmd(SPI1, ENABLE); // enable SPI1
}


/*******************************************************************************
* Function Name  : SPI_Configuration
* Description    :
* Input          : speed in Hz
* Return         : None
*******************************************************************************/
void SPI_WriteWord(uint16_t Value)
{
	SPI1->DR = Value; // write data to be transmitted to the SPI data register
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != RESET);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);

	SPI_NSS_SET_LOW();
	delay_us(100);
	SPI_NSS_SET_HIGHT();
}





/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
