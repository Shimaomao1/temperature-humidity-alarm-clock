#ifndef __APP_SHOW__
#define __APP_SHOW__

#include "Inf_led.h"
#include "App_dateTime.h"
#include "FreeRTOS.h"
#include "task.h"

// 展示模式
typedef enum
{
    SHOW_MODE_NORMAL,    // 正常显示时间
    SHOW_MODE_TIME_SET,  // 进入时间设置显示
    SHOW_MODE_ALARM_SET, // 闹钟设置模式
    SHOW_MODE_VOLUME,    // 音量设置模式
} Show_Mode_type;

/**
 * @brief 初始化 开启LED灯
 *
 */
void App_show_start(void);

/**
 * @brief 对应一般模式的LED显示
 *
 */
void App_show_normal(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type);

/**
 * @brief 对应时间设置模式的LED显示
 *
 * @param clock_date_time
 */
void App_show_time_set(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type);

/**
 * @brief 对应闹钟设置模式的LED显示
 *
 */
void App_show_alarm_set(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type);

#endif // __APP_SHOW__
