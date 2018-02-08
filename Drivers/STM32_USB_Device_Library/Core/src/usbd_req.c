/**
  ******************************************************************************
  * @file     usbd_req.c
  * @author
  * @version  V1.0.0
  * @date
  * @brief    This file provides the standard USB requests following chapter 9.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "usbd_req.h"
#include "usbd_core.h"
#include "usbd_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN uint32_t USBD_ep_status __ALIGN_END  = 0;
__ALIGN_BEGIN uint32_t USBD_default_cfg __ALIGN_END  = 0;
__ALIGN_BEGIN uint32_t USBD_cfg_status __ALIGN_END  = 0;
__ALIGN_BEGIN uint8_t USBD_StrDesc[USB_MAX_STR_DESC_SIZ] __ALIGN_END ;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Return the string length
  * @param  buf : pointer to the ascii string buffer
  * @retval string length
  */
static __inline uint16_t USBD_GetLen(uint8_t *buf) {
    uint16_t len = 0;

    while(*buf != NULL) {
        len++;
        buf++;
    }
    return len;
}

/**
  * @brief  Handle Get Descriptor requests
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_GetDescriptor(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
  uint16_t len;
  uint8_t *pbuf;

  switch (req->wValue >> 8) {

    case USB_DESC_TYPE_DEVICE:
      pbuf = pdev->dev.usr_device->GetDeviceDescriptor(pdev->cfg.speed, &len);
      if ((req->wLength == 64) || (pdev->dev.device_status == USB_OTG_DEFAULT)) {
        len = 8;
      }
    break;

    case USB_DESC_TYPE_CONFIGURATION:
      #ifdef USB_OTG_HS_CORE
      if((pdev->cfg.speed == USB_OTG_SPEED_FULL) && (pdev->cfg.phy_itface == USB_OTG_ULPI_PHY)) {
        pbuf = (uint8_t*)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
      }
      else {
        pbuf = (uint8_t*)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);
      }
      #endif
      pbuf[1] = USB_DESC_TYPE_CONFIGURATION;
      pdev->dev.pConfig_descriptor = pbuf;
    break;

    case USB_DESC_TYPE_STRING:
      switch ((uint8_t)(req->wValue))
      {
        case USBD_IDX_LANGID_STR:
          pbuf = pdev->dev.usr_device->GetLangIDStrDescriptor(pdev->cfg.speed, &len);
        break;

        case USBD_IDX_MFC_STR:
          pbuf = pdev->dev.usr_device->GetManufacturerStrDescriptor(pdev->cfg.speed, &len);
        break;

        case USBD_IDX_PRODUCT_STR:
          pbuf = pdev->dev.usr_device->GetProductStrDescriptor(pdev->cfg.speed, &len);
        break;

        case USBD_IDX_SERIAL_STR:
          pbuf = pdev->dev.usr_device->GetSerialStrDescriptor(pdev->cfg.speed, &len);
        break;

        case USBD_IDX_CONFIG_STR:
          pbuf = pdev->dev.usr_device->GetConfigurationStrDescriptor(pdev->cfg.speed, &len);
        break;

        case USBD_IDX_INTERFACE_STR:
          pbuf = pdev->dev.usr_device->GetInterfaceStrDescriptor(pdev->cfg.speed, &len);
        break;

        default:
          #ifdef USB_SUPPORT_USER_STRING_DESC
          pbuf = pdev->dev.class_cb->GetUsrStrDescriptor(pdev->cfg.speed, (req->wValue) , &len);
          break;
          #else
          USBD_CtlError(pdev , req);
          return;
          #endif /* USBD_CtlError(pdev , req); */
        }
        break;

    case USB_DESC_TYPE_DEVICE_QUALIFIER:
      #ifdef USB_OTG_HS_CORE
      if(pdev->cfg.speed == USB_OTG_SPEED_HIGH  )
      {

        pbuf   = (uint8_t *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len);

        //      USBD_DeviceQualifierDesc[4]= pbuf[14];
        //      USBD_DeviceQualifierDesc[5]= pbuf[15];
        //      USBD_DeviceQualifierDesc[6]= pbuf[16];

        pbuf = USBD_DeviceQualifierDesc;
        len  = USB_LEN_DEV_QUALIFIER_DESC;
        break;
      }
      else
      {
        USBD_CtlError(pdev , req);
        return;
      }
      #else
        USBD_CtlError(pdev , req);
        return;
      #endif

    case USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION:
      #ifdef USB_OTG_HS_CORE
      if(pdev->cfg.speed == USB_OTG_SPEED_HIGH) {
        pbuf   = (uint8_t *)pdev->dev.class_cb->GetOtherConfigDescriptor(pdev->cfg.speed, &len);
        pbuf[1] = USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION;
        break;
      }
      else {
        USBD_CtlError(pdev , req);
        return;
      }
      #else
      USBD_CtlError(pdev , req);
      return;
      #endif


    default:
      USBD_CtlError(pdev , req);
      return;
  }

  if((len != 0)&& (req->wLength != 0)) {
    USBD_CtlSendData( pdev, pbuf, MIN(len, req->wLength) );
  }
}

/**
  * @brief  Set device address
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_SetAddress(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
    uint8_t dev_addr = (uint8_t)(req->wValue) & 0x7F;

    if( ((req->wIndex != 0) || (req->wLength != 0)) && (pdev->dev.device_status == USB_OTG_CONFIGURED) ) {
        USBD_CtlError(pdev , req);
        return;
    }

    pdev->dev.device_address = dev_addr;
    DCD_EP_SetAddress(pdev, dev_addr);
    USBD_CtlSendStatus(pdev);

    if(dev_addr) {
        pdev->dev.device_status  = USB_OTG_ADDRESSED;
    }
    else {
        pdev->dev.device_status  = USB_OTG_DEFAULT;
    }
}

/**
  * @brief  Handle Set device configuration request
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_SetConfig(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

	uint8_t status = pdev->dev.device_status;
    uint8_t cfgidx = (uint8_t)(req->wValue);

    if( cfgidx > USBD_CFG_MAX_NUM || status > USB_OTG_CONFIGURED ) {
        USBD_CtlError(pdev, req);
        return;
    }

    switch (status) {

        case USB_OTG_ADDRESSED:
            if(cfgidx) {
                pdev->dev.device_config = cfgidx;
                pdev->dev.device_status = USB_OTG_CONFIGURED;
                USBD_SetCfg(pdev, cfgidx);
            }
        break;

        case USB_OTG_CONFIGURED:
          if(cfgidx == 0) {
              pdev->dev.device_status = USB_OTG_ADDRESSED;
              pdev->dev.device_config = cfgidx;
              USBD_ClrCfg(pdev, cfgidx);
          }
          else if (cfgidx != pdev->dev.device_config) {
              /* Clear old configuration */
              USBD_ClrCfg(pdev , pdev->dev.device_config);
              /* set new configuration */
              pdev->dev.device_config = cfgidx;
              USBD_SetCfg(pdev, cfgidx);
          }
        break;

        default:
        break;
    }

    USBD_CtlSendStatus(pdev);
}

/**
  * @brief  Handle Get device configuration request
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_GetConfig(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
    if (req->wLength != 1) {
        USBD_CtlError(pdev, req);
        return;
    }

    switch(pdev->dev.device_status) {
        case USB_OTG_ADDRESSED:
            USBD_CtlSendData(pdev, (uint8_t*)&USBD_default_cfg, 1);
        break;

        case USB_OTG_CONFIGURED:
            USBD_CtlSendData(pdev, &pdev->dev.device_config, 1);
        break;

        default:
            USBD_CtlError(pdev , req);
        break;
    }
}

/**
  * @brief  Handle Get Status request
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_GetStatus(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

  switch (pdev->dev.device_status) {

      case USB_OTG_ADDRESSED:
      break;

      case USB_OTG_CONFIGURED:
          if (pdev->dev.DevRemoteWakeup) {
              USBD_cfg_status = USB_CONFIG_SELF_POWERED | USB_CONFIG_REMOTE_WAKEUP;
          }
          else {
              USBD_cfg_status = USB_CONFIG_SELF_POWERED;
          }
          USBD_CtlSendData(pdev, (uint8_t*)&USBD_cfg_status, 1);
      break;

      default:
          USBD_CtlError(pdev , req);
      break;
  }
}

/**
  * @brief  Handle Set device feature request
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_SetFeature(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

  USB_OTG_DCTL_TypeDef dctl;
  uint8_t test_mode = 0;

  if (req->wValue == USB_FEATURE_REMOTE_WAKEUP) {
    pdev->dev.DevRemoteWakeup = 1;
    pdev->dev.class_cb->Setup (pdev, req);
    USBD_CtlSendStatus(pdev);
  }

  else if ((req->wValue == USB_FEATURE_TEST_MODE) && ((req->wIndex & 0xFF) == 0)) {
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    test_mode = req->wIndex >> 8;

    switch (test_mode) {
      case 1: // TEST_J
        dctl.b.tstctl = 1;
      break;

      case 2: // TEST_K
        dctl.b.tstctl = 2;
      break;

      case 3: // TEST_SE0_NAK
        dctl.b.tstctl = 3;
      break;

      case 4: // TEST_PACKET
        dctl.b.tstctl = 4;
      break;

      case 5: // TEST_FORCE_ENABLE
        dctl.b.tstctl = 5;
      break;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);
    USBD_CtlSendStatus(pdev);
  }
}

/**
  * @brief  Handle clear device feature request
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
static __inline void USBD_ClrFeature(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
  switch (pdev->dev.device_status) {

    case USB_OTG_ADDRESSED:
    break;

    case USB_OTG_CONFIGURED:
        if(req->wValue == USB_FEATURE_REMOTE_WAKEUP) {
            pdev->dev.DevRemoteWakeup = 0;
            pdev->dev.class_cb->Setup(pdev, req);
            USBD_CtlSendStatus(pdev);
        }
    break;

    default:
        USBD_CtlError(pdev , req);
    break;
  }
}


/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Handle standard usb device requests
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval status
  */
USBD_Status USBD_StdDevReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

    switch (req->bRequest) {

        case USB_REQ_GET_DESCRIPTOR:
            USBD_GetDescriptor(pdev, req) ;
        break;

        case USB_REQ_SET_ADDRESS:
            USBD_SetAddress(pdev, req);
        break;

        case USB_REQ_SET_CONFIGURATION:
            USBD_SetConfig(pdev, req);
        break;

        case USB_REQ_GET_CONFIGURATION:
            USBD_GetConfig(pdev, req);
        break;

        case USB_REQ_GET_STATUS:
            USBD_GetStatus(pdev, req);
        break;

        case USB_REQ_SET_FEATURE:
            USBD_SetFeature(pdev, req);
        break;

        case USB_REQ_CLEAR_FEATURE:
            USBD_ClrFeature(pdev, req);
        break;

        case USB_REQ_SYNCH_FRAME:
            USBD_SetFeature(pdev, req);
        break;

        default:
            USBD_CtlError(pdev, req);
        break;
    }

    return USBD_OK;
}

/**
  * @brief  Handle standard usb interface requests
  * @param  pdev: USB OTG device instance
  * @param  req: usb request
  * @retval status
  */
USBD_Status USBD_StdItfReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

    switch (pdev->dev.device_status) {

        case USB_OTG_CONFIGURED:
            if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM) {
                pdev->dev.class_cb->Setup(pdev, req);
                if(!req->wLength) {
                    USBD_CtlSendStatus(pdev);
                }
            }
            else {
                USBD_CtlError(pdev , req);
            }
        break;

        default:
            USBD_CtlError(pdev , req);
        break;
    }
    return USBD_OK;
}

/**
  * @brief  Handle standard usb endpoint requests
  * @param  pdev: USB OTG device instance
  * @param  req: usb request
  * @retval status
  */
USBD_Status USBD_StdEPReq(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {

    uint8_t ep_addr = LOBYTE(req->wIndex);

    switch (req->bRequest) {

        case USB_REQ_SET_FEATURE:

            switch (pdev->dev.device_status) {

                case USB_OTG_ADDRESSED:
                    if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                        DCD_EP_Stall(pdev , ep_addr);
                    }
                break;

                case USB_OTG_CONFIGURED:
                    if (req->wValue == USB_FEATURE_EP_HALT) {
                        if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                            DCD_EP_Stall(pdev , ep_addr);
                        }
                    }
                    // USBD_CtlSendStatus(pdev);
                    pdev->dev.class_cb->Setup(pdev, req);
                break;

                default:
                    USBD_CtlError(pdev , req);
                break;
            }
        break;

        case USB_REQ_CLEAR_FEATURE:

            switch (pdev->dev.device_status) {

                case USB_OTG_ADDRESSED:
                    if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                        DCD_EP_Stall(pdev , ep_addr);
                    }
                break;

                case USB_OTG_CONFIGURED:
                  if (req->wValue == USB_FEATURE_EP_HALT) {
                      if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                          DCD_EP_ClrStall(pdev , ep_addr);
                      }
                  }
                  // USBD_CtlSendStatus(pdev);
                  pdev->dev.class_cb->Setup(pdev, req);
                break;

                default:
                   USBD_CtlError(pdev , req);
                break;
            }
        break;

        case USB_REQ_GET_STATUS:

            switch (pdev->dev.device_status) {

                case USB_OTG_ADDRESSED:
                  if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                      DCD_EP_Stall(pdev , ep_addr);
                  }
                break;

                case USB_OTG_CONFIGURED:
                  USBD_ep_status = (pdev->dev.out_ep[ep_addr].is_stall)? 0x0001 : 0x0000;
                  USBD_CtlSendData(pdev, (uint8_t *)&USBD_ep_status, 2);
                break;

                default:
                   USBD_CtlError(pdev , req);
                break;
            }
        break;

        default:
        break;
    }

    return USBD_OK;
}

/**
  * @brief  Copy buffer into setup structure
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval None
  */
void USBD_ParseSetupRequest(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
    req->bmRequest     = *(uint8_t *)  (pdev->dev.setup_packet);
    req->bRequest      = *(uint8_t *)  (pdev->dev.setup_packet +  1);
    req->wValue        = SWAPBYTE      (pdev->dev.setup_packet +  2);
    req->wIndex        = SWAPBYTE      (pdev->dev.setup_packet +  4);
    req->wLength       = SWAPBYTE      (pdev->dev.setup_packet +  6);
    /* */
    pdev->dev.in_ep[0].ctl_data_len = req->wLength  ;
    pdev->dev.device_state = USB_OTG_EP0_SETUP;
}

/**
  * @brief  Handle USB low level Error
  * @param  pdev: device instance
  * @param  req: usb request
  * @retval None
  */
void USBD_CtlError(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_REQ *req) {
    uint8_t epnum = 0x00;

    if( (req->bmRequest & 0x80) == 0x80 || (req->wLength == 0) ) {
        epnum = 0x80;
    }

    DCD_EP_Stall(pdev, epnum);
    USB_OTG_EP0_OutStart(pdev);
}

/**
  * @brief  USBD_GetString
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len) {
    uint8_t idx = 0;

    if(desc != NULL) {
        *len =  USBD_GetLen(desc) * 2 + 2;
        unicode[idx++] = *len;
        unicode[idx++] =  USB_DESC_TYPE_STRING;

        while (*desc != NULL) {
          unicode[idx++] = *desc++;
          unicode[idx++] =  0x00;
        }
    }
}

/**
  * @brief  send data on the ctl pipe
  * @param  pdev: device instance
  * @param  buff: pointer to data buffer
  * @param  len: length of data to be sent
  * @retval status
  */
USBD_Status USBD_CtlSendData(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len) {
    /* */
    pdev->dev.in_ep[0].total_data_len = len;
    pdev->dev.in_ep[0].rem_data_len   = len;
    pdev->dev.device_state = USB_OTG_EP0_DATA_IN;
    /* */
    return DCD_EP_Tx(pdev, 0, pbuf, len);
}

/**
  * @brief  Continue sending data on the ctl pipe
  * @param  pdev: device instance
  * @param  buff: pointer to data buffer
  * @param  len: length of data to be sent
  * @retval status
  */
USBD_Status USBD_CtlContinueSendData(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len) {
    return DCD_EP_Tx(pdev, 0, pbuf, len);
}

/**
  * @brief  Receive data on the ctl pipe
  * @param  pdev: USB OTG device instance
  * @param  buff: pointer to data buffer
  * @param  len: length of data to be received
  * @retval status
  */
USBD_Status USBD_CtlPrepareRx(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len) {
    /* */
    pdev->dev.out_ep[0].total_data_len = len;
    pdev->dev.out_ep[0].rem_data_len   = len;
    pdev->dev.device_state = USB_OTG_EP0_DATA_OUT;
    /* */
    return DCD_EP_PrepareRx(pdev, 0, pbuf, len);
}

/**
  * @brief  Continue receive data on the ctl pipe
  * @param  pdev: USB OTG device instance
  * @param  buff: pointer to data buffer
  * @param  len: length of data to be received
  * @retval status
  */
USBD_Status USBD_CtlContinueRx(USB_OTG_CORE_HANDLE *pdev, uint8_t *pbuf, uint16_t len) {
  return DCD_EP_PrepareRx(pdev, 0, pbuf, len);
}

/**
  * @brief  Send zero lzngth packet on the ctl pipe
  * @param  pdev: USB OTG device instance
  * @retval status
  */
USBD_Status USBD_CtlSendStatus(USB_OTG_CORE_HANDLE *pdev) {
    pdev->dev.device_state = USB_OTG_EP0_STATUS_IN;
    DCD_EP_Tx(pdev, 0, NULL, 0);
    USB_OTG_EP0_OutStart(pdev);
    return USBD_OK;
}

/**
  * @brief  Receive zero lzngth packet on the ctl pipe
  * @param  pdev: USB OTG device instance
  * @retval status
  */
USBD_Status USBD_CtlReceiveStatus(USB_OTG_CORE_HANDLE *pdev) {
    pdev->dev.device_state = USB_OTG_EP0_STATUS_OUT;
    DCD_EP_PrepareRx(pdev, 0, NULL, 0);
    USB_OTG_EP0_OutStart(pdev);
    return USBD_OK;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/