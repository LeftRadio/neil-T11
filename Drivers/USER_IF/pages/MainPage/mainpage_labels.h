/**
  ******************************************************************************
  * @file       MainPage.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.171295
  * @brief      
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAINPAGE_LABELS_H
#define __MAINPAGE_LABELS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported define -----------------------------------------------------------*/
#define MAINPAGE_LABELS_CNT		2

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/

/* Internal variables --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* MainPage nglLabel_6 item */
static char nglLabel_6_text[8] = "ext CLK";

NGL_Label nglLabel_6 = {
    140,
    306,
    0xe71c,
    Transparent,
    (char*)nglLabel_6_text,
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
};

/* MainPage nglLabel_7 item */
static char nglLabel_7_text[9] = "24576000";

NGL_Label nglLabel_7 = {
    135,
    291,
    0xe71c,
    Transparent,
    (char*)nglLabel_7_text,
    (NGL_Font*)&ms_shell_dlg_2_8_bold,
};


static const NGL_Label* mainpage_labels[MAINPAGE_LABELS_CNT] = {&nglLabel_6, &nglLabel_7, };



#endif /* __MAINPAGE_LABELS_H */
/*******************************************************************************
      END FILE
*******************************************************************************/