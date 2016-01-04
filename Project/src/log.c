/*
 * log.c
 *
 *  Created on: Jan 2, 2016
 *      Author: mori
 */

#include "log.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define LOGGER_MAXLEN 	250
#define LOGGER_TIMEOUT	10

static UART_HandleTypeDef hUartHandle;

uint8_t LOG_INIT(USART_TypeDef *usart, uint32_t baudrate)
{
	hUartHandle.Instance          = usart;
	hUartHandle.Init.BaudRate     = baudrate;
	hUartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	hUartHandle.Init.StopBits     = UART_STOPBITS_1;
	hUartHandle.Init.Parity       = UART_PARITY_NONE;
	hUartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	hUartHandle.Init.Mode         = UART_MODE_TX_RX;
	hUartHandle.Init.OverSampling = UART_OVERSAMPLING_8;

	return (HAL_UART_Init(&hUartHandle) == HAL_OK);
}

void write_string(const char *data)
{
	HAL_UART_Transmit(&hUartHandle, (uint8_t *)data, strlen(data), LOGGER_TIMEOUT);
}

void LOG(const char *__msg, ...)
{
    char buffer[LOGGER_MAXLEN] = {0};
    va_list ap;
    va_start (ap, __msg);
    vsprintf (buffer, __msg, ap);
    va_end (ap);

    write_string(buffer);
    write_string("\r\n");
}

void LOG1(const char *__msg, ...)
{
    char buffer[LOGGER_MAXLEN] = {0};
    va_list ap;
    va_start (ap, __msg);
    vsprintf (buffer, __msg, ap);
    va_end (ap);

    write_string(buffer);
}

void LOG_ARRAY1(const uint8_t *data, int size)
{
    int i =0;
    for(i = 0; i < size; ++i)
    {
        LOG1("%02X ", data[i]);
    }
}

void LOG_ARRAY(const uint8_t *data, int size)
{
	LOG_ARRAY1(data, size);
    write_string("\r\n");
}
