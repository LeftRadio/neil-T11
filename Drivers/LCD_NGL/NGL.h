/*************************************************************************************
*
Description : HX8352 Grafics Library :: API
Version :  1.0.2
Date    :  20.08.2011
Author  :  Left Radio
Comments:
*
**************************************************************************************/

#ifndef __NGL_H
#define __NGL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define ------------------------------------------------------------*/
#define MAX_BUTTONS				20
#define MAX_LABELS				30
#define NGL_MAX_CHARS			30

#define __R61581__              0
#define __SSD1289__             1

/* Exported typedef -----------------------------------------------------------*/
/* --------------------------  HAL/GL elements enums & structs  -------------------------- */
//typedef void (*pfunc)(void);
//
//typedef enum {_0_degree, _90_degree, _180_degree, _270_degree} LCD_RotationState;
//typedef enum{ LCD_FSMC_Connect = 0, LCD_GPIO_Connect = 1 } HardConnectType;
//typedef enum {IN_OBJECT = 0, OUT_OBJECT = !IN_OBJECT} ClipMode;
//typedef enum {CLEAR = 0, DRAW = 1, ReDRAW = 2, CLEAR_ALL = 3} DrawState;
//typedef enum{ TextButton, ColorFillButton, IconButton } NGL_ButtonType;
//typedef enum {ReClick_ENABLE, ReClick_DISABLE} NGL_ReClickState;
//typedef enum {ResetButton = 0, SelectedButton = 1} NGL_ButtonState;
typedef void (*pfunc)(void);
typedef enum {false = 0, TRUE = !false} bool;
typedef enum { _0_degree, _90_degree, _180_degree, _270_degree } NGL_LCD_RotationState;
typedef enum { LCD_FSMC_Connect = 0, LCD_GPIO_Connect = 1 } NGL_HardConnectType;
typedef enum { IN_OBJECT = 0, OUT_OBJECT = !IN_OBJECT } NGL_ClipMode;
typedef enum { CLEAR = 0, DRAW = 1 } NGL_DrawState;
typedef enum { TextButton, ColorFillButton, IconButton } NGL_ButtonType;
typedef enum { ReClick_ENABLE, ReClick_DISABLE } NGL_ReClickState;
typedef enum { ResetButton = 0, SelectedButton = 1 } NGL_ButtonState;


/* LCD object type */
/* LCD object type */
typedef struct
{
	NGL_HardConnectType ConnectionType;
	uint8_t DataBusBits;
	uint8_t ColorBits;
	uint16_t X_Max, Y_Max;
	uint32_t Max_Points;
	uint16_t X_Register, Y_Register;
	NGL_LCD_RotationState Rotation;

	void (*WriteRAM_Prepare)(void);
	void (*WriteCommand)(uint16_t Command);
	void (*WriteData)(uint16_t Data);
	uint16_t (*ReadData)(void);
	void (*WritePixel)(uint16_t Data);

	void (*ControllerInit)(void);
	void (*SetRotation)(NGL_LCD_RotationState NewState);
	void (*SetArea)(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);

	void (*SetCursor)(uint16_t X, uint16_t Y);
	void (*GetPixels)(uint8_t *ReadData, uint16_t NumPixels);

} LCD_Typedef;

/* Font chars table element type */
typedef struct {
	const uint16_t	width;			// Ширина символа
	const uint16_t	start;			// Стартовый индекс на первый байт символа в массиве данных символов
} NGL_CharInfo;

/* NGL Font type */
typedef struct
{
	const uint8_t Height;			// Высота символов
	const uint8_t HeightBytes;		// Высота символов в байтах
	const uint8_t FirstChar;		// Индекс первого символа
	const uint8_t LastChar;			// Индекс последнего символа
	const uint8_t FontSpace;		// Пробел между символами
	const NGL_CharInfo *CharTable;	// Таблица индексов символов
	const uint8_t *CharBitmaps;		// Указатель на массив с данными о символах
} NGL_Font;

/* NGL Image type */
typedef struct
{
   const uint16_t Width;			// Picture Width
   const uint16_t Height;			// Picture Height
   const uint8_t RLE;				// // Copressed flag, 0 - NonCompressed, 1- RLE, 2 - picoJPG

   const uint8_t ColorBits;			// color bit per pixel
   const uint8_t DataBits;			// bitmap data array value bits
   const uint32_t DataArraySize;	// Last index of bitmap data array

   const void *Data;				// data pointer

} NGL_Image;


/* --------------------------------  UI elements structs  -------------------------------- */
/* NGL Label type */
typedef struct
{
	const uint16_t X;					// Left X coordinate
	const uint16_t Y;					// Down Y coordinate
	uint16_t Color;						// Text color
	const FunctionalState Transparent;	// Transparent state
	char* Text;//[NGL_MAX_CHARS];			// Text
	const NGL_Font *Font;				// Label font
} NGL_Label;

/* NGL UI Button type */
typedef struct
{
	const uint16_t X;					// Left X coordinate
	const uint16_t Y;					// Down Y coordinate
	const uint16_t Width;				// Button width (X1 = X0 + Width)
	const uint16_t Height;				// Button height (Y1 = Y0 + height)
	const NGL_ButtonType Type;				// Type of button
	uint16_t Color;						// Forecolor for "Fill" type
	uint16_t SelectedColor;				// Forecolor selected button for "Fill" type
	const FunctionalState ColorShift;			// Forecolor shift for "Fill" type
	const NGL_Image *ICO;						// Icon/bitmap
	const NGL_Font *Font;						// Font for text
	const int16_t TextOffset_X;				// Text X offset
	const int16_t TextOffset_Y;				// Text Y offset
	uint16_t TextColor;					// Color for text
//	char Text[NGL_MAX_CHARS];						// Text
    char* Text;

	const NGL_ReClickState ReClickState;		// Enable or disable repeated clicks events
	FunctionalState Visible;			// Visible state
	FlagStatus Enabled;					// Enabled state, if RESET button not active and not request all clicks events

	const pfunc ClickEvent;             // Click event function pointer
} NGL_Button;

/* NGL UI Grid type */
typedef struct
{
	const uint16_t CenterX;						//
	const uint16_t CenterY;						//
	const uint16_t Width;						//
	const uint16_t Height;						//
	const uint16_t Color;						//
	const FunctionalState  EnableCentralLines;	//

} NGL_Grid;

/* NGL Pages type */
typedef struct
{
	const uint16_t Size[4];				// [X0, Y0, X1, Y1]
	const FunctionalState ClipState;	// Clip for Page/Menu
	const uint8_t Clip_ID;     			// Clip object ID
	const uint16_t BackColor;
	const uint8_t FirstButtonsIndex;     	// First item index
	const uint8_t ButtonsCount;				// All items count
	uint8_t SelectedButtonsIndex;		// Selected item index
	uint8_t OldButtonsIndex;			// Old selected item index
	uint8_t LabelsCount;			// Old selected item index
	const NGL_Button* Buttons[MAX_BUTTONS];	// Items pointers array
	const NGL_Label* Labels[MAX_LABELS];			// Items pointers array
	const bool ExitAllowed;				// Exit allowed

	const pfunc Draw;
	const pfunc CallBack;
} NGL_Page;

/* Exported define -----------------------------------------------------------*/
/* Exported constant ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
//extern LCD_Typedef R61581, SSD1289;
extern LCD_Typedef *LCD;


/* Exported function --------------------------------------------------------*/

/* --------------------------------  Hardware Aplication Level  -------------------------------- */
void LCD_Set_RESET_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_RD_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_RS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_WR_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_Set_CS_Pin(GPIO_TypeDef* GPIOx, uint16_t Pin);
void LCD_SetDataPort(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins);

int8_t LCD_Set_ControllerType(LCD_Typedef* LCD_init);
LCD_Typedef* LCD_Get_ControllerType(uint8_t index);

void LCD_DelayInit(void *pfunc);
extern void (*NGL_Delay)(uint16_t delay);
int8_t LCD_ControllerInitialization(void);
void LCD_SetRotation(NGL_LCD_RotationState NewState);
void LCD_WriteRegister(uint16_t RegisterAddress, uint16_t Data);

/* ----------------------------------  Mid Aplication Level  ----------------------------------- */
void LCD_SetFullScreen(void);
void LCD_ClearArea(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
void LCD_Clear_AllScreen(uint16_t Color);

void LCD_SetCursor(uint16_t X, uint16_t Y);
void LCD_PutPixel(uint16_t Color);
void LCD_GetPixels(uint8_t *ReadData, uint16_t NumPixels);



/* -------------------------------------  Set/Get Colors  -------------------------------------- */
extern void LCD_SetTextColor(uint16_t color);
extern uint16_t LCD_GetTextColor(void);
extern void LCD_SetGraphicsColor(uint16_t color);
extern uint16_t LCD_GetGraphicsColor(void);
extern void LCD_SetBackColor(uint16_t color);
extern uint16_t LCD_GetBackColor(void);


/* -----------------------------------  Graphics primitive  ------------------------------------ */
extern void LCD_DrawPixel(uint16_t X, uint16_t Y, uint16_t Color);
extern void LCD_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color);
extern void LCD_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);
extern void LCD_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Color, NGL_DrawState border, uint16_t borderColor);
extern void LCD_DrawCircle(uint16_t X, uint16_t Y, uint16_t Radius);
extern void DrawBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius);
extern void DrawFillBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint8_t border, uint16_t borderColor);
extern void LCD_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
extern void LCD_DrawFillTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);
extern void LCD_DrawSIN(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, float periods, uint16_t Color);


/* ---------------------------------  Fonts, draw char/string  --------------------------------- */
extern void LCD_SetFont(const NGL_Font *selectFont);
extern uint8_t LCD_DrawChar(uint16_t x, uint16_t y, uint8_t trspr, char c);
extern uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);
extern uint16_t LCD_PutColorStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str, uint16_t Color);
extern uint16_t LCD_PutCropStrig(uint16_t X, uint16_t Y, uint8_t trspr, char *str, uint8_t Pos, uint8_t Lenght, uint16_t Color);

extern uint16_t LCD_MeasureStrigWidth(char *str);
extern uint16_t LCD_MeasureCropStrigWidth(char *str, uint8_t NumSymbols);


/* -----------------------------------------  Images  ----------------------------------------- */
extern void NGL_DrawImage(uint16_t X, uint16_t Y, NGL_Image *Image);


/* -------------------------------------  GUI Functions  -------------------------------------- */
extern void NGL_DrawGrid(NGL_Grid *Grid, NGL_DrawState state);
extern void NGL_DrawButton(const NGL_Button *Button, uint8_t PushState);
extern void NGL_DrawLabel(const NGL_Label *Label);
extern void NGL_UpdateLabel(const NGL_Label *Label, char *NewText);
extern void NGL_ButtonShiftColorSelect(uint8_t ShiftPixelNum);



/* ------------------------------  Object clip service function  ------------------------------ */
extern ErrorStatus Set_New_ClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, NGL_ClipMode NewClipMode, uint8_t NumInd);
extern ErrorStatus Clear_ClipObject(uint8_t NumInd);
extern FlagStatus Verify_Clip_Point(uint16_t X0, uint16_t Y0);
extern FlagStatus Verify_Clip_Line(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);
//extern FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1);





#endif /* __NGL_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

