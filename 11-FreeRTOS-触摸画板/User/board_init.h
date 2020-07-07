#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * �˴����԰������������ͷ�ļ�����main������ֻ�������ļ�����
 */
#include "bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include ".\key\bsp_key.h" 
#include "./flash/bsp_spi_flash.h"
#include "ff.h"
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./touch/gt5xx.h"
#include "./touch/palette.h"
#include "./touch/bsp_i2c_touch.h"

void SystemClock_Config(void);




#endif
