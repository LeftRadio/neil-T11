/**
  ******************************************************************************
  * @file       MainPage.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.178295
  * @brief      
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAINPAGE_FILLBARS_H
#define __MAINPAGE_FILLBARS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported define -----------------------------------------------------------*/
#define MAINPAGE_FILLBARS_CNT		3

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/

/* Internal variables --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* MainPage nglFillBar item */
NGL_FillBar nglFillBar = {
    20, 248, 388, 260,     // coordinates
    NGL_Horizontal,                // orientation
    0, 65535,   // min/max levels
    2048,                    // level
    20, 248,             // old level coords (used for lcd draw optimization)
    ENABLE,              // log mode
    90,             // full scale dB (only for log mode)
    DISABLE,                   // draw border state
    ENABLE,                  // draw markers state
    0xffff,             // markers color
    0xe28b                     // main fillbar color
};

/* MainPage nglFillBar_16 item */
NGL_FillBar nglFillBar_16 = {
    15, 63, 389, 71,     // coordinates
    NGL_Horizontal,                // orientation
    0, 100,   // min/max levels
    50,                    // level
    15, 63,             // old level coords (used for lcd draw optimization)
    DISABLE,              // log mode
    120,             // full scale dB (only for log mode)
    DISABLE,                   // draw border state
    DISABLE,                  // draw markers state
    0xffff,             // markers color
    0x8550                     // main fillbar color
};

/* MainPage nglFillBar_2 item */
NGL_FillBar nglFillBar_2 = {
    20, 233, 388, 245,     // coordinates
    NGL_Horizontal,                // orientation
    0, 65535,   // min/max levels
    128,                    // level
    20, 233,             // old level coords (used for lcd draw optimization)
    ENABLE,              // log mode
    90,             // full scale dB (only for log mode)
    DISABLE,                   // draw border state
    ENABLE,                  // draw markers state
    0xffff,             // markers color
    0x6b7c                     // main fillbar color
};


static const NGL_FillBar* mainpage_fillbars[MAINPAGE_FILLBARS_CNT] = {&nglFillBar, &nglFillBar_16, &nglFillBar_2, };



#endif /* __MAINPAGE_FILLBARS_H */
/*******************************************************************************
      END FILE
*******************************************************************************/
