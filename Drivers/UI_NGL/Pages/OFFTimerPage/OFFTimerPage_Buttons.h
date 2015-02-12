/**
  ****************************************************************************** 
  * @file    OFFTimerPage_Items.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page OFFTimerPage Buttons structs
  ****************************************************************************** 
**/

#ifndef __OFFTIMERPAGE_ITEMS_H
#define __OFFTIMERPAGE_ITEMS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported typedef -----------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
static void ItemEvent(void);

/* Exported variables ---------------------------------------------------------*/
/* OFFTimerPage Button1 item */
NGL_Button Button1 = {
	150,
	260,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button1",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button2 item */
NGL_Button Button2 = {
	425,
	260,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button2",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button3 item */
NGL_Button Button3 = {
	150,
	220,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button3",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button4 item */
NGL_Button Button4 = {
	250,
	220,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button4",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button5 item */
NGL_Button Button5 = {
	325,
	220,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button5",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button6 item */
NGL_Button Button6 = {
	425,
	220,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button6",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button7 item */
NGL_Button Button7 = {
	240,
	60,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button7",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button8 item */
NGL_Button Button8 = {
	425,
	60,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button8",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button9 item */
NGL_Button Button9 = {
	150,
	180,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button9",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button10 item */
NGL_Button Button10 = {
	425,
	180,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button10",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button11 item */
NGL_Button Button11 = {
	150,
	140,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button11",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button12 item */
NGL_Button Button12 = {
	425,
	140,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button12",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button13 item */
NGL_Button Button13 = {
	150,
	100,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button13",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button14 item */
NGL_Button Button14 = {
	425,
	100,
	32,
	32,
	IconButton,
	0xFFFF,
	0xFFFF,
	ENABLE,
	(NGL_Image*)&minusInfo,
	(void*)0,
	0,
	0,
	0xF7BE,
	"Button14",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button15 item */
NGL_Button Button15 = {
	160,
	10,
	140,
	22,
	ColorFillButton,
	0x1143,
	0x1143,
	ENABLE,
	(void*)0,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	1,
	0xF7BE,
	"Save & Exit",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};

/* OFFTimerPage Button16 item */
NGL_Button Button16 = {
	325,
	10,
	140,
	22,
	ColorFillButton,
	0x4000,
	0x4000,
	ENABLE,
	(void*)0,
	(NGL_Font*)&BankGothic_Lt_BT_11Info,
	0,
	1,
	0xF7BE,
	"Discard & Exit",
	ReClick_ENABLE,
	DISABLE,
	ENABLE,
	ItemEvent,
};





#endif /* __OFFTIMERPAGE_ITEMS_H */
/*********************************************************************************************************
 END FILE
*********************************************************************************************************/;

