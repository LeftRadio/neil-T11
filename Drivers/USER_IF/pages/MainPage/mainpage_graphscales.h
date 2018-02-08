/**
  ******************************************************************************
  * @file       MainPage.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.155294
  * @brief      
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAINPAGE_GRAPHSCALES_H
#define __MAINPAGE_GRAPHSCALES_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported define -----------------------------------------------------------*/
#define MAINPAGE_GRAPHSCALES_CNT		4

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/

/* Internal variables --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* MainPage ngGraphScale_3 item */
NGL_GraphScale ngGraphScale_3 = {
    15, 43, 399, 60,     // coordinates
    0, 100,       // min/max levels
    5,                // scale cent
    {100, 90, 80, 70, 60, 50, 40, 30, 20, 10,  0},                   // labels
    11,               // labels count
    "%",                    // scale units
    ENABLE,               // show labels state
    ENABLE,                // show lines state
    DISABLE,                     // flip
    NGL_Horizontal,              // vertical/horizontal orientation
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0x8550
};

/* MainPage ngGraphScale_4 item */
NGL_GraphScale ngGraphScale_4 = {
    20, 213, 400, 230,     // coordinates
    -90, 0,       // min/max levels
    -3,                // scale cent
    {-90, -60, -42, -30, -24, -18, -12, -6,  -0},                   // labels
    9,               // labels count
    "dB",                    // scale units
    ENABLE,               // show labels state
    ENABLE,                // show lines state
    DISABLE,                     // flip
    NGL_Horizontal,              // vertical/horizontal orientation
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0xc638
};

/* MainPage ngGraphScale_5 item */
NGL_GraphScale ngGraphScale_5 = {
    20, 263, 400, 280,     // coordinates
    -90, 0,       // min/max levels
    -3,                // scale cent
    {-90, -60, -42, -30, -24, -18, -12, -6,  -0},                   // labels
    9,               // labels count
    "dB",                    // scale units
    ENABLE,               // show labels state
    ENABLE,                // show lines state
    ENABLE,                     // flip
    NGL_Horizontal,              // vertical/horizontal orientation
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0xc638
};

/* MainPage ngGraphScale_2 item */
NGL_GraphScale ngGraphScale_2 = {
    15, 6, 390, 10,     // coordinates
    -60, 0,       // min/max levels
    -3,                // scale cent
    {-90, -60, -42, -30, -24, -18, -12, -6,  -0},                   // labels
    9,               // labels count
    "",                    // scale units
    DISABLE,               // show labels state
    ENABLE,                // show lines state
    DISABLE,                     // flip
    NGL_Horizontal,              // vertical/horizontal orientation
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0xffff
};


static const NGL_GraphScale* mainpage_graphscales[MAINPAGE_GRAPHSCALES_CNT] = {&ngGraphScale_3, &ngGraphScale_4, &ngGraphScale_5, &ngGraphScale_2, };



#endif /* __MAINPAGE_GRAPHSCALES_H */
/*******************************************************************************
      END FILE
*******************************************************************************/