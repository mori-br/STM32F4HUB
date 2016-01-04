/**
  ******************************************************************************
  * @file    usbh_hid_mouse.h 
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contains all the prototypes for the usbh_hid_mouse.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HID_MOUSE_H
#define __USBH_HID_MOUSE_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbh_hid.h"

/*typedef struct _HID_MOUSE_Info
{
	int16_t    x;
	int16_t    y;
	uint8_t    wheel;
	uint16_t   button;

} HID_MOUSE_Info_TypeDef;*/

 typedef struct _HID_MOUSE_Info
 {
   uint8_t              x;
   uint8_t              y;
   uint8_t              buttons[3];
 }
 HID_MOUSE_Info_TypeDef;


USBH_StatusTypeDef USBH_HID_MouseInit(USBH_HandleTypeDef *phost);
HID_MOUSE_Info_TypeDef *USBH_HID_GetMouseInfo(USBH_HandleTypeDef *phost);
//USBH_StatusTypeDef USBH_HID_Mouse_DecodeData(USBH_HandleTypeDef *phost, HID_Report_ItemTypedef *parser_data, uint8_t *pData, uint16_t length);


#ifdef __cplusplus
}
#endif

#endif /* __USBH_HID_MOUSE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
