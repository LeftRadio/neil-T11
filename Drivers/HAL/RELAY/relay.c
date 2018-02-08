/**
  ******************************************************************************
  * @file         RELAY.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief        sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "relay.h"

/* Private define ------------------------------------------------------------*/
#define __RELAY_CNT                                   ((uint8_t)(2))

#define MUTE_RELAY_PIN                                GPIO_Pin_4
#define SOURSE_RELAY_PIN                              GPIO_Pin_13

#define MUTE_INDX                                     ((uint8_t)(0))
#define SOURSE_INDX                                   ((uint8_t)(1))

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void DeInit(void);
static void Mute(FunctionalState state);
static void Source(uint8_t source);

/* Private variables ---------------------------------------------------------*/
T11_HAL_RELAY_TypeDef T11_HRELAY = {
    { GPIOC, GPIOC },
    { MUTE_RELAY_PIN , SOURSE_RELAY_PIN },

    Init,
    DeInit,
    Mute,
    Source
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init
  * @param  platform: platform instance
  * @retval None
  */
static void Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;
    /* EXS0 (Mute relay, PC4); EXS2 (Audio sourse DAC/External, PC13) */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    /* */
    for (i = 0; i < __RELAY_CNT; i++) {
        GPIO_InitStructure.GPIO_Pin = T11_HRELAY.pins[i];
        GPIO_Init( T11_HRELAY.ports[i], &GPIO_InitStructure );
    }
    T11_HRELAY.Source( 1 );
    T11_HRELAY.Mute( ENABLE );
}

/**
  * @brief  DeInit
  * @param  platform: platform instance
  * @retval None
  */
static void DeInit(void) {
}

/**
  * @brief  Mute
  * @param  new state
  * @retval None
  */
static void Mute(FunctionalState state) {
    if ( state != DISABLE ) {
        T11_HRELAY.ports[MUTE_INDX]->BSRRH = T11_HRELAY.pins[MUTE_INDX];
    }
    else {
        T11_HRELAY.ports[MUTE_INDX]->BSRRL = T11_HRELAY.pins[MUTE_INDX];
    }
}

/**
  * @brief  Source
  * @param  None
  * @retval None
  */
static void Source(uint8_t source) {
    if( !source ) {
        T11_HRELAY.ports[SOURSE_INDX]->BSRRH = T11_HRELAY.pins[SOURSE_INDX];
    }
    else {
        T11_HRELAY.ports[SOURSE_INDX]->BSRRL = T11_HRELAY.pins[SOURSE_INDX];
    }
}


/*******************************************************************************
      END FILE
*******************************************************************************/