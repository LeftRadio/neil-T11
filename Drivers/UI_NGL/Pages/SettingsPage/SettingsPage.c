/**
  ****************************************************************************** 
  * @file    SettingsPage.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page SettingsPage sourse
  ****************************************************************************** 
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"
#include "Fonts.h"
#include "Images.h"
#include "SettingsPage_Buttons.h"
#include "SettingsPage_Labels.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SettingsPage_Draw(void);

/* Private variables ---------------------------------------------------------*/
NGL_Page SettingsPage = {

		{ 0, 0, 480, 320 },     // [X0, Y0, X1, Y1]
		DISABLE,     // Clip for Page
		2,     // Clip object ID
		0x4A49,     // BackColor
		0,     // first button index
		0,     // All buttons count
		0,        // Selected button index
		0,        // Old selected button index
		0,        // Labels count

		/* Items pointers array */
		{  },

		/* Items pointers array */
		{  },

		TRUE,        // Exit allowed
		SettingsPage_Draw,        // Page draw function
		(void*)0,    // Page CallBack function
};


/* Extern variables ----------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


/*******************************************************************************
* Function Name  : SettingsPage_Draw
* Description    : Draw page function
* Input          : None
* Return         : None
*******************************************************************************/
void SettingsPage_Draw(void)
{
	uint8_t cnt = 0;

	/* Clear all screen */
	LCD_Clear_AllScreen(0x4A49);


	/* Draw all page labels */
	for(cnt = 0; cnt < SettingsPage.LabelsCount; cnt++)
	{
		NGL_DrawLabel(SettingsPage.Labels[cnt]);
	}

	/* Draw all page buttons */
	for(cnt = 0; cnt < SettingsPage.ButtonsCount; cnt++)
	{
		NGL_DrawButton(SettingsPage.Buttons[cnt], ResetButton);
	}


}






