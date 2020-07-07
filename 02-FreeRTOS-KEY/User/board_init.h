#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * 此处用以包含各种外设的头文件，在main函数中只包含本文件即可
 */
#include "bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include ".\key\bsp_key.h" 

void SystemClock_Config(void);




#endif
