/**
  ******************************************************************************
  * @file       pga2311.c
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "platform_types.h"
#include "systick.h"
#include "pga2311.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PGA2311_CS_PIN                  GPIO_Pin_5
#define PGA2311_CS_PORT                 GPIOC

/* Private macro -------------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void* platform);
static void DeInit(void* platform);
static void SetLevel(void* platform, float* vol);

/* Private variables ---------------------------------------------------------*/
T11_AUDIO_IF_VOLUME_TypeDef T11_Audio_IF_Volume = {
    2,
    -95.5, +31.5,
    255,
    Init,
    DeInit,
    SetLevel
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize and configures all required resources.
  * @param  None
  * @retval None
  */
static void Init(void* platform) {
    /* */
    GPIO_InitTypeDef GPIO_InitStruct;
    float vol[2] = { -127.5, -127.5 };
    /* Configure SPI CS Pin */
    GPIO_SetBits(PGA2311_CS_PORT, PGA2311_CS_PIN);
    GPIO_InitStruct.GPIO_Pin = PGA2311_CS_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(PGA2311_CS_PORT, &GPIO_InitStruct);
    /* */
    SetLevel( platform, vol );
}

/**
  * @brief  Free all used resources.
  * @param  None
  * @retval None
  */
static void DeInit(void* platform) {

}

/**
  * @brief  SetVolume
  * @param  float vol_db_L - Left volume level
  * @param  float vol_db_R - Right volume level
  * @retval None
  */
static void SetLevel(void* platform, float* invol) {
	/* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)(platform);
    T11_AUDIO_IF_VOLUME_TypeDef* aifvol = plt->audio_if->volume;
    /* */
    uint16_t reg_ch_value[aifvol->channels];
    uint16_t reg_value;
    float vol_tmp;
    float vol_range = aifvol->max - aifvol->min;
    /* */
    for (int i = 0; i < aifvol->channels; ++i) {
        if(invol[i] > aifvol->max) invol[i] = aifvol->max;
        else if(invol[i] <= aifvol->min) invol[i] = aifvol->min;
        /* normalaze to 0-255 range */
        vol_tmp = ((invol[i] - aifvol->min) * aifvol->reg_range) / vol_range;
        reg_ch_value[i] = (uint16_t)(vol_tmp);
    }
    /* */
    reg_value = (reg_ch_value[0] << 8) | reg_ch_value[1];
    /* */
    GPIO_ResetBits(PGA2311_CS_PORT, PGA2311_CS_PIN);
    plt->hal->spi->Write(&reg_value, 1);
    GPIO_SetBits(PGA2311_CS_PORT, PGA2311_CS_PIN);
}


/*******************************************************************************
      END FILE
*******************************************************************************/
