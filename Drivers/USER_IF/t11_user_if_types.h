/**
  ******************************************************************************
  * @file       t11_user_if_types.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_IF_TYPES_H
#define __USER_IF_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef struct {
    const uint8_t channels;
    NGL_FillBar* fillbars[16];
    void (*Init)(void* platform);
    void (*Update)(void* platform);
} T11_PeekLevel_TypeDef;

typedef struct _User_IF {
    NGL_HAL_Typedef* lcd_struct;
    NGL_TouchScreen* touchscreen;
    T11_PeekLevel_TypeDef* peeklevel;

    void (*Init)(void* platform);
    void (*DeInit)(void* platform);
    void (*Update)(void* platform);
    uint16_t (*BackLight)(uint16_t bl_precent);
} T11_USER_IF_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/


#endif /* __USER_IF_TYPES_H */
/*******************************************************************************
      END FILE
*******************************************************************************/