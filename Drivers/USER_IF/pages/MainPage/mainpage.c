/**
  ******************************************************************************
  * @file       MainPage.c
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date       2017-07-16 17:06:57.137293
  * @brief      NGL Page MainPage sourse
  ******************************************************************************
**/

/* Define --------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "NGL.h"
#include "fonts.h"
#include "bitmaps.h"
#include "mainpage_graphscales.h"
#include "mainpage_seekbars.h"
#include "mainpage_labels.h"
#include "mainpage_fillbars.h"
#include "mainpage_buttons.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern functions ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void MainPage_Draw(void);
static void MainPage_Click(Coordinate data, NGL_TouchType type);

/* Private variables ---------------------------------------------------------*/
NGL_Page MainPage = {
    { 0, 0, 480, 320 },       // [X0, Y0, X1, Y1]
    0,               // ID
    0x3166,           // BackColor
    TRUE,               // Exit allowed

    /* Objects pointers array (Buttons, Labels, FillBars, etc.) */
    {
        mainpage_buttons, 4,
        mainpage_labels, 2,
        mainpage_fillbars, 3,
        mainpage_seekbars, 1,
        mainpage_graphscales, 4,

    },

    MainPage_Draw,    // Page draw function
    MainPage_Click,   // Page click (or index change) function
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  MainPage_Draw
  *         Draw page objects function
  * @param  None
  * @retval None
*/
static void MainPage_Draw(void)
{
//      uint8_t cnt = 0;

    /* Clear all screen */
    NGL_Color_SetBackColor(0x3166);
    NGL_LCD_Clear_AllScreen(0x3166);

    /* Draw all static graphics primitive, labels, etc. */
    NGL_GP_DrawFillRect(10, 90, 395, 205, 0x3249, DRAW, 0x3249);
		NGL_GP_DrawFillRect(5, 290, 475, 315, 0x3249, DRAW, 0x3249);
		NGL_Font_DrawString(5, 250, 0xfff5, (NGL_Font*)&ms_shell_dlg_2_8_normal, Transparent, "L");
		NGL_Font_DrawString(5, 230, 0xfff5, (NGL_Font*)&ms_shell_dlg_2_8_normal, Transparent, "R");
		NGL_Font_DrawString(10, 298, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_10_bold, Transparent, "21:34:08");
		NGL_Font_DrawString(86, 306, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "USB");
		NGL_Font_DrawString(80, 291, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "EXT IN");
		NGL_Font_DrawString(200, 306, 0xdffe, (NGL_Font*)&ms_shell_dlg_2_8_normal, Transparent, "Sample Frequency");
		NGL_Font_DrawString(220, 291, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "192000");
		NGL_Font_DrawString(305, 306, 0xdffe, (NGL_Font*)&ms_shell_dlg_2_8_normal, Transparent, "Sample Resolution");
		NGL_Font_DrawString(335, 291, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "32");
		NGL_Font_DrawString(415, 306, 0xdffe, (NGL_Font*)&ms_shell_dlg_2_8_normal, Transparent, "Channels");
		NGL_Font_DrawString(435, 291, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "2");
		NGL_Font_DrawString(15, 76, 0xe71c, (NGL_Font*)&ms_shell_dlg_2_8_bold, Transparent, "AUDIO BUFFER:");
		NGL_GP_DrawFillRect(0, 317, 479, 318, 0x9d75, DRAW, 0x9d75);
		NGL_GP_DrawFillRect(0, 286, 479, 287, 0x9d75, DRAW, 0x9d75);


    /* Draw objects */
    NGL_GUI_DrawPageObjects();
}

/**
  * @brief  MainPage_Click
  *         Common click page objects
  * @param  None
  * @retval None
*/
static void MainPage_Click(Coordinate data, NGL_TouchType type)
{

}

/**
  * @brief  nglSeekBar_click
  *         Item/Items click event
  * @param  None
  * @retval None
*/
static void nglSeekBar_click(void) {
    /* */
	T11_Platform_TypeDef* plt = &T11_Platform;
    NGL_SeekBar* seek = (NGL_SeekBar*)MainPage.Objects.seekbars[0];
    float level = (float)seek->Level;
    float range = (float)(seek->Level_MAX - seek->Level_MIN);
    /* normalaze to -120..0 dB range */
    level = ((level * 120.0F) / range) - 120.0F;
    float vol[2] = { level, level };
    /* */
    plt->audio_if->volume->SetLevel( plt, vol );
}

/**
  * @brief  nglButton_4_click
  *         Item/Items click event
  * @param  None
  * @retval None
*/
static void nglButton_4_click(void) {
    T11_Platform_TypeDef* plt = &T11_Platform;
    /* */
    if( plt->audio_if->GetMute() ) {
        plt->audio_if->MuteCtl( plt, DISABLE );
    }
    else {
        plt->audio_if->MuteCtl( plt, ENABLE );
    }
}

/**
  * @brief  nglButton_3_click
  *         Item/Items click event
  * @param  None
  * @retval None
*/
static void nglButton_3_click(void) {
    static uint8_t st = 0;
    T11_Platform_TypeDef* plt = &T11_Platform;
    /* */
    plt->hal->relay->Source( st );
    st ^= 0x01;
}

/**
  * @brief  nglButton_2_click
  *         Item/Items click event
  * @param  None
  * @retval None
*/
static void nglButton_2_click(void)
{

}

/**
  * @brief  nglButton_click
  *         Item/Items click event
  * @param  None
  * @retval None
*/
static void nglButton_click(void)
{

}






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
