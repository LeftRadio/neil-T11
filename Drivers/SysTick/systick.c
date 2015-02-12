/**
  ******************************************************************************
  * @file	 	temp.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "stm32f10x_systick.h"
#include "core_cm4.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t delay_fac_us = 0;
volatile uint32_t delay_fac_ms = 0;
volatile uint32_t temp = 0;

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : delay_init
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_Configuration(void)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  /* HCLK/8 */
	SysTick_ITConfig(DISABLE);
    delay_fac_us = RCC_ClocksStatus.HCLK_Frequency / 8000000;
    delay_fac_ms = RCC_ClocksStatus.HCLK_Frequency / 8000;
}


/*******************************************************************************
* Function Name  : delay_us
* Description    :
* Input          : us
* Output         : None
* Return         : None
*******************************************************************************/            
void delay_us(u32 nus)
{
    temp = delay_fac_us * nus;
    if (temp > 0x00ffffff) temp = 0x00ffffff;

    SysTick->LOAD = temp;
    SysTick->VAL = SysTick_Counter_Clear;
    SysTick->CTRL |= SysTick_Counter_Enable;

    while((SysTick->CTRL & ((u32)1 << SysTick_FLAG_COUNT)) == (u32)RESET)
    {
    }

    SysTick->CTRL &= SysTick_Counter_Disable;
    SysTick->VAL = SysTick_Counter_Clear;
}


/*******************************************************************************
* Function Name  : delay_ms
* Description    :  
* Input          : nms
* Output         : None
* Return         : None
*******************************************************************************/  
void delay_ms(uint16_t nms)
{
	temp = delay_fac_ms * nms;
    if (temp > 0x00ffffff) temp = 0x00ffffff;

    SysTick->LOAD = temp;
    SysTick->VAL = SysTick_Counter_Clear;
    SysTick->CTRL |= SysTick_Counter_Enable;

    while((SysTick->CTRL & ((u32)1 << SysTick_FLAG_COUNT)) == (u32)RESET)
    {
    }

    SysTick->CTRL &= SysTick_Counter_Disable;
    SysTick->VAL = SysTick_Counter_Clear;
}







/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
