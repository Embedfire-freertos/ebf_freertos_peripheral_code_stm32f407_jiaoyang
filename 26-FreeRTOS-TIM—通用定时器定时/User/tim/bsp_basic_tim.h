#ifndef __BASIC_TIM_H
#define	__BASIC_TIM_H

#include "stm32f4xx.h"

#define BASIC_TIM           			TIM2
#define BASIC_TIM_CLK_ENABLE()    __TIM2_CLK_ENABLE()

#define BASIC_TIM_IRQn						TIM2_IRQn
#define BASIC_TIM_IRQHandler    	TIM2_IRQHandler

extern TIM_HandleTypeDef TIM_TimeBaseStructure;
void TIMx_Configuration(void);

#endif /* __BASIC_TIM_H */

