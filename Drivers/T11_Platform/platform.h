/**
  ******************************************************************************
  * @file	 	platform.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_H
#define __PLATFORM_H

/* Includes ------------------------------------------------------------------*/
#include "platform_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void T11_IN_Select(T11_AudioIn_Typedef AudioIn);
T11_AudioIn_Typedef T11_IN_GetState(void);
void T11_Mute(T11_AudioMute_Typedef NewMuteState);
void T11_Mute_AmpRelay(T11_AudioMute_Typedef NewMuteState);
T11_AudioMute_Typedef T11_Mute_GetAmpRelayState(void);

/* Exported variables --------------------------------------------------------*/
extern T11_Platform_TypeDef T11_Platform;


#endif /* __PLATFORM_H */
/*******************************************************************************
      END FILE
*******************************************************************************/