/**
  ******************************************************************************
  * @file	 	temp.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "Audio_Interface.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/
//extern AUDIO_FOPS_TypeDef UsbAudioFops;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern volatile DAC_HAL_TypeDef DAC_AD1852;

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

volatile I2S_InitTypeDef AI_I2S = {
		I2S_Mode_MasterTx,
		I2S_Phillips,
		I2S_DataFormat_16bextended,
		I2S_MCLKOutput_Disable,
		I2S_MCLKExternal_Enable,
		AUDIO_FREQUENCY,
		I2S_CPOL_Low
};


AUDIO_IF_TypeDef AudioInterface =
{
	&AI_I2S,
	&DAC_AD1852,
    &si5351,
    &UsbAudioFops,
    AI_NO_INIT,
};

volatile uint8_t AudioState = AUDIO_STATE_INACTIVE;

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
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
    if (AudioInterface.InitFlag == AI_NO_INIT)
    {
    	if(AudioInterface.I2S->I2S_MCLK_External == I2S_MCLKExternal_Enable)
    	{
    		/* Configure Si5351 and start oscillation */
    		AudioInterface.Si5351->RegistersConfigurate();
    	}

    	/* Configure I2S/DMA/DAC */
    	AudioInterface.DAC_HAL->Init();

    	/* Set the init flag */
    	AudioInterface.InitFlag = AI_INIT;
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
    /* DeInitialization the I2S/DMA/DAC/Si5351 peripherals */
//	AudioInterface.DAC_HAL->DeInit();
//
//	if(AudioInterface.MCLK_I2S_OUT_State == I2S_MCLKOutput_Disable)
//	{
//		AudioInterface.Si5351->DeInit();
//	}

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
	static uint16_t cnt = 0;

//    /* Check the current state */
//    if ((AudioState == AUDIO_STATE_INACTIVE) || (AudioState == AUDIO_STATE_ERROR))
//    {
//        AudioState = AUDIO_STATE_ERROR;
//        return AUDIO_FAIL;
//    }


    switch (cmd)
    {
        /* Process the PLAY command ----------------------------*/
    case AUDIO_CMD_PLAY:
        /* If current state is Active or Stopped */
        if ((AudioState == AUDIO_STATE_ACTIVE) || (AudioState == AUDIO_STATE_STOPPED) || (AudioState == AUDIO_STATE_PLAYING))
        {
            if(++cnt >= 1000)
            {
            	cnt = 0;
            	T11_LED_Toggle(T11_LED_0);
            }

        	/* Call the DAC HAL Play function */
//        	AudioInterface.DAC_HAL->Play();

            AudioState = AUDIO_STATE_PLAYING;
            return AUDIO_OK;
        }
        /* If current state is Paused */
        else if (AudioState == AUDIO_STATE_PAUSED)
        {
            /* Call the DAC HAL Pause/Resume function */
        	AudioInterface.DAC_HAL->PauseResume(AUDIO_CMD_PLAY);

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
        	AudioInterface.DAC_HAL->Stop();

        	T11_LED_OFF(T11_LED_0);

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
        	AudioInterface.DAC_HAL->PauseResume(AUDIO_CMD_PAUSE);

        	T11_LED_OFF(T11_LED_0);

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
