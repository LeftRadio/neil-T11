/**
  ******************************************************************************
  * @file     usbd_usr.c
  * @author   Neil Lab :: Left Radio
  * @version  v1.0.0
  * @date
  * @brief    sourse
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "usbd_usr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void USBD_USR_Init(void);
static void USBD_USR_DeviceReset (uint8_t speed);
static void USBD_USR_DeviceConfigured (void);
static void USBD_USR_DeviceSuspended(void);
static void USBD_USR_DeviceResumed(void);
static void USBD_USR_DeviceConnected(void);
static void USBD_USR_DeviceDisconnected(void);

// static void USBD_USR_FS_Init(void);
// static void USBD_USR_FS_DeviceReset (uint8_t speed);
// static void USBD_USR_FS_DeviceConfigured (void);
// static void USBD_USR_FS_DeviceSuspended(void);
// static void USBD_USR_FS_DeviceResumed(void);
// static void USBD_USR_FS_DeviceConnected(void);
// static void USBD_USR_FS_DeviceDisconnected(void);

// static void USBD_USR_HS_Init(void);
// static void USBD_USR_HS_DeviceReset (uint8_t speed);
// static void USBD_USR_HS_DeviceConfigured (void);
// static void USBD_USR_HS_DeviceSuspended(void);
// static void USBD_USR_HS_DeviceResumed(void);
// static void USBD_USR_HS_DeviceConnected(void);
// static void USBD_USR_HS_DeviceDisconnected(void);

/* Private variables ---------------------------------------------------------*/
USBD_Usr_cb_TypeDef USR_cb = {
    USBD_USR_Init,
    USBD_USR_DeviceReset,
    USBD_USR_DeviceConfigured,
    USBD_USR_DeviceSuspended,
    USBD_USR_DeviceResumed,
    USBD_USR_DeviceConnected,
    USBD_USR_DeviceDisconnected
};

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Device Init Event
* @param  None
* @retval None
*/
static void USBD_USR_Init(void) {
}

/**
* @brief  Device Reset Event
* @param  speed : device speed
* @retval None
*/
static void USBD_USR_DeviceReset(uint8_t speed) {
}

/**
* @brief  Device configuration Event
* @param  None
* @retval Staus
*/
static void USBD_USR_DeviceConfigured(void) {
}

/**
* @brief  device connection Event
* @param  None
* @retval Staus
*/
static void USBD_USR_DeviceConnected(void) {
}

/**
* @brief  Device disconnection Event
* @param  None
* @retval Staus
*/
static void USBD_USR_DeviceDisconnected(void) {
}

/**
* @brief  Device suspend Event
* @param  None
* @retval None
*/
static void USBD_USR_DeviceSuspended(void) {
}

/**
* @brief  Device resume Event
* @param  None
* @retval None
*/
static void USBD_USR_DeviceResumed(void) {
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/





























