/**
  ****************************************************************************** 
  * @file    OFFTimerPage_Labels.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page OFFTimerPage Labels structs
  ****************************************************************************** 
**/

#ifndef __OFFTIMERPAGE_LABELS_H
#define __OFFTIMERPAGE_LABELS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported typedef -----------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
static void ItemEvent(void);

/* Exported variables ---------------------------------------------------------*/
char Label1_text[11] = "Timer Mode:";
char Label3_text[10] = "OFF after:";
char Label4_text[1] = ":";
char Label5_text[15] = "Event 1 on off:";
char Label6_text[15] = "Event 2 on off:";
char Label7_text[15] = "Event 3 on off:";
char Label10_text[23] = "start after playing off";
char Label11_text[2] = "24";
char Label12_text[2] = "00";
char Label13_text[25] = "Delay between events, ms:";
char Label14_text[3] = "100";
char Label15_text[18] = "acustics relay OFF";
char Label16_text[23] = "mute all audio channels";
char Label17_text[17] = "amplify power OFF";

/* OFFTimerPage Label1 item */
NGL_Label Label1 = {
	40,
	267,
	0xFD49,
	DISABLE,
	Label1_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label3 item */
NGL_Label Label3 = {
	55,
	227,
	0xFD49,
	DISABLE,
	Label3_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label4 item */
NGL_Label Label4 = {
	300,
	225,
	0xFFFF,
	DISABLE,
	Label4_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label5 item */
NGL_Label Label5 = {
	20,
	187,
	0xFD49,
	DISABLE,
	Label5_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label6 item */
NGL_Label Label6 = {
	20,
	147,
	0xFD49,
	DISABLE,
	Label6_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label7 item */
NGL_Label Label7 = {
	20,
	107,
	0xFD49,
	DISABLE,
	Label7_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label10 item */
NGL_Label Label10 = {
	203,
	265,
	0xB676,
	DISABLE,
	Label10_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label11 item */
NGL_Label Label11 = {
	203,
	225,
	0xB676,
	DISABLE,
	Label11_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label12 item */
NGL_Label Label12 = {
	376,
	225,
	0xB676,
	DISABLE,
	Label12_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label13 item */
NGL_Label Label13 = {
	20,
	67,
	0xFD49,
	DISABLE,
	Label13_text,
	&Square721_BT_11Info,
};

/* OFFTimerPage Label14 item */
NGL_Label Label14 = {
	325,
	65,
	0xB676,
	DISABLE,
	Label14_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label15 item */
NGL_Label Label15 = {
	215,
	185,
	0xB676,
	DISABLE,
	Label15_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label16 item */
NGL_Label Label16 = {
	195,
	145,
	0xB676,
	DISABLE,
	Label16_text,
	&Square721_BT_14Info,
};

/* OFFTimerPage Label17 item */
NGL_Label Label17 = {
	215,
	105,
	0xB676,
	DISABLE,
	Label17_text,
	&Square721_BT_14Info,
};






#endif /* __OFFTIMERPAGE_LABELS_H */
/*********************************************************************************************************
 END FILE
*********************************************************************************************************/;

