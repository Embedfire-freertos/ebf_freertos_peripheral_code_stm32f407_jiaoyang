#ifndef __BOARD_INIT_H
#define __BOARD_INIT_H

/*****************************************************/
/*
 * �˴����԰������������ͷ�ļ�����main������ֻ�������ļ�����
 */
#include "bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include ".\key\bsp_key.h" 
#include "usb_host.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"
#include "string.h"

void SystemClock_Config(void);
void Error_Handler(void);


#endif
