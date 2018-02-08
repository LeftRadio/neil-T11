/**
  ******************************************************************************
  * @file       platform_types.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_TYPES_H
#define __PLATFORM_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "t11_hal_types.h"
#include "t11_audio_if_types.h"
#include "t11_user_if_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef struct _T11_Platform {
    T11_HAL_TypeDef* hal;
    T11_AUDIO_IF_TypeDef* audio_if;
    T11_USER_IF_TypeDef* user_if;
    void (*Init)      (void);
    void (*DeInit)    (void);
} T11_Platform_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/


#endif /* __PLATFORM_TYPES_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/