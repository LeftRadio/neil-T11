/**
  ******************************************************************************
  * @file    main.c
  * @author
  * @version V1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "arm_math.h"
#include "math_helper.h"
#include "stm32f4xx.h"

#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_audio_core.h"

#include "platform.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN volatile USB_OTG_CORE_HANDLE USB_OTG_dev __ALIGN_END;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {
    /* */
    SystemInit();
    /* */
    T11_Platform.Init();
    /* */
    USBD_Init(
        (USB_OTG_CORE_HANDLE*)&USB_OTG_dev,
        USB_OTG_HS_CORE_ID,
        &USR_desc,
        &UAC1_Core_Callbacks,
        &USR_cb
    );
    /* Main cycle */
    while (1) {
        T11_Platform.audio_if->Update( &T11_Platform );
        T11_Platform.user_if->Update( &T11_Platform );

        if(T11_Platform.audio_if->dac->i2s_tx_complite != RESET) {
        	T11_Platform.audio_if->dac->i2s_tx_complite = RESET;
        }
    }
}

/**
  * @brief
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void) {
    //	uint8_t bufs[3];
    //	float Fs_Fb = 192.000;
    //	uint16_t val_int = (uint16_t)(Fs_Fb);
    //	uint16_t val_frac = (uint16_t)((Fs_Fb - val_int) * 1000);

    //	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    //	{
    //		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    //
    //		/* LED4 toggling */
    //		STM_EVAL_LEDToggle(LED3);
    //
    //		bufs[0] = (uint8_t)((val_int & 0x3FF) >> 2);
    //		bufs[1] = (uint8_t)((val_int & 0x03) << 6) | (uint8_t)((val_frac & 0x3FF) >> 4);
    //		bufs[2] = (uint8_t)(val_frac & 0x0F) << 4;
    //
    //		DCD_EP_Tx (&USB_OTG_dev, AUDIO_IN_EP, bufs, 3);
    //	}
}


/*******************************************************************************
      END FILE
*******************************************************************************/
