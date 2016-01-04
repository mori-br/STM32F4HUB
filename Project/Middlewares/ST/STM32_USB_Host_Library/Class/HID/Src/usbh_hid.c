/**
  ******************************************************************************
  * @file    usbh_hid.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    19-June-2014
  * @brief   This file is the HID Layer Handlers for USB Host HID class.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse and Keyboard protocols
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid.h"


static USBH_StatusTypeDef USBH_HID_InterfaceInit  (USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_InterfaceDeInit  (USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_ClassRequest(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_Process(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_SOFProcess(USBH_HandleTypeDef *phost);
static USBH_StatusTypeDef USBH_HID_Parameter(USBH_HandleTypeDef *phost, uint8_t param, uint8_t *data, uint16_t *length);

static void  USBH_HID_ParseHIDDesc (HID_DescTypeDef *desc, uint8_t *buf);

extern USBH_StatusTypeDef USBH_HID_MouseInit(USBH_HandleTypeDef *phost);
extern USBH_StatusTypeDef USBH_HID_KeybdInit(USBH_HandleTypeDef *phost);

USBH_ClassTypeDef  HID_Class =
{
	"HID",
	USB_HID_CLASS,
	USBH_HID_InterfaceInit,
	USBH_HID_InterfaceDeInit,
	USBH_HID_ClassRequest,
	USBH_HID_Process,
	USBH_HID_SOFProcess,
	USBH_HID_Parameter,
};

static USBH_StatusTypeDef create_endpoints(USBH_HandleTypeDef *phost, HID_HandleTypeDef *HID_Handle, USBH_InterfaceDescTypeDef *pif)
{
	uint8_t max_ep;
	uint8_t num = 0;

	HID_Handle->state     = HID_INIT;
	HID_Handle->ctl_state = CTRL_STATE_HID_REQ_INIT;

	/* Check fo available number of endpoints */
	/* Find the number of EPs in the Interface Descriptor */
	/* Choose the lower number in order not to overrun the buffer allocated */
	max_ep = ( (pif->bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ? pif->bNumEndpoints : USBH_MAX_NUM_ENDPOINTS);

	/* Decode endpoint IN and OUT address from interface descriptor */
	for (num = 0; num < max_ep; num++)
	{
		if(pif->Ep_Desc[num].bEndpointAddress & 0x80)
		{
			HID_Handle->InEp = (pif->Ep_Desc[num].bEndpointAddress);
			HID_Handle->InPipe  = USBH_AllocPipe(phost, HID_Handle->InEp);

			HID_Handle->ep_addr[0] = pif->Ep_Desc[num].bEndpointAddress;
			HID_Handle->length[0]  = pif->Ep_Desc[num].wMaxPacketSize;
			HID_Handle->poll[0]    = pif->Ep_Desc[num].bInterval;

			if (HID_Handle->poll[0]  < HID_MIN_POLL)
				HID_Handle->poll[0] = HID_MIN_POLL;

			/* Open pipe for IN endpoint */
			USBH_OpenPipe  (phost, HID_Handle->InPipe, HID_Handle->InEp, phost->device.address,
						phost->device.speed, USB_EP_TYPE_INTR, HID_Handle->length[0]);

			USBH_LL_SetToggle (phost, HID_Handle->InPipe, 0);

USBH_UsrLog("IN  size=%d, num %d, addr 0x%02X, pool %d, lowspeed? %d",

		HID_Handle->length[0], HID_Handle->InPipe, HID_Handle->ep_addr[0], HID_Handle->poll[0], (phost->device.speed==2));

		}
		else
		{
			HID_Handle->OutEp = (pif->Ep_Desc[num].bEndpointAddress);
			HID_Handle->OutPipe = USBH_AllocPipe(phost, HID_Handle->OutEp);

			HID_Handle->ep_addr[1] = pif->Ep_Desc[num].bEndpointAddress;
			HID_Handle->length[1]  = pif->Ep_Desc[num].wMaxPacketSize;
			HID_Handle->poll[1]    = pif->Ep_Desc[num].bInterval;

			if (HID_Handle->poll[1]  < HID_MIN_POLL)
				HID_Handle->poll[1] = HID_MIN_POLL;

			/* Open pipe for OUT endpoint */
			USBH_OpenPipe  (phost, HID_Handle->OutPipe, HID_Handle->OutEp, phost->device.address,
                    	phost->device.speed, USB_EP_TYPE_INTR, HID_Handle->length[1]);

			USBH_LL_SetToggle (phost, HID_Handle->OutPipe, 0);

USBH_UsrLog("OUT size=%d, num %d, addr 0x%02X, pool %d",
			HID_Handle->length[1], HID_Handle->OutPipe, HID_Handle->ep_addr[1], HID_Handle->poll[1]);
		}
	}

	return USBH_OK;
}

static HID_HandleTypeDef *create_handle(USBH_HandleTypeDef *phost)
{
	phost->USBH_ClassTypeDef_pData[phost->interfaces] = (HID_HandleTypeDef *)USBH_malloc(sizeof(HID_HandleTypeDef));
	memset(phost->USBH_ClassTypeDef_pData[phost->interfaces], 0, sizeof(HID_HandleTypeDef));
	HID_HandleTypeDef *p = (HID_HandleTypeDef *)phost->USBH_ClassTypeDef_pData[phost->interfaces];
	p->state = HID_ERROR;
	phost->interfaces++;
	return p;
}

/**
  * @brief  USBH_HID_InterfaceInit 
  *         The function init the HID class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_InterfaceInit (USBH_HandleTypeDef *phost)
{
	USBH_StatusTypeDef status = USBH_BUSY;

	static uint8_t if_ix = 0;
	static uint8_t itfnum = 0;
	static uint8_t state = 0;
	static uint8_t num_itf = 0;
	static HID_HandleTypeDef *HID_Handle = 0;
	static USBH_InterfaceDescTypeDef *pif = (USBH_InterfaceDescTypeDef *)0;
	static HID_DescTypeDef hid_desc;

	USBH_CfgDescTypeDef *pcfg = &phost->device.CfgDesc;

	switch(state)
	{
		case 0:
		{
			if_ix = 0;
			pif = (USBH_InterfaceDescTypeDef *)0;

USBH_UsrLog("INTERFACES: %d",phost->device.CfgDesc.bNumInterfaces);

			num_itf = phost->device.CfgDesc.bNumInterfaces;
			if(phost->device.CfgDesc.bNumInterfaces > USBH_MAX_NUM_INTERFACES)
				num_itf = USBH_MAX_NUM_INTERFACES;

			state = 1;
		}
		break;

		case 1:

			HID_Handle = NULL;
			itfnum = phost->device.CfgDesc.Itf_Desc[if_ix].bInterfaceNumber;

			USBH_SelectInterface(phost, itfnum);
			state = 2;

USBH_UsrLog ("Class    : %xh", phost->device.CfgDesc.Itf_Desc[if_ix].bInterfaceClass );
USBH_UsrLog ("SubClass : %xh", phost->device.CfgDesc.Itf_Desc[if_ix].bInterfaceSubClass );
USBH_UsrLog ("Protocol : %xh", phost->device.CfgDesc.Itf_Desc[if_ix].bInterfaceProtocol );

			break;

		case 2:
			/* Get HID Desc */
			if (USBH_HID_GetHIDDescriptor (phost, itfnum, USB_HID_DESC_SIZE)== USBH_OK)
			{
				USBH_HID_ParseHIDDesc(&hid_desc, phost->device.Data);
USBH_UsrLog("HID LEN %d - %d",hid_desc.wItemLength, phost->device.current_interface);
				state = 3;
			}
			break;

		case 3:
			/* Get Report Desc */
			if (USBH_HID_GetHIDReportDescriptor(phost, itfnum, hid_desc.wItemLength) == USBH_OK)
			{
				pif = &pcfg->Itf_Desc[if_ix];

				if(pif->bInterfaceClass == 0x03 && pif->bInterfaceSubClass == 0x01 && pif->bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
				{
USBH_UsrLog ("KeyBoard found.");
					HID_Handle = create_handle(phost);
					HID_Handle->Init =  USBH_HID_KeybdInit;
				}
				else if(pif->bInterfaceClass == 0x03 && pif->bInterfaceSubClass == 0x01 && pif->bInterfaceProtocol == HID_MOUSE_BOOT_CODE)
				{
USBH_UsrLog ("Mouse found.");
					HID_Handle = create_handle(phost);
					HID_Handle->Init =  USBH_HID_MouseInit;
				}

				if(if_ix > 0 && HID_Handle == NULL)
				{
//LOG_ARRAY(phost->device.Data, hid_desc.wItemLength);

					//Usage Page (Generic Desktop) 05 01
					//Usage (Keyboard) 09 06
					//Usage (Mouse) 09 02
					uint16_t usage_page = phost->device.Data[0] << 8 | phost->device.Data[1];
					uint16_t usage = phost->device.Data[2] << 8 | phost->device.Data[3];

					if(usage_page == 0x0501 && usage == 0x0906)
					{
USBH_UsrLog ("KeyBoard found.");
						HID_Handle = create_handle(phost);
						HID_Handle->Init =  USBH_HID_KeybdInit;
					}
					else if(usage_page == 0x0501 && usage == 0x0902)
					{
USBH_UsrLog ("Mouse found.");
						HID_Handle = create_handle(phost);
						HID_Handle->Init =  USBH_HID_MouseInit;
					}
				}

				if(HID_Handle != NULL)
				{
					memcpy(&HID_Handle->HID_Desc, &hid_desc, USB_HID_DESC_SIZE);
				}

				state = 4;
			}
			break;

		case 4:
			pif = &pcfg->Itf_Desc[if_ix];

			if(HID_Handle != NULL)
				create_endpoints(phost, HID_Handle, pif);

			state = 1;

			if(++if_ix >= num_itf)
				state = 5;

			break;

		case 5:

			USBH_SelectInterface(phost, phost->device.CfgDesc.Itf_Desc[0].bInterfaceNumber);
			state = 0;
			status = USBH_OK;
			break;
	}

	return status;
}

/**
  * @brief  USBH_HID_InterfaceDeInit 
  *         The function DeInit the Pipes used for the HID class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_InterfaceDeInit (USBH_HandleTypeDef *phost )
{
USBH_UsrLog("USBH_HID_InterfaceDeInit %d", phost->address);

	uint8_t idx = 0;
	for(; idx < phost->interfaces; ++idx)
	{
		HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[idx];

		if(HID_Handle->InPipe != 0x00)
		{
			USBH_LL_StopHC(phost, HID_Handle->InPipe);

			USBH_ClosePipe  (phost, HID_Handle->InPipe);
			USBH_FreePipe  (phost, HID_Handle->InPipe);

			HID_Handle->InPipe = 0;     /* Reset the pipe as Free */
		}
  
		if(HID_Handle->OutPipe != 0x00)
		{
			USBH_LL_StopHC(phost, HID_Handle->OutPipe);

			USBH_ClosePipe(phost, HID_Handle->OutPipe);
			USBH_FreePipe  (phost, HID_Handle->OutPipe);

			HID_Handle->OutPipe = 0;     /* Reset the pipe as Free */
		}
  
		if(phost->USBH_ClassTypeDef_pData[idx] != NULL)
			USBH_free(phost->USBH_ClassTypeDef_pData[idx]);

		phost->USBH_ClassTypeDef_pData[idx] = NULL;
	}

	phost->interfaces = 0;

	return USBH_OK;
}

/**
  * @brief  USBH_HID_ClassRequest 
  *         The function is responsible for handling Standard requests
  *         for HID class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_ClassRequest(USBH_HandleTypeDef *phost)
{   
	USBH_StatusTypeDef status         = USBH_BUSY;
	USBH_StatusTypeDef classReqStatus = USBH_BUSY;

	HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[0];

	/* Switch HID state machine */
	switch (HID_Handle->ctl_state)
	{
		case CTRL_STATE_HID_REQ_INIT:
		case CTRL_STATE_HID_REQ_GET_HID_DESC:
		case CTRL_STATE_HID_REQ_GET_REPORT_DESC:
			HID_Handle->ctl_state = CTRL_STATE_HID_REQ_SET_IDLE;
			break;

		//--------------------------------------------------------------------------------------------------
		// Device type 		   Get Report	Set Report 	 Get Idle 	Set Idle   Get Protocol   Set Protocol
		// Boot Mouse 			Required  	 Optional 	 Optional 	Optional 	Required 		Required
		// Non-Boot Mouse 		Required 	 Optional 	 Optional 	Optional 	Optional 		Optional
		// Boot Keyboard 		Required 	 Optional 	 Required 	Required 	Required 		Required
		// Non-Boot Keyboard 	Required 	 Optional 	 Required 	Required 	Optional 		Optional
		// Other Device 		Required 	 Optional 	 Optional 	Optional 	Optional 		Optional
		//--------------------------------------------------------------------------------------------------
		case CTRL_STATE_HID_REQ_SET_IDLE:
			//HID class request 'Set Idle', to tell the device only to respond to an interrupt IN transaction if a new event occurs.
			//(In any case when an IN request is sent and there is no change to report, the device will respond with a NAK packet.
			classReqStatus = USBH_HID_SetIdle (phost, 0, 0);
    
			/* set Idle */
			if (classReqStatus == USBH_OK)
			{
				HID_Handle->ctl_state = CTRL_STATE_HID_REQ_SET_PROTOCOL;
			}
			else if(classReqStatus == USBH_NOT_SUPPORTED)
			{
				HID_Handle->ctl_state = CTRL_STATE_HID_REQ_SET_PROTOCOL;
			}
			break;
    
		case CTRL_STATE_HID_REQ_SET_PROTOCOL:
			/* set protocol */
			classReqStatus = USBH_HID_SetProtocol (phost, 0);

			if (classReqStatus == USBH_OK || classReqStatus == USBH_NOT_SUPPORTED)
			{
				HID_Handle->ctl_state = CTRL_STATE_HID_REQ_IDLE;

				/* all requests performed*/
				if(phost->pUser != NULL)
					phost->pUser(phost, HOST_USER_CLASS_ACTIVE);

				phost->busy = 0;
				status = USBH_OK;
			}
			break;

		case CTRL_STATE_HID_REQ_IDLE:
		default:
			break;
	}
  
	return status;
}

static void USBH_HID_IncrementInterface(USBH_HandleTypeDef *phost)
{
	// switch interface
	phost->device.current_interface++;
	if(phost->device.current_interface >= phost->interfaces)
		phost->device.current_interface = 0;
}

/**
  * @brief  USBH_HID_Process 
  *         The function is for managing state machine for HID data transfers 
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_Process(USBH_HandleTypeDef *phost)
{
	USBH_StatusTypeDef status = USBH_OK;

	uint8_t idx = phost->device.current_interface;
	HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[idx];

	switch (HID_Handle->state)
	{
		case HID_INIT:
			if(HID_Handle->Init(phost) == USBH_OK)
				HID_Handle->state = HID_SYNC;
			break;

		case HID_SYNC:
			/* Sync with start of Even Frame */
			if(phost->Timer & 1)
			{
				HID_Handle->state = HID_GET_DATA;
			}

#if (USBH_USE_OS == 1)
			osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif   
			break;
    
		case HID_GET_DATA:
			USBH_InterruptReceiveData(phost, HID_Handle->pData, HID_Handle->length[0], HID_Handle->InPipe);
    
			HID_Handle->state = HID_POLL;
			HID_Handle->timer = phost->Timer;
			HID_Handle->DataReady = 0;
			break;
    
		case HID_POLL:
			if(USBH_LL_GetURBState(phost , HID_Handle->InPipe) == USBH_URB_DONE)
			{
				if(HID_Handle->DataReady == 0)
				{
//LOG_ARRAY(HID_Handle->pData, HID_Handle->length[0]);

					fifo_write(&HID_Handle->fifo, HID_Handle->pData, HID_Handle->length[0]);
        			HID_Handle->DataReady = 1;
        			USBH_HID_EventCallback(phost);

#if (USBH_USE_OS == 1)
        			osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif
				}
			}
			else if(USBH_LL_GetURBState(phost, HID_Handle->InPipe) == USBH_URB_STALL) /* IN Endpoint Stalled */
			{
				/* Issue Clear Feature on interrupt IN endpoint */
				if(USBH_ClrFeature(phost, HID_Handle->ep_addr[0]) == USBH_OK)
				{
					/* Change state to issue next IN token */
					HID_Handle->state = HID_GET_DATA;
					USBH_HID_IncrementInterface(phost);
				}
			}
			else if(USBH_LL_GetURBState(phost, HID_Handle->InPipe) == USBH_URB_ERROR)
			{
//LOG1(":");
			}
			break;
    
		case HID_ERROR:
		default:
			break;
	}

	return status;
}

/**
  * @brief  USBH_HID_SOFProcess 
  *         The function is for managing the SOF Process 
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_SOFProcess(USBH_HandleTypeDef *phost)
{
	uint8_t idx = phost->device.current_interface;
	HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[idx];

	if(HID_Handle->state == HID_POLL)
	{
		if(( phost->Timer - HID_Handle->timer) >= HID_Handle->poll[0])
		{
			HID_Handle->state = HID_GET_DATA;

#if (USBH_USE_OS == 1)
			osMessagePut ( phost->os_event, USBH_URB_EVENT, 0);
#endif

			USBH_HID_IncrementInterface(phost);

		}
	}

	return USBH_OK;
}

/**
* @brief  USBH_Get_HID_ReportDescriptor
  *         Issue report Descriptor command to the device. Once the response 
  *         received, parse the report descriptor and update the status.
  * @param  phost: Host handle
  * @param  Length : HID Report Descriptor Length
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GetHIDReportDescriptor (USBH_HandleTypeDef *phost, uint16_t interface, uint16_t length)
{
  USBH_StatusTypeDef status;
  
  status = USBH_GetDescriptor(phost,
                              USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,                                  
                              USB_DESC_HID_REPORT, 
                              interface,
                              phost->device.Data,
                              length);
  
  /* HID report descriptor is available in phost->device.Data.
  In case of USB Boot Mode devices for In report handling ,
  HID report descriptor parsing is not required.
  In case, for supporting Non-Boot Protocol devices and output reports,
  user may parse the report descriptor*/
  
  
  return status;
}

/**
  * @brief  USBH_Get_HID_Descriptor
  *         Issue HID Descriptor command to the device. Once the response 
  *         received, parse the report descriptor and update the status.
  * @param  phost: Host handle
  * @param  Length : HID Descriptor Length
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GetHIDDescriptor (USBH_HandleTypeDef *phost, uint16_t interface, uint16_t length)
{
  
  USBH_StatusTypeDef status;

  status = USBH_GetDescriptor( phost,
                              USB_REQ_RECIPIENT_INTERFACE | USB_REQ_TYPE_STANDARD,                                  
                              USB_DESC_HID,
                              interface,
                              phost->device.Data,
                              length);
  return status;
}

static USBH_StatusTypeDef hid_request(USBH_HandleTypeDef *phost, uint8_t request, uint8_t dataDirection,
										uint16_t value, uint16_t index, uint8_t *buffer, uint16_t size)
{
	phost->Control.setup.b.bmRequestType = dataDirection|USB_REQ_RECIPIENT_INTERFACE|USB_REQ_TYPE_CLASS;
	phost->Control.setup.b.bRequest  	 = request;
	phost->Control.setup.b.wValue.w		 = value;
	phost->Control.setup.b.wIndex.w		 = index;
	phost->Control.setup.b.wLength.w     = size;

	return USBH_CtlReq(phost, buffer, size);
}

/**
  * @brief  USBH_Set_Idle
  *         Set Idle State. 
  * @param  phost: Host handle
  * @param  duration: Duration for HID Idle request
  * @param  reportId : Targetted report ID for Set Idle request
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_SetIdle (USBH_HandleTypeDef *phost,
                                         uint8_t duration,
                                         uint8_t reportId)
{
  return hid_request(phost, USB_HID_SET_IDLE, USB_H2D, (duration << 8 ) | reportId, 0, 0, 0);
}


/**
  * @brief  USBH_HID_Set_Report
  *         Issues Set Report 
  * @param  phost: Host handle
  * @param  reportType  : Report type to be sent
  * @param  reportId    : Targetted report ID for Set Report request
  * @param  reportBuff  : Report Buffer
  * @param  reportLen   : Length of data report to be send
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_SetReport (USBH_HandleTypeDef *phost,
                                    uint8_t reportType,
                                    uint8_t reportId,
                                    uint8_t* reportBuff,
                                    uint16_t reportLen)
{
	return hid_request(phost, USB_HID_SET_REPORT, USB_H2D, (reportType << 8 ) | reportId, 0, reportBuff, reportLen);
}


/**
  * @brief  USBH_HID_GetReport
  *         retreive Set Report 
  * @param  phost: Host handle
  * @param  reportType  : Report type to be sent
  * @param  reportId    : Targetted report ID for Set Report request
  * @param  reportBuff  : Report Buffer
  * @param  reportLen   : Length of data report to be send
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_GetReport (USBH_HandleTypeDef *phost,
                                    uint8_t reportType,
                                    uint8_t reportId,
                                    uint8_t* reportBuff,
                                    uint16_t reportLen)
{
	return hid_request(phost, USB_HID_GET_REPORT, USB_D2H, (reportType << 8 ) | reportId, 0, reportBuff, reportLen);
}

/**
  * @brief  USBH_Set_Protocol
  *         Set protocol State.
  * @param  phost: Host handle
  * @param  protocol : Set Protocol for HID : boot/report protocol
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_SetProtocol(USBH_HandleTypeDef *phost, uint8_t protocol)
{
	return hid_request(phost, USB_HID_SET_PROTOCOL, USB_H2D, protocol != 0 ? 0 : 1, 0, 0, 0);
}

/**
  * @brief  USBH_ParseHIDDesc 
  *         This function Parse the HID descriptor
  * @param  desc: HID Descriptor
  * @param  buf: Buffer where the source descriptor is available
  * @retval None
  */
static void  USBH_HID_ParseHIDDesc (HID_DescTypeDef *desc, uint8_t *buf)
{
  
  desc->bLength                  = *(uint8_t  *) (buf + 0);
  desc->bDescriptorType          = *(uint8_t  *) (buf + 1);
  desc->bcdHID                   =  LE16  (buf + 2);
  desc->bCountryCode             = *(uint8_t  *) (buf + 4);
  desc->bNumDescriptors          = *(uint8_t  *) (buf + 5);
  desc->bReportDescriptorType    = *(uint8_t  *) (buf + 6);
  desc->wItemLength              =  LE16  (buf + 7);
} 

/**
  * @brief  USBH_HID_GetDeviceType
  *         Return Device function.
  * @param  phost: Host handle
  * @retval HID function: HID_MOUSE / HID_KEYBOARD
  */
HID_TypeTypeDef USBH_HID_GetDeviceType(USBH_HandleTypeDef *phost)
{
  HID_TypeTypeDef   type = HID_UNKNOWN;
  
  if(phost->gState == HOST_CLASS)
  {
    
    if(phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol == HID_KEYBRD_BOOT_CODE)
    {
      type = HID_KEYBOARD;  
    }
    else if(phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol == HID_MOUSE_BOOT_CODE)
    {
      type=  HID_MOUSE;  
    }
  }
  return type;
}

/**
  * @brief  USBH_HID_GetPollInterval
  *         Return HID device poll time
  * @param  phost: Host handle
  * @retval poll time (ms)
  */
uint8_t USBH_HID_GetPollInterval(USBH_HandleTypeDef *phost)
{
  uint8_t idx = phost->device.current_interface;
  HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[idx];
    
    if((phost->gState == HOST_CLASS_REQUEST) ||
       (phost->gState == HOST_INPUT) ||
         (phost->gState == HOST_SET_CONFIGURATION) ||
           (phost->gState == HOST_CHECK_CLASS) ||           
             ((phost->gState == HOST_CLASS)))
  {
    return (HID_Handle->poll[0]);
  }
  else
  {
    return 0;
  }
}
/**
  * @brief  fifo_init
  *         Initialize FIFO.
  * @param  f: Fifo address
  * @param  buf: Fifo buffer
  * @param  size: Fifo Size
  * @retval none
  */
void fifo_init(FIFO_TypeDef * f, uint8_t * buf, uint16_t size)
{
     f->head = 0;
     f->tail = 0;
     f->lock = 0;
     f->size = size;
     f->buf = buf;
}

/**
  * @brief  fifo_read
  *         Read from FIFO.
  * @param  f: Fifo address
  * @param  buf: read buffer
  * @param  nbytes: number of item to read
  * @retval number of read items
  */
uint16_t  fifo_read(FIFO_TypeDef * f, void * buf, uint16_t  nbytes)
{
  uint16_t  i;
  uint8_t * p;
  p = (uint8_t*) buf;

  if(f->lock == 0)
  {
    f->lock = 1;
    for(i=0; i < nbytes; i++)
    {
      if( f->tail != f->head )
      {
        *p++ = f->buf[f->tail];
        f->tail++;
        if( f->tail == f->size )
        {
          f->tail = 0;
        }
      } else
      {
        f->lock = 0;
        return i;
      }
    }
  }
  f->lock = 0;
  return nbytes;
}

/**
  * @brief  fifo_write
  *         Read from FIFO.
  * @param  f: Fifo address
  * @param  buf: read buffer
  * @param  nbytes: number of item to write
  * @retval number of written items
  */
uint16_t  fifo_write(FIFO_TypeDef * f, const void * buf, uint16_t  nbytes)
{
  uint16_t  i;
  const uint8_t * p;
  p = (const uint8_t*) buf;
  if(f->lock == 0)
  {
    f->lock = 1;
    for(i=0; i < nbytes; i++)
    {
      if( (f->head + 1 == f->tail) ||
         ( (f->head + 1 == f->size) && (f->tail == 0)) )
      {
        f->lock = 0;
        return i;
      }
      else
      {
        f->buf[f->head] = *p++;
        f->head++;
        if( f->head == f->size )
        {
          f->head = 0;
        }
      }
    }
  }
  f->lock = 0;
  return nbytes;
}

/**
* @brief  The function is a callback about HID Data events
*  @param  phost: Selected device
* @retval None
*/
__weak void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
}

USBH_StatusTypeDef USBH_HID_Parameter(USBH_HandleTypeDef *phost, uint8_t param, uint8_t *data, uint16_t *length)
{
	switch(param)
	{
		case PARAMETER_GET_CLASS_PROTO:
			if(data != NULL && length != NULL)
			{
				if(*length < (4 * phost->interfaces))
				{
USBH_UsrLog("USBH_HID_Parameter - data too small");
					return USBH_FAIL;
				}

				*length = phost->interfaces;

				int i = 0, idx = 0;
				for(; i< phost->interfaces; ++i)
				{
					data[idx++] = i; // interface
					data[idx++] = phost->device.CfgDesc.Itf_Desc[i].bInterfaceClass;
					data[idx++] = phost->device.CfgDesc.Itf_Desc[i].bInterfaceSubClass;
					data[idx++] = phost->device.CfgDesc.Itf_Desc[i].bInterfaceProtocol;
				}
				return USBH_OK;
			}
			break;

		default:
USBH_UsrLog("USBH_HID_Parameter - Invalid parameter");
			break;
	}

	return USBH_NOT_SUPPORTED;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
