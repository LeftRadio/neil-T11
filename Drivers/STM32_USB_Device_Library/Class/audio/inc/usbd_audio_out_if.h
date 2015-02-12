/**
  ******************************************************************************
  * @file    usbd_audio_out_if.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    22-July-2011
  * @brief   header file for the usbd_audio_out_if.c file.
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

#ifndef __USB_AUDIO_OUT_IF_H_
#define __USB_AUDIO_OUT_IF_H_

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup usbd_audio
  * @brief This file is the Header file for USBD_audio.c
  * @{
  */


/** @defgroup usbd_audio_Exported_Defines
  * @{
  */
/* Audio enmuerations */
typedef enum { AUDIO_CMD_PAUSE, AUDIO_CMD_PLAY, AUDIO_CMD_STOP } AUDIO_CMD_TypeDef;
typedef enum { AUDIO_OK = 0, AUDIO_FAIL = 0xFF } AUDIO_CMD_STATE_TypeDef;
typedef enum {
	AUDIO_STATE_INACTIVE = 0x00,
	AUDIO_STATE_ACTIVE   = 0x01,
	AUDIO_STATE_PLAYING  = 0x02,
	AUDIO_STATE_PAUSED   = 0x03,
	AUDIO_STATE_STOPPED  = 0x04,
	AUDIO_STATE_ERROR    = 0x05
} AUDIO_MACHINE_STATE_TypeDef;

typedef enum { AUDIO_MUTE, AUDIO_UNMUTE } AUDIO_MUTE_TypeDef;

typedef struct _Audio_Fops
{
    uint8_t  (*Init)         (void);
    uint8_t  (*DeInit)       (void);
    uint8_t  (*AudioCmd)     (uint8_t cmd);
    uint8_t  (*VolumeCtl)    (uint8_t vol);
    uint8_t  (*MuteCtl)      (uint8_t cmd);
    uint8_t  (*PeriodicTC)   (uint8_t cmd);
    uint8_t  (*GetState)     (void);
} AUDIO_FOPS_TypeDef;


extern AUDIO_FOPS_TypeDef UsbAudioFops;

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

//extern AUDIO_IF_TypeDef AudioInterface;

/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
/**
  * @}
  */

#endif  /* __USB_AUDIO_OUT_IF_H_ */
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
