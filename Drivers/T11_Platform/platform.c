/**
  ******************************************************************************
  * @file	 	  platform.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "t11_hal.h"
#include "t11_audio_if.h"
#include "t11_user_if.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void DeInit(void);

/* Private variables ---------------------------------------------------------*/
T11_Platform_TypeDef T11_Platform = {
    &T11_HAL,
    &T11_Audio_IF,
    &T11_User_IF,
    Init,
    DeInit
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Function_Name
  * @param  None
  * @retval None
  */
void Init(void) {
    /* Configure platform common HAL */
    T11_Platform.hal->Init( &T11_Platform );
    /* Configure platform Audio Intrface */
    T11_Platform.audio_if->Init( &T11_Platform );
    /* Configure platform User Intrface */
    T11_Platform.user_if->Init( &T11_Platform );
}

/**
  * @brief  Function_Name
  * @param  None
  * @retval None
  */
void DeInit(void) {
    /* */
    T11_Platform.hal->DeInit( &T11_Platform );
    T11_Platform.audio_if->DeInit( &T11_Platform );
}


/*******************************************************************************
      END FILE
*******************************************************************************/