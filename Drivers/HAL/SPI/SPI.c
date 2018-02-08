/**
  ******************************************************************************
  * @file         SPI.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief        sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "SPI.h"
// #include "platform.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// #define SPI_NSS_PORT                        GPIOA
// #define SPI_NSS_PIN                         GPIO_Pin_4
// #define SPI_NSS_SET_HIGHT()                 SPI_NSS_PORT->BSRRL = SPI_NSS_PIN
// #define SPI_NSS_SET_LOW()                   SPI_NSS_PORT->BSRRH = SPI_NSS_PIN

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(void);
static void Write(uint16_t* buf, uint16_t len);
static void IRQ_Callback(void);

/* Private variables ---------------------------------------------------------*/
T11_HAL_SPI_TypeDef T11_HSPI = {
    SPI1,
    0,
    { (void*)0, 0, 0 },
    { (void*)0, 0, 0 },
    0,
    Init,
    Write,
    IRQ_Callback
};

/* Extern function -----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  T11_SPI_Init
  * @param  None
  * @retval None
  */
static void Init(void) {

    SPI_InitTypeDef init_struct;
    GPIO_InitTypeDef gpio_initstruct;

    if( T11_HSPI.configured) {
        return;
    }

    /* Enable SPI peripheral clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* Configure pins used by T11_HSPI.instance */
    gpio_initstruct.GPIO_Pin = GPIO_Pin_7;
    gpio_initstruct.GPIO_Mode = GPIO_Mode_AF;
    gpio_initstruct.GPIO_OType = GPIO_OType_PP;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_25MHz;
    gpio_initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &gpio_initstruct);

    gpio_initstruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_Init(GPIOB, &gpio_initstruct);

    /* Connect T11_HSPI.instance pins to SPI alternate function */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    /* Initialization parameters for control SPI interface */
    init_struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    init_struct.SPI_Mode = SPI_Mode_Master;
    init_struct.SPI_DataSize = SPI_DataSize_16b;
    init_struct.SPI_CPOL = SPI_CPOL_High;
    init_struct.SPI_CPHA = SPI_CPHA_2Edge;
    init_struct.SPI_NSS = SPI_NSS_Soft;
    init_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
    init_struct.SPI_FirstBit = SPI_FirstBit_MSB;

    /* Init T11_HSPI.instance */
    SPI_Init(T11_HSPI.instance, &init_struct);
    /* Chip Select - SOFTWARE */
    SPI_NSSInternalSoftwareConfig(T11_HSPI.instance, SPI_NSSInternalSoft_Set);
    /* Enable T11_HSPI.instance */
    SPI_Cmd(T11_HSPI.instance, ENABLE);

    // NVIC_EnableIRQ(SPI1_IRQn);
    // NVIC_SetPriority(SPI1_IRQn, 10);

    T11_HSPI.configured = 1;
}

/**
  * @brief  T11_SPI_WriteWordBuf
  * @param  None
  * @retval None
  */
static void Write(uint16_t* buf, uint16_t len) {
	/* */
	uint16_t i = len;
	// while (T11_HSPI.tx_busy) {};
	// T11_HSPI.tx_busy = 1;
	/* */
    // T11_HSPI.tx.buf = buf;
    // T11_HSPI.tx.indx = 0;
    // T11_HSPI.tx.len = len;
    /* Enable the Tx buffer empty interrupt */
    // SPI_I2S_ITConfig(T11_HSPI.instance, SPI_I2S_IT_TXE, ENABLE);

    do {
        SPI1->DR = buf[ i - len ];
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != RESET);
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) != RESET);
    } while(--len);
}

/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
static void IRQ_Callback(void) {
    /* Receiver mode */
    if (SPI_I2S_GetITStatus(T11_HSPI.instance, SPI_I2S_IT_RXNE) == SET) {
        if (T11_HSPI.rx.indx < T11_HSPI.rx.len) {
            T11_HSPI.rx.buf[ T11_HSPI.rx.indx++ ] = SPI_I2S_ReceiveData(T11_HSPI.instance);
        }
        else {
            SPI_I2S_ITConfig(T11_HSPI.instance, SPI_I2S_IT_RXNE, DISABLE);
        }
    }
    /* Tramitter mode */
    if (SPI_I2S_GetITStatus(T11_HSPI.instance, SPI_I2S_IT_TXE) == SET) {
        if (T11_HSPI.tx.indx < T11_HSPI.tx.len) {
            T11_HSPI.instance->DR = T11_HSPI.tx.buf[ T11_HSPI.tx.indx++ ];
        }
        else {
           SPI_I2S_ITConfig(T11_HSPI.instance, SPI_I2S_IT_TXE, DISABLE);
           T11_HSPI.tx_busy = 0;
        }
    }
}


/*******************************************************************************
      END FILE
*******************************************************************************/
