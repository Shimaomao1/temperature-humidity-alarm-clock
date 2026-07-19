#ifndef __INF_DS1302Z__
#define __INF_DS1302Z__

#include "gpio.h"

#define DS1302Z_SECOND_REG 0x80
#define DS1302Z_MINUTE_REG 0x82
#define DS1302Z_HOUR_REG 0x84
#define DS1302Z_DAY_REG 0x86
#define DS1302Z_MONTH_REG 0x88
#define DS1302Z_WEEK_REG 0x8A
#define DS1302Z_YEAR_REG 0x8C

// 控制寄存器
#define DS1302Z_CONTROL_REG 0x8E

#define DS1302Z_READ_REG 0x01
#define DS1302Z_WRITE_REG 0x00

// 控制引脚
#define DS_CLK_H HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_SET)
#define DS_CLK_L HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_RESET)

#define DS_IO_H HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_SET)
#define DS_IO_L HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_RESET)
// 读取DS_IO_Pin的值
#define DS_IO_READ HAL_GPIO_ReadPin(DS_IO_GPIO_Port, DS_IO_Pin)

#define DS_RST_H HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_SET)
#define DS_RST_L HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_RESET)




/** @brief Busy-wait for approximately @p us microseconds at 72 MHz. */
void Inf_Delay_us(uint32_t us);

/** @brief Read one DS1302 register (the command is sent LSB first). */
uint8_t Inf_DS1302Z_Read_Byte(uint8_t reg_addr);

/** @brief Write one DS1302 register (the command and data are LSB first). */
void Inf_DS1302Z_Write_Byte(uint8_t reg_addr, uint8_t data);

#endif // __INF_DS1302Z__
