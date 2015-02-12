/*************************************************************************************
*
Description :  AutoLogic R61581 LCD Sourse
Version     :  1.0.0
Date        :
Author      :  Left Radio
Comments:
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "LCD.h"
#include "Global_Init.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO LCD_Typedef *LCD_Struct;

volatile uint32_t BL_AutoOff_Seconds = 0;
uint8_t BL_OFF_ticks = 0;
uint8_t BL_Precent = 80;
uint8_t FAN_Precent = 80;
uint8_t gBL_Precent = 0;


/* Private function prototypes -----------------------------------------------*/
static void LCD_CtrlLinesConfig(void);
static void LCD_FSMCConfig(void);

/* Extern function prototypes ------------------------------------------------*/
extern void Delay(volatile uint32_t nTime);



/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void T11_LCD_Configuration(uint16_t init_color)
{
	/* set delay_ms as delay function for internal LCD library use */
	LCD_DelayInit(delay_ms);

    /* Configure the LCD Control pins */
    LCD_CtrlLinesConfig();

    /* Configure the FSMC Parallel interface -------------------------------------*/
    LCD_FSMCConfig();

    /* Reset LCD -----------------------------------------------------------------*/
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);
    delay_ms(50);
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    delay_ms(50);

    /* Initialization LCD Controller ---------------------------------------------*/
    LCD_Struct = LCD_Get_ControllerType(__R61581__);
    LCD_Struct->DataBusBits = 16;
    LCD_Struct->ColorBits = 16;
    LCD_Struct->ConnectionType = LCD_FSMC_Connect;
    LCD_Set_ControllerType((LCD_Typedef*)LCD_Struct);

    LCD_ControllerInitialization();

    /**< set rotation, clear screen, set backlight */
    LCD_SetRotation(_90_degree);
    LCD_Clear_AllScreen(init_color);

    //LCD_Set_BackLightPWM(90);
//	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}


/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_CtrlLinesConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-- GPIO Configuration ------------------------------------------------------*/
    /* SRAM Data lines,  NOE and NWE configuration */

    /**< Set common modes for all pins */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

    /**< Set used pins for port D */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_7 | \
                                  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_14 | \
                                  GPIO_Pin_15;

    /**< perform init port D */
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /**< Set alternate func for port D */
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

    /**< Set used pins for port E */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;

    /**< perform init for port E */
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


    /**< Configurate LCD reset pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /**< Configurate LCD BL pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BSRRL = GPIO_Pin_2;

//	 TE Input Pin
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//	LCD_Set_TE_InputPin(GPIOD, GPIO_Pin_6);
}


/*******************************************************************************
* Function Name  : LCD_FSMCConfig
* Description    : Configures the Parallel interface (FSMC) for LCD(Parallel mode)
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    /* Enable FSMC clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

    /*-- FSMC Configuration ------------------------------------------------------*/
    /*----------------------- SRAM Bank 3 ----------------------------------------*/
    /* FSMC_Bank1_NORSRAM4 configuration */
    p.FSMC_AddressSetupTime = 0x2;
    p.FSMC_AddressHoldTime = 0;
    p.FSMC_DataSetupTime = 0x2;
    p.FSMC_BusTurnAroundDuration = 0;
    p.FSMC_CLKDivision = 0;
    p.FSMC_DataLatency = 0;
    p.FSMC_AccessMode = FSMC_AccessMode_A;
    /* Color LCD configuration ------------------------------------
       LCD configured as follow:
          - Data/Address MUX = Disable
          - Memory Type = SRAM
          - Data Width = 16bit
          - Write Operation = Enable
          - Extended Mode = Enable
          - Asynchronous Wait = Disable */

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /* Enable FSMC NOR/SRAM Bank3 */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}


/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void T11_LCD_Set_BackLightPWM(uint8_t BL_Precent)
{
//	uint32_t PWM_Val = (TIM5->ARR * BL_Precent) / 100;
//	TIM_SetCompare1(TIM5, (uint16_t)PWM_Val);
//	gBL_Precent = BL_Precent;
}


/*******************************************************************************
* Function Name  : LCD_Configuration
* Description    : Configure the LCD Control pins and FSMC Parallel interface
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t T11_LCD_Get_BackLightPWM(void)
{
    return gBL_Precent;
}






/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

