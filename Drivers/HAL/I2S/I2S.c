/**
  ******************************************************************************
  * @file        I2S.c
  * @author        Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date        2015-01-12-15.07
  * @brief        sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "I2S.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2S_1                               SPI3
#define I2S_1_CLK                           RCC_APB1Periph_SPI3
#define I2S_1_ADDRESS                       0x40003C0C
#define I2S_1_GPIO_AF                       GPIO_AF_SPI3

#define I2S_1_WS_PIN                        GPIO_Pin_15
#define I2S_1_WS_PINSRC                     GPIO_PinSource15
#define I2S_1_WS_GPIO                       GPIOA

#define I2S_1_SCK_PIN                       GPIO_Pin_10
#define I2S_1_SD_PIN                        GPIO_Pin_12
#define I2S_1_SCK_PINSRC                    GPIO_PinSource10
#define I2S_1_SD_PINSRC                     GPIO_PinSource12
#define I2S_1_GPIO                          GPIOC

#define I2S_1_DMA_STREAM                    DMA1_Stream7

#define I2S_1_IRQ                           SPI3_IRQn
#define AUDIO_I2S_IRQHandler                SPI3_IRQHandler
#define AUDIO_I2S_DMA_IRQHandler            DMA1_Stream7_IRQHandler

/* Private macro -------------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Init(I2S_InitTypeDef* initstruct, uint8_t bit_res, uint32_t buff_ptr, uint32_t size);
static void DeInit(void);
static void Start(uint8_t bit_res, uint32_t buff_ptr, uint32_t size);
static void Stop(void);
static void IRQ_Callback(void);

/* Private variables ---------------------------------------------------------*/
T11_HAL_I2S_TypeDef T11_HI2S = {
    I2S_1,
    0,
    Init,
    DeInit,
    Start,
    Stop,
    IRQ_Callback
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Init
  * @param  None
  * @retval None
  */
static void Init(I2S_InitTypeDef* initstruct, uint8_t bit_res, uint32_t buff_ptr, uint32_t size) {

    if( T11_HI2S.configured) {
        return;
    }

    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t periph_data_size, mem_data_size;

    /* Enable the I2S peripheral clock */
    RCC_APB1PeriphClockCmd(I2S_1_CLK, ENABLE);
    RCC_I2SCLKConfig(RCC_I2S2CLKSource_Ext);
    /* Enable the DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* I2S pins configuration: SCK, SD, WS pins ---------------------------*/
    GPIO_InitStructure.GPIO_Pin = I2S_1_SCK_PIN | I2S_1_SD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(I2S_1_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2S_1_WS_PIN ;
    GPIO_Init(I2S_1_WS_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect pins to I2S peripheral ----------------------------------------*/
    GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SCK_PINSRC, I2S_1_GPIO_AF);
    GPIO_PinAFConfig(I2S_1_GPIO, I2S_1_SD_PINSRC, I2S_1_GPIO_AF);
    GPIO_PinAFConfig(I2S_1_WS_GPIO, I2S_1_WS_PINSRC, I2S_1_GPIO_AF);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_SPI1);

    /* I2S configuration -----------------------------------------------------*/
    I2S_Init( I2S_1, initstruct );

    /* DMA configuration -----------------------------------------------------*/
    /* Set new PeripheralDataSize and MemoryDataSize */
    // switch ( aif->settings.bit_resolution[ aif->settings.index ] ) {
    switch ( bit_res ) {
        case 16:
        	periph_data_size = DMA_PeripheralDataSize_HalfWord;
        	mem_data_size = DMA_MemoryDataSize_HalfWord;
        break;
        case 24:
        	periph_data_size = DMA_PeripheralDataSize_Word;
        	mem_data_size = DMA_MemoryDataSize_Word;
        break;
        case 32:
        	periph_data_size = DMA_PeripheralDataSize_Word;
        	mem_data_size = DMA_MemoryDataSize_Word;
        break;
        default:
        	periph_data_size = DMA_PeripheralDataSize_Byte;
        	mem_data_size = DMA_MemoryDataSize_Byte;
        break;
    }
    /* Set the parameters to be configured */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2S_1_ADDRESS;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_Memory0BaseAddr = buff_ptr;
    DMA_InitStructure.DMA_BufferSize = size / (bit_res / 8);
    DMA_InitStructure.DMA_PeripheralDataSize = periph_data_size;
    DMA_InitStructure.DMA_MemoryDataSize = mem_data_size;
    /*  */
    DMA_Init(I2S_1_DMA_STREAM, &DMA_InitStructure);

    /* Enable the selected DMA interrupts */
     DMA_ITConfig(I2S_1_DMA_STREAM, DMA_IT_TC, ENABLE);

     /* I2S DMA IRQ Channel configuration */
     NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

    /* Enable the I2S DMA request */
    SPI_I2S_DMACmd(I2S_1, SPI_I2S_DMAReq_Tx, ENABLE);

    T11_HI2S.configured = 1;
}

/**
  * @brief  DeInit
  * @param  None
  * @retval None
  */
static void DeInit(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Deinitialize the NVIC interrupt for the I2S DMA Stream */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Disable the DMA stream before the deinit */
    I2S_1_DMA_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
    /* Dinitialize the DMA Stream */
    DMA_DeInit(I2S_1_DMA_STREAM);
    /* Disable the I2S peripheral */
    I2S_1->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SE);
    /* Deinitialize the I2S peripheral */
    SPI_I2S_DeInit(I2S_1);

    T11_HI2S.configured = 0;
}

/**
  * @brief  Start
  * @param  None
  * @retval None
  */
static void Start(uint8_t bit_res, uint32_t buff_ptr, uint32_t size) {
    /* If DMA Stream is stopped */
    if((I2S_1_DMA_STREAM->CR & (uint32_t)DMA_SxCR_EN) == 0) {
        /* If the I2S peripheral is still not enabled, enable it */
        I2S_1->I2SCFGR |= SPI_I2SCFGR_I2SE;
        /* Reset DMA memory pointer and transfer size */
        I2S_1_DMA_STREAM->M0AR = buff_ptr;
        I2S_1_DMA_STREAM->NDTR = (uint32_t)(size / (bit_res / 8));
        /* Run the DMA Stream */
        I2S_1_DMA_STREAM->CR |= (uint32_t)DMA_SxCR_EN;
    }
}

/**
  * @brief  Stop
  * @param  None
  * @retval None
  */
static void Stop(void) {
    /* Disable the selected DMAy Streamx by clearing EN bit */
    I2S_1_DMA_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
    /* Clear all the DMA flags for the next transfer */
    DMA_ClearFlag(I2S_1_DMA_STREAM, DMA_FLAG_TCIF7 | DMA_FLAG_HTIF7 | DMA_FLAG_TEIF7 | DMA_FLAG_DMEIF7 | DMA_FLAG_FEIF7);
    /* Disable the I2S peripheral */
    I2S_1->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SE);
}

/**
  * @brief  AUDIO_I2S_DMA_IRQHandler
  * @param  None
  * @retval None
  */
static void IRQ_Callback(void) {
    //    uint32_t WritePoint = (uint32_t)(IsocOutWrPtr - AudioBuffer);
    //    static uint16_t i = 0, j = 0;
    static uint16_t cnt = 0;
    //    uint16_t SOF_Cnt_tmp = TIM2->CNT;

    //    int Wr = (int)WritePoint - (int)AUDIO_BUFFER_HALFSIZE;

        // if (DMA_GetFlagStatus(I2S_1_DMA_STREAM, DMA_FLAG_TCIF7) != RESET)
        // {
    //        while (DMA_GetCmdStatus(I2S_1_DMA_STREAM) != DISABLE)
    //        {}
    //
    ////        I2S_1_DMA_STREAM->CR &= ~(uint32_t)DMA_SxCR_EN;
    //
    //        /* Reset DMA memory pointer and transfer size */
    //        I2S_1_DMA_STREAM->M0AR = (uint32_t)AudioBuffer;
    //        I2S_1_DMA_STREAM->NDTR = (uint32_t)(AUDIO_BUFFER_SIZE_BYTES / 2);
    //
    //        /* Run the DMA Stream */
    //        I2S_1_DMA_STREAM->CR |= (uint32_t)DMA_SxCR_EN;

    //        if(PlayFlag != 0)
    //        {
            if(++cnt >= 10)
            {
                cnt = 0;
                // T11_LED_Toggle(T11_LED_0);
            }
    //        }

    //        if(++i >= 10)        // 1 sec
    //        {
    //            if(j > 0)
    //            {
    //                Frequency_IN = ((float)USB_ReciveAudioPackedCnt / 4) + ((float)Wr);
    //            }
    //            else j = 1;
    //
    //            USB_ReciveAudioPackedCnt = 0;
    //            i = 0;
    //        }
    //
    //        if(Wr == 0)
    //        {
    ////            STM_EVAL_LEDOn(LED3); STM_EVAL_LEDOn(LED6);
    //        }
    //        else if(Wr > 0)
    //        {
    ////            STM_EVAL_LEDOn(LED3); STM_EVAL_LEDOff(LED6);
    //        }
    //        else
    //        {
    ////            STM_EVAL_LEDOn(LED6); STM_EVAL_LEDOff(LED3);
    //        }
    //
    //        if((Wr > AUDIO_BUFFER_HALFSIZE/2) || (Wr < -(AUDIO_BUFFER_HALFSIZE/2)))
    //        {
    ////            STM_EVAL_LEDOn(LED4);
    //        }
    //        else
    //        {
    ////            STM_EVAL_LEDOff(LED4);
    //        }

        //     /* Clear the Interrupt flag */
        //     DMA_ClearFlag(I2S_1_DMA_STREAM, DMA_FLAG_TCIF7);
        // }
}


/*******************************************************************************
      END FILE
*******************************************************************************/
