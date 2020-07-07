#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

//���Ŷ���
/*******************************************************/

#define LED1_PIN                  GPIO_PIN_15               
#define LED1_GPIO_PORT            GPIOA                     
#define LED1_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE()

#define LED2_PIN                  GPIO_PIN_2                 
#define LED2_GPIO_PORT            GPIOE                  
#define LED2_GPIO_CLK_ENABLE()    __GPIOE_CLK_ENABLE()

#define LED3_PIN                  GPIO_PIN_15               
#define LED3_GPIO_PORT            GPIOG                     
#define LED3_GPIO_CLK_ENABLE()    __GPIOG_CLK_ENABLE()

#define LED4_PIN                  GPIO_PIN_8               
#define LED4_GPIO_PORT            GPIOB                     
#define LED4_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()
/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)

#define LED2(a)	HAL_GPIO_WritePin(LED2_GPIO_PORT,LED2_PIN,a)

#define LED3(a)	HAL_GPIO_WritePin(LED3_GPIO_PORT,LED3_PIN,a)

#define LED4(a)	HAL_GPIO_WritePin(LED4_GPIO_PORT,LED4_PIN,a)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF		digitalHi(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			digitalLo(LED1_GPIO_PORT,LED1_PIN)

#define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_PIN)
#define LED2_OFF		digitalHi(LED2_GPIO_PORT,LED2_PIN)
#define LED2_ON			digitalLo(LED2_GPIO_PORT,LED2_PIN)

#define LED3_TOGGLE		digitalToggle(LED3_GPIO_PORT,LED3_PIN)
#define LED3_OFF		digitalHi(LED3_GPIO_PORT,LED3_PIN)
#define LED3_ON			digitalLo(LED3_GPIO_PORT,LED3_PIN)

#define LED4_TOGGLE		digitalToggle(LED4_GPIO_PORT,LED4_PIN)
#define LED4_OFF		digitalHi(LED4_GPIO_PORT,LED4_PIN)
#define LED4_ON			digitalLo(LED4_GPIO_PORT,LED4_PIN)

/* ������ɫ������߼��÷�ʹ��PWM�ɻ��ȫ����ɫ,��Ч������ */
					
//(ȫ����)
#define LED_ALLON	\
					LED1_ON;\
					LED2_ON\
					LED3_ON\
					LED4_ON
					
//(ȫ���ر�)
#define LED_ALLOFF	\
					LED1_OFF;\
					LED2_OFF\
					LED3_OFF\
					LED4_OFF

#define LED_ALLTOGGLE \
					LED1_TOGGLE;\
					LED2_TOGGLE\
					LED3_TOGGLE\
					LED4_TOGGLE



void LED_GPIO_Config(void);

#endif /* __LED_H */