/**
  ******************************************************************************
  * @file	 	NGL :: API
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.1.0
  * @date
  * @brief		header // fpv4-sp-d16
  ******************************************************************************
**/

#ifndef __NGL_H
#define __NGL_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "NGL_types.h"

/* Exported define -----------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constant ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern NGL_HAL_Typedef HX8352, R61581, SSD1289;

/* Exported function ---------------------------------------------------------*/

///* --------------------------------  Hardware Aplication Level  -------------------------------- */
extern void LCD_Set_RESET_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_RD_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_RS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_WR_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_Set_CS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void LCD_SetDataPort(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins);

extern int8_t NGL_HAL_Init(NGL_HAL_Typedef* controller, void *pDelayFunc);
extern void NGL_HAL_WriteRegister(uint16_t RegisterAddress, uint16_t Data);

///* ----------------------------------  Mid Aplication Level  ----------------------------------- */
extern void NGL_LCD_SetFullScreen(void);
extern void NGL_LCD_SetCursor(uint16_t X, uint16_t Y);
extern void NGL_LCD_ClearArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_LCD_Clear_AllScreen(uint16_t Color);
extern void NGL_LCD_PutPixel(uint16_t Color);
extern void NGL_LCD_GetPixels(uint8_t *ReadData, uint16_t NumPixels);
extern void NGL_LCD_SetRotation(NGL_RotationLCD NewState);

///* -------------------------------------  Set/Get Colors  -------------------------------------- */
extern void NGL_Color_SetTextColor(uint16_t color);
extern uint16_t NGL_Color_GetTextColor(void);
extern void NGL_Color_SetGraphicsColor(uint16_t color);
extern uint16_t NGL_Color_GetGraphicsColor(void);
extern void NGL_Color_SetBackColor(uint16_t color);
extern uint16_t NGL_Color_GetBackColor(void);
extern uint16_t NGL_Color_SetBrightness(uint16_t ColorIn, int8_t BrightLevel);

///* -----------------------------------  Graphics primitive  ------------------------------------ */
extern void NGL_GP_SetAntialiasingLine_State(uint8_t NewState);
extern void NGL_GP_DrawPixel(uint16_t X, uint16_t Y, uint16_t Color);
extern void NGL_GP_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_GP_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void NGL_GP_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color, NGL_DrawState border, uint16_t borderColor);
extern void NGL_GP_DrawCircle(uint16_t X, uint16_t Y, uint16_t Radius, uint16_t Color);
extern void NGL_GP_DrawBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color);
extern void NGL_GP_DrawFillBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint16_t Color, uint8_t border, uint16_t borderColor);
extern void NGL_GP_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
extern void NGL_GP_DrawFillTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);

///* ------------------------------  Object clip service function  ------------------------------ */
extern ErrorStatus NGL_GP_NewClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, NGL_ClipType clipType, uint8_t NumInd);
extern ErrorStatus NGL_GP_ClipClearObject(uint8_t NumInd);

extern FlagStatus NGL_GP_ClipPoint(uint16_t X0, uint16_t Y0);
extern FlagStatus NGL_GP_ClipLine(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);

///* ---------------------------------  Fonts, draw char/string  --------------------------------- */
extern void NGL_Font_SetFont(const NGL_Font *selectFont);
extern uint8_t NGL_Font_DrawChar(uint16_t x, uint16_t y, NGL_TransparentState trspr, char c);
extern uint16_t NGL_Font_DrawString(uint16_t X, uint16_t Y, uint16_t Color, const NGL_Font *font, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawFastString(uint16_t XPos, uint16_t YPos, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawColorString(uint16_t X, uint16_t Y, uint16_t Color, NGL_TransparentState trspr, char *str);
extern uint16_t NGL_Font_DrawCropString(uint16_t X, uint16_t Y, NGL_TransparentState trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color);
extern uint16_t NGL_Font_MeasureStringWidth(char *str);
extern uint16_t NGL_Font_MeasureCropStringWidth(char *str, uint8_t NumSymbols);
//extern void NGL_uintToString(uint32_t Num, char* Str);
extern void NGL_uintToFixedString(uint32_t Num, char* Str, uint8_t NumSymbol);

///* -----------------------------------------  Images  ----------------------------------------- */
extern void NGL_DrawImage(uint16_t X, uint16_t Y, const NGL_Image *Image);

///* -------------------------------------  GUI Functions  -------------------------------------- */
extern void NGL_GUI_DrawGrid(const void* Grid, NGL_DrawState state);
extern void NGL_GUI_DrawButton(const void* Button);

extern void NGL_GUI_DrawLabel(const void* Label);
extern void NGL_GUI_UpdateLabel(const void* Label, char *NewText);

extern void NGL_GUI_DrawFillBar(const void* fillbar);
extern void NGL_GUI_FillBarUpdate(const void* fillbar);

extern void NGL_GUI_DrawSeekBar(const void* SeekBar);
extern void NGL_GUI_DrawGraphScale(const void* gScale);
extern void NGL_GUI_DrawCheckBox(const void* checkbox);

extern void NGL_GUI_SelectPage(const NGL_Page *page);
extern NGL_Page* NGL_GUI_GetSelectedPage(void);
extern void NGL_GUI_DrawPageObjects(void);
extern int NGL_GUI_ClickPage(Coordinate point, NGL_TouchType type);

extern void NGL_GUI_DrawPageObjects(void);


#endif /* __NGL_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/