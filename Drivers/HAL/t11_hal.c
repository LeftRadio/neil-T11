/**
  ******************************************************************************
  * @file	 	  t11_hal.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "t11_hal.h"
#include "platform.h"
#include "LEDS.h"
#include "relay.h"
#include "BKP_SRAM4K.h"
#include "I2C.h"
#include "SPI.h"
#include "I2S.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EX_RESET_PORT                             GPIOB
#define EX_RESET_PIN                              GPIO_Pin_7

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void* platform);
static void DeInit(void* platform);
static void Reset(void* platform);

/* Private variables ---------------------------------------------------------*/
T11_HAL_TypeDef T11_HAL = {
    &T11_HLEDS,
    &T11_HRELAY,
    &T11_HBKPSRAM,
    &T11_HI2C,
    &T11_HSPI,
    &T11_HI2S,
    Init,
    DeInit,
    Reset,
};

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize and configures all required resources.
  * @param  None
  * @retval None
  */
static void Init(void* platform) {
	/* */
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_HAL_TypeDef* hal = plt->hal;
    GPIO_InitTypeDef GPIO_InitStruct;
    /* */
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | \
        RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);
    /* */
    Delay_Configuration();
    /* Global reset pin, Active LOW */
    GPIO_SetBits( EX_RESET_PORT, EX_RESET_PIN );
    GPIO_InitStruct.GPIO_Pin = EX_RESET_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( EX_RESET_PORT, &GPIO_InitStruct );
    /*  */
    hal->leds->Init();
    hal->relay->Init();
    hal->bkpsram->Init();
    hal->i2c->Init( 100000 );
    hal->spi->Init();
}

/**
  * @brief  DeInitialize and configures all required resources.
  * @param  None
  * @retval None
  */
static void DeInit(void* platform){
}

/**
  * @brief  Reset
  * @param  None
  * @retval None
  */
static void Reset(void* platform) {
    EX_RESET_PORT->BSRRH = EX_RESET_PIN;
    delay_ms(1);
    EX_RESET_PORT->BSRRL = EX_RESET_PIN;
}


/*******************************************************************************
      END FILE
*******************************************************************************/
