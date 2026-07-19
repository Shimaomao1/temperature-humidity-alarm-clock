#ifndef __APP_DATETIME__
#define __APP_DATETIME__

#include "Inf_DS1302Z.h"

// 保存所有时间信息的结构体  => 读取芯片的时间 => 保存到结构体中
typedef struct
{
    uint16_t year; // 2025
    uint8_t month;
    uint8_t day;
    int8_t hour;
    int8_t min;
    uint8_t sec;
    uint8_t week;
    uint8_t is_pm;
    uint8_t is_12_hour;
} Clock_Date_time_type;

typedef struct
{
    int8_t hour;
    int8_t min;
    uint8_t is_on;
    uint8_t is_done;
} Alarm_Time_Type;

/**
 *
 *
 * @brief 读取所有的时间
 *
 */
void App_dateTime_get(Clock_Date_time_type *date_time);

/**
 * @brief 设置所有的时间
 *
 * @param date_time
 */
void App_dateTime_set(Clock_Date_time_type *date_time);

#endif // __APP_DATETIME__
