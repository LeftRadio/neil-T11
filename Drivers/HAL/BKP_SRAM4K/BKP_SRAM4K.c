/**
  ******************************************************************************
  * @file        BKP_SRAM4K.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief       sourse for BKP_SRAM4K driver
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "BKP_SRAM4K.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void DeInit(void);
static void Write(uint32_t addr, uint32_t val);
static uint32_t Read(uint32_t addr);

/* Private variables ---------------------------------------------------------*/
T11_HAL_BKP_SRAM_TypeDef T11_HBKPSRAM = {
    Init,
    DeInit,
    Write,
    Read
};

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  T11_BKPSRAM_Init
  * @param  None
  * @retval None
  */
static void Init(void) {
    __disable_irq();
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    PWR_BackupRegulatorCmd(ENABLE);
    while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET) { ; }
    __enable_irq();
}

/**
  * @brief  T11_BKPSRAM_DeInit
  * @param  None
  * @retval None
  */
static void DeInit(void) {
}

/**
  * @brief  T11_BKPSRAM_Write
  * @param  None
  * @retval None
  */
static void Write(uint32_t addr, uint32_t val) {
    *(volatile uint32_t *)(BKPSRAM_BASE + (addr * 4)) = val;
}

/**
  * @brief  T11_BKPSRAM_Init
  * @param  None
  * @retval None
  */
static uint32_t Read(uint32_t addr) {
    uint32_t val = *(volatile uint32_t *)(BKPSRAM_BASE + (addr * 4));
    return val;
}


/*******************************************************************************
      END FILE
*******************************************************************************/