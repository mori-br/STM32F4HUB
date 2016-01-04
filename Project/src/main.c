/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  *
  * Copyright (c) 2016 Mori
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
  * associated documentation files (the "Software"), to deal in the Software without restriction,
  * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
  * subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
  * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
			
#include "usbh_core.h"
#include "usbh_hid.h"
#include "usbh_hub.h"

#include "log.h"



USBH_HandleTypeDef hUSBHost[5];

static void USBH_UserProcess (USBH_HandleTypeDef *pHost, uint8_t vId);
static void hub_process();
static void system_clock_config(void);

int main(void)
{
	uint32_t i = 0;

	HAL_Init();
	system_clock_config();

	BSP_LED_Init(LED4);

	BSP_UART_Init();

	LOG_INIT(USARTx, 3800000);

	LOG("\033[2J\033[H");
	LOG(" ");
	LOG("APP RUNNING...");
	LOG("MCU-ID %08X", DBGMCU->IDCODE);

	memset(&hUSBHost[0], 0, sizeof(USBH_HandleTypeDef));

	hUSBHost[0].valid   = 1;
	hUSBHost[0].address = USBH_DEVICE_ADDRESS;
	hUSBHost[0].Pipes   = USBH_malloc(sizeof(uint32_t) * USBH_MAX_PIPES_NBR);

	USBH_Init(&hUSBHost[0], USBH_UserProcess, ID_USB_HOST_FS);
	USBH_RegisterClass(&hUSBHost[0], USBH_HID_CLASS);
	USBH_RegisterClass(&hUSBHost[0], USBH_HUB_CLASS);

	USBH_Start(&hUSBHost[0]);

	while(1)
	{
		if (i++ > 150000)
			i = 0;

		if(i > 0 && i <= 2500)
			BSP_LED_On(LED4);
		else
			BSP_LED_Off(LED4);

		hub_process();
	}
}

void hub_process()
{
	static uint8_t current_loop = -1;
	static USBH_HandleTypeDef *_phost = 0;

	if(_phost != NULL && _phost->valid == 1)
	{
		USBH_Process(_phost);

		if(_phost->busy)
			return;
	}

	while(1)
	{
		current_loop++;

		if(current_loop > MAX_HUB_PORTS)
			current_loop = 0;

		if(hUSBHost[current_loop].valid)
		{
			_phost = &hUSBHost[current_loop];
			USBH_LL_SetupEP0(_phost);

			if(_phost->valid == 3)
			{
//LOG("PROCESSING ATTACH %d", _phost->address);
				_phost->valid = 1;
				_phost->busy  = 1;
			}

			break;
		}
	}

	if(_phost != NULL && _phost->valid)
	{
		HID_MOUSE_Info_TypeDef *minfo;
		minfo = USBH_HID_GetMouseInfo(_phost);
		if(minfo != NULL)
		{
LOG("BUTTON %d", minfo->buttons[0]);
		}
		else
		{
			HID_KEYBD_Info_TypeDef *kinfo;
			kinfo = USBH_HID_GetKeybdInfo(_phost);
			if(kinfo != NULL)
			{
LOG("KEYB %d", kinfo->keys[0]);
			}
		}
	}

}

void USBH_UserProcess (USBH_HandleTypeDef *pHost, uint8_t vId)
{
	switch (vId)
	{
		case HOST_USER_SELECT_CONFIGURATION:
			break;

		case HOST_USER_CLASS_SELECTED:
			break;

		case HOST_USER_CLASS_ACTIVE:
			break;

		case HOST_USER_CONNECTION:
			break;

		case HOST_USER_DISCONNECTION:
			break;

		case HOST_USER_UNRECOVERED_ERROR:
			break;

		default:
			break;
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
void system_clock_config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
     	 clocked below the maximum system frequency, to update the voltage scaling value
     	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13|GPIO_PIN_14);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13|GPIO_PIN_14);
	}
}
