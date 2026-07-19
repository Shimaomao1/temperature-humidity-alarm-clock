#ifndef __INF_DHT11__
#define __INF_DHT11__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"

#define DHT11_DATA_H HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET)
#define DHT11_DATA_L HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET)
// 读取data引脚值
#define DHT11_DATA_Read HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin)

/**
 * @brief Wait for the sensor to complete its power-up sequence.
 */
void Inf_dht11_init(void);

/**
 * @brief Read one DHT11 frame and return integer temperature/humidity.
 *
 * If the frame is invalid, the output values are left unchanged.
 *
 * @param temperature Destination for signed temperature in degrees Celsius.
 * @param humidity Destination for relative humidity in percent.
 */
void Inf_dht11_get_data(int8_t *temperature, int8_t *humidity);

#endif // __INF_DHT11__
