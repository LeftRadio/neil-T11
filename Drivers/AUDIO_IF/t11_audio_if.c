/**
  ******************************************************************************
  * @file       t11_audio_if.c
  * @author    Neil Lab :: Left Radio
  * @version   v1.0.0
  * @date
  * @brief    sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "t11_audio_if.h"
#include "pga2311.h"
#include "AD1852.h"
#include "Si5351.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AUDIO_BUFFER_SIZE_BYTES                     ((uint32_t)(19200))

#define AUDIO_IF_GLOBAL_MUTE_PORT                   GPIOC
#define AUDIO_IF_GLOBAL_MUTE_PIN                    GPIO_Pin_1

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint8_t Init(void* platform);
static uint8_t DeInit(void* platform);
static void Update(void* platform);
static void StreamUpdate(void* platform);
static uint8_t AudioCmd(void* platform, uint8_t cmd);
static uint8_t VolumeCtl(void* platform, uint8_t vol);
static uint8_t MuteCtl(void* platform, uint8_t cmd);
static uint8_t GetState(void);
static uint8_t GetMute(void);
static void Reset(void);

/* Private variables ---------------------------------------------------------*/
volatile uint8_t AudioBuffer[AUDIO_BUFFER_SIZE_BYTES];

/* Audio Alternate Settings */
volatile T11_AUDIO_IF_SETTINGS_Typedef T11_Audio_IF_Settings = {
    0,                      /* index */
    48000,                  /* sampling_freq */
    { 1, 2, 2 },            /* channels */
    { 1, 16, 24 }           /* bit_resolution */
};
/* Audio Stream */
volatile T11_AUDIO_IF_STREAM_TypeDef T11_Audio_IF_Stream = {
    (uint8_t*)AudioBuffer,
    (uint8_t*)AudioBuffer,
    0,
    48 * 2 * 2,                 /* frame_size */
    AUDIO_BUFFER_SIZE_BYTES,
    AUDIO_BUFFER_SIZE_BYTES,
    0
};
/* Audio Interface */
volatile T11_AUDIO_IF_TypeDef T11_Audio_IF = {
    AUDIO_IF_STATE_STOPPED,

    &T11_Audio_IF_Settings,
    &T11_Audio_IF_Volume,
    &DAC_AD1852,
    &si5351,
    &T11_Audio_IF_Stream,

    Init,
    DeInit,
    Update,
    StreamUpdate,
    AudioCmd,
    VolumeCtl,
    MuteCtl,
    GetState,
    GetMute,
    Reset
};

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize all required resources for audio interface.
  * @param  None
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t Init(void* platform) {
	  /* */
	  GPIO_InitTypeDef GPIO_InitStruct;
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_TypeDef* aif = plt->audio_if;
    /* DeInitialization interface */
    aif->DeInit( platform );
    /* Global MUTE (EXMUTE, PC1) */
    GPIO_ResetBits( AUDIO_IF_GLOBAL_MUTE_PORT, AUDIO_IF_GLOBAL_MUTE_PIN );
    GPIO_InitStruct.GPIO_Pin = AUDIO_IF_GLOBAL_MUTE_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( AUDIO_IF_GLOBAL_MUTE_PORT, &GPIO_InitStruct );
    /* Initialization interface */
    aif->clock->Init( platform );
    aif->dac->Init( platform );
    aif->volume->Init( platform );
    return AUDIO_IF_OK;
}

/**
  * @brief  Free all resources used by low layer and stops audio-play function.
  * @param  None
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t DeInit(void* platform) {
	/* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
	T11_AUDIO_IF_TypeDef* aif = plt->audio_if;
    /* stop audio-play function */
	aif->AudioCmd( platform, AUDIO_IF_STOP );
    /* DeInitialization peripherals */
	aif->dac->DeInit( platform );
	aif->clock->DeInit( platform );
    return AUDIO_IF_OK;
}

/**
  * @brief  Update
  * @param  platform pointer
  * @retval None
  */
static void Update(void* platform) {
	/* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
	T11_AUDIO_IF_TypeDef* aif = plt->audio_if;
	/* */
  if( aif->settings->index == 0 ) {
      aif->AudioCmd( platform, AUDIO_IF_STOP );
  }
  else {
      if( aif->stream->filled_by >= 50 ) {
          aif->AudioCmd( platform, AUDIO_IF_PLAY );
      }
  }
	/* */
    // aif->dac->Update( platform );
}

/**
  * @brief  StreamUpdate
  * @param  platform pointer
  * @retval None
  */
static void StreamUpdate(void* platform) {
    /* */
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_TypeDef* aif = plt->audio_if;
    T11_AUDIO_IF_SETTINGS_Typedef* sett = aif->settings;
    T11_AUDIO_IF_STREAM_TypeDef* stream = aif->stream;

    if(sett->index == 0) {
    	return;
    }

    uint16_t sampling_count_1ms = sett->sampling_freq / 1000;
    uint16_t sample_widht = sett->bit_resolution[ sett->index ] / 8;
    uint16_t channels = sett->channels[ sett->index ];
    uint16_t frame_size = sampling_count_1ms * channels * sample_widht;
    float buffer_size = (float)stream->buffer_bytes_size;
    float buffer_frames = buffer_size / (float)frame_size;

    stream->cpy_ptr = (uint8_t*)AudioBuffer;
    stream->read_cnt = 0;
    stream->frame_size = frame_size;
    stream->buffer_align_size = (uint32_t)buffer_frames * (uint32_t)frame_size;
    stream->filled_by = 0;

    aif->Init( platform );
}

/**
  * @brief  Play, Stop, Pause or Resume current file.
  * @param  cmd: command to be executed, can be AUDIO_CMD_PLAY , AUDIO_IF_PAUSE,
  *              AUDIO_CMD_RESUME or AUDIO_IF_STOP.
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t AudioCmd(void* platform, uint8_t cmd) {
    /* */
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_TypeDef* aif = plt->audio_if;

    switch (cmd) {

        /* Process the PLAY command ----------------------------*/
        case AUDIO_IF_PLAY:
            /* If current state is Active or Stopped */
            if ( aif->state == AUDIO_IF_STATE_STOPPED ) {
                /* Call the DAC HAL Play function */
                aif->dac->Play( platform );
                /* */
                aif->state = AUDIO_IF_STATE_PLAYING;
                return AUDIO_IF_OK;
            }
            /* If current state is Paused */
            else if ( aif->state == AUDIO_IF_STATE_PAUSED ) {
                /* Call the DAC HAL Pause/Resume function */
                aif->dac->PauseResume( platform, AUDIO_IF_PLAY );
                /* */
                aif->state = AUDIO_IF_STATE_PLAYING;
                return AUDIO_IF_OK;
            }
        break;

        /* Process the STOP command ----------------------------*/
        case AUDIO_IF_STOP:
            if ( aif->state != AUDIO_IF_STATE_STOPPED ) {
                aif->dac->Stop( platform );
                aif->state = AUDIO_IF_STATE_STOPPED;
                return AUDIO_IF_OK;
            }
        break;

        /* Process the PAUSE command ---------------------------*/
        case AUDIO_IF_PAUSE:
            if ( aif->state == AUDIO_IF_STATE_PLAYING ) {
                /* Call the Audio IF */
                aif->dac->PauseResume(platform, AUDIO_IF_PAUSE);
                // T11_LED_OFF(T11_LED_0);
                aif->state = AUDIO_IF_STATE_PAUSED;
                return AUDIO_IF_OK;
            }
        break;

        /* Unsupported command ---------------------------------*/
        default:
        break;
    }

    return AUDIO_IF_FAIL;
}

/**
  * @brief  Set the volume level in %
  * @param  vol: volume level to be set in % (from 0% to 100%)
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t VolumeCtl(void* platform, uint8_t vol) {
    return AUDIO_IF_OK;
}

/**
  * @brief  Mute or Unmute the audio current output
  * @param  cmd: can be 0 to unmute, or 1 to mute.
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t MuteCtl(void* platform, uint8_t cmd) {
  /* */
  T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
  /* */
  if ( cmd != DISABLE ) {
    plt->hal->relay->Mute( AUDIO_IF_MUTE );
    AUDIO_IF_GLOBAL_MUTE_PORT->BSRRL = AUDIO_IF_GLOBAL_MUTE_PIN;
  }
  else {
    AUDIO_IF_GLOBAL_MUTE_PORT->BSRRH = AUDIO_IF_GLOBAL_MUTE_PIN;
    plt->hal->relay->Mute( AUDIO_IF_UNMUTE );
  }
  /* */
  return AUDIO_IF_OK;
}

/**
  * @brief  Return the current state of the audio machine
  * @param  None
  * @retval Current State.
  */
static uint8_t GetState(void) {
    return T11_Audio_IF.state;
}

/**
  * @brief  Mute or Unmute the audio current output
  * @param  cmd: can be 0 to unmute, or 1 to mute.
  * @retval AUDIO_IF_OK if all operations succeed, AUDIO_IF_FAIL else.
  */
static uint8_t GetMute(void) {
    if( AUDIO_IF_GLOBAL_MUTE_PORT->ODR & AUDIO_IF_GLOBAL_MUTE_PIN ) {
        return 1;
    }
    return 0;
}

/**
  * @brief  Audio interface Reset
  * @param  None
  * @retval None
  */
static void Reset(void) {
}

/*******************************************************************************
      END FILE
*******************************************************************************/
