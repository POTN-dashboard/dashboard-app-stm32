#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include "stm32f1xx_hal.h"
#include "display_menu.h"


void Buffer_Set(u8 *buffer,u8 num);
void transform_CPUGPU(void);
void transform_DATA(void);

#endif

