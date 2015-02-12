/**
  ******************************************************************************
  * @file	 	Audio_Interface.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AUDIO_INTERFACE_H
#define __AUDIO_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "platform.h"
#include "usbd_audio_core.h"
#include "I2S.h"
#include "Si5351.h"

/* Exported define -----------------------------------------------------------*/
#define MCLK_OUT_ENABLE					(ENABLE)
#define MCLK_OUT_DISABLE				(DISABLE)
#define AI_INIT							(SET)
#define AI_NO_INIT						(RESET)

/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
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
typedef enum { DeEmphasisOFF, DeEmphasisON } DeEmphasis_TypeDef;
typedef enum {
	I2S_Phillips 		= (uint16_t)0x0000,
	I2S_LeftJustified 	= (uint16_t)0x0010,
	I2S_RightJustified 	= (uint16_t)0x0020,
	I2S_PCMShort 		= (uint16_t)0x0030,
	I2S_PCMLong 		= (uint16_t)0x00B0,
} I2S_Standart_TypeDef;

typedef enum { INT_NONE, INT_2X, INT_4X, INT_8X } Interpolator_Typedef;
typedef struct
{
	DeEmphasis_TypeDef DeEmphasis;
	I2S_Standart_TypeDef I2S_Standart;
	uint16_t I2S_nBits;
	Interpolator_Typedef Interpolator;
	void (*Init)(void);
	void (*DeInit)(void);
	void (*Play)(void);
	void (*PauseResume)(AUDIO_CMD_TypeDef Cmd);
	void (*Stop)(void);
	void (*VolumeCtrl)(uint8_t Volume);
	void (*Mute)(AUDIO_MUTE_TypeDef State);
	uint32_t UsrData[24];
} DAC_HAL_TypeDef;

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

typedef struct _Audio_IF
{
	volatile I2S_InitTypeDef* I2S;
	volatile DAC_HAL_TypeDef* DAC_HAL;
	volatile Si5351_TypeDef* Si5351;
	AUDIO_FOPS_TypeDef* Audio_Fops;
	FlagStatus InitFlag;
} AUDIO_IF_TypeDef;


/* Exported variables --------------------------------------------------------*/
extern AUDIO_IF_TypeDef AudioInterface;

/* Exported function ---------------------------------------------------------*/






#endif /* __AUDIO_INTERFACE_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
