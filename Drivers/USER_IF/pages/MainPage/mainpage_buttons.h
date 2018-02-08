/**
  ******************************************************************************
  * @file       MainPage.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.189296
  * @brief
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAINPAGE_BUTTONS_H
#define __MAINPAGE_BUTTONS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported define -----------------------------------------------------------*/
#define MAINPAGE_BUTTONS_CNT		4

/* Exported macro ------------------------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported function ---------------------------------------------------------*/
static void nglButton_4_click(void);
static void nglButton_3_click(void);
static void nglButton_2_click(void);
static void nglButton_click(void);

/* Internal variables --------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
/* MainPage nglButton item */
NGL_Button nglButton = {
    405,
    220,
    70,
    46,
    IconButton,
    0xc000,
    0xc000,
    TRUE,
    (NGL_Image*)&Globe,
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0,
    0,
    0xffff,
    "NGL_Button",
    ReClick_DISABLE,
    TRUE,
    ENABLE,
    nglButton_click,
};

/* MainPage nglButton_2 item */
NGL_Button nglButton_2 = {
    405,
    155,
    70,
    46,
    IconButton,
    0xc000,
    0xc000,
    TRUE,
    (NGL_Image*)&List,
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0,
    0,
    0xffff,
    "NGL_Button",
    ReClick_DISABLE,
    TRUE,
    ENABLE,
    nglButton_2_click,
};

/* MainPage nglButton_4 item */
NGL_Button nglButton_4 = {
    405,
    25,
    70,
    46,
    IconButton,
    0xc000,
    0xc000,
    TRUE,
    (NGL_Image*)&Clock,
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0,
    0,
    0xffff,
    "NGL_Button",
    ReClick_DISABLE,
    TRUE,
    ENABLE,
    nglButton_4_click,
};

/* MainPage nglButton_3 item */
NGL_Button nglButton_3 = {
    405,
    90,
    71,
    46,
    IconButton,
    0xc000,
    0xc000,
    TRUE,
    (NGL_Image*)&Work,
    (NGL_Font*)&ms_shell_dlg_2_8_normal,
    0,
    0,
    0xffff,
    "NGL_Button",
    ReClick_DISABLE,
    TRUE,
    ENABLE,
    nglButton_3_click,
};


static const NGL_Button* mainpage_buttons[MAINPAGE_BUTTONS_CNT] = {&nglButton, &nglButton_2, &nglButton_4, &nglButton_3, };



#endif /* __MAINPAGE_BUTTONS_H */
/*******************************************************************************
      END FILE
*******************************************************************************/