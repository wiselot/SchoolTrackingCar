#ifndef _74HC165_H
#define _74HC165_H

#include "stm32f10x.h"
#include "sys.h"

void _74HC165_Init(void);
uint8_t Read_74HC165(void);

#define GET_BIT(dat,bit) (((dat) >> (bit)) & 0x01)

#define _74HC165_LD		PAout(2)
#define _74HC165_DAT	PAin(0)
#define _74HC165_CLK	PAout(1)

#endif