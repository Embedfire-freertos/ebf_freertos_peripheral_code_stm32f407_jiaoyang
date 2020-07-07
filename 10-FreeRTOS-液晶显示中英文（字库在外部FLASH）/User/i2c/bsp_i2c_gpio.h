#ifndef _BSP_I2C_GPIO_H
#define _BSP_I2C_GPIO_H

#include "stm32f4xx.h"
#include <inttypes.h>


#define EEPROM_I2C_WR	0		/* 写控制bit */
#define EEPROM_I2C_RD	1		/* 读控制bit */


/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define EEPROM_I2C_GPIO_PORT				GPIOH			/* GPIO端口 */
#define EEPROM_I2C_GPIO_CLK_ENABLE()    	__GPIOH_CLK_ENABLE()
#define EEPROM_I2C_SCL_PIN					GPIO_PIN_4			/* 连接到SCL时钟线的GPIO */
#define EEPROM_I2C_SDA_PIN					GPIO_PIN_5			/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define EEPROM_I2C_SCL_1()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN,GPIO_PIN_SET)		/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SCL_PIN,GPIO_PIN_RESET)		/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN,GPIO_PIN_SET)		/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  HAL_GPIO_WritePin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN,GPIO_PIN_RESET)		/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  HAL_GPIO_ReadPin(EEPROM_I2C_GPIO_PORT, EEPROM_I2C_SDA_PIN)	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define EEPROM_I2C_SCL_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SCL_PIN				/* SCL = 1 */
	#define EEPROM_I2C_SCL_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SCL_PIN				/* SCL = 0 */
	
	#define EEPROM_I2C_SDA_1()  EEPROM_I2C_GPIO_PORT->BSRRL = EEPROM_I2C_SDA_PIN				/* SDA = 1 */
	#define EEPROM_I2C_SDA_0()  EEPROM_I2C_GPIO_PORT->BSRRH = EEPROM_I2C_SDA_PIN				/* SDA = 0 */
	
	#define EEPROM_I2C_SDA_READ()  ((EEPROM_I2C_GPIO_PORT->IDR & EEPROM_I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif


void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
uint8_t i2c_CheckDevice(uint8_t _Address);


#endif

