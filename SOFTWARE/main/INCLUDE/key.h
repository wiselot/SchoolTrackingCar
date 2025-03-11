#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"

typedef void (*KEY_CALLBACK)(void);

void Key_Init(KEY_CALLBACK,KEY_CALLBACK);


#endif