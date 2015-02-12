/**
  ******************************************************************************
  * @file	 	touchscreen.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCHSCREEN_H
#define __TOUCHSCREEN_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef enum {
	Touch_AxisX_State = 0,
	Touch_AxisY_State	= 1,
	Touch_Interrupt = 2
} TouchState;

/* Exported variables --------------------------------------------------------*/
extern volatile FlagStatus TouchScreen_Done;

/* Exported function ---------------------------------------------------------*/
void TouchPanel_Configuration(FlagStatus ReCalibration);
void TouchScreen_UpdateState(void);
void TouchScreen_SetState(TouchState NewState);
int8_t TouchScreen_GetCoordinate(uint16_t* X, uint16_t* Y);








#endif /* __TOUCHSCREEN_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
