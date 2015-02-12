/**
  ******************************************************************************
  * @file	 	usbd_audio_out_if.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse file provides the Audio interface API.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
//#include "usbd_audio_core.h"
//#include "usbd_audio_out_if.h"
#include "Audio_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint8_t  Init         (void);
static uint8_t  DeInit       (void);
static uint8_t  AudioCmd     (uint8_t cmd);
static uint8_t  VolumeCtl    (uint8_t vol);
static uint8_t  MuteCtl      (uint8_t cmd);
static uint8_t  PeriodicTC   (uint8_t cmd);
static uint8_t  GetState     (void);

/* Private variables ---------------------------------------------------------*/
AUDIO_FOPS_TypeDef UsbAudioFops = {
		Init,
		DeInit,
		AudioCmd,
		VolumeCtl,
		MuteCtl,
		PeriodicTC,
		GetState
};

volatile uint8_t AudioState = AUDIO_STATE_INACTIVE;



/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Init
* Description    : Initialize and configures all required resources for audio play function.
* Input          :
* Return         : AUDIO_OK if all operations succeed, AUDIO_FAIL else
*******************************************************************************/
static uint8_t Init (void)
{
    /* Check if the low layer has already been initialized */
    if (AudioInterface.InitFlag == RESET)
    {
    	/* Configure Si5351 and start oscillation */
    	AudioInterface.Si5351->RegistersConfigurate();
    	delay_ms(10);

    	/* Configure the I2S/DMA peripherals */
    	I2S_Configuration();

    	/* Configure DAC */
    	AudioInterface.DAC_HAL->Init();
    }

    /* Update the Audio state machine */
    AudioState = AUDIO_STATE_ACTIVE;

    return AUDIO_OK;
}


/**
  * @brief  DeInit
  *         Free all resources used by low layer and stops audio-play function.
  * @param  options: options passed to low layer function.
  * @retval AUDIO_OK if all operations succeed, AUDIO_FAIL else.
  */
static uint8_t DeInit (void)
{
    /* DeInitialization the I2S/DMA/DAC peripherals */
	AudioInterface.DAC_HAL->DeInit();
	I2S_DeConfiguration();
	AudioInterface.Si5351->RegistersConfigurate();

    /* Update the Audio state machine */
    AudioState = AUDIO_STATE_INACTIVE;

    return AUDIO_OK;
}


/**
  * @brief  AudioCmd
  *         Play, Stop, Pause or Resume current file.
  * @param  pbuf: address from which file shoud be played.
  * @param  size: size of the current buffer/file.
  * @param  cmd: command to be executed, can be AUDIO_CMD_PLAY , AUDIO_CMD_PAUSE,
  *              AUDIO_CMD_RESUME or AUDIO_CMD_STOP.
  * @retval AUDIO_OK if all operations succeed, AUDIO_FAIL else.
  */
static uint8_t AudioCmd (uint8_t cmd)
{
    /* Check the current state */
    if ((AudioState == AUDIO_STATE_INACTIVE) || (AudioState == AUDIO_STATE_ERROR))
    {
        AudioState = AUDIO_STATE_ERROR;
        return AUDIO_FAIL;
    }

    switch (cmd)
    {
        /* Process the PLAY command ----------------------------*/
    case AUDIO_CMD_PLAY:
        /* If current state is Active or Stopped */
        if ((AudioState == AUDIO_STATE_ACTIVE) || (AudioState == AUDIO_STATE_STOPPED) || (AudioState == AUDIO_STATE_PLAYING))
        {
            /* Call the DAC HAL Play function */
//            DAC_HAL_Play();

            AudioState = AUDIO_STATE_PLAYING;
            return AUDIO_OK;
        }
        /* If current state is Paused */
        else if (AudioState == AUDIO_STATE_PAUSED)
        {
            /* Call the DAC HAL Pause/Resume function */
//            DAC_HAL_PauseResume(AUDIO_CMD_PLAY);

            AudioState = AUDIO_STATE_PLAYING;
            return AUDIO_OK;
        }
        else /* Not allowed command */
        {
            return AUDIO_FAIL;
        }

        /* Process the STOP command ----------------------------*/
    case AUDIO_CMD_STOP:
        if (AudioState != AUDIO_STATE_PLAYING)
        {
            /* Unsupported command */
            return AUDIO_FAIL;
        }
        else
        {
            /* Call the DAC HAL Stop function */
//            DAC_HAL_Stop();

            AudioState = AUDIO_STATE_STOPPED;
            return AUDIO_OK;
        }

        /* Process the PAUSE command ---------------------------*/
    case AUDIO_CMD_PAUSE:
        if (AudioState != AUDIO_STATE_PLAYING)
        {
            /* Unsupported command */
            return AUDIO_FAIL;
        }
        else
        {
            /* Call the DAC HAL Pause/Resume function */
//            DAC_HAL_PauseResume(AUDIO_CMD_PAUSE);

            AudioState = AUDIO_STATE_PAUSED;
            return AUDIO_OK;
        }

        /* Unsupported command ---------------------------------*/
    default:
        return AUDIO_FAIL;
    }
}


/**
  * @brief  VolumeCtl
  *         Set the volume level in %
  * @param  vol: volume level to be set in % (from 0% to 100%)
  * @retval AUDIO_OK if all operations succeed, AUDIO_FAIL else.
  */
static uint8_t VolumeCtl (uint8_t vol)
{
    /* Call low layer volume setting function */
//  if (EVAL_AUDIO_VolumeCtl(vol) != 0)
//  {
//    AudioState = AUDIO_STATE_ERROR;
//    return AUDIO_FAIL;
//  }

    return AUDIO_OK;
}


/**
  * @brief  MuteCtl
  *         Mute or Unmute the audio current output
  * @param  cmd: can be 0 to unmute, or 1 to mute.
  * @retval AUDIO_OK if all operations succeed, AUDIO_FAIL else.
  */
static uint8_t MuteCtl (uint8_t cmd)
{
    /* Call low layer mute setting function */
//  if (EVAL_AUDIO_Mute(cmd) != 0)
//  {
//    AudioState = AUDIO_STATE_ERROR;
//    return AUDIO_FAIL;
//  }

    return AUDIO_OK;
}


/**
  * @brief
  *
  * @param
  * @param
  * @retval AUDIO_OK if all operations succeed, AUDIO_FAIL else.
  */
static uint8_t PeriodicTC (uint8_t cmd)
{
    return AUDIO_OK;
}


/**
  * @brief  GetState
  *         Return the current state of the audio machine
  * @param  None
  * @retval Current State.
  */
static uint8_t GetState (void)
{
    return AudioState;
}






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
