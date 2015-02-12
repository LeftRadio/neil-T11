/**
  ******************************************************************************
  * @file    OFFTimerPage.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page OFFTimerPage sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"
#include "Fonts.h"
#include "Images.h"
#include "OFFTimerPage_Buttons.h"
#include "OFFTimerPage_Labels.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void OFFTimerPage_Draw(void);

/* Private variables ---------------------------------------------------------*/
NGL_Page OFFTimerPage = {

		{ 0, 0, 480, 320 },     // [X0, Y0, X1, Y1]
		DISABLE,     // Clip for Page
		3,     // Clip object ID
		0x0000,     // BackColor
		0,     // first button index
		16,     // All buttons count
		0,        // Selected button index
		0,        // Old selected button index
		14,        // Labels count

		/* Items pointers array */
		{  &Button1, &Button2, &Button3, &Button4, &Button5, &Button6,
			 &Button7, &Button8, &Button9, &Button10, &Button11, &Button12,
			 &Button13, &Button14, &Button15, &Button16, },

		/* Items pointers array */
		{  &Label1, &Label3,
			 &Label4, &Label5, &Label6, &Label7, &Label10, &Label11,
			 &Label12, &Label13, &Label14, &Label15, &Label16, &Label17,
			 },

		TRUE,        // Exit allowed
		OFFTimerPage_Draw,        // Page draw function
		(void*)0,    // Page CallBack function
};


/* Extern variables ----------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


/*******************************************************************************
* Function Name  : OFFTimerPage_Draw
* Description    : Draw page function
* Input          : None
* Return         : None
*******************************************************************************/
void OFFTimerPage_Draw(void)
{
	uint8_t cnt = 0;

	/* Clear all screen */
	LCD_Clear_AllScreen(0x0000);

	LCD_DrawLine(15, 257, 465, 257, 0x8410);
	LCD_DrawLine(15, 217, 465, 217, 0x8410);
	LCD_DrawLine(15, 177, 465, 177, 0x8410);
	LCD_DrawLine(15, 137, 465, 137, 0x8410);
	LCD_DrawLine(15, 97, 465, 97, 0x8410);
	LCD_DrawLine(5, 305, 475, 305, 0xB676);
	LCD_DrawLine(5, 40, 475, 40, 0xB676);
	LCD_DrawFillRect(5, 50, 5, 50, 0x1124, DRAW, 0x1124);

	/* Draw all page labels */
	for(cnt = 0; cnt < OFFTimerPage.LabelsCount; cnt++)
	{
		NGL_DrawLabel(OFFTimerPage.Labels[cnt]);
	}

	/* Draw all page buttons */
	for(cnt = 0; cnt < OFFTimerPage.ButtonsCount; cnt++)
	{
		NGL_DrawButton(OFFTimerPage.Buttons[cnt], ResetButton);
	}


}



/*******************************************************************************
* Function Name  : ItemEvent
* Description    : Item/Items click event
* Input          : None
* Return         : None
*******************************************************************************/
void ItemEvent(void)
{
	// you code
}




