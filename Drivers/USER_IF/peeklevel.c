/**
  ******************************************************************************
  * @file        peeklevel.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "peeklevel.h"
#include "platform.h"
// #include "t11_user_if.h"
#include "pages.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void* platform);
static void Update(void* platform);

/* Private variables ---------------------------------------------------------*/
T11_PeekLevel_TypeDef T11_UIF_PeekLevel = {
    2,
    { (void*)0 },

    Init,
    Update
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void __PeekLevelUpdate(uint16_t* data) {
    /* */
    for(int i = 0; i < T11_UIF_PeekLevel.channels; i++) {
        T11_UIF_PeekLevel.fillbars[i]->Level = *data;
        NGL_GUI_FillBarUpdate( T11_UIF_PeekLevel.fillbars[i] );
        data++;
    }
}

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void Init(void* platform) {
    /* */
    T11_UIF_PeekLevel.fillbars[0] = (NGL_FillBar*)MainPage.Objects.fillbars[0];
    T11_UIF_PeekLevel.fillbars[1] = (NGL_FillBar*)MainPage.Objects.fillbars[2];
}

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void Update(void* platform) {
    /* */
    static FlagStatus updated = RESET;

    T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    T11_AUDIO_IF_STREAM_TypeDef* audio_stream = plt->audio_if->stream;
    T11_AUDIO_IF_SETTINGS_Typedef* sett = plt->audio_if->settings;

    /* */
    if( audio_stream->filled_by < 50 ) {
        updated = RESET;
        return;
    }
    else if( updated == SET ) {
        return;
    }

    uint32_t audio_half_sze = audio_stream->buffer_align_size / 2;
    uint8_t* audio_buff = audio_stream->buff_ptr;
    uint8_t audio_channels = sett->channels[ sett->index ];
    uint8_t audio_data_width = sett->bit_resolution[ sett->index ] / 8;

    int32_t tmp_data;
    int32_t max_data[ audio_channels ];
    int32_t min_data[ audio_channels ];
    uint16_t peek_data[ audio_channels ];
    uint32_t i = 0;
    uint8_t dw = 0, ch = 0;
    uint8_t bit_shift = 32 - sett->bit_resolution[sett->index];
    uint32_t norm_max = UINT32_MAX >> bit_shift;

    plt->hal->leds->Toggle(0);

    /* */
    for (ch = 0; ch < audio_channels; ch++) {
        max_data[ch] = INT32_MIN; min_data[ch] = INT32_MAX;
    }

    /* */
    do {
        tmp_data = 0;
        for (dw = 0; dw < audio_data_width; dw++) {
            tmp_data |= (int32_t)( (int32_t)audio_buff[i+dw] << (dw*8) );
        }
        tmp_data <<= bit_shift;

        if(max_data[ch] < tmp_data) max_data[ch] = tmp_data;
        if(min_data[ch] > tmp_data) min_data[ch] = tmp_data;

        i += audio_data_width;
        ch++;
        if(ch >= audio_channels) ch = 0;
    } while (i < audio_half_sze);

    /* Normalaze data to uint16_t */
    for (ch = 0; ch < audio_channels; ch++) {
        peek_data[ch] = (((uint64_t)(max_data[ch] - min_data[ch])) * (uint64_t)norm_max) / (uint64_t)UINT32_MAX;
    }

    __PeekLevelUpdate( peek_data );

    updated = SET;
}


/*******************************************************************************
      END FILE
*******************************************************************************/
