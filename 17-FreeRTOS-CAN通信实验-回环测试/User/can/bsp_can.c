/**
  ******************************************************************************
  * @file    bsp_can.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   can驱动（回环模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./can/bsp_can.h"
#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#include "semphr.h"
CAN_HandleTypeDef Can_Handle;
CAN_TxHeaderTypeDef TxHeader;		//发送头
CAN_RxHeaderTypeDef RxHeader;		//接收头

/* 封装数据 */
Tx_Packge my_Packge = 
{
	.TxHeader = &TxHeader,
};

uint8_t Rx_Buff[8];

/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能引脚时钟 */
    CAN_TX_GPIO_CLK_ENABLE();
    CAN_RX_GPIO_CLK_ENABLE();	

    /* 配置CAN发送引脚 */
    GPIO_InitStructure.Pin = CAN_TX_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Pull  = GPIO_PULLUP;
    GPIO_InitStructure.Alternate =  GPIO_AF9_CAN1;
    HAL_GPIO_Init(CAN_TX_GPIO_PORT, &GPIO_InitStructure);

    /* 配置CAN接收引脚 */
    GPIO_InitStructure.Pin = CAN_RX_PIN ;
    HAL_GPIO_Init(CAN_RX_GPIO_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_NVIC_Config(void)
{
	/*中断设置，抢占优先级0，子优先级为0*/
	HAL_NVIC_SetPriority(CAN_RX_IRQ, 8 ,0);
	HAL_NVIC_EnableIRQ(CAN_RX_IRQ);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 8 ,0);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
	/********************CAN通信参数设置******************/
	/* 使能CAN时钟 */
	CAN_CLK_ENABLE();
	Can_Handle.Instance = CANx;
	/* CAN单元初始化 */
	//MCR-TTCM  关闭时间触发通信模式使能
	Can_Handle.Init.TimeTriggeredMode=DISABLE;			   
	 //MCR-ABOM  自动离线管理
	Can_Handle.Init.AutoBusOff=ENABLE;			
	 //MCR-AWUM  使用自动唤醒模式
	Can_Handle.Init.AutoWakeUp=ENABLE;			 
	//MCR-NART  禁止报文自动重传	  DISABLE-自动重传
	Can_Handle.Init.AutoRetransmission=DISABLE;			 
   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
	Can_Handle.Init.ReceiveFifoLocked=DISABLE;			  
	 //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
	Can_Handle.Init.TransmitFifoPriority=DISABLE;			  
	//回环模式
	Can_Handle.Init.Mode = CAN_MODE_LOOPBACK;  
	//BTR-SJW 重新同步跳跃宽度 2个时间单元
	Can_Handle.Init.SyncJumpWidth=CAN_SJW_1TQ;		   
	 
	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+6)  */
	 //BTR-TS1 时间段1 占用了6个时间单元
	Can_Handle.Init.TimeSeg1=CAN_BS1_5TQ;		  
	//BTR-TS1 时间段2 占用了3个时间单元	
	Can_Handle.Init.TimeSeg2=CAN_BS2_3TQ;		   
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 54 MHz) */
	Can_Handle.Init.Prescaler =6;	//BTR-BRP 波特率分频器  定义了时间单元的时间长度 54/(1+5+3)/5=1 Mbps
	HAL_CAN_Init(&Can_Handle);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Filter_Config(void)
{
	CAN_FilterTypeDef  CAN_FilterInitStructure;

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.FilterBank=14;	//筛选器组0
	//工作在掩码模式
	CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK;	
	//筛选器位宽为单个32位。
	CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT;	
	/* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */
	//要筛选的ID高位
	CAN_FilterInitStructure.FilterIdHigh= ((((uint32_t)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF0000)>>16;	
	//要筛选的ID低位 
	CAN_FilterInitStructure.FilterIdLow= (((uint32_t)0x1314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; 
	//筛选器高16位每位必须匹配
	CAN_FilterInitStructure.FilterMaskIdHigh= 0xFFFF;			
	//筛选器低16位每位必须匹配
	CAN_FilterInitStructure.FilterMaskIdLow= 0xFFFF;			
	//筛选器被关联到FIFO0
	CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0 ;	
	//使能筛选器
	CAN_FilterInitStructure.FilterActivation=ENABLE;			
	HAL_CAN_ConfigFilter(&Can_Handle,&CAN_FilterInitStructure);
}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  CAN_Filter_Config();
  Init_RxMes(); 
  HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING); 	
  HAL_CAN_Start(&Can_Handle);
}


/**
  * @brief  初始化 Rx Message数据结构体
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */
void Init_RxMes(void)
{

  /*把接收结构体清零*/
  RxHeader.StdId = 0x00;
  RxHeader.ExtId = 0x00;
  RxHeader.IDE = CAN_ID_STD;
  RxHeader.DLC = 0;
  RxHeader.FilterMatchIndex = 0;

}


/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：发送报文结构体
 * 输出  : 无
 * 调用  ：外部调用
 */	 
void CAN_SetMsg(void)
{	  
  my_Packge.TxHeader->StdId=0x00;						 
  my_Packge.TxHeader->ExtId=0x1314;					 //使用的扩展ID
  my_Packge.TxHeader->IDE=CAN_ID_EXT;				  //扩展模式
  my_Packge.TxHeader->RTR=CAN_RTR_DATA;				 //发送的是数据
  my_Packge.TxHeader->DLC=8;							 //数据长度为8字节
  for(uint8_t i = 0;i<8;i++)
	{
		my_Packge.Payload[i] = i;
	}
  HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_TX_MAILBOX_EMPTY);
}

/**
  * @brief  CAN错误回调函数
  * @param  hcan: CAN句柄指针
  * @retval 无
  */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	printf("\r\nCAN出错\r\n");
}

extern SemaphoreHandle_t BinarySem_Handle;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint32_t ulReturn;
	BaseType_t pxHigherPriorityTaskWoken;
  /* 进入临界段，临界段可以嵌套 */
  ulReturn = taskENTER_CRITICAL_FROM_ISR();
	/* 比较ID是否为0x1314 */ 
 	HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0,&RxHeader,Rx_Buff);
	if((RxHeader.ExtId==0x1314) && (RxHeader.IDE==CAN_ID_EXT) && (RxHeader.DLC==8) )
	{
		//给出二值信号量 ，发送接收到新数据标志，供前台程序查询
		xSemaphoreGiveFromISR(BinarySem_Handle,&pxHigherPriorityTaskWoken);	//释放二值信号量
		//如果需要的话进行一次任务切换，系统会判断是否需要进行切换
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}

	/* 准备中断接收 */
	HAL_CAN_ActivateNotification(&Can_Handle, CAN_IT_RX_FIFO0_MSG_PENDING);
	/* 退出临界段 */
  taskEXIT_CRITICAL_FROM_ISR( ulReturn );
}
/**************************END OF FILE************************************/
