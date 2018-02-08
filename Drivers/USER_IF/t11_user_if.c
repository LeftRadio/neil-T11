/**
  ******************************************************************************
  * @file       t11_user_if.c
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "t11_user_if.h"
#include "systick.h"
#include "platform_types.h"
#include "NGL.h"
#include "pages.h"
#include "touchscreen.h"
#include "peeklevel.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void __T11_LCD_CtrlLinesConfig(void);
static void __T11_LCD_FSMCConfig(void);

static void Init(void* platform);
static void DeInit(void* platform);
static void Update(void* platform);
uint16_t BackLight(uint16_t bl_precent);

/* Private variables ---------------------------------------------------------*/

/* */
T11_USER_IF_TypeDef T11_User_IF = {
    (void*)0,
    &T11_UIF_Touch,
    &T11_UIF_PeekLevel,

    Init,
    DeInit,
    Update,
    BackLight
};

/* Extern function prototypes ------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * @brief  Configures LCD Control lines (FSMC Pins) in alternate function
 * @param  None
 * @retval None
 */
static void __T11_LCD_CtrlLinesConfig(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    /* SRAM Data lines, NOE and NWE configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | \
        GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
        GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    /* perform init port D */
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Set alternate func for port D */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

    /* Set used pins for port E */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | \
        GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | \
        GPIO_Pin_14 | GPIO_Pin_15;
    /* perform init for port E */
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /**< Set alternate func for port E */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

    /* Configurate LCD reset pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configurate LCD BL pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BSRRL = GPIO_Pin_2;

    //   TE Input Pin
    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    //  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    //  GPIO_Init(GPIOD, &GPIO_InitStructure);
    //
    //  LCD_Set_TE_InputPin(GPIOD, GPIO_Pin_6);
}

/**
 * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
 * @param  None
 * @retval None
 */
static void __T11_LCD_FSMCConfig(void) {
    FSMC_NORSRAMInitTypeDef fnosram;
    FSMC_NORSRAMTimingInitTypeDef p;
    /* Enable FSMC clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    /* FSMC_Bank1_NORSRAM4 configuration */
    p.FSMC_AddressSetupTime = 0x2;
    p.FSMC_AddressHoldTime = 0;
    p.FSMC_DataSetupTime = 0x2;
    p.FSMC_BusTurnAroundDuration = 0;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    p.FSMC_AccessMode = FSMC_AccessMode_A;
    /* ------------------------------------
       LCD configured as follow:
          - Data/Address MUX = Disable
          - Memory Type = SRAM
          - Data Width = 16bit
          - Write Operation = Enable
          - Extended Mode = Disable
          - Asynchronous Wait = Disable */
    fnosram.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    fnosram.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    fnosram.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    fnosram.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    fnosram.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    fnosram.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    fnosram.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    fnosram.FSMC_WrapMode = FSMC_WrapMode_Disable;
    fnosram.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    fnosram.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    fnosram.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    fnosram.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    fnosram.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    fnosram.FSMC_ReadWriteTimingStruct = &p;
    fnosram.FSMC_WriteTimingStruct = &p;
    FSMC_NORSRAMInit(&fnosram);
    /* Enable FSMC NOR/SRAM Bank3 */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/**
 * @brief  Configure the LCD Control pins and FSMC Parallel interface
 * @param  None
 * @retval None
 */
static void Init(void* platform) {
    /* */
    T11_USER_IF_TypeDef* uif = ((T11_Platform_TypeDef*)platform)->user_if;
    /* Configure the LCD Control pins */
    __T11_LCD_CtrlLinesConfig();
    /* Configure the FSMC Parallel interface */
    __T11_LCD_FSMCConfig();
    /* Reset LCD */
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    delay_ms(50);
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    delay_ms(50);
    /* Initialization LCD Controller */
    uif->lcd_struct = &R61581;
    uif->lcd_struct->DataBusBits = 16;
    uif->lcd_struct->ColorBits = 16;
    uif->lcd_struct->ConnectionType = LCD_FSMC_Connect;
    /* Init LCD controller,
       Set delay function (for internal LCD library use only) */
    if( NGL_HAL_Init( uif->lcd_struct, delay_ms ) ) {
        /* Error */
        while(1) {
            // T11_LED_Toggle(T11_LED_0);
            delay_ms(25);
        }
    }
    /* Set rotation, clear screen, set backlight */
    NGL_LCD_SetRotation( _90_degree );
    // LCD_Set_BackLightPWM(90);
    // GPIO_SetBits(GPIOA, GPIO_Pin_0);

    /*  */
    NGL_Touch_InitFoops( uif->touchscreen );
    uif->touchscreen->foops->configuration();

    /* Select ad draw MainPage */
    NGL_GUI_SelectPage( &MainPage );
    NGL_GUI_GetSelectedPage()->Draw();

    /*  */
    uif->peeklevel->Init( platform );
}

/**
 * @brief  DeInit
 * @param  None
 * @retval None
 */
static void DeInit(void* platform) {
}

/**
 * @brief  Update
 * @param  None
 * @retval None
 */
static void Update(void* platform) {
    /* */
	T11_Platform_TypeDef* plt = (T11_Platform_TypeDef*)platform;
    /* */
    plt->user_if->peeklevel->Update( platform );
    /* */
    plt->user_if->touchscreen->foops->do_touch();

    if(plt->audio_if->dac->i2s_tx_complite != RESET) {
    	NGL_FillBar* audio_buff_bar = (NGL_FillBar*)MainPage.Objects.fillbars[1];
    	audio_buff_bar->Level = plt->audio_if->stream->filled_by;
    	NGL_GUI_FillBarUpdate( audio_buff_bar );
    }
    // if(T11_LED_Read(T11_LED_0) != 0) {
    //     lblUSBstream.Color = 0x87F0;
    // }
    // else {
    //     lblUSBstream.Color = MainPage.BackColor;
    // }

    // NGL_DrawLabel(&lblUSBstream);
}

/**
 * @brief  BackLight
 * @param  None
 * @retval None
 */
uint16_t BackLight(uint16_t bl_precent) {
    static uint16_t gbl_precent = 0;
    //	uint32_t PWM_Val = (TIM5->ARR * BL_Precent) / 100;
    //	TIM_SetCompare1(TIM5, (uint16_t)PWM_Val);
    //	gbl_precent = BL_Precent;
    return gbl_precent;
}


/*******************************************************************************
      END FILE
*******************************************************************************/
