/**
  ******************************************************************************
  * @file    usb_conf.h
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   General low level driver configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CONF__H__
#define __USB_CONF__H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup USB_OTG_DRIVER
  * @{
  */

/** @defgroup USB_CONF
  * @brief USB low level driver configuration file
  * @{
  */

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */
#ifdef USE_USB_OTG_FS
 #define USB_OTG_FS_CORE
#endif

#ifdef USE_USB_OTG_HS
 #define USB_OTG_HS_CORE
#endif

#ifdef USE_ULPI_PHY
	#define USB_OTG_ULPI_PHY_ENABLED
#endif
	#ifdef USE_EMBEDDED_PHY
	#define USB_OTG_EMBEDDED_PHY_ENABLED
	#endif
#endif

#ifndef USE_USB_OTG_HS
 #ifndef USE_USB_OTG_FS
    #error  "USE_USB_OTG_HS or USE_USB_OTG_FS should be defined"
 #endif
#else //USE_USB_OTG_HS
 #ifndef USE_ULPI_PHY
  #ifndef USE_EMBEDDED_PHY
     #error  "USE_ULPI_PHY or USE_EMBEDDED_PHY should be defined"
  #endif
 #endif
#endif

/****************** USB OTG HS CONFIGURATION **********************************/
#ifdef USB_OTG_HS_CORE
 #define RX_FIFO_HS_SIZE                           384
 #define TX0_FIFO_HS_SIZE                          64
 #define TX1_FIFO_HS_SIZE                          192
 #define TX2_FIFO_HS_SIZE                          0
 #define TX3_FIFO_HS_SIZE                          0
 #define TX4_FIFO_HS_SIZE                          0
 #define TX5_FIFO_HS_SIZE                          0
 #define TXH_NP_HS_FIFOSIZ                         96
 #define TXH_P_HS_FIFOSIZ                          96

// #define USB_OTG_HS_LOW_PWR_MGMT_SUPPORT
// #define USB_OTG_HS_SOF_OUTPUT_ENABLED
#define USB_OTG_HS_INTERNAL_DMA_ENABLED
//#define USB_OTG_HS_DEDICATED_EP1_ENABLED


/****************** USB OTG FS CONFIGURATION **********************************/
#ifdef USB_OTG_FS_CORE
 #define RX_FIFO_FS_SIZE                          128
 #define TX0_FIFO_FS_SIZE                          64
 #define TX1_FIFO_FS_SIZE                         128
 #define TX2_FIFO_FS_SIZE                          0
 #define TX3_FIFO_FS_SIZE                          0
 #define TXH_NP_HS_FIFOSIZ                         96
 #define TXH_P_HS_FIFOSIZ                          96

// #define USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
// #define USB_OTG_FS_SOF_OUTPUT_ENABLED
#endif


/****************** USB OTG MISC CONFIGURATION ********************************/
#define VBUS_SENSING_ENABLED
#define USB_OTG_INTERNAL_VBUS_ENABLED
//#define USB_OTG_EXTERNAL_VBUS_ENABLED


/****************** USB OTG MODE CONFIGURATION ********************************/
//#define USE_HOST_MODE
#define USE_DEVICE_MODE
//#define USE_OTG_MODE

#ifndef USE_DEVICE_MODE
 #ifndef USE_HOST_MODE
    #error  "USE_DEVICE_MODE or USE_HOST_MODE should be defined"
 #endif
#endif



/****************** C Compilers dependant keywords ****************************/
/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined   (__GNUC__)        /* GNU Compiler */
    #define __ALIGN_END    __attribute__ ((aligned (4)))
    #define __ALIGN_BEGIN
  #else
    #define __ALIGN_END
    #if defined   (__CC_ARM)      /* ARM Compiler */
      #define __ALIGN_BEGIN    __align(4)
    #elif defined (__ICCARM__)    /* IAR Compiler */
      #define __ALIGN_BEGIN
    #elif defined  (__TASKING__)  /* TASKING Compiler */
      #define __ALIGN_BEGIN    __align(4)
    #endif /* __CC_ARM */
  #endif /* __GNUC__ */
#else
  #define __ALIGN_BEGIN
  #define __ALIGN_END
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* __packed keyword used to decrease the data type alignment to 1-byte */
#if defined (__CC_ARM)         /* ARM Compiler */
  #define __packed    __packed
#elif defined (__ICCARM__)     /* IAR Compiler */
  #define __packed    __packed
#elif defined   ( __GNUC__ )   /* GNU Compiler */
  #define __packed    __attribute__ ((__packed__))
#elif defined   (__TASKING__)  /* TASKING Compiler */
  #define __packed    __unaligned
#endif /* __CC_ARM */

/**
  * @}
  */


/** @defgroup USB_CONF_Exported_Types
  * @{
  */
/**
  * @}
  */


/** @defgroup USB_CONF_Exported_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_CONF_Exported_Variables
  * @{
  */
/**
  * @}
  */

/** @defgroup USB_CONF_Exported_FunctionsPrototype
  * @{
  */
/**
  * @}
  */


#endif //__USB_CONF__H__
