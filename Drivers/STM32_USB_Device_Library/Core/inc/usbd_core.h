/**
  ******************************************************************************
  * @file     usbd_core.h
  * @author   Neil Lab :: Left Radio
  * @version  v1.0.0
  * @date
  * @brief    header file for the usbd_core.c file
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CORE_H
#define __USBD_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"
#include "usb_core.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void USBD_Init(USB_OTG_CORE_HANDLE *pdev, USB_OTG_CORE_ID_TypeDef coreID, USBD_DEVICE *pDevice, USBD_Class_cb_TypeDef *class_cb, USBD_Usr_cb_TypeDef *usr_cb);

USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pdev);
USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE *pdev, uint8_t cfgidx);
USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE *pdev, uint8_t cfgidx);

uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pdev);
uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , uint8_t epnum);
uint8_t USBD_SOF(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_EOF(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE  *pdev);
uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE  *pdev);


#endif /* __USBD_CORE_H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/