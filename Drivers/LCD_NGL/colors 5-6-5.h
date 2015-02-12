/**
  ******************************************************************************
  * @file    init.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date
  * @brief   Main Init Peripherals header file.
  ******************************************************************************
**/

#ifndef __COLOR565_H
#define __COLOR565_H

/* Includes ------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define White       		0xffff
#define Black       		0x0000
#define LightBlack  		0x841
#define LightBlack2  		0x3186
#define LightBlack3			0x39c7
#define DarkGray            0x632c
#define Gray        		0x8c71
#define LightGray4   		0xf79e
#define LightGray   		0xce59
#define LightGray2			0xbdf7
#define LightGray3          0x94b2
#define LightSky    		0x7e7f
#define Red         		0xf800
#define DarkRed     		0x5000
#define DarkRed2     		0x5800
#define LightRed    		0xfcb2
#define LightRed2           0xf904
#define LightGreen  		0x679
#define Blue1        		0x1f
#define LightBlue1  		0x3bd
#define LighGreen   		0x87f0
#define Green1      		0x160
#define Green2      		0x3a0
#define GreenLine			0x5cb
#define Orange      		0xeba0
#define Orange1     		0xfb23
#define DarkOrange  		0xa9a0
#define DarkOrange2			0x8960
#define DarkOrange3         0x28a0
#define Auqa				0x332c
#define DarkAuqa			0x1e4
#define GrayBlue    		0x41f
#define Blue				0x10
#define GreenBlue           0x386
#define BlueGray          	0x249f
#define Yello         		0xfff0
#define StillBlue     		0x10a4

#define ButtonTextColor		0xdffd

/* Exported variables --------------------------------------------------------*/
extern const uint16_t M256_Colors[256];
extern const uint16_t grayScalle[256];

/* Exported function ---------------------------------------------------------*/
uint16_t Color_ChangeBrightness(uint16_t ColorIn, int8_t BrightLevel);

#endif /* __COLOR565_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

