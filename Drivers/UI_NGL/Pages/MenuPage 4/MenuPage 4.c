/**
  ****************************************************************************** 
  * @file    MenuPage 4.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 0:44:20
  * @brief   NGL Page MenuPage 4 sourse
  ****************************************************************************** 
**/

/* Includes ------------------------------------------------------------------*/
#include "NGL.h"
#include "User_Interface.h"
#include "MenuPage 4_Items.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void MenuPage 4_Draw(void);

/* Private variables ---------------------------------------------------------*/
NGL_Page MenuPage 4 = {

		{ 0, 0, 480, 320 },     // [X0, Y0, X1, Y1]
		DISABLE,     // Clip for Page
		4,     // Clip object ID
		0,     // First item index
		0,     // All items count
		0,        // Selected item index
		0,        // Old selected item index

		/* Items pointers array */
		{  },

		/* Items pointers array */
		{  &Label1, },

		TRUE,        // Exit allowed
		MenuPage 4_Draw,        // Page draw function
		(void*)0,    // Page CallBack function
};


/* Extern variables ----------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


/*******************************************************************************
* Function Name  : MenuPage 4_Draw
* Description    : Draw page function
* Input          : None
* Return         : None
*******************************************************************************/
void MenuPage 4_Draw(void)
{

	uint8_t label_cnt = 0;
	for(label_cnt = 0; label_cnt < MenuPage 4.LabelsCount; label_cnt++)
	{
		NGL_DrawLabel(MenuPage 4.Labels[label_cnt]);
	}


	uint8_t button_cnt = 0;
	for(button_cnt = 0; button_cnt < MenuPage 4.ButtonsCount; button_cnt++)
	{
		NGL_DrawLabel(MenuPage 4.Buttons[button_cnt]);
	}


}






