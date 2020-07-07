/**
  ******************************************************************************
  * @file    GPIO/GPIO_EXTI/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "board_init.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_EXTI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("HardFault_Handler\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
		if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
	#endif  /* INCLUDE_xTaskGetSchedulerState */  
		xPortSysTickHandler();
	#if (INCLUDE_xTaskGetSchedulerState  == 1 )
		}
	#endif  /* INCLUDE_xTaskGetSchedulerState */
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/* ���������ⲿ���� & ��ֵ�ź��� */
extern QueueHandle_t Test_Queue;
extern SemaphoreHandle_t BinarySem_Handle;

static uint32_t send_data1 = 1;
static uint32_t send_data2 = 2;


/*********************************************************************************
  * @ ������  �� KEY1_IRQHandler
  * @ ����˵���� �жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void KEY1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;

  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
	//ȷ���Ƿ������EXTI Line�ж�
	if(__HAL_GPIO_EXTI_GET_IT(KEY1_INT_GPIO_PIN) != RESET) 
	{
    /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
		xQueueSendFromISR(Test_Queue, /* ��Ϣ���еľ�� */
											&send_data1,/* ���͵���Ϣ���� */
											&pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//����жϱ�־λ
		__HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);     
	}
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

/*********************************************************************************
  * @ ������  �� KEY1_IRQHandler
  * @ ����˵���� �жϷ�����
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  ********************************************************************************/
void KEY2_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
  uint32_t ulReturn;
  /* �����ٽ�Σ��ٽ�ο���Ƕ�� */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
  
  //ȷ���Ƿ������EXTI Line�ж�
	if(__HAL_GPIO_EXTI_GET_IT(KEY2_INT_GPIO_PIN) != RESET) 
	{
    /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
		xQueueSendFromISR(Test_Queue, /* ��Ϣ���еľ�� */
											&send_data2,/* ���͵���Ϣ���� */
											&pxHigherPriorityTaskWoken);
		
		//�����Ҫ�Ļ�����һ�������л�
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		
		//����жϱ�־λ
		__HAL_GPIO_EXTI_CLEAR_IT(KEY2_INT_GPIO_PIN);  
	}  
  
  /* �˳��ٽ�� */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}

void KEY3_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(KEY3_INT_GPIO_PIN);
}
void KEY4_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(KEY4_INT_GPIO_PIN);
}
void KEY5_IRQHandler(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(KEY5_INT_GPIO_PIN);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
