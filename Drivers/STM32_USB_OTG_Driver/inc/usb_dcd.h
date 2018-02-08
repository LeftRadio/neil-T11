/**
  ******************************************************************************
  * @file        usb_dcd.h
  * @author      Neil Lab :: Left Radio
  * @version
  * @date
  * @brief       header file for the usb_dcd.c file.
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DCD_H
#define __USB_DCD_H

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"

/* Exported define -----------------------------------------------------------*/
#define USB_OTG_EP_CONTROL                       0
#define USB_OTG_EP_ISOC                          1
#define USB_OTG_EP_BULK                          2
#define USB_OTG_EP_INT                           3
#define USB_OTG_EP_MASK                          3
/*  Device Status */
#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        (uint8_t)2
#define USB_OTG_CONFIGURED                       (uint8_t)3
#define USB_OTG_SUSPENDED                        4

/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
typedef struct {
  uint8_t  bLength;
  uint8_t  bDescriptorType;
  uint8_t  bEndpointAddress;
  uint8_t  bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t  bInterval;
} EP_DESCRIPTOR, *PEP_DESCRIPTOR;

/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void DCD_Init(USB_OTG_CORE_HANDLE *pdev, USB_OTG_CORE_ID_TypeDef coreID);
void DCD_DevConnect(USB_OTG_CORE_HANDLE *pdev);
void DCD_DevDisconnect(USB_OTG_CORE_HANDLE *pdev);
void DCD_EP_SetAddress(USB_OTG_CORE_HANDLE *pdev, uint8_t address);
uint32_t DCD_EP_Open(USB_OTG_CORE_HANDLE *pdev, uint8_t addr, uint16_t mps, uint8_t type);
uint32_t DCD_EP_Close(USB_OTG_CORE_HANDLE *pdev, uint8_t addr);
uint32_t DCD_EP_PrepareRx(USB_OTG_CORE_HANDLE *pdev, uint8_t addr, uint8_t *pbuf, uint16_t len);
uint32_t DCD_EP_Tx(USB_OTG_CORE_HANDLE *pdev, uint8_t addr, uint8_t *pbuf, uint32_t len);
uint32_t DCD_EP_Stall(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint32_t DCD_EP_ClrStall(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint32_t DCD_EP_Flush(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
uint32_t DCD_Handle_ISR(USB_OTG_CORE_HANDLE *pdev);
uint32_t DCD_GetEPStatus(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum);
void DCD_SetEPStatus(USB_OTG_CORE_HANDLE *pdev, uint8_t epnum, uint32_t status);


#endif /* __USB_DCD_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/