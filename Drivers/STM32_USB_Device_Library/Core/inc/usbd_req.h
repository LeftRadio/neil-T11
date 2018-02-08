/**
  ******************************************************************************
  * @file     usbd_req.h
  * @author   Neil Lab :: Left Radio
  * @version  v1.0.0
  * @date
  * @brief    header file for the usbd_req.c file
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_REQUEST_H_
#define __USB_REQUEST_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_def.h"
#include "usb_dcd.h"

/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
USBD_Status USBD_StdDevReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req);
USBD_Status USBD_StdItfReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req);
USBD_Status USBD_StdEPReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req);

void USBD_ParseSetupRequest(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req);
void USBD_CtlError(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req);
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len);

USBD_Status USBD_CtlSendData(USB_OTG_CORE_HANDLE *pdev, uint8_t *buf, uint16_t len);
USBD_Status USBD_CtlContinueSendData(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len);
USBD_Status USBD_CtlPrepareRx(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len);
USBD_Status USBD_CtlContinueRx(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len);
USBD_Status USBD_CtlSendStatus(USB_OTG_CORE_HANDLE *pdev);
USBD_Status USBD_CtlReceiveStatus(USB_OTG_CORE_HANDLE *pdev);


#endif /* __USB_REQUEST_H_ */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/