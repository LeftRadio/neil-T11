/**
  ******************************************************************************
  * @file    MainPage.c
  * @author  LeftRadio
  * @version V1.0.0
  * @date    11.12.2014 17:57:24
  * @brief   NGL Page MainPage sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "Global_Init.h"
#include "platform.h"

#include "NGL.h"
#include "Fonts.h"
#include "Images.h"
#include "MainPage_Buttons.h"
#include "MainPage_Labels.h"
#include "colors 5-6-5.h"

#include "PeekLevel.h"
#include "Spectrum.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void MainPage_Draw(void);

/* Private variables ---------------------------------------------------------*/
NGL_Page MainPage = {

		{ 0, 0, 480, 320 },     // [X0, Y0, X1, Y1]
		DISABLE,     // Clip for Page
		1,     // Clip object ID
		0x29E7,     // BackColor
		0,     // first button index
		12,     // All buttons count
		0,        // Selected button index
		0,        // Old selected button index
		26,        // Labels count

		/* Items pointers array */
		{  &btnRewind, &btnStop, &btnPlay, &btnPause, &btnForward, &btnTracks,
			 &btnVisual, &btnSettings, &btnControls, &btnMute, &btnSliderTrack, &btnOFFtimer,
			 },

		/* Items pointers array */
		{  &lblTime, &lblWeekDay, &lblYear, &lblFrequency, &lblBits, &lblChannels,
			 &lblINTCLK, &lblEXTCLK, &lblUSBstream, &lbl44_1k, &lbl48k, &lbl96k,
			 &lbl192k, &lbl16bit, &lbl24bit, &lbl32bit, &lblCH1, &lblCH2,
			 &lblCH4, &lblCH6, &lblCH8, &lblNeilT11, &lblTrackInfo, &lblTrackPlayTime,
			 &lblAtr, &lblFW_ver, },

		TRUE,        // Exit allowed
		MainPage_Draw,        // Page draw function
		(void*)0,    // Page CallBack function
};


/* Extern variables ----------------------------------------------------------*/
extern NGL_Label lblUSBstream;


/* Extern function -----------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : MainPage_Draw
* Description    : Draw page function
* Input          : None
* Return         : None
*******************************************************************************/
void MainPage_Draw(void)
{
	uint8_t cnt = 0;

	/* Clear all screen */
	LCD_Clear_AllScreen(0x29E7);

	LCD_DrawLine(5, 305, 475, 305, 0xB676);
	LCD_DrawLine(5, 57, 475, 57, 0xB696);
	LCD_DrawLine(5, 3, 475, 3, 0x6C95);
	LCD_DrawLine(5, 290, 475, 290, 0xC000);
	LCD_DrawLine(10, 87, 385, 87, 0xFC00);
	LCD_DrawLine(370, 48, 370, 51, 0xFFFF);
	LCD_DrawLine(10, 250, 385, 250, 0xEBA0);

	LCD_DrawFillRect(5, 70, 390, 285, 0x1143, DRAW, 0x1143);
	LCD_DrawFillRect(5, 8, 475, 44, 0x1143, DRAW, 0x1143);
	LCD_DrawFillRect(395, 70, 475, 285, 0x1143, DRAW, 0x1143);


	/* Draw all page buttons */
	for(cnt = 0; cnt < MainPage.ButtonsCount; cnt++)
	{
		NGL_DrawButton(MainPage.Buttons[cnt], ResetButton);
	}

    /* Draw all page labels */
	for(cnt = 0; cnt < MainPage.LabelsCount; cnt++)
	{
		NGL_DrawLabel(MainPage.Labels[cnt]);
	}

	/* Init peek level render */
	PeekLevel_Init(6, 90, 20, 385, Orange1, 0x1143, SET);		// Color_ChangeBrightness(0x4410, +8)
	Spectrum_Init(15, 72, 385, 162, 16, 80, 0x1143, SET);
}


/*******************************************************************************
* Function Name  : MainPage_Render
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void MainPage_Render(void)
{
	PeekLevel_Cacl_and_Render();
	Spectrum_Cacl_and_Render();

	if(T11_LED_Read(T11_LED_0) != 0)
	{
		lblUSBstream.Color = 0x87F0;
	}
	else
	{
		lblUSBstream.Color = MainPage.BackColor;
	}

	NGL_DrawLabel(&lblUSBstream);
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




