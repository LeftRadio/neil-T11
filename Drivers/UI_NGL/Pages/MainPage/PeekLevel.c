/**
  ******************************************************************************
  * @file	 	PeekLevel.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "arm_math.h"
#include "math_helper.h"

#include "Global_Init.h"
#include "Audio_Interface.h"
#include "PeekLevel.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint8_t Channels;
	float FullScale_dB;		// Full scale in dB

	uint16_t X0;
	uint16_t X1;
	uint16_t FullScale_Points;

	uint16_t CH_Y0[16];

	uint16_t Color;
	uint16_t LabelsColor;
} PeekLevel_TypeDef;

/* Private define ------------------------------------------------------------*/
//#define PEEK_FUUL_SCALE_POINTS						((uint16_t)360) // width of peek level visualisation rectangles
//#define X0											((uint16_t)15)
//#define XMAX										((uint16_t)(X0 + PEEK_FUUL_SCALE_POINTS))


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern FlagStatus AudioBuffer_HalfFull;

int MAX_LEFT, MAX_RIGHT;
int MIN_LEFT, MIN_RIGHT;

volatile PeekLevel_TypeDef PeekLevel =
{
		2,
		120.0F,

		15,
		375,
		360,

		{0},

		0x4410,
		0x1143
};

const char PeekChannelNames[8][4] = {
		"Lf", "Rf",
		"Lr", "Rr",
		"Sb", "Fr",
		"sL", "sR"
};


/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void PeekLevelRender(int LeftCHLevel, int RightCHLevel);
extern void ConvertToString(uint32_t Num, char* Str, uint8_t NumSymbol);
extern void ConvertFloatToString(char *ValText, float Value, uint8_t Length);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void PeekLevel_Set_Scale_Label(float level, uint16_t Color)
{
	char LabelsText[8] = "-000";
	int X = (int)((float)PeekLevel.X1 - (((float)PeekLevel.FullScale_Points * level) / PeekLevel.FullScale_dB));		//(int)((float)X0 + ((float)PEEK_FUUL_SCALE_POINTS / pow10(level / 20.0F)));

	if(X > PeekLevel.X1) X = PeekLevel.X1;
	else if(X < PeekLevel.X0) X = PeekLevel.X0;

	LabelsText[0] = '-';
	if(level >= 100)
	{
		ConvertToString((int)level, &LabelsText[1], 3);
		LabelsText[4] = 0;
	}
	else if(level >= 10)
	{
		ConvertToString((int)level, &LabelsText[1], 2);
		LabelsText[3] = 0;
	}
	else if(level > 0)
	{
		ConvertToString((int)level, &LabelsText[1], 1);
		LabelsText[2] = 0;
	}
	else
	{
		LabelsText[0] = '0';
		LabelsText[1] = 0;
	}

	X = X - (LCD_MeasureStrigWidth(LabelsText) / 2);

//	if(X > XMAX) X = XMAX;
//	else if(X < X0) X = X0;

	LCD_SetFont(&Square721_BT_9Info);
	LCD_PutColorStrig(X, PeekLevel.CH_Y0[0] + 15, 1, LabelsText, Color);
	LCD_PutColorStrig(X, PeekLevel.CH_Y0[PeekLevel.Channels - 1] - 15, 1, LabelsText, Color);
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void PeekLevel_Init(uint8_t channels, int fullScale, uint16_t X0, uint16_t X1, uint16_t Color, uint16_t LabelsColor, FlagStatus LogTypeStatus)
{
	uint8_t i;

	PeekLevel.Channels = channels;
	PeekLevel.FullScale_dB = fullScale;
	PeekLevel.X0 = X0;
	PeekLevel.X1 = X1;
	PeekLevel.FullScale_Points = PeekLevel.X1 - PeekLevel.X0;
	PeekLevel.Color = Color;
	PeekLevel.LabelsColor = LabelsColor;

	LCD_SetTextColor(GreenLine);
	LCD_SetFont(&Square721_BT_9Info);

	/* Init Y position for every channel */
	for(i = 0; i < PeekLevel.Channels; i++)
	{
		PeekLevel.CH_Y0[i] = 255 - (i * 15);

		/* Draw minimum/maximum level lines */
		LCD_DrawLine(PeekLevel.X0 - 1, PeekLevel.CH_Y0[i], PeekLevel.X0 - 1, PeekLevel.CH_Y0[i] + 12, Color_ChangeBrightness(PeekLevel.Color, +2));
		LCD_DrawLine(PeekLevel.X1 + 1, PeekLevel.CH_Y0[i], PeekLevel.X1 + 1, PeekLevel.CH_Y0[i] + 12, White);

		LCD_PutStrig(PeekLevel.X0 - 18, PeekLevel.CH_Y0[i], 1, (char*)PeekChannelNames[i]);
	}

	/* Sets scale labels */
	PeekLevel_Set_Scale_Label(PeekLevel.FullScale_dB, Color_ChangeBrightness(PeekLevel.LabelsColor, +14));
	PeekLevel_Set_Scale_Label(60, Color_ChangeBrightness(PeekLevel.LabelsColor, +14));
	PeekLevel_Set_Scale_Label(40, Color_ChangeBrightness(PeekLevel.LabelsColor, +16));
	PeekLevel_Set_Scale_Label(30, Color_ChangeBrightness(PeekLevel.LabelsColor, +18));
	PeekLevel_Set_Scale_Label(20, Color_ChangeBrightness(PeekLevel.LabelsColor, +20));
	PeekLevel_Set_Scale_Label(12, Color_ChangeBrightness(PeekLevel.LabelsColor, +24));
	PeekLevel_Set_Scale_Label(6, Color_ChangeBrightness(PeekLevel.LabelsColor, +26));
	PeekLevel_Set_Scale_Label(3, Color_ChangeBrightness(Yello, -1));
	PeekLevel_Set_Scale_Label(0, Yello);

	/* Render peek level visualisation */
	PeekLevelRender(1, 1);
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void PeekLevel_Cacl_and_Render(void)
{
	static uint16_t update = 0;
	int t_data, i;

	if((AudioBuffer_HalfFull != RESET) && (update == 1))
	{
		MAX_RIGHT = MAX_LEFT = INT32_MIN;
		MIN_RIGHT = MIN_LEFT = INT32_MAX;
		i = 0;

		do
		{
			t_data = (int16_t)(((int16_t)AudioBuffer[i + 1] << 8) | (int16_t)AudioBuffer[i]);
			if(MAX_LEFT < t_data) MAX_LEFT = t_data;
			if(MIN_LEFT > t_data) MIN_LEFT = t_data;

			t_data = (int16_t)(((int16_t)AudioBuffer[i + 3] << 8) | (int16_t)AudioBuffer[i + 2]);
			if(MAX_RIGHT < t_data) MAX_RIGHT = t_data;
			if(MIN_RIGHT > t_data) MIN_RIGHT = t_data;

			i += 4;
		} while (i < AUDIO_BUFFER_HALFSIZE);

		PeekLevelRender(MAX_LEFT - MIN_LEFT, MAX_RIGHT - MIN_RIGHT);
		update = 0;
	}
	else if(AudioBuffer_HalfFull == RESET)
	{
		update = 1;
	}
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void PeekLevelRender(int LeftCHLevel, int RightCHLevel)
{
	volatile static uint16_t OLD_DrawLevel[16] = {0};
	volatile int Level[16] = {0};
	volatile float DrawLevel_F;
	volatile int DrawLevel;
	uint8_t i;

	Level[0] = LeftCHLevel;
	Level[1] = RightCHLevel;

	for(i = 0; i < PeekLevel.Channels; i++)
	{
		/* Calc in dB and calc new X coordinate */
		DrawLevel_F = -20.0F * log10f(65535.0F / (float)Level[i]);
		DrawLevel = (int)((float)PeekLevel.X1 + (((float)PeekLevel.FullScale_Points * DrawLevel_F) / PeekLevel.FullScale_dB));

		/* Verify */
		if(DrawLevel > PeekLevel.X1) DrawLevel = PeekLevel.X1;
		else if(DrawLevel < PeekLevel.X0) DrawLevel = PeekLevel.X0;

		/* Draw */
		if(DrawLevel > OLD_DrawLevel[i])
		{
			LCD_DrawFillRect(PeekLevel.X0, PeekLevel.CH_Y0[i], DrawLevel, PeekLevel.CH_Y0[i] + 12, PeekLevel.Color, DRAW, PeekLevel.Color);
		}
		else if(DrawLevel < OLD_DrawLevel[i])
		{
			LCD_DrawFillRect(DrawLevel, PeekLevel.CH_Y0[i], OLD_DrawLevel[i], PeekLevel.CH_Y0[i] + 12, 0x1143, DRAW, 0x1143);
		}

		OLD_DrawLevel[i] = DrawLevel;
	}
}








/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
