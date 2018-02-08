/**
  ******************************************************************************
  * @file       AD1852.c
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "platform_types.h"
#include "AD1852.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define AD185x_NSS_PORT                 GPIOA
#define AD185x_NSS_PIN                  GPIO_Pin_4
#define AD185x_NSS_SET_HIGHT()          AD185x_NSS_PORT->BSRRL = AD185x_NSS_PIN
#define AD185x_NSS_SET_LOW()            AD185x_NSS_PORT->BSRRH = AD185x_NSS_PIN

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void* platform);
static void DeInit(void* platform);
static void Play(void* platform);
static void PauseResume(void* platform, AUDIO_IF_CMD_TypeDef cmd);
static void Stop(void* platform);
static void VolumeCtrl(void* platform, uint8_t Volume);
static void MuteCtl(void* platform, FunctionalState state);

/* Private variables ---------------------------------------------------------*/
I2S_InitTypeDef AI_I2S = {
    I2S_Mode_MasterTx,
    I2S_Standard_Phillips,
    I2S_DataFormat_16bextended,
    I2S_MCLKOutput_Disable,
    I2S_MCLKExternal_Enable,
    192000,
    I2S_CPOL_Low
};

T11_AUDIO_IF_DAC_TypeDef DAC_AD1852 = {
    &AI_I2S,
    RESET,
    DISABLE,
    INT_NONE,
    {0},

    Init,
    DeInit,
    Play,
    PauseResume,
    Stop,
    VolumeCtrl,
    MuteCtl
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Writes a word to a given register
  * @param  addr: The address (location) of the register to be written.
  * @param  val: the word value to be written into destination register.
  * @retval None
  */
static __inline void __AD_RegWrite(void* platform, uint8_t addr, uint16_t val) {
    T11_HAL_SPI_TypeDef* spi = ((T11_Platform_TypeDef*)platform)->hal->spi;
    GPIO_InitTypeDef gpio_initstruct;
    /* */
    DAC_AD1852.UsrData[addr] = (val & 0xFFFC) | addr;
    spi->Write( &DAC_AD1852.UsrData[addr], 1 );
    /* Data Latch */
    GPIOB->BSRRH = GPIO_Pin_3;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_OUT;
    gpio_initstruct.GPIO_OType = GPIO_OType_PP;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_25MHz;
    gpio_initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio_initstruct);
    /* */
    AD185x_NSS_SET_LOW();
    delay_us(1);
    AD185x_NSS_SET_HIGHT();
    delay_us(1);
    /* */
    GPIOB->BSRRL = GPIO_Pin_3;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio_initstruct);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
}

/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  addr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
static __inline uint16_t __AD_RegRead(void* platform, uint8_t addr) {
    return DAC_AD1852.UsrData[addr];
}

/**
  * @brief  __AD_ConfigStream
  * @param  platform struct pointer
  * @retval None
  */
static __inline void __AD_ConfigStream(void* platform) {
    /* */
    uint16_t i;
    uint16_t i2s_standard_bit;
    uint16_t serial_mode = 0;
    uint16_t reg_data;

    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_TypeDef* aif = plt->audio_if;

    switch(aif->dac->i2s_config->I2S_Standard) {
        case I2S_Standard_Phillips:
            i2s_standard_bit = 0x01 << 4;
        break;
        case I2S_Standard_MSB:
            i2s_standard_bit = 0x01 << 5;
        break;
        case I2S_Standard_LSB:
            i2s_standard_bit = 0x00;
            serial_mode = 0x01 << 9;
        break;
    }

    /* Initialize registers the AD185x */
    reg_data = i2s_standard_bit | serial_mode;
    /* */
    for (i = 0; i < 5; ++i) {
        __AD_RegWrite( plt, 0x01, reg_data );
    }
}

/**
  * @brief  Initializes specefic function(IOs etc) used by the Audio DAC
  * @param  None
  * @retval None
  */
static void Init(void* platform) {
    /* */

    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_SETTINGS_Typedef* settings = plt->audio_if->settings;
    T11_AUDIO_IF_STREAM_TypeDef* stream = plt->audio_if->stream;
    /* */
    GPIO_InitTypeDef GPIO_InitStruct;
    AD185x_NSS_SET_HIGHT();
    GPIO_InitStruct.GPIO_Pin = AD185x_NSS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(AD185x_NSS_PORT, &GPIO_InitStruct);
    /* Configure the I2S/DMA peripherals */
    DAC_AD1852.i2s_config->I2S_AudioFreq = settings->sampling_freq;
    /* */
    plt->hal->i2s->Init(
        (I2S_InitTypeDef*)DAC_AD1852.i2s_config,
        settings->bit_resolution[ settings->index ],
        (uint32_t)stream->buff_ptr,
        (uint32_t)stream->buffer_align_size
    );
    /* Global Reset signal */
    //plt->hal->Reset( platform );
    __AD_ConfigStream( platform );
}

/**
  * @brief  Restores specefic function(IOs etc) used by the Audio DAC
  * @param  None
  * @retval None
  */
static void DeInit(void* platform) {
    ((T11_Platform_TypeDef*)platform)->hal->i2s->DeInit();
}

/**
  * @brief  Play
  * @param  None
  * @retval None
  */
static void Play(void* platform) {
    /* */
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_SETTINGS_Typedef* settings = plt->audio_if->settings;
    T11_AUDIO_IF_STREAM_TypeDef* stream = plt->audio_if->stream;
    /* */
    plt->hal->i2s->Start(
        settings->bit_resolution[ settings->index ],
        (uint32_t)stream->buff_ptr,
        (uint32_t)stream->buffer_align_size
    );
    /* UnMute audio */
    plt->audio_if->dac->MuteCtl(platform, DISABLE);
}

/**
  * @brief  PauseResume
  * @param  None
  * @retval None
  */
static void PauseResume(void* platform, AUDIO_IF_CMD_TypeDef cmd) {
	/* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
	FunctionalState mute_state = (cmd == AUDIO_IF_PAUSE)? ENABLE : DISABLE;
	/* */
    plt->audio_if->dac->MuteCtl(platform, mute_state);
}

/**
  * @brief  Stop
  * @param  None
  * @retval None
  */
static void Stop(void* platform) {
    /* */
    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    /* Previosly mute audio */
    plt->audio_if->dac->MuteCtl( platform, ENABLE );
    /* Stop I2S HAL */
    plt->hal->i2s->Stop();
}

/**
  * @brief  VolumeCtrl
  * @param  None
  * @retval None
  */
static void VolumeCtrl(void* platform, uint8_t Volume) {
    /* not used for DAC, volume sets external */
}

/**
  * @brief  Mute
  * @param  None
  * @retval None
  */
static void MuteCtl(void* platform, FunctionalState state) {
    uint16_t tmp = __AD_RegRead(platform, 0x01);
    /* Set the Mute mode */
    if (state == ENABLE) tmp |= (1 << 6);
    else tmp &= (uint16_t)(~(1 << 6));
    /* */
    __AD_RegWrite(platform, 0x01, tmp);
}


/*******************************************************************************
      END FILE
*******************************************************************************/
