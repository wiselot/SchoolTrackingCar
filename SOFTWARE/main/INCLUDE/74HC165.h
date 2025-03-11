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

// 设计的乱的。。。
// 近侧从右往左
#define GET_LINE_A0(dat) (GET_BIT(dat,3))
#define GET_LINE_A1(dat) (GET_BIT(dat,2))
#define GET_LINE_A2(dat) (GET_BIT(dat,1))
#define GET_LINE_A3(dat) (GET_BIT(dat,4))
#define GET_LINE_A4(dat) (GET_BIT(dat,0))
// 远侧从右往左
#define GET_LINE_B0(dat) (GET_BIT(dat,5))
#define GET_LINE_B1(dat) (GET_BIT(dat,6))
#define GET_LINE_B2(dat) (GET_BIT(dat,7))

#define GET_BUS_B(dat) ((GET_LINE_B0(dat)<<2)|(GET_LINE_B1(dat)<<1)|(GET_LINE_B2(dat)))
#define GET_BUS_A(dat) ((GET_LINE_A0(dat)<<4)|(GET_LINE_A1(dat)<<3)|(GET_LINE_A2(dat)<<2)|(GET_LINE_A3(dat)<<1)|(GET_LINE_A4(dat)))

#endif