#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * �˴����԰������������ͷ�ļ�����main������ֻ�������ļ�����
 */
#include "bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include ".\key\bsp_key.h" 
#include "./tim/bsp_general_tim.h"
#include "./dac/bsp_dac.h"


void SystemClock_Config(void);

void Error_Handler(void);
#endif
