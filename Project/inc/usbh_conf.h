/*
 * usbh_conf.h
 *
 *  Created on: 30/12/2015
 *      Author: Mori
 */

#ifndef USBH_CONF_H_
#define USBH_CONF_H_

#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"

#define USBH_MAX_NUM_ENDPOINTS                3
#define USBH_MAX_NUM_INTERFACES               3
#define USBH_MAX_NUM_CONFIGURATION            1
#define USBH_MAX_NUM_SUPPORTED_CLASS          4
#define USBH_KEEP_CFG_DESCRIPTOR              0
#define USBH_MAX_SIZE_CONFIGURATION           0x200
#define USBH_MAX_DATA_BUFFER                  0x200
#define USBH_DEBUG_LEVEL                      3

#define USBH_USE_OS                           0

#define ID_USB_HOST_FS 						  0
#define ID_USB_HOST_HS 						  1


 /* Memory management macros */
#define USBH_malloc               malloc
#define USBH_free                 free
#define USBH_memset               memset
#define USBH_memcpy               memcpy


 /* DEBUG macros */
#if (USBH_DEBUG_LEVEL > 0)
#define  USBH_UsrLog 	LOG
#else
#define USBH_UsrLog(...)
#endif


#if (USBH_DEBUG_LEVEL > 1)
#define  USBH_ErrLog 	LOG
#else
#define USBH_ErrLog(...)
#endif


#if (USBH_DEBUG_LEVEL > 2)
#define  USBH_DbgLog	LOG
#else
#define USBH_DbgLog(...)
#endif


#endif /* USBH_CONF_H_ */
