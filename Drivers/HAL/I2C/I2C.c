/**
  ******************************************************************************
  * @file         I2C.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief        sourse for I2C driver
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "I2C.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FLAG_TIMEOUT                        ((long)0x1000)
#define LONG_TIMEOUT                        ((long)(300 * FLAG_TIMEOUT))

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void T11_I2C_Init(uint32_t speed);
static int T11_I2C_Write(uint8_t addr, uint8_t reg, uint8_t val);
static void T11_I2C_IRQ_Callback(void);

/* Private variables ---------------------------------------------------------*/
T11_HAL_I2C_TypeDef T11_HI2C = {
    I2C1,
    0,
    T11_I2C_Init,
    T11_I2C_Write,
    T11_I2C_IRQ_Callback
};

/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  T11_I2C_Init
  * @param  None
  * @retval None
  */
static void T11_I2C_Init(uint32_t speed) {
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    /* I2C SCL and SDA pins configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Connect pins to I2C peripheral */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
    /* Enable the I2C peripheral clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    /* peripheral configuration */
    I2C_DeInit( T11_HI2C.instance );
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x33;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = speed;
    I2C_Init( T11_HI2C.instance, &I2C_InitStructure );
    /* Enable the I2C peripheral */
    I2C_Cmd( T11_HI2C.instance, ENABLE);
}

/**
  * @brief  Writes a Byte to a given register
  * @param  I2C_I2C_Addr
  * @param  reg: The I2C_Address (location) of the register to be written.
  * @param  val: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
int T11_I2C_Write(uint8_t addr, uint8_t reg, uint8_t val) {
    /*  */
    __IO long Timeout = LONG_TIMEOUT;
    /*!< While the bus is busy */
    while(I2C_GetFlagStatus(T11_HI2C.instance, I2C_FLAG_BUSY)) {
        if((Timeout--) <= 0) return -1;
    }
    /* Start the config sequence */
    I2C_GenerateSTART( T11_HI2C.instance, ENABLE );
    /* Test on EV5 and clear it */
    Timeout = FLAG_TIMEOUT;
    while (!I2C_CheckEvent(T11_HI2C.instance, I2C_EVENT_MASTER_MODE_SELECT)) {
        if((Timeout--) <= 0) return -1;
    }
    /* Transmit the slave I2C_Address and enable writing operation */
    I2C_Send7bitAddress(T11_HI2C.instance, addr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    Timeout = FLAG_TIMEOUT;
    while (!I2C_CheckEvent(T11_HI2C.instance, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if((Timeout--) <= 0) return -1;
    }
    /* Transmit the register for write operation */
    I2C_SendData(T11_HI2C.instance, reg);
    /* Test on EV8 and clear it */
    Timeout = FLAG_TIMEOUT;
    while (!I2C_CheckEvent(T11_HI2C.instance, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
        if((Timeout--) <= 0) return -1;
    }
    /* Prepare the register value to be sent */
    I2C_SendData(T11_HI2C.instance, val);
    /*!< Wait till all data have been physically transferred on the bus */
    Timeout = LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(T11_HI2C.instance, I2C_FLAG_BTF)) {
        if((Timeout--) <= 0) return -1;
    }
    /* End the configuration sequence */
    I2C_GenerateSTOP(T11_HI2C.instance, ENABLE);

    #ifdef VERIFY_WRITTENDATA
    /* Verify that the data has been correctly written */
    result = (I2C_Read(addr, reg) == val)? 0:1;
    #endif /* VERIFY_WRITTENDATA */

    /* Return the verifying value: 0 (Passed) */
    return 0;
}

/**
  * @brief  AUDIO_I2S_DMA_IRQHandler
  * @param  None
  * @retval None
  */
static void T11_I2C_IRQ_Callback(void) {
}


/*******************************************************************************
      END FILE
*******************************************************************************/