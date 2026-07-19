#ifndef __INF_LED__
#define __INF_LED__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Inf_DS1302Z.h"

#define LED_CLK_H HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET)
#define LED_CLK_L HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET)

#define LED_SDI_H HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_SET)
#define LED_SDI_L HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_RESET)

#define LED_LE_H HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_SET)
#define LED_LE_L HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_RESET)

#define LED_OEA_H HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_SET)
#define LED_OEA_L HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_RESET)

#define LED_OEB_H HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_SET)
#define LED_OEB_L HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_RESET)

extern uint16_t led_clk_h_s[3];
extern uint16_t led_clk_h_g[10];
extern uint16_t led_clk_m_s[10];
extern uint16_t led_clk_m_g[10];

// 展示类型选择
// 表示12/24h  表示 AM/PM  表示摄氏度/华氏度  闹钟12标识  充电标识  5天闹钟标识
typedef struct
{
    uint8_t is_12_hour;
    uint8_t is_pm;
    uint8_t is_celsius;
    uint8_t is_alarm_1;
    uint8_t is_alarm_2;
    uint8_t is_alarm_five_day;
    uint8_t is_charging;
} Show_type_Struct;

typedef enum
{
    TIME_SET_YEAR = 0,
    TIME_SET_MONTH,
    TIME_SET_DAY,
    TIME_SET_HOUR,
    TIME_SET_MIN,
    TIME_SET_END,
} Show_Time_Set;

typedef enum
{
    ALARM_SET_HOUR = 0,
    ALARM_SET_MIN,
    ALARM_SET_END,
}Show_alarm_Set;

/**
 * @brief 控制总开关
 *
 */
void Inf_led_start(void);

/**
 * @brief 关闭总开关
 *
 */
void Inf_led_stop(void);

/**
 * @brief 写入数据
 *
 * @param p00 VCCP00 电源控制
 * @param p01 VCCP01 电源控制
 * @param p02 VCCP02 电源控制
 * @param data 两个驱动芯片的数据
 */
void Inf_led_write(uint8_t p00, uint8_t p01, uint8_t p02, uint32_t data);

/**
 * @brief 设置时钟
 *
 * @param h 小时
 * @param m 分钟
 */
void Inf_led_set_clock(uint8_t h, uint8_t m, uint8_t flag);

/**
 * @brief 同时展示温湿度和闹钟
 *
 * @param h
 * @param m
 * @param flag
 * @param temperature
 * @param humidity
 */
void Inf_led_set_all(uint8_t h, uint8_t m, uint8_t flag, int8_t temperature, int8_t humidity, Show_type_Struct *show_type);

/**
 * @brief 进入到设置时间页面之后  LED显示的内容
 *
 * @param time_set 显示的不同页面
 * @param value 显示的具体值
 */
void Inf_led_time_set(Show_Time_Set time_set, uint16_t value);

#endif // __INF_LED__
