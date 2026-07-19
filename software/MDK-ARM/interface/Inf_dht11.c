#include "Inf_dht11.h"
#include <stddef.h>

#define DHT11_POWER_ON_DELAY_MS 2000U
#define DHT11_START_LOW_MS 20U
#define DHT11_SAMPLE_DELAY_US 40U
#define DHT11_TIMEOUT_COUNT 0x00FFFFFFUL

/* DHT11 sends four data bytes followed by an 8-bit checksum. */
static uint8_t dht11_frame[5] = {0U};

static uint8_t dht11_wait_for_level(GPIO_PinState level, uint32_t timeout)
{
    while (DHT11_DATA_Read == level)
    {
        if (timeout-- == 0U)
        {
            return 0U;
        }
    }

    return 1U;
}

/**
 * @brief Wait for the DHT11 power-up delay.
 */
void Inf_dht11_init(void)
{
    /* Leave the open-drain bus released while the sensor starts. */
    DHT11_DATA_H;
    vTaskDelay(pdMS_TO_TICKS(DHT11_POWER_ON_DELAY_MS));
}

/**
 * @brief 获取DHT11模块的温湿度数据   只保留整数部分即可  温度要确定正负
 *
 * @param temperature
 * @param humidity
 */
void Inf_dht11_get_data(int8_t *temperature, int8_t *humidity)
{
    uint8_t frame_valid = 1U;
    int8_t temp = 0;
    int8_t hum = 0;
    uint32_t checksum = 0U;

    if ((temperature == NULL) || (humidity == NULL))
    {
        debug_printf("DHT11 output pointer is null");
        return;
    }

    /* Start a conversion, then release the open-drain bus for the sensor. */
    DHT11_DATA_L;
    vTaskDelay(pdMS_TO_TICKS(DHT11_START_LOW_MS));
    DHT11_DATA_H;

    taskENTER_CRITICAL();

    /* Sensor response: low about 80 us, then high about 80 us. */
    if (!dht11_wait_for_level(GPIO_PIN_SET, DHT11_TIMEOUT_COUNT) ||
        !dht11_wait_for_level(GPIO_PIN_RESET, DHT11_TIMEOUT_COUNT) ||
        !dht11_wait_for_level(GPIO_PIN_SET, DHT11_TIMEOUT_COUNT))
    {
        frame_valid = 0U;
    }

    /* Each bit starts with a low pulse. Sampling 40 us into the high pulse
     * separates the short (0) and long (1) timings with useful margin. */
    for (uint8_t i = 0U; (i < 5U) && frame_valid; i++)
    {
        dht11_frame[i] = 0U;
        for (uint8_t j = 0U; (j < 8U) && frame_valid; j++)
        {
            if (!dht11_wait_for_level(GPIO_PIN_RESET, DHT11_TIMEOUT_COUNT))
            {
                frame_valid = 0U;
                break;
            }

            Inf_Delay_us(DHT11_SAMPLE_DELAY_US);
            if (DHT11_DATA_Read == GPIO_PIN_SET)
            {
                dht11_frame[i] |= (uint8_t)(1U << (7U - j));
                if (!dht11_wait_for_level(GPIO_PIN_SET, DHT11_TIMEOUT_COUNT))
                {
                    frame_valid = 0U;
                    break;
                }
            }
        }
    }

    if (frame_valid)
    {
        checksum = (uint32_t)dht11_frame[0] + dht11_frame[1] +
                   dht11_frame[2] + dht11_frame[3];
        if ((uint8_t)checksum == dht11_frame[4])
        {
            hum = (int8_t)dht11_frame[0];
            temp = (int8_t)dht11_frame[2];
            if ((dht11_frame[3] & 0x80U) != 0U)
            {
                temp = (int8_t)-temp;
            }

            *humidity = hum;
            *temperature = temp;
        }
        else
        {
            frame_valid = 0U;
        }
    }

    taskEXIT_CRITICAL();

    if (!frame_valid)
    {
        debug_printf("DHT11 frame timeout or checksum error");
    }
}
