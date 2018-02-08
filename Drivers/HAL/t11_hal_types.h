/**
  ******************************************************************************
  * @file       hal_types.h
  * @author     Neil Lab :: Left Radio
  * @version    v1.0.0
  * @date
  * @brief      header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_TYPES_H
#define __HAL_TYPES_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef struct _HAL_LEDS {
    GPIO_TypeDef* ports[2];
    const uint16_t pins[2];
    void (*Init)(void);
    void (*DeInit)(void);
    void (*Write)(uint8_t indx, uint8_t state);
    uint8_t (*Read)(uint8_t indx);
    void (*Toggle)(uint8_t indx);
} T11_HAL_LEDS_TypeDef;

typedef struct _HAL_RELAY {
    GPIO_TypeDef* ports[2];
    const uint16_t pins[2];
    void (*Init)(void);
    void (*DeInit)(void);
    void (*Mute)(FunctionalState state);
    void (*Source)(uint8_t source);
} T11_HAL_RELAY_TypeDef;

typedef struct _HAL_BKP_SRAM {
    void (*Init)(void);
    void (*DeInit)(void);
    void (*Write)(uint32_t addr, uint32_t val);
    uint32_t (*Read)(uint32_t addr);
} T11_HAL_BKP_SRAM_TypeDef;

typedef struct _HAL_Xfer {
    uint16_t* buf;
    uint32_t len;
    int32_t indx;
} T11_HAL_Xfer_TypeDef;

typedef struct _HAL_SPI {
    SPI_TypeDef* instance;
    uint8_t configured;
    T11_HAL_Xfer_TypeDef tx;
    T11_HAL_Xfer_TypeDef rx;
    uint8_t tx_busy;
    void (*Init)(void);
    void (*Write)(uint16_t* buf, uint16_t len);
    void (*irq_callback)(void);
} T11_HAL_SPI_TypeDef;

typedef struct _HAL_I2C {
    I2C_TypeDef* instance;
    uint8_t configured;
    void (*Init)(uint32_t speed);
    int (*Write)(uint8_t addr, uint8_t reg, uint8_t val);
    void (*irq_callback)(void);
} T11_HAL_I2C_TypeDef;

typedef struct _HAL_I2S {
    SPI_TypeDef* instance;
    uint8_t configured;
    void (*Init)(I2S_InitTypeDef* initstruct, uint8_t bit_res, uint32_t buff_ptr, uint32_t align_size);
    void (*DeInit)(void);
    void (*Start)(uint8_t bit_res, uint32_t buff_ptr, uint32_t size);
    void (*Stop)(void);
    void (*irq_callback)(void);
} T11_HAL_I2S_TypeDef;

/* */
typedef struct _HAL {
    T11_HAL_LEDS_TypeDef* leds;
    T11_HAL_RELAY_TypeDef* relay;
    T11_HAL_BKP_SRAM_TypeDef* bkpsram;
    T11_HAL_I2C_TypeDef* i2c;
    T11_HAL_SPI_TypeDef* spi;
    T11_HAL_I2S_TypeDef* i2s;

    void (*Init)(void* platform);
    void (*DeInit)(void* platform);
    void (*Reset)(void* platform);
} T11_HAL_TypeDef;

/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/


#endif /* __HAL_TYPES_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
