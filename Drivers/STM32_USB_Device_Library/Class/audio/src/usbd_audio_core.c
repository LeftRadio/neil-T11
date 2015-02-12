/**
  ******************************************************************************
  * @file	 	usbd_audio_core.c
  * @author  	Neil Lab :: Left Radio
  * @version 	v1.0.0
  * @date
  * @brief		sourse for usbd_audio_core
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "Audio_Interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN __IO uint8_t IsocOutBuff[AUDIO_OUT_PACKET * 4]; __ALIGN_END
int32_t WR_Delta_Out = AUDIO_BUFFER_HALFSIZE;
volatile uint8_t AudioBuffer[AUDIO_BUFFER_SIZE_BYTES];

uint8_t* IsocOutWrPtr = (uint8_t*)IsocOutBuff;
uint8_t* IsocOutRdPtr = (uint8_t*)IsocOutBuff;

FlagStatus AudioBuffer_HalfFull = RESET;
FlagStatus AudioBuffer_Full = RESET;

uint32_t SOF_Cnt = 0;
//uint32_t USB_ReciveAudioPackedCnt = 0;
float Frequency_IN = AUDIO_FREQUENCY;
volatile uint32_t AudioPlayState = 0;


/* Main Buffer for Audio Control Rrequests transfers and its relative variables */
__ALIGN_BEGIN uint8_t  AudioCtl[64]; __ALIGN_END
uint8_t  AudioCtlCmd = 0;
uint32_t AudioCtlLen = 0;
uint8_t  AudioCtlUnit = 0;
volatile uint32_t usbd_audio_AltSet = 0;



#ifdef USE_AUDIO_CLASS_01_00
/* USB AUDIO device Configuration Descriptor */
uint8_t usbd_audio_CfgDesc[AUDIO_CONFIG_DESC_SIZE] =
{
  /* Configuration 1 */
  0x09,                                 /* bLength */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
  LOBYTE(AUDIO_CONFIG_DESC_SIZE),       /* wTotalLength */
  HIBYTE(AUDIO_CONFIG_DESC_SIZE),
  0x02,                                 /* bNumInterfaces */
  0x01,                                 /* bConfigurationValue */
  0x00,                                 /* iConfiguration */
  0xC0,                                 /* bmAttributes BUS Powred */
  0x0A,                                 /* bMaxPower */
  /* 09 byte*/

  /* USB Speaker Standard interface descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  0x00,                                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x00,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

  /* USB Speaker Class-specific AC Interface Descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
  0x00,          /* 1.00 */             /* bcdADC */
  0x01,
  LOBYTE(0x27),                         /* wTotalLength = 39*/
  HIBYTE(0x27),
  0x01,                                 /* bInCollection */
  0x01,                                 /* baInterfaceNr */
  /* 09 byte*/

  /* USB Speaker Input Terminal Descriptor */
  AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
  AUDIO_INPUT_TERMINAL_ID,              /* bTerminalID */
  0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
  0x01,
  0x00,                                 /* bAssocTerminal */
  AUDIO_CHANNELS_COUNT,                 /* bNrChannels */
  LOBYTE(AUDIO_CHANNELS_INPUT_TERMINAL_SETTINGS),	/* wChannelConfig 0x0001 - LF, 0x0002 - RF, 0x0004 - CF, 0x0008 - LFE */
  HIBYTE(AUDIO_CHANNELS_INPUT_TERMINAL_SETTINGS),
  0x00,                                 /* iChannelNames */
  0x00,                                 /* iTerminal */
  /* 12 byte*/

  /* USB Speaker Audio Feature Unit Descriptor */
  0x09,                                 /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
  AUDIO_FEATURE_UNIT_TERMINAL_ID,       /* bUnitID */
  AUDIO_INPUT_TERMINAL_ID,       		/* bSourceID */
  0x01,                                 /* bControlSize */
  0x01,                   				/* bmaControls(0) MUTE */
  0x00,                                 /* bmaControls(1) */
  0x00,                                 /* iTerminal */
  /* 09 byte*/

  /*USB Speaker Output Terminal Descriptor */
  0x09,      /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
  AUDIO_OUTPUT_TERMINAL_ID,             /* bTerminalID */
  0x05,                                 /* wTerminalType  0x0301 - headphone, 0x0602 - Digital Interface, 0x0305 - Room speaker*/
  0x03,
  0x00,                                 /* bAssocTerminal */
  AUDIO_FEATURE_UNIT_TERMINAL_ID,      	/* bSourceID */
  0x00,                                 /* iTerminal */
  /* 09 byte*/

  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
  /* Interface 1, Alternate Setting 0                                             */
  AUDIO_INTERFACE_DESC_SIZE,  /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  0x01,                                 /* bInterfaceNumber */
  0x00,                                 /* bAlternateSetting */
  0x00,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
  /* Interface 1, Alternate Setting 1                                           */
  AUDIO_INTERFACE_DESC_SIZE,  /* bLength */
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
  0x01,                                 /* bInterfaceNumber */
  0x01,                                 /* bAlternateSetting */
  0x01,                                 /* bNumEndpoints */
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
  AUDIO_PROTOCOL_UNDEFINED,      		/* bInterfaceProtocol */
  0x00,                                 /* iInterface */
  /* 09 byte*/

  /* Class-specific Audio Streaming Interface Descriptor */
  AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
  AUDIO_INPUT_TERMINAL_ID,             	/* bTerminalLink */
  0x01,                                 /* bDelay */
  0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
  0x00,
  /* 07 byte*/

  /* USB Speaker Audio Type I Format Interface Descriptor */
  0x0B,                                 /* bLength */
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
  AUDIO_STREAMING_FORMAT_SPECIFIC,      /* bDescriptorSubtype */
  AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
  AUDIO_CHANNELS_COUNT,                 /* bNrChannels */
  AUDIO_FRAME_SIZE,                   	/* bSubFrameSize */
  AUDIO_BITRESOLUTION,                  /* bBitResolution*/
  0x01,                                 /* bSamFreqType only one frequency supported */
  SAMPLE_FREQ,							/* Audio sampling frequency coded on 3 bytes */
  /* 11 byte*/

  /* Endpoint 1 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
  USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
  AUDIO_OUT_EP,                         /* bEndpointAddress 1 out endpoint*/
  USB_ENDPOINT_TYPE_ISOCHRONOUS, 		/* bmAttributes, isochronous, async */
  LOBYTE(AUDIO_OUT_PACKET),
  HIBYTE(AUDIO_OUT_PACKET),
//  LOBYTE((1024 | 0x1000)),
//  HIBYTE((1024 | 0x1000)),
  0x04,                                 /* bInterval */
  0x00,                                 /* bRefresh */
  0x00,                                 /* bSynchAddress */
  /* 09 byte*/

  /* Endpoint - Audio Streaming Descriptor*/
  AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
  AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
  AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
  0x00,                                 /* bmAttributes */
  0x00,                                 /* bLockDelayUnits */
  0x00,                                 /* wLockDelay */
  0x00,
  /* 07 byte*/
} ;
#endif

#ifdef USE_AUDIO_CLASS_02_00
/* USB AUDIO UAC2 device Configuration Descriptor */
static volatile uint8_t usbd_audio_CfgDesc[USB_AUDIO_DESC_SIZ] =
{
		/* Configuration 1 */
		0x09,                                 /* bLength */
		USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
		LOBYTE(AUDIO_CONFIG_DESC_SIZE),       /* wTotalLength */
		HIBYTE(AUDIO_CONFIG_DESC_SIZE),
		0x02,								  /* bNumInterfaces */
		0x01,                                 /* bConfigurationValue */
		0x00,                                 /* iConfiguration */
		0xC0,                                 /* bmAttributes  BUS Powred*/
		250,                                  /* bMaxPower = 100 mA*/
		/* 09 byte*/

/* --- UAC2 configuration description, see page 46 in
 *     "Universal Serial Bus Device Class Definition for Audio Devices" (Audio20 final.pdf) --- */

		/* Standard IAD Interface Descriptor */
		8,										/* bLength */
		AUDIO_IAD_DESCRIPTOR_TYPE,				/* bDescriptorType */
		0x00,									/* bFirstInterface */
		0x02,									/* bInterfaceNumber */
		USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass AUDIO */
		0x00,									/* bInterfaceSubClass UNDEFINED */
		AUDIO_PROTOCOL_IP_VERSION_02_00,		/* bInterfaceProtocol */
		0x00,									/* iInterface */
		/* 08 byte*/

		/* Standard AC(AudioControl) Interface Descriptor */
		AUDIO_INTERFACE_DESC_SIZE,				/* bLength */
		USB_INTERFACE_DESCRIPTOR_TYPE, 		  	/* bDescriptorType */
		0x00,									/* bInterfaceNumber */
		0x00,									/* bAlternateSetting */
		0x00,									/* bNumEndpoints */
		USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass */
		AUDIO_SUBCLASS_AUDIOCONTROL,			/* bInterfaceSubClass */
		AUDIO_PROTOCOL_IP_VERSION_02_00,		/* bInterfaceProtocol */
		0x00,									/* iInterface */
		/* 09 byte*/

		/* Class-specific AC(AudioControl) Interface Descriptor */
		AUDIO_INTERFACE_DESC_SIZE,				/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_CONTROL_HEADER,					/* bDescriptorSubtype */
		0x00, 0x02,								/* bcdADC 2.00 */
		0x01,									/* bCategory */
		LOBYTE(0x003C),							/* wTotalLength Total number of bytes returned for the class-specific */
		HIBYTE(0x003C),							/* AudioControl interface descriptor. Includes the combined length */
												/* of this descriptor header and all Clock Source, Unit and Terminal descriptors.*/
		0x00,									/* bmControls */
		/* 09 byte*/

		/* Standart CSD(Clock Sourse) 44.1k Interface Descriptor */
		8,            												/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_CONTROL_CLOCK_SOURSE,				/* bDescriptorSubtype */
		0x10,									/* bClockID */
		0x01,									/* bmAttributes, 0x01 - Internal fixed Clock */
		0x07,	 	 	 	 	 	 	 	 	/* bmControls, 0b00000111 -	freq r/w, validity r */
		0x00,						 	 	 	/* bAssocTerminal */
		0x00,									/* iClockSource */
		/* 08 byte*/

		/* Audio Input Terminal Descriptor */
		0x11,          							/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_CONTROL_INPUT_TERMINAL,			/* bDescriptorSubtype */
		0x20,									/* bTerminalID */
		0x01, 0x01,								/* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
		0x00,									/* bAssocTerminal */
		0x10,									/* bCSourceID - CSD/CXD ID */
		0x01,									/* bNrChannels */
		0x00, 0x00, 0x00, 0x00,					/* bmChannelConfig 0x00000003 - Left&Right; 0x0000 - Mono; 0x0004 - FrontCenter */
		0x00,									/* iChannelNames */
		0x00,	0x00,							/* bmControls */
		0x00,									/* iTerminal */
		/* 17 byte*/

		/* USB Speaker Audio Feature Unit Descriptor */
		0x0E, 									/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_CONTROL_FEATURE_UNIT,				/* bDescriptorSubtype */
		0x30,									/* bUnitID */
		0x20,									/* bSourceID */
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00,                                 	/* iTerminal */
		/* 6 + ((ch + 1) * 4) byte */

		/* USB Speaker Output Terminal Descriptor */
		0x0C,									/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_CONTROL_OUTPUT_TERMINAL,			/* bDescriptorSubtype */
		0x40,									/* bTerminalID */
		0x01,									/* wTerminalType  0x0301, headphones*/
		0x03,
		0x00,									/* bAssocTerminal */
		0x30,									/* bSourceID */
		0x10,									/* bCSourceID - CXD ID */
		0x00, 0x00,								/* bmControls */
		0x00,									/* iTerminal */
		/* 12 byte*/


		/* Standard AS Interface Descriptor - Audio Streaming Zero Bandwith
		   Interface 1, Alternate Setting 0 */
		AUDIO_INTERFACE_DESC_SIZE,				/* bLength */
		USB_INTERFACE_DESCRIPTOR_TYPE,			/* bDescriptorType */
		0x01,									/* bInterfaceNumber */
		0x00,									/* bAlternateSetting */
		0x00,									/* bNumEndpoints */
		USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass */
		AUDIO_SUBCLASS_AUDIOSTREAMING,			/* bInterfaceSubClass */
		AUDIO_PROTOCOL_IP_VERSION_02_00,    	/* bInterfaceProtocol */
		0x00,									/* iInterface */
		/* 09 byte*/

		/* Standard AS Interface Descriptor - Audio Streaming
		   Interface 1, Alternate Setting 1 */
		AUDIO_INTERFACE_DESC_SIZE,				/* bLength */
		USB_INTERFACE_DESCRIPTOR_TYPE,			/* bDescriptorType */
		0x01,									/* bInterfaceNumber */
		0x01,									/* bAlternateSetting */
		0x01,									/* bNumEndpoints */
		USB_DEVICE_CLASS_AUDIO,					/* bInterfaceClass */
		AUDIO_SUBCLASS_AUDIOSTREAMING,			/* bInterfaceSubClass */
		AUDIO_PROTOCOL_IP_VERSION_02_00,		/* bInterfaceProtocol */
		0x00,									/* iInterface */
		/* 09 byte*/

		/* Class-specific AS Interface Descriptor */
		0x10,									/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_STREAMING_GENERAL,				/* bDescriptorSubtype */
		0x20,									/* bTerminalLink - Input Terminal */
		0x00,									/* active alt settings r/w, valid alt settings r */
		AUDIO_FORMAT_TYPE_I,					/* bFormatType */
		0x01, 0x00, 0x00, 0x00,					/* bFormatTag - AUDIO_FORMAT_PCM  0x0001*/
		0x01,									/* bNrChannels */
		0x00, 0x00, 0x00, 0x00,					/* L+R front */
		0x00,									/* iChannelNames */
		/* 16 byte*/

		/* USB Speaker Audio Type I Format Interface Descriptor */
		0x06,									/* bLength */
		AUDIO_INTERFACE_DESCRIPTOR_TYPE,		/* bDescriptorType */
		AUDIO_STREAMING_FORMAT_TYPE,			/* bDescriptorSubtype */
		AUDIO_FORMAT_TYPE_I,					/* bFormatType */
		0x02,									/* bSubFrameSize; bytes per frame */
		0x18,									/* bBitResolution; bits per sample */
		/* 06 byte*/


		/* Endpoint 1 - Standard Descriptor */
		0x07,									/* bLength */
		USB_ENDPOINT_DESCRIPTOR_TYPE,			/* bDescriptorType */
		AUDIO_OUT_EP,							/* bEndpointAddress 1 out endpoint*/
		0x01,									/* bmAttributes, isochronus, async, explict fb */
		0xC0, 0x00,								/* wMaxPacketSize in Bytes (Freq(Samples)*2(Stereo)*2(HalfWord)) */
		0x04,									/* bInterval - One packet per 2 uframe, each uF 125us, so 192khz */
		/* 07 byte*/

		/* Endpoint - Audio Streaming Descriptor*/
		0x08,									/* bLength */
		AUDIO_ENDPOINT_DESCRIPTOR_TYPE,			/* bDescriptorType */
		AUDIO_ENDPOINT_GENERAL,					/* bDescriptor */
		0x00,									/* bmAttributes, default 0x00 */
		0x00,									/* bmControls, default 0x00 */
		0x00,									/* bLockDelayUnits */
		0x00, 0x00, 							/* wLockDelay */
		/* 08 byte*/
};
#endif


/* Extern function -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* AUDIO Device library callbacks */
static uint8_t  usbd_audio_Init       (void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_audio_DeInit     (void  *pdev, uint8_t cfgidx);
static uint8_t  usbd_audio_Setup      (void  *pdev, USB_SETUP_REQ *req);
static uint8_t  usbd_audio_EP0_RxReady(void *pdev);
static uint8_t  usbd_audio_DataIn     (void *pdev, uint8_t epnum);
static uint8_t  usbd_audio_DataOut    (void *pdev, uint8_t epnum);
static uint8_t  usbd_audio_SOF        (void *pdev);
//static uint8_t  usbd_audio_OUT_Incplt (void  *pdev);

/* AUDIO Requests management functions */
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req);
static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req);
static uint8_t  *USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length);

/* Other support functions */
//static volatile uint8_t* AudioBufferCopy(volatile uint8_t *InAddr, volatile uint8_t *OutAddr, uint16_t Size, uint8_t DataWidht);


/* AUDIO interface class callbacks structure */
USBD_Class_cb_TypeDef  AUDIO_cb =
{
  usbd_audio_Init,
  usbd_audio_DeInit,
  usbd_audio_Setup,
  NULL, 					/* EP0_TxSent */
  usbd_audio_EP0_RxReady,
  usbd_audio_DataIn,
  usbd_audio_DataOut,
  usbd_audio_SOF,
  NULL,
  NULL, 					/* usbd_audio_OUT_Incplt */
  USBD_audio_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
  USBD_audio_GetCfgDesc, 	/* use same config as per FS */
#endif
};


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : usbd_audio_Init
* Description    : Initilaizes the AUDIO interface
* Input          : pdev - device instance
* 				 : cfgidx - Configuration index
* Return         : None
*******************************************************************************/
//static void usbd_audio_FeedBack(void *pdev)
//{
//	uint8_t bufs[3];
//	uint32_t FB_rate;
//	int32_t FB_rate_t;
//	static volatile float FB_rate_div = 10;
//
//	if (AUDIO_FREQUENCY == 44100) FB_rate = (44 << 14) + ((1 << 14) / 10);
//	else FB_rate = (AUDIO_FREQUENCY / 1000) << 14;
//
//	FB_rate_t = (int)(((float)AUDIO_FREQUENCY - (float)Frequency_IN) / FB_rate_div);
//	FB_rate += FB_rate_t;
//
//	bufs[0] = (uint8_t)(FB_rate);
//	bufs[1] = (uint8_t)(FB_rate >> 8);
//	bufs[2] = (uint8_t)(FB_rate >> 16);
//
////	DCD_EP_Flush(pdev, AUDIO_IN_EP);
//	DCD_EP_Tx (pdev, AUDIO_IN_EP, bufs, 3);
//}


/**
* @brief  usbd_audio_Init
*         Initilaizes the AUDIO interface.
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
static uint8_t usbd_audio_Init (void  *pdev, uint8_t cfgidx)
{
	/* Open EP OUT/IN */
//	DCD_EP_Open(pdev, AUDIO_IN_EP, AUDIO_IN_PACKET, USB_OTG_EP_ISOC);
	DCD_EP_Open(pdev, AUDIO_OUT_EP, AUDIO_OUT_PACKET, USB_OTG_EP_ISOC);

	/* Initialize the Audio output Hardware layer */
	if (AudioInterface.Audio_Fops->Init() != USBD_OK)
	{
		return USBD_FAIL;
	}

	/* Prepare Out endpoint to receive audio data */
	DCD_EP_PrepareRx(pdev, AUDIO_OUT_EP, (uint8_t*)IsocOutBuff, AUDIO_OUT_PACKET);

	return USBD_OK;
}

/**
* @brief  usbd_audio_DeInit
*         DeInitializes the AUDIO layer.
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
static uint8_t  usbd_audio_DeInit (void  *pdev, uint8_t cfgidx)
{
	DCD_EP_Close (pdev , AUDIO_OUT_EP);
	DCD_EP_Close (pdev , AUDIO_IN_EP);

	/* DeInitialize the Audio output Hardware layer */
	if (AudioInterface.Audio_Fops->DeInit() != USBD_OK)
	{
		return USBD_FAIL;
	}

	return USBD_OK;
}

/**
  * @brief  usbd_audio_Setup
  *         Handles the Audio control request parsing.
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  usbd_audio_Setup (void  *pdev, USB_SETUP_REQ *req)
{
	uint16_t len = 0;
	uint8_t  *pbuf;

	switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{

	/* AUDIO Class Requests -------------------------------*/
	case USB_REQ_TYPE_CLASS :

		switch (req->bRequest)
		{
		case AUDIO_REQ_GET_CUR:
			AUDIO_Req_GetCurrent(pdev, req);
			break;

		case AUDIO_REQ_SET_CUR:
			AUDIO_Req_SetCurrent(pdev, req);
			break;

		default:
			USBD_CtlError (pdev, req);
			return USBD_FAIL;
		}
	break;

	/* Standard Requests -------------------------------*/
	case USB_REQ_TYPE_STANDARD:

		switch (req->bRequest)
		{
		case USB_REQ_GET_DESCRIPTOR:
			if( (req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
			{
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
				pbuf = usbd_audio_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
#else
				pbuf = usbd_audio_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
#endif
				len = MIN(USB_AUDIO_DESC_SIZ , req->wLength);

				USBD_CtlSendData (pdev, pbuf, len);
			}
			break;

		case USB_REQ_GET_INTERFACE :
			USBD_CtlSendData (pdev, (uint8_t *)&usbd_audio_AltSet, 1);
		break;

		case USB_REQ_SET_INTERFACE :
			if ((uint8_t)(req->wValue) < AUDIO_TOTAL_IF_NUM)
			{
				usbd_audio_AltSet = (uint8_t)(req->wValue);
			}
			else
			{
				/* Call the error management function (command will be nacked) */
				USBD_CtlError(pdev, req);
			}
		break;
		}
	break;

	default: break;
	}

	return USBD_OK;
}

/**
  * @brief  usbd_audio_EP0_RxReady
  *         Handles audio control requests data.
  * @param  pdev: device device instance
  * @retval status
  */
static uint8_t  usbd_audio_EP0_RxReady (void  *pdev)
{
	/* Check if an AudioControl request has been issued */
	if (AudioCtlCmd == AUDIO_REQ_SET_CUR)
	{	/* In this driver, to simplify code, only SET_CUR request is managed */
		/* Check for which addressed unit the AudioControl request has been issued */
		if (AudioCtlUnit == AUDIO_OUT_STREAMING_CTRL)
		{	/* In this driver, to simplify code, only one unit is manage */
			/* Call the audio interface mute function */
			AudioInterface.Audio_Fops->MuteCtl(AudioCtl[0]);

			/* Reset the AudioCtlCmd variable to prevent re-entering this function */
			AudioCtlCmd = 0;
			AudioCtlLen = 0;
		}
	}

	return USBD_OK;
}


/**
  * @brief  usbd_audio_DataIn
  *         Handles the audio IN data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_audio_DataIn (void *pdev, uint8_t epnum)
{
	return USBD_OK;
}


/**
  * @brief  usbd_audio_DataOut
  *         Handles the Audio Out data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_audio_DataOut (void *pdev, uint8_t epnum)
{
//	const uint8_t *EndAudioBuffer = AudioBuffer + AUDIO_BUFFER_SIZE_BYTES;
	uint32_t XferCount = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;// ;
//	uint32_t _32bit_XferCountPacked = (XferCount * 4) / AUDIO_FRAME_SIZE;
//	uint16_t TransferCropSize;
//    uint16_t i = 0;


	if (epnum == AUDIO_OUT_EP)
	{
		IsocOutWrPtr += XferCount;

		/* Increment the Buffer pointer or roll it back when all buffers are full */
		if (IsocOutWrPtr >= (IsocOutBuff + (AUDIO_OUT_PACKET * 4)))
		{
			/* Buffer are full: roll back */
			IsocOutWrPtr = (uint8_t*)IsocOutBuff;
		}
//		else if(IsocOutWrPtr > EndAudioBuffer - _32bit_XferCountPacked )
//		{
//			/* Crop transfer and copy */
//			TransferCropSize = ((EndAudioBuffer - IsocOutWrPtr) / 4) * AUDIO_FRAME_SIZE;
//			AudioBufferCopy(IsocOutBuff, IsocOutWrPtr, TransferCropSize, AUDIO_FRAME_SIZE);
//
//			/* Buffer are full: roll back and copy rest data */
//			IsocOutWrPtr = AudioBufferCopy(IsocOutBuff + TransferCropSize, AudioBuffer,
//					XferCount - TransferCropSize, AUDIO_FRAME_SIZE);
//		}
		else
		{
			/* Copy data and increment the buffer pointer */
//			IsocOutWrPtr = AudioBufferCopy(IsocOutBuff, IsocOutWrPtr, XferCount, AUDIO_FRAME_SIZE);
		}

		/* Toggle the frame index */
		((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame =
				(((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame)? 0:1;

		/* Prepare Out endpoint to receive next audio packet */
		DCD_EP_PrepareRx(pdev, AUDIO_OUT_EP, (uint8_t*)(IsocOutWrPtr), AUDIO_OUT_PACKET);




		/* Trigger the start of streaming only when half buffer is full */
		if ((AudioPlayState == 0) && (IsocOutWrPtr >= (IsocOutBuff + (AUDIO_OUT_PACKET * 2))))
		{
			/* Enable streaming */
			AudioPlayState = 1;
		}
	}

	return USBD_OK;
}


/**
  * @brief  usbd_audio_SOF
  *         Handles the SOF event (data buffer update and synchronization).
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_audio_SOF (void *pdev)
{
	uint32_t i = 0;
//	static uint8_t Start = 0;
	static uint32_t AudioBuffer_CopyIndex = 0;

	/* Explit FeedBack for AUDIO_OUT_FB */
//	if(usbd_audio_AltSet != 0)
//	{
//		if(i++ >= (1<<5))
//		{
//			usbd_audio_FeedBack(pdev);
//			i = 0;
//		}
//	}

	/* If Play started */
	if(AudioPlayState != 0)
	{
		AudioInterface.Audio_Fops->AudioCmd(AUDIO_CMD_PLAY);

		/* Increment sub buffer read poiner */
		IsocOutRdPtr += AUDIO_OUT_PACKET;

		/* If end of sub buffer roll back to the start */
		if ( IsocOutRdPtr >= (IsocOutBuff + (AUDIO_OUT_PACKET * 4)) )
		{
			IsocOutRdPtr = (uint8_t*)IsocOutBuff;
		}

		/* If all available sub buffer have been consumed, stop playing */
		if (IsocOutRdPtr == IsocOutWrPtr)
		{
			/* Pause the audio stream */
			AudioInterface.Audio_Fops->AudioCmd(AUDIO_CMD_PAUSE);

			/* Stop entering play loop */
			AudioPlayState = 0;

			/* Reset buffer pointers */
			IsocOutRdPtr = (uint8_t*)IsocOutBuff;
			IsocOutWrPtr = (uint8_t*)IsocOutBuff;
		}
		else
		{
			/* Increment destination main audio buffer index */
			AudioBuffer_CopyIndex += AUDIO_OUT_PACKET;

			/* If end of the main audio buffer roll back to the start */
			if ( AudioBuffer_CopyIndex >= AUDIO_BUFFER_SIZE_BYTES )
			{
				AudioBuffer_CopyIndex = 0;
				AudioBuffer_HalfFull = RESET;
			}
			else if ( AudioBuffer_CopyIndex >= (AUDIO_BUFFER_SIZE_BYTES >> 1) )
			{
				AudioBuffer_HalfFull = SET;
			}

			/* Copy from sub-buffer to main audio buffer */
			do
			{
				AudioBuffer[AudioBuffer_CopyIndex + i] =  *(IsocOutRdPtr + i);
				i++;
			} while (i < AUDIO_OUT_PACKET);
		}


//		if(usbd_audio_AltSet != 0)
//		{
////			if(Start != 1){ TIM2->CNT = 0; Start = 1; }
//
//			/* Start playing received packet */
//			AudioInterface.Audio_Fops->AudioCmd(AUDIO_CMD_PLAY);           /* Command to be processed */
//		}
//		else //if(usbd_audio_AltSet == 0)
//		{
//			/* Pause the audio stream */
//			AudioInterface.Audio_Fops->AudioCmd(AUDIO_CMD_PAUSE);
//
//			/* Reset all */
//			WR_Delta_Out = AUDIO_BUFFER_SIZE_BYTES >> 1;
//			IsocOutWrPtr = (uint8_t*)AudioBuffer;
//			Start = PlayFlag = 0;
////			USB_ReciveAudioPackedCnt = 0;
//			SOF_Cnt = 0;
//		}
	}

	return USBD_OK;
}




/******************************************************************************
     AUDIO Class Requests Management
******************************************************************************/
/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req)
{
	/* Send the current mute state */
	USBD_CtlSendData (pdev, AudioCtl, req->wLength);
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req)
{
	if (req->wLength)
	{
		/* Prepare the reception of the buffer over EP0 */
		USBD_CtlPrepareRx (pdev, AudioCtl, req->wLength);

		/* Set the global variables indicating current request and its length
    	   to the function usbd_audio_EP0_RxReady() which will process the request */
		AudioCtlCmd = AUDIO_REQ_SET_CUR;     /* Set the request value */
		AudioCtlLen = req->wLength;          /* Set the request data length */
		AudioCtlUnit = HIBYTE(req->wIndex);  /* Set the request target unit */
	}
}


/**
  * @brief  USBD_audio_GetCfgDesc
  *         Returns configuration descriptor.
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  	*length = sizeof (usbd_audio_CfgDesc);
  	return usbd_audio_CfgDesc;
}


/**
  * @brief  AudioBufferCopy
  * @param
  * @param
  * @retval
  */
//static volatile uint8_t* AudioBufferCopy(volatile uint8_t *InAddr, volatile uint8_t *OutAddr, uint16_t Size, uint8_t DataWidht)
//{
//	uint16_t RealDataSizePacked = Size / DataWidht;
//	uint16_t i, j, k;
//
//	for(i = 0; i < RealDataSizePacked; i++)
//	{
//		for(j = 0; j < DataWidht; j++)
//		{
//			(*OutAddr) = (*InAddr);
//			OutAddr++;
//			InAddr++;
//		}
//		for(k = 0; k < (4 - DataWidht); k++)
//		{
//			(*OutAddr) = 0;
//			OutAddr++;
//		}
//	}
//
//	return OutAddr;
//}


