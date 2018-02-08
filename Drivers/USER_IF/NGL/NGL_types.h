/**
  ******************************************************************************
  * @file		NGL_types.h
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date		2015-02-12-10.50
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NGL_TYPES_H
#define __NGL_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define ------------------------------------------------------------*/
#define NGL_MAX_BUTTONS						20
#define NGL_MAX_LABELS						30
#define NGL_LABEL_MAX_CHARS					30
#define NGL_GUI_TOUCH_NO_OBJECT				(int8_t)(-1)

#define __R61581__                  		0
#define __SSD1289__                 		1
#define __HX8352__                          2

/* Exported macro ------------------------------------------------------------*/
#define nMIN(a,b) (((a)<(b))?(a):(b))
#define nMAX(a,b) (((a)>(b))?(a):(b))

#define nRANGE(a,b,h) ( ((a)<((b)+(h)) && ((a)>((b)-(h))) )?(0):(-1) )


/* Exported typedef ----------------------------------------------------------*/
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

/* --------------------------  HAL/GL elements enums & structs  -------------------------- */
typedef enum {
    _0_degree,
    _90_degree,
    _180_degree,
    _270_degree
} NGL_RotationLCD;

typedef enum {
    NGL_Vertical,
    NGL_Horizontal
} NGL_VertHoriz_Type;

typedef enum {
    LCD_FSMC_Connect = 0,
    LCD_GPIO_Connect = 1
} NGL_HardConnectType;


typedef enum {
    CLEAR = 0,
    DRAW = 1
} NGL_DrawState;

typedef enum {
    Transparent = 0,
    NotTransparent = 1
} NGL_TransparentState;

typedef enum {
    TextButton,
    ColorFillButton,
    IconButton
} NGL_ButtonType;

typedef enum {
    ReClick_ENABLE,
    ReClick_DISABLE
} NGL_ReClickState;

typedef enum {
    ResetButton = 0,
    SelectedButton = 1
} NGL_ButtonState;

typedef struct {
   uint16_t x, y;
} Coordinate;

typedef struct {
    long double An, Bn, Cn, Dn, En, Fn, Divider;
} Matrix_Typedef;

typedef enum {
    Touch_Default,
    Touch_AxisX_State,
    Touch_AxisY_State,
    Touch_Interrupt,
    Touch_Polling,
    Touch_Data,
    Touch_Stop
} NGL_TouchState;

typedef enum {
    Touch_ADC_Interface,
    Touch_SPI_Interface
} NGL_TouchInterface;

typedef enum {
    NGL_Touch_NoTouch,
    NGL_Touch_SingleTouch,
    NGL_Touch_RepeatTouch,
} NGL_TouchType;

typedef enum { NGL_Clip_IN, NGL_Clip_OUT } NGL_ClipType;


/* LCD HAL object type */
typedef struct {
	NGL_HardConnectType ConnectionType;
	uint8_t DataBusBits;
	uint8_t ColorBits;
	uint16_t X_Max, Y_Max;
	uint32_t Max_Points;
	uint16_t X_Register, Y_Register;
	NGL_RotationLCD Rotation;

    void (*WriteRAM_Prepare)(void);
    void (*WriteCommand)(uint16_t Command);
    void (*WriteData)(uint16_t Data);
    uint16_t (*ReadData)(void);
    void (*WritePixel)(uint16_t Data);

    void (*ControllerInit)(void);
    void (*SetRotation)(NGL_RotationLCD NewState);
    void (*SetArea)(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);

    void (*SetCursor)(uint16_t X, uint16_t Y);
    void (*GetPixels)(uint8_t *ReadData, uint16_t NumPixels);

} NGL_HAL_Typedef;


/* --------------------------------  Font elements structs  -------------------------------- */

/* Font chars table element type */
typedef struct {
	const uint16_t	width;
	const uint16_t	height;
	const uint16_t	start;
} NGL_CharInfo;

/* NGL Font type */
typedef struct {
	const uint8_t Height;
	const uint8_t FirstChar;
	const uint8_t LastChar;
	const uint8_t FontSpace;
	const NGL_CharInfo *CharTable;
	const uint8_t *CharBitmaps;
} NGL_Font;


/* --------------------------------  Image elements structs  -------------------------------- */

/* NGL Image type */
typedef struct {
   const uint16_t Width;			// Picture Width
   const uint16_t Height;			// Picture Height
   const uint8_t Compressed;		// Copressed flag, 0 - NonCompressed, 1- RLE, 2 - picoJPG

   const uint8_t ColorBits;			// color bit per pixel
   const uint8_t DataBits;			// bitmap data array value bits
   const uint32_t DataArraySize;	// Last index of bitmap data array

   const void *Data;				// data pointer
} NGL_Image;


/* --------------------------------  UI elements structs  -------------------------------- */
typedef void (*pEvent)(void);
typedef void (*pPageClick)(Coordinate data, NGL_TouchType type);

/* NGL Point type */
typedef struct {
  uint16_t X;
  uint16_t Y;
} NGL_Point;

/* NGL Label type */
typedef struct {
	const uint16_t X;					        // Left X coordinate
	const uint16_t Y;					        // Down Y coordinate
	uint16_t Color;						        // Text color
	const NGL_TransparentState Transparent;     // Transparent state
	char* Text;                                 // Text
	const NGL_Font *Font;				        // Label font
} NGL_Label;

/* NGL UI Button type */
typedef struct {
	const uint16_t X;					    // Left X coordinate
	const uint16_t Y;					    // Down Y coordinate
	const uint16_t Width;				    // Button width (X1 = X0 + Width)
	const uint16_t Height;				    // Button height (Y1 = Y0 + height)
	const NGL_ButtonType Type;			    // Type of button
	uint16_t Color;						    // Forecolor for "Fill" type
	uint16_t SelectedColor;				    // Forecolor selected button for "Fill" type
	const FunctionalState ColorShift;	    // Forecolor shift for "Fill" type
	const NGL_Image *ICO;				    // Icon/bitmap
	const NGL_Font *Font;				    // Font for text
	const int16_t TextOffset_X;			    // Text X offset
	const int16_t TextOffset_Y;			    // Text Y offset
	uint16_t TextColor;					    // Color for text
	char* Text;                             // Text

	const NGL_ReClickState ReClickState;    // Enable or disable repeated clicks events
	FunctionalState Visible;			    // Visible state
	FlagStatus Enabled;					    // Enabled state, if RESET button not active and not request all clicks events

	const pEvent ClickEvent;                 // Click event function pointer
} NGL_Button;

/* NGL UI Grid type */
typedef struct {
	const uint16_t CenterX;					//
	const uint16_t CenterY;					//
	const uint16_t Width;					//
	const uint16_t Height;					//
	const uint16_t Color;					//
	const FunctionalState  CentralLines;	//
} NGL_Grid;

/* NGL UI FillBar type */
typedef struct {
	const uint16_t X0, Y0, X1, Y1;
	const NGL_VertHoriz_Type Orientation;
	const uint16_t Level_MIN, Level_MAX;
	uint16_t Level;
    uint16_t sfX1, sfY1;
	FunctionalState Logarithmic;
	uint16_t FullScale_dB;
	FunctionalState Border;
	FunctionalState Markers;
	uint16_t MarkersColor;
	uint16_t Color;
} NGL_FillBar;

/* NGL UI SeekBar type */
typedef struct {
	const uint16_t X0, Y0, X1, Y1;
    const uint16_t SliderSize;
    uint16_t old_posX, old_posY;
	const NGL_VertHoriz_Type Orientation;
	const int16_t Level_MIN, Level_MAX;
	int16_t Level;
    FunctionalState ShowProgress;
	uint16_t Color;
    uint16_t SliderColor;
    const pEvent ClickEvent;
} NGL_SeekBar;

/* NGL UI FillBar type */
typedef struct {
    const uint16_t X0, Y0, X1, Y1;
    int MIN, MAX;
    int Cent;
    const int Labels[20];
    uint8_t LabelsCount;
    char *Units;
    FunctionalState ShowLabels;
    FunctionalState ShowLines;
    FunctionalState Flip;
    const NGL_VertHoriz_Type Orientation;
    NGL_Font* Font;
    uint16_t Color;
} NGL_GraphScale;

/* NGL UI CheckBox type */
typedef struct {
    const uint16_t X0, Y0, X1, Y1;
    FunctionalState Checked;
    const NGL_Font* Font;
    const char* Text;
    uint16_t Color;
    uint16_t TextColor;
    const pEvent ClickEvent;
} NGL_CheckBox;

/* NGL UI page objects type */
typedef struct {

    const NGL_Button **buttons;
    uint16_t btn_count;

    const NGL_Label **lables;
    uint16_t lbl_count;

    const NGL_FillBar **fillbars;
    uint16_t flb_count;

    const NGL_SeekBar **seekbars;
    uint16_t skb_count;

    const NGL_GraphScale **graphscales;
    uint16_t grf_count;

    const NGL_CheckBox **checkboxs;
    uint16_t chxb_count;

} NGL_Objects;

/* NGL Pages type */
typedef struct {
	const uint16_t Size[4];		// [X0, Y0, X1, Y1]
	const uint8_t ID;			// Clip object ID
	const uint16_t BackColor;
	const bool ExitAllowed;		// Exit allowed

	NGL_Objects Objects;

	const pEvent Draw;
	const pPageClick Click; // Page click (or index change) function
} NGL_Page;



// /* --------------------------------  TouchScreen elements structs  -------------------------------- */
typedef int8_t (*__pv_func)(void);
typedef int8_t (*__ppFoops)(Coordinate sc, Coordinate* ds);
typedef int8_t (*__peFoops)(uint16_t data, NGL_TouchState state);

typedef struct  {
    const __pv_func configuration;
    const __pv_func calibrate;
    const __ppFoops point;
    const __peFoops event;
    const __pv_func do_touch;
    const __pv_func reset;
} NGL_TouchScreenFoops;

typedef int8_t (*__psCallBack)(NGL_TouchState state);
typedef int8_t (*__prCallBack)(Coordinate* samples, uint8_t len);
typedef void (*__peCallBack)(Coordinate data, NGL_TouchType type);

typedef struct  {
    const __pv_func HAL_Init;
    const __psCallBack HAL_SetState;
    const __prCallBack HAL_ReadSamples;
    const __prCallBack HAL_SaveSamples;
    const __pv_func HAL_NoTouch;
    const __peCallBack EventSignal;
} NGL_TouchScreen_Callbacks;

typedef struct {
    NGL_TouchInterface Interface;

    Matrix_Typedef Matrix;
    const Coordinate calibrateSamples[3];
    Coordinate data;
    NGL_TouchState state;
    uint16_t repeatDelay;

    const NGL_TouchScreen_Callbacks* callbacks;
    const NGL_TouchScreenFoops* foops;
} NGL_TouchScreen;




/* --------------------------------  Clip support structs  -------------------------------- */

typedef struct  {
    uint16_t X_Left;
    uint16_t Y_Down;
    uint16_t X_Right;
    uint16_t Y_Up;
    uint8_t NumInd;
    NGL_ClipType Type;
    FunctionalState State;
    FlagStatus (*pClipLine)(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);
} NGL_ClipObject;


/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/







#endif /* __NGL_TYPES_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
