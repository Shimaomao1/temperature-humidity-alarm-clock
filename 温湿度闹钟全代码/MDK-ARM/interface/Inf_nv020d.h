#ifndef __INF_NV020D__
#define __INF_NV020D__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Inf_DS1302Z.h"
#include "Com_debug.h"

#define NV020D_CLK_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_SET)
#define NV020D_CLK_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_RESET)

#define NV020D_DATA_H HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_SET)
#define NV020D_DATA_L HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_RESET)

#define NV020D_BUSY_READ HAL_GPIO_ReadPin(NVD_BUSY_GPIO_Port, NVD_BUSY_Pin)

// 单线模式 IOA2 是数据控制线
#define NV020D_CTR_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_SET)
#define NV020D_CTR_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_RESET)

/** @brief Wait for NV020D power-up initialization to complete. */
void Inf_nv020d_init(void);

/** @brief Send one low-bit-first playback/control command. */
void Inf_nv020d_send_cmd(uint8_t cmd);

/** @brief Set the eight-level NV020D volume (0 is the quietest). */
void Inf_nv020d_set_volume(uint8_t volume);


/** @brief Stop the current voice playback. */
void Inf_nv020d_stop(void);

#endif // __INF_NV020D__
