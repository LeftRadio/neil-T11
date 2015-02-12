/**
  ******************************************************************************
  * @file
  * @author  	Left Radio
  * @version
  * @date
  * @brief		header
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GLOBAL_INIT_H
#define __GLOBAL_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "systick.h"

/* Exported define -----------------------------------------------------------*/
#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

/* Exported macro ------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function ---------------------------------------------------------*/
void Global_peripheal_Configuration(void);
void DMA_memcpy8(void* pDstAddr, void* pSrcAddr, unsigned int uSize);






#endif /* __GLOBAL_INIT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
