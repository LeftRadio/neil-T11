/**
  ******************************************************************************
  * @file       MainPage.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.160294
  * @brief      
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAINPAGE_SEEKBARS_H
#define __MAINPAGE_SEEKBARS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported define -----------------------------------------------------------*/
#define MAINPAGE_SEEKBARS_CNT		1

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/
static void nglSeekBar_click(void);

/* Internal variables --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* MainPage nglSeekBar item */
NGL_SeekBar nglSeekBar = {
    15, 11, 390, 40,     // coordinates
    12,              // slider size
    (uint16_t)(65535), (uint16_t)(65535),     // old slider positions
    NGL_Horizontal,                // orientation
    0, 100,   // min/max levels
    50,                    // level
    ENABLE,             // show progress level
    0xe261,                    // main seekbar color
    0x8430,              // slider color
    nglSeekBar_click,                  // click func pointer
};



static const NGL_SeekBar* mainpage_seekbars[MAINPAGE_SEEKBARS_CNT] = {&nglSeekBar, };



#endif /* __MAINPAGE_SEEKBARS_H */
/*******************************************************************************
      END FILE
*******************************************************************************/