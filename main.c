/**
  ******************************************************************************
  * @file    main.c
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "arm_math.h"
#include "math_helper.h"

#include "Audio_Interface.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "Global_Init.h"
#include "LCD.h"
#include "PeekLevel.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef USE_USB_OTG_HS
    #define _CORE_ID      USB_OTG_HS_CORE_ID
#else
    #define _CORE_ID      USB_OTG_FS_CORE_ID
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN __IO USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    SystemInit();
    Global_peripheal_Configuration();
    USBD_Init((USB_OTG_CORE_HANDLE*)&USB_OTG_dev, _CORE_ID, &USR_desc, &AUDIO_cb, &USR_cb);

    T11_LCD_Configuration(Black);
    TouchScreen_Configuration(RESET);
 	MainPage.Draw();

    /* Indicate OK ON state */
    T11_LED_ON(T11_LED_1);


    /* Main cycle */
    while (1)
    {
   		MainPage_Render();
    }
}





/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    :
* Input          :
* Return         :
*******************************************************************************/
void TIM3_IRQHandler(void)
{
//	uint8_t bufs[3];
//	float Fs_Fb = 192.000;
//	uint16_t val_int = (uint16_t)(Fs_Fb);
//	uint16_t val_frac = (uint16_t)((Fs_Fb - val_int) * 1000);

//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//
//		/* LED4 toggling */
//		STM_EVAL_LEDToggle(LED3);
//
//		bufs[0] = (uint8_t)((val_int & 0x3FF) >> 2);
//		bufs[1] = (uint8_t)((val_int & 0x03) << 6) | (uint8_t)((val_frac & 0x3FF) >> 4);
//		bufs[2] = (uint8_t)(val_frac & 0x0F) << 4;
//
//		DCD_EP_Tx (&USB_OTG_dev, AUDIO_IN_EP, bufs, 3);
//	}
}



/*******************************************************************************
* Function Name  : int_to_char
* Description    :
* Input          :
* Return         :
*******************************************************************************/
static __inline char int_to_char(uint8_t val)
{
	return (48 + val);
}


/*******************************************************************************
* Function Name  : ConvertToString
* Description    :
* Input          : None
* Return         : None
*******************************************************************************/
void ConvertToString(uint32_t Num, char* Str, uint8_t NumSymbol)
{
	int32_t i, j, Rate;
	uint8_t A[NumSymbol];
	uint32_t NumPow = 1;

	for(i = 0; i < NumSymbol+1; i++) NumPow *= 10;
	if(Num > NumPow - 1) Num = NumPow - 1;

	for(i = NumSymbol - 1; i >= 0; i--)
	{
		A[i] = 0; Rate = 1;

		for(j = 0; j < i; j++) Rate *= 10;
		while(Num >= Rate){ Num = Num - Rate; A[i]++; }

		(*Str) = int_to_char(A[i]);
		Str++;
	}
}


/*******************************************************************************
* Function Name  :
* Description    :
* Input          : value - value for convert
* Return         : None
* Output		 : *string
*******************************************************************************/
void ConvertFloatToString(char *ValText, float Value, uint8_t Length)
{
	int d1, d2;     // integer & fractional parts
	float f2;     	// fractional part
	uint8_t i = Length;

	/* --- Convert to string, fixed 5 digit with float point position --- */
	if(Value < 0) Value *= -1;

	if(Value > 1e4){ Value = 99999; i = 0; }
	else if(Value < 1) i = Length - 1;
	else
	{
		f2 = Value;
		while(f2 >= 1)
		{
			f2 /= 10;
			i--;
		}
	}

	d1 = Value;		// Get the integer part

	if(i != 0)
	{
		f2 = Value - d1;     					// Get fractional part
		d2 = trunc(f2 * pow10f(i));		// Turn into integer

		ConvertToString(d1, ValText, Length - i);
		ValText[strlen(ValText)] = '.';
		ConvertToString(d2, &ValText[strlen(ValText)], i);
	}
	else ConvertToString((uint32_t)Value, ValText, Length);

	ValText[strlen(ValText)] = 0;
}






/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
