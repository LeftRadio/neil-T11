/**
  ******************************************************************************
  * @file    MainPage_Labels.h
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page MainPage Labels structs
  ******************************************************************************
**/

#ifndef __MAINPAGE_LABELS_H
#define __MAINPAGE_LABELS_H

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"

/* Exported typedef -----------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
static void ItemEvent(void);

/* Exported variables ---------------------------------------------------------*/
char lblTime_text[9] = "20:34:45";
char lblWeekDay_text[10] = "Wed";
char lblYear_text[5] = "2014";
char lblFrequency_text[10] = "Frequency";
char lblBits_text[5] = "Bits";
char lblChannels_text[9] = "Channels";
char lblINTCLK_text[7] = "intCLK";
char lblEXTCLK_text[7] = "extCLK";
char lblUSBstream_text[10] = "USBstream";
char lbl44_1k_text[4] = "44k";
char lbl48k_text[4] = "48k";
char lbl96k_text[4] = "96k";
char lbl192k_text[5] = "192k";
char lbl16bit_text[3] = "16";
char lbl24bit_text[3] = "24";
char lbl32bit_text[3] = "32";
char lblCH1_text[2] = "1";
char lblCH2_text[2] = "2";
char lblCH4_text[2] = "4";
char lblCH6_text[2] = "6";
char lblCH8_text[2] = "8";
char lblNeilT11_text[9] = "Neil T11";
char lblTrackInfo_text[44] = "";		//"Coil - The Final Report A   3:48   1267kbps";
char lblTrackPlayTime_text[6] = "03:05";
char lblAtr_text[22] = "Neil Lab :: leftradio";
char lblFW_ver_text[8] = "fw1.4.7";

/* MainPage lblTime item */
NGL_Label lblTime = {
	10,
	305,
	0xFFFF,
	ENABLE,
	lblTime_text,
	&Square721_BT_9Info,
};

/* MainPage lblWeekDay item */
NGL_Label lblWeekDay = {
	100,
	305,
	0xFFFF,
	ENABLE,
	lblWeekDay_text,
	&Square721_BT_9Info,
};

/* MainPage lblYear item */
NGL_Label lblYear = {
	165,
	305,
	0xFFFF,
	ENABLE,
	lblYear_text,
	&Square721_BT_9Info,
};

/* MainPage lblFrequency item */
NGL_Label lblFrequency = {
	227,
	305,
	0xFFFF,
	ENABLE,
	lblFrequency_text,
	&Square721_BT_9Info,
};

/* MainPage lblBits item */
NGL_Label lblBits = {
	342,
	305,
	0xFFFF,
	ENABLE,
	lblBits_text,
	&Square721_BT_9Info,
};

/* MainPage lblChannels item */
NGL_Label lblChannels = {
	402,
	305,
	0xFFFF,
	ENABLE,
	lblChannels_text,
	&Square721_BT_9Info,
};

/* MainPage lblINTCLK item */
NGL_Label lblINTCLK = {
	10,
	290,
	0xCE79,
	ENABLE,
	lblINTCLK_text,
	&Square721_BT_9Info,
};

/* MainPage lblEXTCLK item */
NGL_Label lblEXTCLK = {
	59,
	290,
	0xFC00,
	ENABLE,
	lblEXTCLK_text,
	&Square721_BT_9Info,
};

/* MainPage lblUSBstream item */
NGL_Label lblUSBstream = {
	111,
	290,
	0x87F0,
	ENABLE,
	lblUSBstream_text,
	&Square721_BT_9Info,
};

/* MainPage lbl44_1k item */
NGL_Label lbl44_1k = {
	205,
	290,
	0xC618,
	ENABLE,
	lbl44_1k_text,
	&Square721_BT_9Info,
};

/* MainPage lbl48k item */
NGL_Label lbl48k = {
	234,
	290,
	0xC618,
	ENABLE,
	lbl48k_text,
	&Square721_BT_9Info,
};

/* MainPage lbl96k item */
NGL_Label lbl96k = {
	260,
	290,
	0xC618,
	ENABLE,
	lbl96k_text,
	&Square721_BT_9Info,
};

/* MainPage lbl192k item */
NGL_Label lbl192k = {
	284,
	290,
	0x87F0,
	ENABLE,
	lbl192k_text,
	&Square721_BT_9Info,
};

/* MainPage lbl16bit item */
NGL_Label lbl16bit = {
	325,
	290,
	0xC618,
	ENABLE,
	lbl16bit_text,
	&Square721_BT_9Info,
};

/* MainPage lbl24bit item */
NGL_Label lbl24bit = {
	345,
	290,
	0xC618,
	ENABLE,
	lbl24bit_text,
	&Square721_BT_9Info,
};

/* MainPage lbl32bit item */
NGL_Label lbl32bit = {
	365,
	290,
	0xBFF7,
	ENABLE,
	lbl32bit_text,
	&Square721_BT_9Info,
};

/* MainPage lblCH1 item */
NGL_Label lblCH1 = {
	395,
	290,
	0xC618,
	ENABLE,
	lblCH1_text,
	&Square721_BT_9Info,
};

/* MainPage lblCH2 item */
NGL_Label lblCH2 = {
	410,
	290,
	0x87F0,
	ENABLE,
	lblCH2_text,
	&Square721_BT_9Info,
};

/* MainPage lblCH4 item */
NGL_Label lblCH4 = {
	425,
	290,
	0xC618,
	ENABLE,
	lblCH4_text,
	&Square721_BT_9Info,
};

/* MainPage lblCH6 item */
NGL_Label lblCH6 = {
	440,
	290,
	0xC618,
	ENABLE,
	lblCH6_text,
	&Square721_BT_9Info,
};

/* MainPage lblCH8 item */
NGL_Label lblCH8 = {
	455,
	290,
	0xC618,
	ENABLE,
	lblCH8_text,
	&Square721_BT_9Info,
};

/* MainPage lblNeilT11 item */
NGL_Label lblNeilT11 = {
	13,
	19,
	0xB676,
	ENABLE,
	lblNeilT11_text,
	&Square721_BT_14Info,
};

/* MainPage lblTrackInfo item */
NGL_Label lblTrackInfo = {
	60,
	70,
	0xFFFF,
	ENABLE,
	lblTrackInfo_text,
	&Square721_BT_10Info,
};

/* MainPage lblTrackPlayTime item */
NGL_Label lblTrackPlayTime = {
	349,
	51,
	0xFFFF,
	ENABLE,
	lblTrackPlayTime_text,
	&Square721_BT_10Info,
};

/* MainPage lblAtr item */
NGL_Label lblAtr = {
	40,
	10,
	0xFFFF,
	ENABLE,
	lblAtr_text,
	&Square721_BT_8Info,
};

/* MainPage lblFW_ver item */
NGL_Label lblFW_ver = {
	85,
	23,
	0xB676,
	ENABLE,
	lblFW_ver_text,
	&Square721_BT_9Info,
};






#endif /* __MAINPAGE_LABELS_H */
/*********************************************************************************************************
 END FILE
*********************************************************************************************************/;

