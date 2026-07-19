#include "Inf_nv020d.h"

#define NV020D_POWER_ON_DELAY_MS 500U
#define NV020D_START_LOW_MS 4U
#define NV020D_BIT_HIGH_MS 3U
#define NV020D_BIT_LOW_MS 1U
#define NV020D_COMMAND_GUARD_MS 100U

static void Inf_nv020d_send_bit(uint8_t bit)
{
    NV020D_CTR_H;
    vTaskDelay(pdMS_TO_TICKS(bit ? NV020D_BIT_HIGH_MS : NV020D_BIT_LOW_MS));
    NV020D_CTR_L;
    vTaskDelay(pdMS_TO_TICKS(bit ? NV020D_BIT_LOW_MS : NV020D_BIT_HIGH_MS));
}

/**
 * @brief Wait for the voice chip to finish its power-up sequence.
 */
void Inf_nv020d_init(void)
{
    vTaskDelay(pdMS_TO_TICKS(NV020D_POWER_ON_DELAY_MS));
}

/**
 * @brief 写入一个字节
 *
 * @param data
 */
void Inf_nv020d_send_byte(uint8_t data)
{
    /* The two-wire implementation is retained for board experiments; the
     * application currently uses the one-wire command path below. */
    NV020D_CLK_H;

    // 提前设置好D1的值
    if (data & (1))
    {
        NV020D_DATA_H;
    }
    else
    {
        NV020D_DATA_L;
    }
    // 2. 发送起始命令
    NV020D_CLK_L;
    vTaskDelay(pdMS_TO_TICKS(NV020D_START_LOW_MS));
    // 循环发送指令
    for (uint8_t i = 0; i < 8; i++)
    {
        if (data & (1U << i))
        {
            NV020D_DATA_H;
        }
        else
        {
            NV020D_DATA_L;
        }

        NV020D_CLK_L;
        Inf_Delay_us(400U);
        NV020D_CLK_H;
        Inf_Delay_us(400U);
    }

    NV020D_DATA_H;
}

/**
 * @brief 写入一个指令
 *
 * @param cmd
 */
void Inf_nv020d_send_cmd(uint8_t cmd)
{
    /* One-wire mode: the command line is idle high. */
    NV020D_CTR_H;

    /* Start pulse. */
    NV020D_CTR_L;
    vTaskDelay(pdMS_TO_TICKS(NV020D_START_LOW_MS));

    /* Each command bit is encoded by a high/low pulse-width ratio. */
    for (uint8_t i = 0U; i < 8U; i++)
    {
        Inf_nv020d_send_bit((uint8_t)((cmd >> i) & 0x01U));
    }

    NV020D_CTR_H;
    vTaskDelay(pdMS_TO_TICKS(NV020D_COMMAND_GUARD_MS));
}

/**
 * @brief 设置音量   0-7
 *
 * @param volume
 */
void Inf_nv020d_set_volume(uint8_t volume)
{
    if (volume > 7U)
    {
        debug_printf("volume is out of range");
        volume = 7;
    }

    Inf_nv020d_send_cmd(0xE0 + volume);
}

/**
 * @brief 发送一个空指令 => 停止播放
 *
 */
void Inf_nv020d_stop(void)
{
    // 可以添加一个判断 当前是否在播放   busy线 拉低的情况

    // 也可以不判断  直接发送停止
    Inf_nv020d_send_cmd(0xFE);
}
