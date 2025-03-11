#ifndef _UART_H
#define _UART_H

#include "stm32f10x.h"

#define TRANSFER_WIFI_LOG

void UART1_Init(uint32_t baudrate);
void UART3_Init(uint32_t baudrate);
void UART1_SendByte(uint8_t data);
void UART3_SendByte(uint8_t data);
void UART1_SendString(char *str);
void UART3_SendString(char *str);


#endif