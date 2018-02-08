/**
  ******************************************************************************
  * @file         LEDS.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief        sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "LEDS.h"

/* Private define ------------------------------------------------------------*/
#define __LED_CNT                                   ((uint8_t)(2))

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void T11_LEDS_Init(void);
static void T11_LEDS_DeInit(void);
static void T11_LEDS_Write(uint8_t indx, uint8_t val);
static uint8_t T11_LEDS_Read(uint8_t indx);
static void T11_LEDS_Toggle(uint8_t indx);

/* Private variables ---------------------------------------------------------*/
T11_HAL_LEDS_TypeDef T11_HLEDS = {
    { GPIOC, GPIOC },
    { GPIO_Pin_7 , GPIO_Pin_8 },

    T11_LEDS_Init,
    T11_LEDS_DeInit,
    T11_LEDS_Write,
    T11_LEDS_Read,
    T11_LEDS_Toggle
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  T11_LEDS_Init
  * @param  platform: platform instance
  * @retval None
  */
static void T11_LEDS_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;
    /* */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    /* */
    for (i = 0; i < 2; i++) {
        GPIO_InitStructure.GPIO_Pin = T11_HLEDS.pins[i];
        GPIO_Init( (GPIO_TypeDef*)T11_HLEDS.ports[i], &GPIO_InitStructure );
        T11_LEDS_Write(i, 0);
    }
}

/**
  * @brief  T11_LEDS_DeInit
  * @param  platform: platform instance
  * @retval None
  */
static void T11_LEDS_DeInit(void) {
}

/**
  * @brief  T11_LEDS_Write
  * @param  None
  * @retval None
  */
static void T11_LEDS_Write(uint8_t indx, uint8_t val) {
    if ( indx >= __LED_CNT || val >= 2 ) {
        return;
    }
    switch (val) {
        case 0:
            T11_HLEDS.ports[ indx ]->BSRRH = T11_HLEDS.pins[ indx ];
        break;
        case 1:
            T11_HLEDS.ports[ indx ]->BSRRL = T11_HLEDS.pins[ indx ];
        break;
        default:
        break;
    }
}

/**
  * @brief  T11_LEDS_Read
  * @param  None
  * @retval None
  */
static uint8_t T11_LEDS_Read(uint8_t indx) {
	  if (indx >= __LED_CNT) {
        return 0xFF;
    }
	  return ((T11_HLEDS.ports[indx]->ODR & T11_HLEDS.pins[indx]) != 0)? 1 : 0;
}

/**
  * @brief  Function_Name
  * @param  None
  * @retval None
  */
static void T11_LEDS_Toggle(uint8_t indx) {
	uint8_t s = T11_LEDS_Read(indx) ^ 0x01;
	T11_LEDS_Write(indx, s);
}

/*******************************************************************************
      END FILE
*******************************************************************************/
