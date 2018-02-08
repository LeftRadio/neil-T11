/**
  ******************************************************************************
  * @file       t11_audio_if_types.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __T11_AIF_TYPES_H
#define __T11_AIF_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/

typedef enum { AUDIO_IF_PLAY, AUDIO_IF_PAUSE, AUDIO_IF_STOP } AUDIO_IF_CMD_TypeDef;
typedef enum { AUDIO_IF_OK = 0, AUDIO_IF_FAIL } AUDIO_IF_STATUS_TypeDef;

//typedef enum { AUDIO_MUTE, AUDIO_UNMUTE } AUDIO_MUTE_TypeDef;
typedef enum { INT_NONE, INT_2X, INT_4X, INT_8X } Interpolator_Typedef;

typedef enum { AUDIO_IF_IN_DAC = 0, AUDIO_IF_IN_EXT } T11_AudioIn_Typedef;
typedef enum { AUDIO_IF_MUTE = 1, AUDIO_IF_UNMUTE = 0} T11_AudioMute_Typedef;

typedef enum {
    AUDIO_IF_STATE_PLAYING = 0x00,
    AUDIO_IF_STATE_PAUSED,
    AUDIO_IF_STATE_STOPPED
} T11_AUDIO_IF_STATE_TypeDef;

/* ---------------------------------------------------------------------------*/

typedef struct _AIF_VOLUME_ {
    const uint8_t channels;
    const float min, max;
    const uint32_t reg_range;
    void (*Init)(void* platform);
    void (*DeInit)(void* platform);
    void (*SetLevel)(void* platform, float* vol);
} T11_AUDIO_IF_VOLUME_TypeDef;

typedef struct _AIF_Clock_ {
    uint32_t Frequency;
    uint8_t  OUT_num;
    void (*Init)(void* platform);
    void (*DeInit)(void* platform);
} T11_AUDIO_IF_CLOCK_TypeDef;

typedef struct {
    I2S_InitTypeDef* i2s_config;
    FlagStatus i2s_tx_complite;
    FunctionalState DeEmphasis;
    Interpolator_Typedef Interpolator;
    uint16_t UsrData[24];
    void (*Init)(void* platform);
    void (*DeInit)(void* platform);
    void (*Play)(void* platform);
    void (*PauseResume)(void* platform, AUDIO_IF_CMD_TypeDef cmd);
    void (*Stop)(void* platform);
    void (*VolumeCtrl)(void* platform, uint8_t volume);
    void (*MuteCtl)(void* platform, FunctionalState state);
} T11_AUDIO_IF_DAC_TypeDef;

typedef struct _AIF_Stream_ {
    uint8_t *buff_ptr;
    uint8_t *cpy_ptr;
    uint32_t read_cnt;
    uint16_t frame_size;
    const uint32_t buffer_bytes_size;
    uint32_t buffer_align_size;
    uint32_t filled_by;
} T11_AUDIO_IF_STREAM_TypeDef;

typedef struct _Audio_Sett {
    uint8_t index;
    uint32_t sampling_freq;
    const uint32_t channels[12];
    const uint32_t bit_resolution[12];
} T11_AUDIO_IF_SETTINGS_Typedef;

typedef struct _Audio_IF {
    T11_AUDIO_IF_STATE_TypeDef state;

    T11_AUDIO_IF_SETTINGS_Typedef* settings;
    T11_AUDIO_IF_VOLUME_TypeDef* volume;
    T11_AUDIO_IF_DAC_TypeDef* dac;
    T11_AUDIO_IF_CLOCK_TypeDef* clock;
    T11_AUDIO_IF_STREAM_TypeDef* stream;

    uint8_t (*Init)(void* platform);
    uint8_t (*DeInit)(void* platform);
    void (*Update)(void* platform);
    void (*StreamUpdate)(void* platform);
    uint8_t (*AudioCmd)(void* platform, uint8_t cmd);
    uint8_t (*VolumeCtl)(void* platform, uint8_t vol);
    uint8_t (*MuteCtl)(void* platform, uint8_t cmd);
    uint8_t (*GetState)(void);
    uint8_t (*GetMute)(void);
    void (*Reset)(void);
} T11_AUDIO_IF_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/


#endif /* __T11_AIF_TYPES_H */
/*******************************************************************************
      END FILE
*******************************************************************************/
