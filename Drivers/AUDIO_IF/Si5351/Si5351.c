/**
  ******************************************************************************
  * @file       Si5351.c
  * @author     Left Radio
  * @version
  * @date
  * @brief      sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "platform_types.h"
#include "Si5351.h"
#include "Si5351_registers.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SI_I2CADDR                            ((uint8_t)0xC0)

#define SI_OUTS_ENABLE(i2c)                   i2c->Write( SI_I2CADDR, 3, 0x00 )
#define SI_OUTS_DISABLE(i2c)                  i2c->Write( SI_I2CADDR, 3, 0xFF )

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Si5351_Init(void* platform);
static void Si5351_DeInit(void* platform);

/* Private variables ---------------------------------------------------------*/
T11_AUDIO_IF_CLOCK_TypeDef si5351 = {
    24576000,
    1,
    Si5351_Init,
    Si5351_DeInit
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Si5351_Init
  * @param  None
  * @retval None
  */
static void Si5351_Init(void* platform) {
    /* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_HAL_I2C_TypeDef* i2c = plt->hal->i2c;
    uint8_t *pmap;
    uint8_t i;
    int res = 0;
    static uint32_t old_sampling_freq = 0;

    if (plt->audio_if->settings->sampling_freq != old_sampling_freq) {
    	old_sampling_freq = plt->audio_if->settings->sampling_freq;
	}
    else {
    	return;
    }

    /* Disable Outputs */
    SI_OUTS_DISABLE( i2c );
    /*
     * Configure Si5351, see documentation on silabs.com for more detals
     */
    /* Select registers map */
    switch(si5351.Frequency) {
        case 22579200:
            pmap = (uint8_t*)SI5351_RegistersMap_22_579;
        break;
        case 24576000:
            pmap = (uint8_t*)SI5351_RegistersMap_24M;
        break;
        default:
          return;
    }
    /* Powerdown all output drivers */
    for(i = 16; i < 24; i++) {
        res = i2c->Write( SI_I2CADDR, i, 0x80 );
        if(res) return;
    }
    /* Write new configuration to the device */
    for(i = 0; i < 233; i += 2) {
        res = i2c->Write( SI_I2CADDR, *pmap, *(pmap+1) );
        if(res) return;
        pmap += 2;
    }
    /* Apply PLLA and PLLB soft reset */
    res = i2c->Write( SI_I2CADDR, 177, 0xAC );
    if(res) return;
    /* Enable Outputs */
    SI_OUTS_ENABLE( i2c );
}

/**
  * @brief  Si5351_Configuration_Registers
  * @param  None
  * @retval None
  */
static void Si5351_DeInit(void* platform) {
    /* */
//    SI_OUTS_DISABLE( ((T11_Platform_TypeDef*)platform)->hal->i2c );
}

/*******************************************************************************
      END FILE
*******************************************************************************/
