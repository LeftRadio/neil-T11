/**
  ******************************************************************************
  * @file        toucscreen.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "touchscreen.h"
#include "platform.h"
#include "systick.h"
#include "NGL.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TOUCH_FLAG_IS_OK                            ((uint32_t)0xAA88AA88)

#define TOUCH_GPIO_PORT                             GPIOA
#define TOUCH_GPIO_XDP_PIN                          GPIO_Pin_0
#define TOUCH_GPIO_XDN_PIN                          GPIO_Pin_2
#define TOUCH_GPIO_YDP_PIN                          GPIO_Pin_1
#define TOUCH_GPIO_YDN_PIN                          GPIO_Pin_6

#define ADC3_DR_ADDRESS                             ((uint32_t)0x4001224C)

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t _HAL_InitCallback(void);
static int8_t _HAL_SetStateCallback(NGL_TouchState NewState);
static int8_t _HAL_ReadSamplesCallback(Coordinate *samples, uint8_t len);
static int8_t _HAL_SaveSamplesCallback(Coordinate* samples, uint8_t len);
static int8_t _HAL_NoTouchCallback(void);
static void _eventCallback(Coordinate point, NGL_TouchType type);

/* Private variables ---------------------------------------------------------*/

/* Touch screen HAL and event callbacks */
NGL_TouchScreen_Callbacks T11_TouchHAL_CallBacks = {
    _HAL_InitCallback,
    _HAL_SetStateCallback,
    _HAL_ReadSamplesCallback,
    _HAL_SaveSamplesCallback,
    _HAL_NoTouchCallback,
    _eventCallback,
};

/* Touch screen struct */
NGL_TouchScreen T11_UIF_Touch = {
    Touch_ADC_Interface,                   // hardware interface
    {0},                                   // matrix
    { { 45, 45 }, {45, 270}, {430,160} },  // calibrate samples coordinates
    {0, 0},                                // last touch point data
    Touch_Default,                         // touch state
    10,                                    // repeat delay
    &T11_TouchHAL_CallBacks,                    // HAL callbacks
    (void*)0                               // foops, init in NGL_Touch_InitFoops
};

/* Exported function ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  _HAL_InitCallback
 * @param  None
 * @retval None
 */
static int8_t _HAL_InitCallback(void) {
    //--------------------------------------------------------------------------
    ADC_InitTypeDef ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

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

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_144Cycles);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    //--------------------------------------------------------------------------
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    //
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 625 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = 10 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);

    /* Connect EXTI Line0 to PA2 pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);

    /* Configure EXTI Line0 */
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*  */
    NVIC_EnableIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, 12);
    NVIC_SetPriority(EXTI2_IRQn, 13);

    return 0;
}

/**
 * @brief  _HAL_SetStateCallback
 * @param  None
 * @retval None
 */
static int8_t _HAL_SetStateCallback(NGL_TouchState NewState) {
    GPIO_InitTypeDef GPIO_InitStructure;

    if( (NewState == Touch_Default) || (NewState == Touch_Interrupt) || \
        (NewState == Touch_Polling) || (NewState == Touch_Stop) ) {

        /* stop ADC */
        TIM3->CR1 &= ~TIM_CR1_CEN;
        /* */
        if(NewState == Touch_Stop) {
            return 0;
        }
        /* Config as interrupt input */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDN_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* Y */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* */
        GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN);
        delay_us(150);
        /* Clear and enable interrupt input if not polling */
        if(NewState != Touch_Polling) {
            EXTI_ClearITPendingBit(EXTI_Line2);
            EXTI_ClearFlag(EXTI_Line2);
            NVIC_EnableIRQ(EXTI2_IRQn);
        }
    }
    else if(NewState == Touch_AxisX_State) {
        /* */
        NVIC_DisableIRQ(EXTI2_IRQn);
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
        /* Config as output */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN | TOUCH_GPIO_YDN_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* ADC1 regular channel10 configuration */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles);
        /* */
        GPIO_SetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDP_PIN);
        GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_YDN_PIN);
        /* start ADC */
        TIM3->CR1 |= TIM_CR1_CEN;
    }
    else if(NewState == Touch_AxisY_State) {
        /* */
        NVIC_DisableIRQ(EXTI2_IRQn);
        /* */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDP_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_YDN_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* */
        GPIO_InitStructure.GPIO_Pin = TOUCH_GPIO_XDP_PIN | TOUCH_GPIO_XDN_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(TOUCH_GPIO_PORT, &GPIO_InitStructure);
        /* ADC1 regular channel11 configuration */
        ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_15Cycles);
        /* */
        GPIO_SetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_XDN_PIN | TOUCH_GPIO_YDN_PIN);
        GPIO_ResetBits(TOUCH_GPIO_PORT, TOUCH_GPIO_XDP_PIN);
        /* start ADC */
        TIM3->CR1 |= TIM_CR1_CEN;
    }

    return 0;
}

/**
 * @brief  _HAL_ReadSamplesCallback
 * @param  None
 * @retval None
 */
static int8_t _HAL_ReadSamplesCallback(Coordinate* samples, uint8_t len) {
    uint32_t data_32, i;
    /* Read touchpanel calibration flag from bkp_sram */
    if(T11_Platform.hal->bkpsram->Read(0x00) != TOUCH_FLAG_IS_OK) {
        return -1;
    }
    /* Read touchpanel calibration samples */
    for(i = 0; i < 3; i++) {
        data_32 = T11_Platform.hal->bkpsram->Read(0x01 + i);
        samples->x = (uint16_t)(data_32 >> 16);
        samples->y = (uint16_t)(data_32 & 0x0000FFFF);
        samples++;
    }
    return 0;
}

/**
 * @brief  _HAL_ReadSamplesCallback
 * @param  None
 * @retval None
 */
static int8_t _HAL_SaveSamplesCallback(Coordinate* samples, uint8_t len) {
    uint32_t data_32, i;
    /* Write touchpanel calibration flag */
    T11_Platform.hal->bkpsram->Write(0x00, TOUCH_FLAG_IS_OK);
    /* Write touchpanel calibration samples */
    for(i = 0; i < len; i++) {
        data_32 = ((uint32_t)samples[i].x << 16) | (uint32_t)samples[i].y;
        T11_Platform.hal->bkpsram->Write(0x01 + i, data_32);
    }
    return 0;
}

/**
 * @brief  _HAL_NoTouchCallback
 * @param  None
 * @retval None
 */
static int8_t _HAL_NoTouchCallback(void) {
    if( GPIO_ReadInputDataBit(TOUCH_GPIO_PORT, TOUCH_GPIO_XDN_PIN) == Bit_SET) {
        return 0;
    }
    return -1;
}

/**
 * @brief  _eventCallback
 * @param  None
 * @retval None
 */
static void _eventCallback(Coordinate point, NGL_TouchType type) {
}


/*******************************************************************************
      END FILE
*******************************************************************************/