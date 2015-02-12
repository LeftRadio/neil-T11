/**
  ******************************************************************************
  * @file	 	  usbd_audio_core.h
  * @author  	Neil Lab :: Left Radio
  * @version
  * @date
  * @brief		header file for the usbd_audio_core.c file.
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_AUDIO_CORE_H_
#define __USB_AUDIO_CORE_H_

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"
#include "usbd_req.h"
#include "usbd_desc.h"


/* Exported define -----------------------------------------------------------*/
#define	USE_AUDIO_CLASS_01_00
//#define	USE_AUDIO_CLASS_02_00

//#define AUDIO_CHANNELS_COUNT_1							((uint8_t)(1))
#define AUDIO_CHANNELS_COUNT_2							((uint8_t)(2))
//#define AUDIO_CHANNELS_COUNT_4						((uint8_t)(4))

#ifdef AUDIO_CHANNELS_COUNT_1
	#define AUDIO_CHANNELS_COUNT						AUDIO_CHANNELS_COUNT_1
	#define AUDIO_CHANNELS_INPUT_TERMINAL_SETTINGS		((uint16_t)(0x0001))
#elif defined AUDIO_CHANNELS_COUNT_2
	#define AUDIO_CHANNELS_COUNT						AUDIO_CHANNELS_COUNT_2
	#define AUDIO_CHANNELS_INPUT_TERMINAL_SETTINGS		((uint16_t)(0x0001 | 0x0002))
#elif defined AUDIO_CHANNELS_COUNT_4
	#define AUDIO_CHANNELS_COUNT						AUDIO_CHANNELS_COUNT_4
	#define AUDIO_CHANNELS_INPUT_TERMINAL_SETTINGS		(uint16_t)(0x0001 | 0x0002 | 0x0004 | 0x0008)
#endif

#define AUDIO_FRAME_SIZE								((uint8_t)(2))
#define AUDIO_BITRESOLUTION								((uint8_t)(AUDIO_FRAME_SIZE * 8))

/* Audio frequency in Hz */
#define AUDIO_FREQUENCY                					((uint32_t)48000) 	/* Audio frequency in Hz.*/
#define SAMPLE_FREQ										(uint8_t)(AUDIO_FREQUENCY & 0x000000FF), (uint8_t)((AUDIO_FREQUENCY >> 8)), (uint8_t)((AUDIO_FREQUENCY >> 16))

/* Audio default start volume */
#define DEFAULT_VOLUME                  				((uint8_t)80)

/* Audio streaming IN/OUT endpoints */
#define AUDIO_OUT_EP                    				0x01
#define AUDIO_IN_EP                     				0x81

/* AudioFreq * DataSize (AUDIO_FRAME_SIZE in bytes) * NumChannels (Stereo: 2) */
#define AUDIO_OUT_PACKET								(((uint32_t)((AUDIO_FREQUENCY * (uint32_t)AUDIO_FRAME_SIZE * (uint32_t)AUDIO_CHANNELS_COUNT)  / 1000)))
#define AUDIO_IN_PACKET									((uint32_t)(64))

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
//#define OUT_PACKET_NUM                                	4

/* Total size of the audio transfer buffer */
//#define TOTAL_OUT_BUF_SIZE								((uint32_t)(AUDIO_OUT_PACKET * OUT_PACKET_NUM))
#define AUDIO_BUFFER_SIZE_BYTES						 	((uint32_t)(AUDIO_OUT_PACKET * 100))		            // bytes per 100 ms * AUDIO_FRAME_SIZE * 2 chennels
#define AUDIO_BUFFER_HALFSIZE						  	((uint32_t)(AUDIO_BUFFER_SIZE_BYTES / 2))
#define AUDIO_BUFFER_END								((uint32_t)(IsocOutBuff + AUDIO_BUFFER_SIZE_BYTES))


/* Descriptions sizes for full audio configuration, terminals, interfaces and endpoints */
#ifdef USE_AUDIO_CLASS_02_00
	#define USB_AUDIO_DESC_SIZ                          141		// 160
#else
	#define USB_AUDIO_DESC_SIZ                          100		// 109
#endif

#define AUDIO_TOTAL_IF_NUM              				0x02
#define AUDIO_CONFIG_DESC_SIZE							(USB_AUDIO_DESC_SIZ + 9)		// USB_AUDIO_DESC_SIZ + 9
#define AUDIO_INTERFACE_DESC_SIZE						9

#define AUDIO_INPUT_TERMINAL_DESC_SIZE					0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE					0x09

#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE				0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE				0x07
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE				0x07

/* Audio Descriptor Types */
#define AUDIO_DESCRIPTOR_TYPE                         	0x21
#define AUDIO_IAD_DESCRIPTOR_TYPE					  	0x0B
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               	0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                	0x25

/* Audio Control Interface Descriptor SubTypes */
#define AUDIO_CONTROL_HEADER                          	0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  	0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 	0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    	0x06
#define AUDIO_CONTROL_CLOCK_SOURSE                    	0x0A
#define AUDIO_CONTROL_CLOCK_SELECTOR         			0x0B

/* Audio Descriptor Class and SubClass Types */
#define AUDIO_SUBCLASS_UNDEFINED                 		0x00
#define USB_DEVICE_CLASS_AUDIO                        	0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   	0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 	0x02

/* Audio Descriptor Protocol Types */
#define AUDIO_PROTOCOL_UNDEFINED                      	0x00
#define AUDIO_PROTOCOL_IP_VERSION_02_00		   		  	0x20		//! IP_VERSION_02_00 ie UAC V2

/* Audio streaming Types */
#define AUDIO_STREAMING_GENERAL							0x01
#define AUDIO_STREAMING_FORMAT_TYPE						0x02
#define AUDIO_STREAMING_FORMAT_SPECIFIC         		0x03

/* Audio format Types */
//#define AUDIO_AS_FORMAT_TYPE							0x01
#define AUDIO_FORMAT_TYPE_I								0x01
#define AUDIO_FORMAT_TYPE_II							0x02
#define AUDIO_FORMAT_TYPE_III							0x03

/* Audio endpoints Types and SubTypes */
#define AUDIO_ENDPOINT_GENERAL							0x01
#define USB_ENDPOINT_TYPE_ISOCHRONOUS					0x01
#define USB_ENDPOINT_SUBTYPE_ASYNC                 		0x04
#define USB_ENDPOINT_SUBTYPE_IMPLICT_FB            		0x20

#ifdef USE_USB_OTG_HS
	#define AUDIO_SPK_BMA_CONTROLS           			0x0000000F 		// Mute readable and writable
	#define AUDIO_SPK_BMA_CONTROLS_CH_1		   			0x00000000		//
	#define AUDIO_SPK_BMA_CONTROLS_CH_2		  			0x00000000
#else
	#define AUDIO_CONTROL_MUTE                          0x0001
#endif

/* Audio Terminals Sourse ID */
#define AUDIO_INPUT_TERMINAL_ID						((uint8_t)0x01)
#define AUDIO_FEATURE_UNIT_TERMINAL_ID				((uint8_t)(AUDIO_INPUT_TERMINAL_ID + 1))
#define AUDIO_OUTPUT_TERMINAL_ID					((uint8_t)(AUDIO_FEATURE_UNIT_TERMINAL_ID + 1))

#define AUDIO_REQ_GET_CUR                             0x81
#define AUDIO_REQ_SET_CUR                             0x01

#define AUDIO_OUT_STREAMING_CTRL                      0x02


/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern USBD_Class_cb_TypeDef  AUDIO_cb;

extern volatile uint8_t AudioBuffer[AUDIO_BUFFER_SIZE_BYTES];
extern volatile uint8_t IsocOutBuff[AUDIO_OUT_PACKET * 4];

extern uint8_t* IsocOutWrPtr;
extern uint32_t SOF_Cnt, USB_ReciveAudioPackedCnt;
extern float Frequency_IN;
//extern int32_t Frequency_OUT;

/* Exported function ---------------------------------------------------------*/





#endif /* ___H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
