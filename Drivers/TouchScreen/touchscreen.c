/**
  ******************************************************************************
  * @file	 	toucscreen.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "touchscreen.h"
#include "platform.h"
#include "systick.h"
#include "LCD.h"

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT
{
   uint16_t x;
   uint16_t y;
} Coordinate;

typedef struct Matrix
{
long double An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider;
} Matrix;

/* Private define ------------------------------------------------------------*/
#define TOUCH_FLAG_FLASH_ADDRESS				((uint32_t)0x080E0000)
#define TOUCH_DATA_FLASH_ADDRESS				((uint32_t)(TOUCH_FLAG_FLASH_ADDRESS + 4))
#define TOUCH_FLAG_IS_OK						((uint32_t)0xAA88AA88)

#define TOUCH_GPIO_PORT							GPIOA
#define TOUCH_GPIO_XDP_PIN						GPIO_Pin_0
#define TOUCH_GPIO_XDN_PIN						GPIO_Pin_2
#define TOUCH_GPIO_YDP_PIN						GPIO_Pin_1
#define TOUCH_GPIO_YDN_PIN						GPIO_Pin_6

#define ADC3_DR_ADDRESS     					((uint32_t)0x4001224C)
#define TOUC_ADC_DATA_CNT						((uint32_t)1000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile FlagStatus TouchScreen_Done = RESET;
volatile TouchState TouchMode = Touch_AxisX_State;

/*  */
Matrix matrix;
Coordinate display;
Coordinate TouchCoordinate = {0, 0};

/* DisplaySamples */
Coordinate DisplaySample[3] = { { 45, 45 }, {45, 270}, {430,160} } ;
Coordinate ScreenSample[3];

/*  */
__IO uint16_t TouchScreenADCBuffer[TOUC_ADC_DATA_CNT+1] = {0};

char lblDbgTextX[15] = {0};
NGL_Label lblDbgX = {
		100,					// Left X coordinate
		160,					// Down Y coordinate
		White,						// Text color
		ENABLE,				// Transparent state
		lblDbgTextX,			// Text
		&Square721_BT_14Info				// Label font

};
char lblDbgTextY[15] = {0};
NGL_Label lblDbgY = {
		100,					// Left X coordinate
		130,					// Down Y coordinate
		White,						// Text color
		ENABLE,				// Transparent state
		lblDbgTextY,			// Text
		&Square721_BT_14Info				// Label font

};


/* Private function prototypes -----------------------------------------------*/
static void TouchScreen_HAL_Init(void);
static void TouchScreen_Calibrate(void);
static FunctionalState setCalibrationMatrix(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr);
static FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr);
static void DrawCross(uint16_t Xpos, uint16_t Ypos);


/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void TouchScreen_Configuration(FlagStatus ReCalibration)
{
	uint8_t i;
	uint32_t data_32 = 0;

	/* Init HAL level for touch, GPIO, EXTI, ADC and DMA */
	TouchScreen_HAL_Init();

	/* Read touchpanel calibration flag from flash*/
	data_32 = *(volatile uint32_t*)TOUCH_FLAG_FLASH_ADDRESS;

	/* If touchpanel is not calibrated or manual recalibrate */
	if((data_32 != TOUCH_FLAG_IS_OK) || (ReCalibration != RESET))
	{
		TouchScreen_Calibrate();
	}
	else	/* Else if touch already calibrated read coeffs and aplly calibration matrix */
	{
		/* Read */
		for(i = 0; i < 3; i++)
		{
			data_32 = *(volatile uint32_t*)(TOUCH_DATA_FLASH_ADDRESS + (i * 4));

			ScreenSample[i].x = (uint16_t)(data_32 >> 16);
			ScreenSample[i].y = (uint16_t)(data_32 & 0x0000FFFF);
		}

		/* Set matrix */
		setCalibrationMatrix(&DisplaySample[0], &ScreenSample[0], &matrix);
	}
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void TouchScreen_HAL_Init(void)
{
	//------------------------------------------------------------------------------
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	//
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	//------------------------------------------------------------------------------
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) &TouchScreenADCBuffer[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = TOUC_ADC_DATA_CNT;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA2_Stream0, ENABLE);

	//------------------------------------------------------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// базовая нстйрока
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 625 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// выход синхронизации
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);


	/* Connect EXTI Line0 to PA2 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_SetPriority(EXTI2_IRQn, 13);
	TouchScreen_SetState(Touch_Interrupt);

	/*  */
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	NVIC_SetPriority(DMA2_Stream0_IRQn, 12);
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void TouchScreen_Calibrate(void)
{
	uint16_t X, Y;
	volatile uint8_t i = 0;
	volatile uint8_t verify_cnt = 0;
	FlagStatus EndCalibration = RESET;

    LCD_SetFont(&Square721_BT_14Info);
    LCD_PutColorStrig(10, 10, 1, "Touch crosshair to calibrate", White);

	while(EndCalibration == RESET)
	{
		for(i = 0; i < 3; i++)
		{
			DrawCross(DisplaySample[i].x, DisplaySample[i].y);
			NGL_DrawLabel(&lblDbgX);
			NGL_DrawLabel(&lblDbgY);

			while( TouchScreen_GetCoordinate(&ScreenSample[i].x, &ScreenSample[i].y) != 0)
			{
			}

			T11_LED_ON(T11_LED_0);
			delay_ms(500);

			LCD_ClearArea(
			                 DisplaySample[i].x - 20, DisplaySample[i].y - 20,
			                 DisplaySample[i].x + 20, DisplaySample[i].y + 20,
			                 Black);

			while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != (uint8_t)Bit_RESET));
			delay_ms(500);
			TouchScreen_Done = RESET;

			T11_LED_OFF(T11_LED_0);
			delay_ms(50);
		}

		/* Set new calibration matrix */
		setCalibrationMatrix( &DisplaySample[0], &ScreenSample[0], &matrix );

		verify_cnt = 5;
		while(verify_cnt)
		{
			LCD_Clear_AllScreen(Black);
			LCD_DrawFillRect(225, 145, 255, 175, Red, DRAW, White);
			DrawCross(240, 160);
			LCD_PutColorStrig(10, 10, 1, "touch to verify", White);

			while( TouchScreen_GetCoordinate(&X, &Y) != 0)
			{
			}

			T11_LED_ON(T11_LED_0);
			delay_ms(500);

			while((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != (uint8_t)Bit_RESET));
			delay_ms(500);
			TouchScreen_Done = RESET;

			T11_LED_OFF(T11_LED_0);
			delay_ms(50);

			if((X > 225) && (X < 255))
			{
				if((Y > 145) && (Y < 175))
				{
					EndCalibration = SET;
					break;
				}
			}
			else
			{
				verify_cnt--;
			}
		}

		LCD_Clear_AllScreen(Black);
		if(verify_cnt)
		{
			LCD_PutColorStrig(10, 10, 1, "Calibration success", White);
		}
		else
		{
			LCD_PutColorStrig(10, 10, 1, "Calibration error", White);
			matrix.Divider = 0;
		}
		delay_ms(1500);
		LCD_Clear_AllScreen(Black);


//	/* Write success flag and calibration data */
//	__disable_irq();
//	FLASH_Lock();
//
//	EE_WriteVariable(1, TOUCH_FLAG_IS_OK);
//
//	for(i = 0; i < 3; i++)
//	{
//		EE_WriteVariable((i*2) + 2, ScreenSample[i].x);
//		EE_WriteVariable((i*2) + 3, ScreenSample[i].y);
//	}
//
//	FLASH_Unlock();
//	__enable_irq();
}
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static FunctionalState setCalibrationMatrix(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;

  /* K£½(X0£­X2) (Y1£­Y2)£­(X1£­X2) (Y0£­Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A£½((XD0£­XD2) (Y1£­Y2)£­(XD1£­XD2) (Y0£­Y2))£¯K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B£½((X0£­X2) (XD1£­XD2)£­(XD0£­XD2) (X1£­X2))£¯K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C£½(Y0(X2XD1£­X1XD2)+Y1(X0XD2£­X2XD0)+Y2(X1XD0£­X0XD1))£¯K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D£½((YD0£­YD2) (Y1£­Y2)£­(YD1£­YD2) (Y0£­Y2))£¯K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E£½((X0£­X2) (YD1£­YD2)£­(YD0£­YD2) (X1£­X2))£¯K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F£½(Y0(X2YD1£­X1YD2)+Y1(X0YD2£­X2YD0)+Y2(X1YD0£­X0YD1))£¯K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }

  return retTHRESHOLD;
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static FunctionalState getDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr, Matrix * matrixPtr )
{
	FunctionalState retTHRESHOLD = ENABLE ;

	if( matrixPtr->Divider != 0 )
	{
		/* XD = AX+BY+C */
		displayPtr->x = ( (matrixPtr->An * screenPtr->x) + (matrixPtr->Bn * screenPtr->y) + matrixPtr->Cn ) / matrixPtr->Divider ;

		/* YD = DX+EY+F */
		displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + (matrixPtr->En * screenPtr->y) +	matrixPtr->Fn ) / matrixPtr->Divider ;
	}
	else
	{
		retTHRESHOLD = DISABLE;
	}
	return(retTHRESHOLD);
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
static void DrawCross(uint16_t Xpos, uint16_t Ypos)
{
	LCD_DrawLine(Xpos-15, Ypos, Xpos-2, Ypos, White);
	LCD_DrawLine(Xpos+2, Ypos, Xpos+15, Ypos, White);
	LCD_DrawLine(Xpos, Ypos-15, Xpos, Ypos-2, White);
	LCD_DrawLine(Xpos, Ypos+2, Xpos, Ypos+15, White);

	LCD_DrawLine(Xpos-15, Ypos+15, Xpos-7, Ypos+15, White);
	LCD_DrawLine(Xpos-15, Ypos+7, Xpos-15, Ypos+15, White);

	LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15, White);
	LCD_DrawLine(Xpos-15,Ypos-15, Xpos-15, Ypos-7, White);

	LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15, White);
	LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15, White);

	LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15, White);
	LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7, White);
}




/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void TouchScreen_SetState(TouchState NewState)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	if(NewState == Touch_Interrupt)
	{
		/* Config as interrupt input */
		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN);
		delay_ms(50);
		EXTI_ClearITPendingBit(EXTI_Line2);
		EXTI_ClearFlag(EXTI_Line2);
		NVIC_EnableIRQ(EXTI2_IRQn);
	}
	else if(NewState == Touch_AxisX_State)
	{
		/* Configure as analog input -------------------------*/
		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDP_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		/* Config as input */
		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		/* ADC1 regular channel10 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);

		// +3.3 на YD+, 0 на YD- и YD-
		GPIO_SetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDP_PIN);
		GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDN_PIN);
	}
	else if(NewState == Touch_AxisY_State)
	{
		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDP_PIN | TOUCH_GPIO_XDN_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);

		/* ADC1 regular channel11 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);

		// +3.3 на XD+, 0 на XD- и YD-
		GPIO_SetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_XDP_PIN | TOUCH_GPIO_YDN_PIN);
		GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_XDN_PIN);
	}

	TouchMode = NewState;
//	delay_ms(10);
}



/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
void TouchScreen_UpdateState(void)
{
	uint16_t i;
	__IO long ADC_DATA_0 = 0;

	FlagStatus ADC_Correct = SET;
	char dbgStrX[15] = {0};
	char dbgStrY[15] = {0};


//	else
//	{
		/* */
		for(i = 0; i < TOUC_ADC_DATA_CNT; i++)
		{
			ADC_DATA_0 += TouchScreenADCBuffer[i];
		}
		ADC_DATA_0 /= TOUC_ADC_DATA_CNT;

		/* */
		if(TouchMode == Touch_AxisX_State)
		{
			TouchCoordinate.x = ADC_DATA_0;
			TouchScreen_SetState(Touch_AxisY_State);
			TIM_Cmd(TIM3, ENABLE);
		}
		else
		{
			TouchCoordinate.y = ADC_DATA_0;

			/* If calibrate state is OK convert to coordinates */
			if(getDisplayPoint(&display, &TouchCoordinate, &matrix) != DISABLE)
			{
				TouchCoordinate.x = display.x;
				TouchCoordinate.y = display.y;
			}

//            if(GPIO_ReadInputDataBit(TOUCH_GPIO_PORT, TOUCH_GPIO_YDN_PIN) != (uint8_t)Bit_RESET)
//            {
//                TouchScreen_Done = RESET;
                TouchScreen_SetState(Touch_AxisX_State);
                TouchScreen_SetState(Touch_Interrupt);
//                return;
//            }
//            else
//            {
                TouchScreen_Done = SET;
//                TouchScreen_SetState(Touch_AxisX_State);
//            }

			ConvertToString(TouchCoordinate.x, dbgStrX, 6);
			ConvertToString(TouchCoordinate.y, dbgStrY, 6);
			NGL_UpdateLabel(&lblDbgX, dbgStrX);
			NGL_UpdateLabel(&lblDbgY, dbgStrY);
		}


//	}
}


/**
 * @brief  Function_Name
 * @param  None
 * @retval None
 */
int8_t TouchScreen_GetCoordinate(uint16_t* X, uint16_t* Y)
{
    if(TouchScreen_Done == SET)
    {
    	if(X != (void*)0) *X = TouchCoordinate.x;
    	if(Y != (void*)0) *Y = TouchCoordinate.y;
    	return 0;
    }

    return -1;
}


















/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
