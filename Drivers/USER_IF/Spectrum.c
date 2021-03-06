/**
  ******************************************************************************
  * @file	 	Spectrum.c
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

#include "platform.h"
// #include "t11_audio_if.h"
#include "fft.h"
#include "Spectrum.h"
#include "ngl_usr.h"

/* Private define ------------------------------------------------------------*/
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
	uint16_t X0, X1;
	uint16_t Y0, Y1;

	uint8_t BandsNum;
	uint16_t BandDraw_Widht;
	uint16_t Band_X[16];
	uint16_t BandsFreq[16];
	char BandsLabels[16][5];

	FlagStatus LogScale;
	float FullScale_dB;		// Full scale in dB

	int Bands_DrawLevels[16];
	int Bands_OLD_DrawLevel[16];

	uint16_t LabelsColor;
} Spectrum_TypeDef;

/* Private variables ---------------------------------------------------------*/
extern FlagStatus AudioBuffer_HalfFull;

int MAX_LEFT, MAX_RIGHT;
int MIN_LEFT, MIN_RIGHT;

volatile Spectrum_TypeDef Spectrum = {
	5, 360,
	40, 80,

	16,
	0,
	{0},
	{
		50, 100, 200, 400,
		600, 1000, 2000, 3000,
		4000, 6000, 8000, 10000,
		12000, 15000, 18000, 20000
	},
	{
		"50", "100", "200", "400",
		"600", "1k", "2k", "3k",
		"4k", "6k", "8k", "10k",
		"12k", "15k", "18k", "20k"
	},

	SET,
	60,

	{1},
	{0},

	0x4410
};


const float Coeffs_64[64] = {
1, 0, 0.995184726672197, -0.0980171403295606, 0.98078528040323, -0.195090322016128, 0.956940335732209, -0.290284677254462, 0.923879532511287, -0.38268343236509,
0.881921264348355, -0.471396736825998, 0.831469612302545, -0.555570233019602, 0.773010453362737, -0.634393284163645, 0.707106781186548, -0.707106781186547, 0.634393284163645, -0.773010453362737,
0.555570233019602, -0.831469612302545, 0.471396736825998, -0.881921264348355, 0.38268343236509, -0.923879532511287, 0.290284677254462, -0.956940335732209, 0.195090322016128, -0.98078528040323,
0.0980171403295603, -0.995184726672197, -3.82858892158944E-16, -1, -0.0980171403295611, -0.995184726672197, -0.195090322016129, -0.98078528040323, -0.290284677254463, -0.956940335732209,
-0.382683432365091, -0.923879532511286, -0.471396736825998, -0.881921264348355, -0.555570233019603, -0.831469612302545, -0.634393284163646, -0.773010453362736, -0.707106781186548, -0.707106781186547,
-0.773010453362738, -0.634393284163644, -0.831469612302546, -0.555570233019601, -0.881921264348356, -0.471396736825996, -0.923879532511287, -0.382683432365088, -0.956940335732209, -0.290284677254461,
-0.980785280403231, -0.195090322016126, -0.995184726672197, -0.0980171403295586,
};

const float Coeffs_128[128] = {
1, 0, 0.998795456205172, -0.049067674327418, 0.995184726672197, -0.0980171403295606, 0.989176509964781, -0.146730474455362, 0.98078528040323, -0.195090322016128,
0.970031253194544, -0.242980179903264, 0.956940335732209, -0.290284677254462, 0.941544065183021, -0.33688985339222, 0.923879532511287, -0.38268343236509, 0.903989293123443, -0.427555093430282,
0.881921264348355, -0.471396736825998, 0.857728610000272, -0.514102744193222, 0.831469612302545, -0.555570233019602, 0.803207531480645, -0.595699304492433, 0.773010453362737, -0.634393284163646,
0.740951125354959, -0.671558954847019, 0.707106781186547, -0.707106781186548, 0.671558954847018, -0.740951125354959, 0.634393284163645, -0.773010453362737, 0.595699304492433, -0.803207531480645,
0.555570233019602, -0.831469612302545, 0.514102744193221, -0.857728610000272, 0.471396736825997, -0.881921264348355, 0.427555093430282, -0.903989293123444, 0.382683432365089, -0.923879532511287,
0.336889853392219, -0.941544065183021, 0.290284677254462, -0.956940335732209, 0.242980179903263, -0.970031253194544, 0.195090322016127, -0.980785280403231, 0.146730474455361, -0.989176509964781,
0.0980171403295597, -0.995184726672197, 0.049067674327417, -0.998795456205172, -1.04899270693404E-15, -1, -0.0490676743274191, -0.998795456205172, -0.0980171403295618, -0.995184726672197,
-0.146730474455363, -0.989176509964781, -0.195090322016129, -0.98078528040323, -0.242980179903265, -0.970031253194544, -0.290284677254464, -0.956940335732208, -0.336889853392221, -0.94154406518302,
-0.382683432365091, -0.923879532511286, -0.427555093430283, -0.903989293123443, -0.471396736825999, -0.881921264348354, -0.514102744193223, -0.857728610000271, -0.555570233019603, -0.831469612302545,
-0.595699304492434, -0.803207531480644, -0.634393284163646, -0.773010453362737, -0.671558954847019, -0.740951125354959, -0.707106781186548, -0.707106781186547, -0.740951125354959, -0.671558954847018,
-0.773010453362737, -0.634393284163645, -0.803207531480645, -0.595699304492433, -0.831469612302545, -0.555570233019603, -0.857728610000272, -0.514102744193222, -0.881921264348355, -0.471396736825998,
-0.903989293123443, -0.427555093430283, -0.923879532511286, -0.382683432365091, -0.94154406518302, -0.336889853392221, -0.956940335732208, -0.290284677254464, -0.970031253194544, -0.242980179903265,
-0.98078528040323, -0.19509032201613, -0.989176509964781, -0.146730474455364, -0.995184726672197, -0.0980171403295626, -0.998795456205172, -0.0490676743274202,
};

const float Coeffs_256[256] = {
1, 0, 0.999698818696204, -0.0245412285229123, 0.998795456205172, -0.049067674327418, 0.99729045667869, -0.0735645635996674, 0.995184726672197, -0.0980171403295606,
0.99247953459871, -0.122410675199216, 0.989176509964781, -0.146730474455362, 0.985277642388941, -0.170961888760301, 0.98078528040323, -0.195090322016128, 0.975702130038529, -0.21910124015687,
0.970031253194544, -0.242980179903264, 0.96377606579544, -0.266712757474898, 0.956940335732209, -0.290284677254462, 0.949528180593037, -0.313681740398892, 0.941544065183021, -0.33688985339222,
0.932992798834739, -0.359895036534988, 0.923879532511287, -0.38268343236509, 0.914209755703531, -0.40524131400499, 0.903989293123443, -0.427555093430282, 0.893224301195515, -0.449611329654607,
0.881921264348355, -0.471396736825998, 0.870086991108711, -0.492898192229784, 0.857728610000272, -0.514102744193222, 0.844853565249707, -0.534997619887097, 0.831469612302545, -0.555570233019603,
0.817584813151583, -0.575808191417846, 0.803207531480645, -0.595699304492434, 0.788346427626606, -0.615231590580627, 0.773010453362737, -0.634393284163646, 0.757208846506484, -0.653172842953777,
0.740951125354959, -0.671558954847019, 0.724247082951467, -0.689540544737067, 0.707106781186547, -0.707106781186548, 0.689540544737067, -0.724247082951467, 0.671558954847018, -0.740951125354959,
0.653172842953776, -0.757208846506485, 0.634393284163645, -0.773010453362737, 0.615231590580626, -0.788346427626607, 0.595699304492433, -0.803207531480645, 0.575808191417845, -0.817584813151584,
0.555570233019602, -0.831469612302546, 0.534997619887097, -0.844853565249707, 0.514102744193221, -0.857728610000272, 0.492898192229784, -0.870086991108712, 0.471396736825997, -0.881921264348355,
0.449611329654606, -0.893224301195516, 0.427555093430282, -0.903989293123443, 0.40524131400499, -0.914209755703531, 0.38268343236509, -0.923879532511287, 0.359895036534988, -0.932992798834739,
0.33688985339222, -0.941544065183021, 0.313681740398892, -0.949528180593037, 0.290284677254463, -0.956940335732209, 0.266712757474899, -0.96377606579544, 0.242980179903264, -0.970031253194544,
0.21910124015687, -0.975702130038528, 0.195090322016129, -0.98078528040323, 0.170961888760302, -0.985277642388941, 0.146730474455362, -0.989176509964781, 0.122410675199217, -0.99247953459871,
0.0980171403295614, -0.995184726672197, 0.0735645635996683, -0.99729045667869, 0.049067674327419, -0.998795456205172, 0.0245412285229134, -0.999698818696204, 1.17145334231628E-15, -1,
-0.024541228522911, -0.999698818696204, -0.0490676743274167, -0.998795456205172, -0.073564563599666, -0.99729045667869, -0.0980171403295591, -0.995184726672197, -0.122410675199215, -0.99247953459871,
-0.14673047445536, -0.989176509964781, -0.170961888760299, -0.985277642388942, -0.195090322016126, -0.980785280403231, -0.219101240156868, -0.975702130038529, -0.242980179903262, -0.970031253194545,
-0.266712757474896, -0.96377606579544, -0.29028467725446, -0.956940335732209, -0.313681740398889, -0.949528180593037, -0.336889853392218, -0.941544065183022, -0.359895036534986, -0.93299279883474,
-0.382683432365087, -0.923879532511288, -0.405241314004987, -0.914209755703532, -0.42755509343028, -0.903989293123444, -0.449611329654604, -0.893224301195516, -0.471396736825995, -0.881921264348356,
-0.492898192229782, -0.870086991108713, -0.514102744193219, -0.857728610000274, -0.534997619887095, -0.844853565249709, -0.5555702330196, -0.831469612302547, -0.575808191417843, -0.817584813151585,
-0.595699304492431, -0.803207531480647, -0.615231590580624, -0.788346427626608, -0.634393284163643, -0.773010453362739, -0.653172842953774, -0.757208846506487, -0.671558954847016, -0.740951125354961,
-0.689540544737064, -0.724247082951469, -0.707106781186545, -0.70710678118655, -0.724247082951464, -0.68954054473707, -0.740951125354957, -0.671558954847021, -0.757208846506482, -0.65317284295378,
-0.773010453362734, -0.634393284163649, -0.788346427626604, -0.61523159058063, -0.803207531480642, -0.595699304492437, -0.817584813151581, -0.575808191417849, -0.831469612302543, -0.555570233019606,
-0.844853565249705, -0.534997619887101, -0.85772861000027, -0.514102744193226, -0.870086991108709, -0.492898192229788, -0.881921264348353, -0.471396736826002, -0.893224301195513, -0.449611329654611,
-0.903989293123441, -0.427555093430286, -0.914209755703529, -0.405241314004994, -0.923879532511285, -0.382683432365094, -0.932992798834737, -0.359895036534993, -0.941544065183019, -0.336889853392225,
-0.949528180593035, -0.313681740398896, -0.956940335732207, -0.290284677254467, -0.963776065795438, -0.266712757474904, -0.970031253194543, -0.242980179903269, -0.975702130038527, -0.219101240156875,
-0.980785280403229, -0.195090322016134, -0.98527764238894, -0.170961888760307, -0.98917650996478, -0.146730474455368, -0.992479534598709, -0.122410675199222, -0.995184726672196, -0.0980171403295666,
-0.99729045667869, -0.0735645635996735, -0.998795456205172, -0.0490676743274242, -0.999698818696204, -0.0245412285229185,
};


int16_t Buff[1 << NS_P];
int16_t SpectrOut[(1 << NS_P) / 2];

extern NGL_Label lblTime;
char str[50] = {0};
volatile unsigned long timer;

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SpectrumRender(int16_t* Bands_buf, uint16_t Lenght);

extern void ConvertToString(uint32_t Num, char* Str, uint8_t NumSymbol);
extern void ConvertFloatToString(char *ValText, float Value, uint8_t Length);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void Spectrum_Set_Scale_Label(float level, uint16_t Color) {
	//	char LabelsText[8] = "-000";
	//	int X = (int)((float)PeekLevel.X1 - (((float)PeekLevel.FullScale_Points * level) / PeekLevel.FullScale_dB));		//(int)((float)X0 + ((float)PEEK_FUUL_SCALE_POINTS / pow10(level / 20.0F)));
	//
	//	if(X > PeekLevel.X1) X = PeekLevel.X1;
	//	else if(X < PeekLevel.X0) X = PeekLevel.X0;
	//
	//	LabelsText[0] = '-';
	//	if(level >= 100)
	//	{
	//		ConvertToString((int)level, &LabelsText[1], 3);
	//		LabelsText[4] = 0;
	//	}
	//	else if(level >= 10)
	//	{
	//		ConvertToString((int)level, &LabelsText[1], 2);
	//		LabelsText[3] = 0;
	//	}
	//	else if(level > 0)
	//	{
	//		ConvertToString((int)level, &LabelsText[1], 1);
	//		LabelsText[2] = 0;
	//	}
	//	else
	//	{
	//		LabelsText[0] = '0';
	//		LabelsText[1] = 0;
	//	}
	//
	//	X = X - (LCD_MeasureStrigWidth(LabelsText) / 2);

	//	if(X > XMAX) X = XMAX;
	//	else if(X < X0) X = X0;

	//	LCD_SetFont(&Square721_BT_9Info);
	//	LCD_PutColorStrig(X, PeekLevel.CH_Y0[0] + 15, 1, LabelsText, Color);
	//	LCD_PutColorStrig(X, PeekLevel.CH_Y0[PeekLevel.Channels - 1] - 15, 1, LabelsText, Color);
}

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void Spectrum_Init(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint8_t BandsNum, float FullScale_dB, uint16_t LabelsColor, FlagStatus LogTypeStatus) {
	uint8_t i;

	Spectrum.X0 = X0;
	Spectrum.X1 = X1;
	Spectrum.Y0 = Y0;
	Spectrum.Y1 = Y1;
	Spectrum.BandsNum = BandsNum;
	Spectrum.BandDraw_Widht = (uint16_t)((float)(Spectrum.X1 - Spectrum.X0) / (float)Spectrum.BandsNum);
	Spectrum.LabelsColor = LabelsColor;
	Spectrum.LogScale = LogTypeStatus;

	if(Spectrum.LogScale != RESET)
	{
		Spectrum.FullScale_dB = FullScale_dB;
	}
	else
	{
		Spectrum.FullScale_dB = (float)65536;
	}

	/* Init X position for every Band */
	for(i = 0; i < Spectrum.BandsNum; i++)
	{
		Spectrum.Band_X[i] = Spectrum.X0 + (i * Spectrum.BandDraw_Widht);
		Spectrum.Bands_DrawLevels[i] = Spectrum.Y0;
		Spectrum.Bands_OLD_DrawLevel[i] = Spectrum.Y0;

		/* Draw minimum/maximum level lines */
		LCD_DrawLine(
				Spectrum.Band_X[i] + 1,
				Spectrum.Y0 - 1,
				Spectrum.Band_X[i] + (Spectrum.BandDraw_Widht - 1),
				Spectrum.Y0 - 1,
				M256_Colors[(256 * i) / Spectrum.BandsNum]);

		//		LCD_DrawLine(Spectrum.Band_X[i] + 1, Spectrum.Y1 + 1, Spectrum.Band_X[i] + (Spectrum.BandDraw_Widht - 1), Spectrum.Y1 + 1, Red);
	}
}

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void Spectrum_Cacl_and_Render(void) {
	static uint16_t update = 0;
	uint16_t i, j;
	int16_t LeftData, RightData;
	uint8_t* audio_buff = T11_Platform.audio_if->stream->buff_ptr;

	if((AudioBuffer_HalfFull != RESET) && (update == 1))
	{
		SCB_DEMCR |= 0x01000000;
		DWT_CYCCNT  = 0;
		DWT_CONTROL|= 1; // enable counter

		i = j = 0;
		do {
			LeftData = (int16_t)(((int16_t)audio_buff[i + 1] << 8) | (int16_t)audio_buff[i]);
			RightData = (int16_t)(((int16_t)audio_buff[i + 3] << 8) | (int16_t)audio_buff[i + 2]);

			if(LeftData > RightData) Buff[j] = LeftData;
			else Buff[j] = RightData;

			i += 4;
			j++;
		} while(i < (1 << NS_P) * 4);

		/* Calc FFT */
		fft_realMag_int16_calc(Buff, SpectrOut, Coeffs_128);

		/* Render spectrum bands */
		SpectrumRender(&SpectrOut[0], (1 << NS_P) / 2);
		update = 0;

		timer = DWT_CYCCNT;
		DWT_CONTROL &= ~1; // disable counter

		ConvertToString(timer, str, 9);
		NGL_UpdateLabel(&lblTime, str);
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
static void SpectrumRender(int16_t* Bands_buf, uint16_t Lenght) {
	volatile float DrawLevel_F;
	volatile int DrawLevel;
	uint16_t i, j, k;
	float Y;
	float FrqY = (float)T11_Platform.audio_if->dac->i2s_config->I2S_AudioFreq / (float)(1 << NS_P);
	uint16_t index;
	long sum = 0;

	j = 0;
	do
	{
		index = (uint16_t)((float)Spectrum.BandsFreq[j] / FrqY);

		k = 1;
		sum = 0;
		do
		{
			if((index - k) >= 0) sum += *(Bands_buf + index - k);
			if((index - k) < Lenght)sum += *(Bands_buf + index + k);

			k--;
		} while(k > 0);

		Spectrum.Bands_DrawLevels[j] = *(Bands_buf + index);// + (sum / 16);
		j += 1;

	} while(j < Spectrum.BandsNum);


	for(i = 0; i < Spectrum.BandsNum; i++)
	{
		/* Calc in dB and calc new Y coordinate */
		if(Spectrum.LogScale != RESET)
		{
			Y = (float)Spectrum.Y1;
			DrawLevel_F = -20.0F * log10f((65536.0F / 2.0F) / (float)Spectrum.Bands_DrawLevels[i]);
			if(DrawLevel_F > 0) DrawLevel_F = 0;
		}
		else
		{
			Y = (float)Spectrum.Y0;
			DrawLevel_F = (float)Spectrum.Bands_DrawLevels[i] * 10.0F;
		}

		DrawLevel = (int)(Y + (((float)(Spectrum.Y1 - Spectrum.Y0) * DrawLevel_F) / Spectrum.FullScale_dB));

		/* Verify */
		if(DrawLevel > Spectrum.Y1) DrawLevel = Spectrum.Y1;
		else if(DrawLevel < Spectrum.Y0) DrawLevel = Spectrum.Y0;

		/* Draw */
		if(DrawLevel > Spectrum.Bands_OLD_DrawLevel[i])
		{
			LCD_DrawFillRect(
					Spectrum.Band_X[i] + 1,
					(uint16_t)Spectrum.Bands_OLD_DrawLevel[i],
					Spectrum.Band_X[i] + (Spectrum.BandDraw_Widht - 1),
					(uint16_t)DrawLevel,
					M256_Colors[(256 * i)/Spectrum.BandsNum],
					DRAW,
					M256_Colors[(256 * i)/Spectrum.BandsNum]
							);
		}
		else if(DrawLevel < Spectrum.Bands_OLD_DrawLevel[i])
		{
			LCD_DrawFillRect(
					Spectrum.Band_X[i] + 1,
					(uint16_t)DrawLevel,
					Spectrum.Band_X[i] + (Spectrum.BandDraw_Widht - 1),
					(uint16_t)Spectrum.Bands_OLD_DrawLevel[i],
					0x1143,
					DRAW,
					0x1143
							);
		}

		Spectrum.Bands_OLD_DrawLevel[i] = DrawLevel;
	}
}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
