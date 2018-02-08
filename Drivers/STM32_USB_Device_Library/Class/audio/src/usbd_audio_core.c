/**
  ******************************************************************************
  * @file         usbd_audio_core.c
  * @author      Neil Lab :: Left Radio
  * @version     v1.0.0
  * @date
  * @brief        sourse for usbd_audio_core
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usbd_def.h"
#include "usbd_req.h"
#include "usbd_audio_core.h"
#include "platform.h"
#include "t11_audio_if.h"

/* Private define ------------------------------------------------------------*/

#define AUDIO_DESCRIPTOR_TYPE                           0x21
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE                 0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                  0x25
/* Class and SubClass */
#define USB_DEVICE_CLASS_AUDIO                          0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                     0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                   0x02
/* Descriptor Protocol */
#define AUDIO_PROTOCOL_UNDEFINED                        0x00
/* Control Interface Descriptor */
#define AUDIO_CONTROL_HEADER                            0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                    0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                   0x03
#define AUDIO_CONTROL_FEATURE_UNIT                      0x06
/* Control Interface Attributes */
#define AUDIO_CONTROL_BM_MUTE                           0x0001
/* streaming Interface */
#define AUDIO_STREAMING_GENERAL                         0x01
#define AUDIO_STREAMING_FORMAT_TYPE                     0x02
#define AUDIO_STREAMING_FORMAT_SPECIFIC                 0x03
/* Format Types */
#define AUDIO_FORMAT_TYPE_I                             0x01
#define AUDIO_FORMAT_TYPE_II                            0x02
#define AUDIO_FORMAT_TYPE_III                           0x03
/* Endpoints Descriptor Types and SubTypes */
#define AUDIO_ENDPOINT_GENERAL                          0x01
#define USB_ENDPOINT_TYPE_ISOCHRONOUS                   0x01
#define USB_ENDPOINT_SUBTYPE_ASYNC                      0x04
#define USB_ENDPOINT_SUBTYPE_IMPLICT_FB                 0x20
/* Endpoints attributes */
#define AUDIO_ENDPOINT_BM_SAMPLE_FREQ                   0x01
#define AUDIO_ENDPOINT_BM_PITCH                         0x02
#define AUDIO_ENDPOINT_BM_MAX_PACK_ONLY                 0x80
/* Terminals ID */
#define AUDIO_INPUT_TERMINAL_ID                         ((uint8_t)0x01)
#define AUDIO_FEATURE_TERMINAL_ID                       ((uint8_t)(AUDIO_INPUT_TERMINAL_ID + 1))
#define AUDIO_OUTPUT_TERMINAL_ID                        ((uint8_t)(AUDIO_FEATURE_TERMINAL_ID + 1))
/* Audio Requests */
#define AUDIO_REQ_GET_CUR                               0x81
#define AUDIO_REQ_SET_CUR                               0x01
/* Audio Control Requests Types */
#define AUDIO_CNTRL_STREAMING_REQ                       0x01
#define AUDIO_CNTRL_ENDPOINT_REQ                        0x02

/* Audio Decription size */
#define USB_AUDIO_DESC_SIZE                             (66 + 61 + 61)
/* Audio Interfaces and Alternate settings */
#define AUDIO_TOTAL_IF_NUM                              USBD_ITF_MAX_NUM + 1
#define AUDIO_TOTAL_ALT_SET_NUM                         0x03
/* Audio streaming IN/OUT endpoints */
#define AUDIO_OUT_EP                                    0x01
#define AUDIO_IN_EP                                     0x81
//
#define AUDIO_OUT_MAXPACKET                             4096
//
#define AUDIO_IN_PACKET                                 ((uint32_t)(64))

/* Private macro -------------------------------------------------------------*/
#define AUDIO_SAMPLE_FREQ(FREQ)                         (uint8_t)(FREQ & 0x000000FF), (uint8_t)((FREQ >> 8)), (uint8_t)((FREQ >> 16))

/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static USBD_Status usbd_audio_Init(void *pdev, uint8_t cfgidx);
static USBD_Status usbd_audio_DeInit(void *pdev, uint8_t cfgidx);
static USBD_Status usbd_audio_Setup(void *pdev, USB_SETUP_REQ *req);
static USBD_Status usbd_audio_CtlReq_TxSent(void *pdev);
static USBD_Status usbd_audio_CtlReq_RxReady(void *pdev);
static USBD_Status usbd_audio_DataIn(void *pdev, uint8_t epnum);
static USBD_Status usbd_audio_DataOut(void *pdev, uint8_t epnum);
static USBD_Status usbd_audio_SOF(void *pdev);
static USBD_Status usbd_audio_EOF(void *pdev);
static USBD_Status usbd_audio_IN_Incomplete(void *pdev);
static USBD_Status usbd_audio_OUT_Incomplete(void *pdev);
static uint8_t *USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length);

/* Private variables ---------------------------------------------------------*/
/* USB AUDIO device Configuration Descriptor */
__ALIGN_BEGIN uint8_t usbd_audio_CfgDesc[USB_AUDIO_DESC_SIZE] __ALIGN_END = {

    /* ------------------------ Configuration 1 ------------------------ */

    0x09,                                   /* bLength */
    USB_DESC_TYPE_CONFIGURATION,            /* bDescriptorType */
    LOBYTE(USB_AUDIO_DESC_SIZE),             /* wTotalLength */
    HIBYTE(USB_AUDIO_DESC_SIZE),
    USBD_ITF_MAX_NUM + 1,                   /* bNumInterfaces */
    0x01,                                   /* ID of this configuration */
    0x00,                                   /* iConfiguration */
    0xC0,                                   /* bmAttributes  BUS Powred*/
    0x32,                                   /* bMaxPower = 100 mA*/
    /* 09 byte*/

    /* AudioControl, 0 Interface Descriptor ------------------------ */

    /* USB Speaker Standard AC interface descriptor */
    0x09,                                   /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    0x00,                                   /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x00,                                   /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,                 /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOCONTROL,            /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,               /* bInterfaceProtocol */
    0x00,                                   /* iInterface */
    /* 09 byte*/

    /* USB Speaker Class-specific AC interface Descriptor */
    0x09,                                   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_CONTROL_HEADER,                   /* bDescriptorSubtype */
    0x00,          /* 1.00 */               /* bcdADC */
    0x01,
    0x27,                                   /* wTotalLength = 39*/
    0x00,
    0x01,                                   /* bInCollection, The number of interfaces */
    0x01,                                   /* baInterfaceNr */
    /* 9 byte*/

    /* USB Speaker Input Terminal Descriptor */
    0x0C,         /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,           /* bDescriptorSubtype */
    AUDIO_INPUT_TERMINAL_ID,                /* bTerminalID */
    0x01,                                   /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
    0x01,
    0x00,                                   /* bAssocTerminal */
    0x02,                                   /* bNrChannels */
    0x03,                                   /* wChannelConfig 0x0000  Mono */
    0x00,
    0x00,                                   /* iChannelNames */
    0x00,                                   /* iTerminal */
    /* 12 byte*/

    /* USB Speaker Audio Feature Unit Descriptor */
    0x09,                                   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_CONTROL_FEATURE_UNIT,             /* bDescriptorSubtype */
    AUDIO_FEATURE_TERMINAL_ID,              /* bUnitID */
    AUDIO_INPUT_TERMINAL_ID,                /* bSourceID */
    0x01,                                   /* bControlSize */
    LOBYTE(AUDIO_CONTROL_BM_MUTE),          /* bmaControls(0) */
    HIBYTE(AUDIO_CONTROL_BM_MUTE),          /* bmaControls(1) */
    0x00,                                   /* iTerminal */
    /* 09 byte*/

    /*USB Speaker Output Terminal Descriptor */
    0x09,                                   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,          /* bDescriptorSubtype */
    0x03,                                   /* bTerminalID */
    0x01,                                   /* wTerminalType  0x0301*/
    0x03,
    0x00,                                   /* bAssocTerminal */
    AUDIO_FEATURE_TERMINAL_ID,              /* bSourceID */
    0x00,                                   /* iTerminal */
    /* 09 byte*/

    /* AudioStreaming, 1 Interface Descriptor ------------------------ */

    /* Interface 1, Alternate Setting 0 */

    /* USB Speaker Standard AS Interface Descriptor - Zero Bandwith */
    0x09,                                   /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    0x01,                                   /* bInterfaceNumber */
    0x00,                                   /* bAlternateSetting */
    0x00,                                   /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,                 /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,               /* bInterfaceProtocol */
    0x00,                                   /* iInterface */
    /* 09 byte*/

    /* Interface 1, Alternate Setting 1 */

    /* USB Speaker Standard AS Interface Descriptor */
    0x09,              /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    0x01,                                   /* bInterfaceNumber */
    0x01,                                   /* bAlternateSetting */
    0x02,                                   /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,                 /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,               /* bInterfaceProtocol */
    0x00,                                   /* iInterface */
    /* 09 byte*/

    /* USB Speaker Audio Streaming Interface Descriptor */
    0x07,    /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,                /* bDescriptorSubtype */
    0x01,                                   /* bTerminalLink */
    0x01,                                   /* bDelay */
    0x01,                                   /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
    0x00,
    /* 07 byte*/

    /* USB Speaker Audio Type I Format Interface Descriptor */
    (0x0B+9),                               /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,            /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                    /* bFormatType */
    0x02,                                   /* bNrChannels */
    2,                                      /* bSubFrameSize :  2 Bytes per frame (16-bits) */
    16,                                     /* bBitResolution (16-bits per sample) */
    0x04,                                   /* bSamFreqType count of the frequency supported */
    AUDIO_SAMPLE_FREQ(44100),               /* Audio sampling frequency coded on 3 bytes */
    AUDIO_SAMPLE_FREQ(48000),
    AUDIO_SAMPLE_FREQ(96000),
    AUDIO_SAMPLE_FREQ(192000),
    /* 20 byte*/

    /* Endpoint 1 - Standard Descriptor */
    0x09,      /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    0x01,                                   /* bEndpointAddress 1 out endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS | \
    USB_ENDPOINT_SUBTYPE_ASYNC,          	/* bmAttributes */
    LOBYTE((1024 | 0x1000)),
    HIBYTE((1024 | 0x1000)),
    0x04,                                   /* bInterval */
    0x00,                                   /* bRefresh */
    0x00,                            /* bSynchAddress */
    /* 09 byte*/

    /* Endpoint - Audio Streaming Descriptor*/
    0x07,     /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,                 /* bDescriptor */
    AUDIO_ENDPOINT_BM_SAMPLE_FREQ,          /* bmAttributes */
    0x00,                                   /* bLockDelayUnits */
    0x00,                                   /* wLockDelay */
    0x00,
    /* 07 byte*/

    /* Endpoint 2 - Standard Descriptor, FB */
    0x09,                                   /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    AUDIO_IN_EP,                            /* bEndpointAddress 2, in FB endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS|(1<<4),   /* bmAttributes, feedback endpoint(explict) */
    0x03, 0x00,                             /* wMaxPacketSize in Bytes */
    0x04,                                   /* bInterval */
    0x06,                                   /* bRefresh */
    0x00,                                   /* bSynchAddress */
    /* 09 byte*/

    /* Interface 1, Alternate Setting 2 */

    /* USB Speaker Standard AS Interface Descriptor */
    0x09,                                   /* bLength */
    USB_DESC_TYPE_INTERFACE,                /* bDescriptorType */
    0x01,                                   /* bInterfaceNumber */
    0x02,                                   /* bAlternateSetting */
    0x02,                                   /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,                 /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,               /* bInterfaceProtocol */
    0x00,                                   /* iInterface */
    /* 09 byte*/

    /* USB Speaker Audio Streaming Interface Descriptor */
    0x07,    /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,                /* bDescriptorSubtype */
    0x01,                                   /* bTerminalLink */
    0x01,                                   /* bDelay */
    0x01,                                   /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
    0x00,
    /* 07 byte*/

    /* USB Speaker Audio Type I Format Interface Descriptor */
    (0x0B+9),                               /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,            /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                    /* bFormatType */
    0x02,                                   /* bNrChannels */
    3,                                      /* bSubFrameSize :  3 Bytes per frame (24-bits) */
    24,                                     /* bBitResolution (24-bits per sample) */
    0x04,                                   /* bSamFreqType count of the frequency supported */
    AUDIO_SAMPLE_FREQ(44100),               /* Audio sampling frequency coded on 3 bytes */
    AUDIO_SAMPLE_FREQ(48000),
    AUDIO_SAMPLE_FREQ(96000),
    AUDIO_SAMPLE_FREQ(192000),
    /* 20 byte*/

    /* Endpoint 1 - Standard Descriptor */
    0x09,      /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    0x01,                                   /* bEndpointAddress 1 out endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS | \
    USB_ENDPOINT_SUBTYPE_ASYNC,          	/* bmAttributes */
    LOBYTE((1024 | 0x1000)),
    HIBYTE((1024 | 0x1000)),
    0x04,                                   /* bInterval */
    0x00,                                   /* bRefresh */
    AUDIO_IN_EP,                            /* bSynchAddress */
    /* 09 byte*/

    /* Endpoint - Audio Streaming Descriptor*/
    0x07,     /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,                 /* bDescriptor */
    AUDIO_ENDPOINT_BM_SAMPLE_FREQ,          /* bmAttributes */
    0x00,                                   /* bLockDelayUnits */
    0x00,                                   /* wLockDelay */
    0x00,
    /* 07 byte*/

    /* Endpoint 2 - Standard Descriptor, FB */
    0x09,                                   /* bLength */
    USB_DESC_TYPE_ENDPOINT,                 /* bDescriptorType */
    AUDIO_IN_EP,                            /* bEndpointAddress 2, in FB endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS|(1<<4),   /* bmAttributes, feedback endpoint(explict) */
    0x03, 0x00,                             /* wMaxPacketSize in Bytes */
    0x04,                                   /* bInterval */
    0x06,                                   /* bRefresh */
    0x00,                                   /* bSynchAddress */
    /* 09 byte*/
};

USBD_Class_cb_TypeDef UAC1_Core_Callbacks = {
    usbd_audio_Init,
    usbd_audio_DeInit,
    usbd_audio_Setup,
    usbd_audio_CtlReq_TxSent,       /* EP0_TxSent */
    usbd_audio_CtlReq_RxReady,      /* EP0_RxReady */
    usbd_audio_DataIn,
    usbd_audio_DataOut,
    usbd_audio_SOF,
    usbd_audio_EOF,
    usbd_audio_IN_Incomplete,
    usbd_audio_OUT_Incomplete,     /* usbd_audio_OUT_Incplt */
    USBD_audio_GetCfgDesc,
    USBD_audio_GetCfgDesc           /* use same config as per FS */
};

/* Audio Interface index */
volatile uint8_t usbd_audio_Interface = 0;
/* Audio Control Rrequest */
volatile USBD_Class_CntrlReq_TypeDef ControlReuest;
/* Audio isoc out endpoint buffer */
__ALIGN_BEGIN __IO uint8_t IsocOutBuff[AUDIO_OUT_MAXPACKET]; __ALIGN_END


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  usbd_audio_FeedBack
  * @param  pdev: device instance
  * @retval None
  */
static __inline void usbd_audio_FeedBack(void *pdev) {
    static volatile uint8_t bufs[6];
    uint32_t FB_rate;
    uint32_t sf = T11_Audio_IF.settings->sampling_freq+1000;
    //   int32_t FB_rate_t;
    //   static volatile float FB_rate_div = 10;
    //   USB_OTG_DSTS_TypeDef dsts;

    //   if (T11_Audio_IF->settings.sampling_freq == 44100) FB_rate = (44 << 14) + ((1 << 14) / 10);
    //   else FB_rate = (T11_Audio_IF->settings.sampling_freq / 1000) << 14;
    //   FB_rate = ((T11_Audio_IF->settings.sampling_freq / 1000) << 14) + ((1 << 14) / 10);

       // FB_rate_t = (int)(((float)T11_Audio_IF->settings.sampling_freq - (float)Frequency_IN) / FB_rate_div);
    //   FB_rate += FB_rate_t;
    FB_rate = ((sf/1000) << 14) | ((sf%1000));

    bufs[0] = (uint8_t)(FB_rate);
    bufs[1] = (uint8_t)(FB_rate >> 8);
    bufs[2] = (uint8_t)(FB_rate >> 16);

    //    if(!flag) {
    //        dsts.d32 = USB_OTG_READ_REG32(&(((USB_OTG_CORE_HANDLE*)pdev)->regs.DREGS->DSTS));
    //        if( (dsts.b.soffn & 0x01) == dpid ) {
    //            DCD_EP_Tx (pdev, AUDIO_IN_EP, (uint8_t *) &bufs, 3);
    //            flag = 1;
    //        };
    //    };
    //   /* Toggle the frame index */
    //   ((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[AUDIO_IN_EP & 0x80].even_odd_frame =
    //        (((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[AUDIO_IN_EP & 0x80].even_odd_frame)? 0:1;
    //   DCD_EP_Flush(pdev, AUDIO_IN_EP);
    DCD_EP_Tx(pdev, AUDIO_IN_EP, (uint8_t*)bufs, 3);
}

/**
  * @brief  Handles
  * @param  None
  * @retval None
  */
static __inline void __Endpoinds_SetState(void *pdev, uint8_t state) {
    if (state) {
        DCD_EP_Open(pdev, AUDIO_IN_EP, 3, USB_OTG_EP_ISOC);
        DCD_EP_Open(pdev, AUDIO_OUT_EP, T11_Audio_IF.stream->frame_size, USB_OTG_EP_ISOC);
        /* Prepare Out endpoint to receive audio data */
        DCD_EP_PrepareRx(pdev, AUDIO_OUT_EP, (uint8_t*)IsocOutBuff, T11_Audio_IF.stream->frame_size);
    }
    else {
        DCD_EP_Close(pdev, AUDIO_OUT_EP);
        DCD_EP_Close(pdev, AUDIO_IN_EP);
    }
}

/**
  * @brief  Handles
  * @param  None
  * @retval None
  */
static __inline void __SamplingUpdate(void *pdev) {
	/* Close Endpoints */
	__Endpoinds_SetState(pdev, 0);
	/* Update Audio Interface */
	T11_Audio_IF.StreamUpdate( &T11_Platform );
	/* ReOpen Endpoints */
	__Endpoinds_SetState(pdev, 1);
}


/********************** AUDIO Class Requests Management ***********************/

/**
  * @brief  Handles audio control request reset.
  * @param  None
  * @retval None
  */
static __inline void __Request_Reset(void) {
    memset((uint8_t*)ControlReuest.pbuf, 0, 64);
    ControlReuest.Req = 0;
    ControlReuest.Cmd = 0;
    ControlReuest.Unit = 0;
    ControlReuest.Len = 0;
}

/**
  * @brief  Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static __inline void __Request_GetCurrent(void *pdev, USB_SETUP_REQ *req) {

    switch( req->bmRequest & ~(USB_REQ_TYPE_MASK | USB_REQ_TYPE_CLASS) ) {

        case AUDIO_CNTRL_STREAMING_REQ:
            switch( HIBYTE(req->wIndex) ) {
                case AUDIO_FEATURE_TERMINAL_ID:
                    ControlReuest.pbuf[0] = T11_Audio_IF.GetMute();
                break;
            }
        break;

        case AUDIO_CNTRL_ENDPOINT_REQ:
            {}
        break;
    }

	/* Send the current mute state */
    USBD_CtlSendData(pdev, (uint8_t*)ControlReuest.pbuf, req->wLength);
}

/**
  * @brief  Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static __inline void __Request_SetCurrent(void *pdev, USB_SETUP_REQ *req) {
    if (!req->wLength) {
        return;
    }
    /* Prepare the reception of the buffer over EP0 */
    USBD_CtlPrepareRx(pdev, (uint8_t*)ControlReuest.pbuf, req->wLength);
    /* Set the global variables indicating current request and its length
       to the function usbd_audio_CtlReq_RxReady() which will process the request */
    ControlReuest.Req = req->bmRequest & ~(USB_REQ_TYPE_MASK | USB_REQ_TYPE_CLASS);
    ControlReuest.Cmd = AUDIO_REQ_SET_CUR;
    ControlReuest.Unit = HIBYTE(req->wIndex);
    ControlReuest.Len = req->wLength;
}

/**
  * @brief  Handles the audio class requests.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static __inline USBD_Status __Request_Class(void *pdev, USB_SETUP_REQ *req) {

    switch (req->bRequest) {

        case AUDIO_REQ_GET_CUR:
            __Request_GetCurrent(pdev, req);
        break;

        case AUDIO_REQ_SET_CUR:
            __Request_SetCurrent(pdev, req);
        break;

        default:
            return USBD_FAIL;
        break;
    }
    return USBD_OK;
}


/************************ Standart Requests Management ************************/

/**
  * @brief  Handles the Get Descriptor request.
  * @param  req: setup standart request
  * @retval status
  */
static __inline USBD_Status __Request_GetDescriptor(void *pdev, USB_SETUP_REQ *req) {
	if( (req->wValue >> 8) != AUDIO_DESCRIPTOR_TYPE) {
		return USBD_FAIL;
	}
	/* */
	USBD_CtlSendData(
			pdev,
			usbd_audio_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM),
			MIN(USB_AUDIO_DESC_SIZE, req->wLength)
	);
	return USBD_OK;
}

/**
  * @brief  Handles the Set Interface request.
  * @param  req: setup standart request
  * @retval status
  */
static __inline USBD_Status __Request_SetInterface(void *pdev, USB_SETUP_REQ *req) {
	uint8_t itf = (uint8_t)(req->wIndex);
	uint8_t alt = (uint8_t)(req->wValue);

	if (itf > AUDIO_TOTAL_IF_NUM || alt > AUDIO_TOTAL_ALT_SET_NUM) {
		return USBD_FAIL;
	}

	if (usbd_audio_Interface != itf) {
		usbd_audio_Interface = itf;
	}
	if (T11_Audio_IF.settings->index != alt) {
		T11_Audio_IF.settings->index = alt;
	}
    __SamplingUpdate(pdev);

	USBD_CtlSendStatus(pdev);

	return USBD_OK;
}

/**
  * @brief  Handles the standart requests.
  * @param  pdev: instance
  * @param  req: setup standart request
  * @retval status
  */
static __inline USBD_Status __Request_Standart(void *pdev, USB_SETUP_REQ *req) {

    switch (req->bRequest) {

        case USB_REQ_GET_DESCRIPTOR:
        	return __Request_GetDescriptor(pdev, req);
        break;

        case USB_REQ_GET_INTERFACE :
            USBD_CtlSendData(pdev, (uint8_t*)&usbd_audio_Interface, 1);
        break;

        case USB_REQ_SET_INTERFACE :
        	return __Request_SetInterface(pdev, req);
        break;

        default:
            return USBD_FAIL;
        break;
    }
    return USBD_OK;
}


/********************** AUDIO interface class callbacks ***********************/

/**
  * @brief  Handles the Audio control request parsing.
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static USBD_Status usbd_audio_Setup (void *pdev, USB_SETUP_REQ *req) {

    USBD_Status ret;

    switch (req->bmRequest & USB_REQ_TYPE_MASK) {

        case USB_REQ_TYPE_CLASS:
            ret = __Request_Class(pdev, req);
        break;

        case USB_REQ_TYPE_STANDARD:
            ret = __Request_Standart(pdev, req);
        break;

        default:
            ret = USBD_FAIL;
        break;
    }

    if(ret == USBD_FAIL) {
        /* Call the error management function (command will be nacked) */
        USBD_CtlError(pdev, req);
    }
    return ret;
}

/**
  * @brief  Handles audio control requests data.
  * @param  pdev: device device instance
  * @retval status
  */
static USBD_Status usbd_audio_CtlReq_TxSent(void *pdev) {
	return USBD_OK;
}

/**
  * @brief  Handles audio control requests data.
  * @param  pdev: device device instance
  * @retval status
  */
static USBD_Status usbd_audio_CtlReq_RxReady(void *pdev) {
    /* Check if an AudioControl request has been issued */
    if (ControlReuest.Cmd == AUDIO_REQ_SET_CUR) {
        switch(ControlReuest.Req) {

            case AUDIO_CNTRL_STREAMING_REQ:
                switch(ControlReuest.Unit) {
                    case AUDIO_FEATURE_TERMINAL_ID:
                        T11_Audio_IF.MuteCtl( &T11_Platform, ControlReuest.pbuf[0] );
                    break;
                }
            break;

            case AUDIO_CNTRL_ENDPOINT_REQ:
                T11_Audio_IF.settings->sampling_freq = ((uint32_t)ControlReuest.pbuf[2]<<16) | \
                									   ((uint32_t)ControlReuest.pbuf[1]<<8) | \
                									   (uint32_t)ControlReuest.pbuf[0];
            	__SamplingUpdate(pdev);
            break;
        }
    }
    /* Reset the audio control request */
    __Request_Reset();
    USBD_CtlSendStatus(pdev);
    return USBD_OK;
}

/**
  * @brief  Initilaizes the AUDIO interface.
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static USBD_Status usbd_audio_Init (void *pdev, uint8_t cfgidx) {
    /* */
    __SamplingUpdate(pdev);
    return USBD_OK;
}

/**
  * @brief  DeInitializes the AUDIO layer.
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static USBD_Status usbd_audio_DeInit (void *pdev, uint8_t cfgidx) {
    /*  */
    __Endpoinds_SetState(pdev, 0);
    return USBD_OK;
}

/**
  * @brief  Handles the audio IN data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static USBD_Status usbd_audio_DataIn(void *pdev, uint8_t epnum) {
	if (epnum == (AUDIO_IN_EP & 0x7f)) {
		// DCD_EP_Tx(pdev, AUDIO_IN_EP, (uint8_t*)bufs, 3);
        // ((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[epnum].even_odd_frame =
        //     (((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[epnum].even_odd_frame)? 0:1;
    }
    return USBD_OK;
}

/**
  * @brief  handles the Audio Out data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static USBD_Status usbd_audio_DataOut(void *pdev, uint8_t epnum) {
	/* */
    uint8_t* xfer_data = (uint8_t*)IsocOutBuff;
    uint32_t xfer_count = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;
    T11_AUDIO_IF_STREAM_TypeDef* stream = (T11_AUDIO_IF_STREAM_TypeDef*)T11_Audio_IF.stream;
    uint32_t crop_size;

    if (epnum != AUDIO_OUT_EP) {
    	return USBD_OK;
    }

    if(xfer_count) {
    	/* If end of the main audio buffer */
    	if( stream->cpy_ptr - stream->buff_ptr > stream->buffer_align_size - stream->frame_size ) {
    		/* Crop transfer and copy */
    		crop_size = stream->buffer_align_size - (stream->cpy_ptr - stream->buff_ptr);
    		memcpy(stream->cpy_ptr, xfer_data, crop_size);
    		/* Roll back and copy rest data */
    		memcpy(stream->buff_ptr, xfer_data + crop_size, xfer_count - crop_size);
    		stream->cpy_ptr = stream->buff_ptr + xfer_count - crop_size;
    		stream->filled_by = 0;
    	}
    	else {
    		memcpy(stream->cpy_ptr, xfer_data, xfer_count);
    		stream->cpy_ptr += xfer_count;
    	}

        stream->filled_by = (uint32_t)(((stream->cpy_ptr - stream->buff_ptr) * 100) / stream->buffer_bytes_size) ;
    }

    /* Toggle the frame index */
    ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame =
    		(((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame)? 0:1;
    /* Prepare Out endpoint to receive next audio packet */
    DCD_EP_PrepareRx(pdev, AUDIO_OUT_EP, (uint8_t*)IsocOutBuff, stream->frame_size);

    return USBD_OK;
}

/**
  * @brief  Handles the SOF event (data buffer update and synchronization).
  * @param  pdev: instance
  * @retval status
  */
static USBD_Status usbd_audio_SOF(void *pdev) {

    // static uint32_t SOF_Cnt = 0;
    // /* Explit FeedBack for AUDIO_OUT_FB */
    // if(T11_Audio_IF.settings->index != 0) {
    //    if(SOF_Cnt++ >= 9) {
    //        usbd_audio_FeedBack(pdev);
    //        SOF_Cnt = 0;
    //    }
    // }
    return USBD_OK;
}

/**
  * @brief  Handles the EOF event.
  * @param  pdev: instance
  * @retval status
  */
static USBD_Status usbd_audio_EOF(void *pdev) {
    /* Toggle the frame index */
    ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[AUDIO_OUT_EP].even_odd_frame =
    		(((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[AUDIO_OUT_EP].even_odd_frame)? 0:1;
    /* Prepare Out endpoint to receive next audio packet */
    DCD_EP_PrepareRx(pdev, AUDIO_OUT_EP, (uint8_t*)(IsocOutBuff), T11_Audio_IF.stream->frame_size);
    /* */
    return USBD_OK;
}

/**
  * @brief  Handles the EOF event.
  * @param  pdev: instance
  * @retval status
  */
static USBD_Status usbd_audio_IN_Incomplete(void *pdev) {
    DCD_EP_Flush(pdev, AUDIO_IN_EP);
    return USBD_OK;
}

/**
  * @brief  Handles the EOF event.
  * @param  pdev: instance
  * @retval status
  */
static USBD_Status usbd_audio_OUT_Incomplete(void *pdev) {
	return USBD_OK;
}

/**
  * @brief  Returns configuration descriptor.
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t* USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length) {
      *length = sizeof(usbd_audio_CfgDesc);
      return usbd_audio_CfgDesc;
}


/*******************************************************************************
      END FILE
*******************************************************************************/
