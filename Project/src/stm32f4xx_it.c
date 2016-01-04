/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "usbh_conf.h"

extern HCD_HandleTypeDef _hHCD[2];

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
#ifndef USE_RTOS_SYSTICK
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
#endif

void OTG_FS_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(OTG_FS_IRQn);

	if(_hHCD[ID_USB_HOST_FS].Instance == USB_OTG_FS)
		HAL_HCD_IRQHandler(&_hHCD[ID_USB_HOST_FS]);
}

void OTG_HS_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(OTG_HS_IRQn);

	if(_hHCD[ID_USB_HOST_HS].Instance == USB_OTG_HS)
		HAL_HCD_IRQHandler(&_hHCD[ID_USB_HOST_HS]);
}

