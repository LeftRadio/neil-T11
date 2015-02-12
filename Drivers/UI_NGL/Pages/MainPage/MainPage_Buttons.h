/**
  ******************************************************************************
  * @file    MainPage_Items.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page MainPage Buttons structs
  ******************************************************************************
**/

#ifndef __MAINPAGE_ITEMS_H
#define __MAINPAGE_ITEMS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported typedef -----------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
static void ItemEvent(void);

char btnTracks_text[7] = "Tracks";
char btnVisual_text[7] = "Visual";
char btnSettings_text[9] = "Settings";
char btnControls_text[9] = "Controls";


/* Exported variables ---------------------------------------------------------*/
/* MainPage btnRewind item */
NGL_Button btnRewind = {
	220,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&rewindInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnStop item */
NGL_Button btnStop = {
	270,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&stopInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	(void*)0,
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnPlay item */
NGL_Button btnPlay = {
	325,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&playInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnPause item */
NGL_Button btnPause = {
	380,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&pauseInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnForward item */
NGL_Button btnForward = {
	430,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&forwardInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnTracks item */
NGL_Button btnTracks = {
	419,
	250,
	32,
	32,
	IconButton,
	0xFD49,
	0xFD49,
	ENABLE,
	(NGL_Image*)&ListInfo,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	-12,
	0xFC00,
	btnTracks_text,
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnVisual item */
NGL_Button btnVisual = {
	419,
	207,
	32,
	32,
	IconButton,
	0xFC08,
	0xFC08,
	ENABLE,
	(NGL_Image*)&GlobeInfo,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	-12,
	0xFC00,
	btnVisual_text,
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnSettings item */
NGL_Button btnSettings = {
	419,
	165,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&settings_Info,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	-12,
	0xFC00,
	btnSettings_text,
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnControls item */
NGL_Button btnControls = {
	419,
	122,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&Control_PanelInfo,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	-12,
	0xFC00,
	btnControls_text,
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnMute item */
NGL_Button btnMute = {
	155,
	10,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&soundInfo,
	(void*)0,
	0,
	25,
	0xBFF7,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnSliderTrack item */
NGL_Button btnSliderTrack = {
	343,
	48,
	50,
	18,
	ColorFillButton,
	0xFC00,
	0xFC00,
	DISABLE,
	(void*)0,
	(void*)0,
	0,
	25,
	0xBFF7,
	"",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* MainPage btnOFFtimer item */
NGL_Button btnOFFtimer = {
	420,
	80,
	32,
	32,
	IconButton,
	0x6180,
	0x6180,
	ENABLE,
	(NGL_Image*)&ClockInfo,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	-12,
	0xFC00,
	"OFFtimer",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};





#endif /* __MAINPAGE_ITEMS_H */
/*********************************************************************************************************
 END FILE
*********************************************************************************************************/;

