#ifndef __INF_KEY__
#define __INF_KEY__

#include "gpio.h"
#include "Com_debug.h"
#include "FreeRTOS.h"
#include "task.h"
typedef enum
{
    KEY_NONE = 0,
    KEY_TIME_SET,       // 表示第一个按键开关的短按
    KEY_TIME_SET_LONG,  // 表示第一个按键开关的长按
    KEY_UP,             // 上调
    KEY_DOWN,           // 下调
    KEY_ALARM_SET,      // 时钟设置短按
    KEY_ALARM_SET_LONG, // 时钟设置长按
    KEY_ALARM_EN,       // 闹钟使能
    KEY_ALARM_5,        // 5天闹钟
} Key_type_value;

typedef enum
{
    LED_OFF = 0,
    LED_ON,
} LED_ON_Type_value;

typedef enum
{
    LIGHT_OFF = 0,
    LIGHT_ON,
} Light_Type_value;

/**
 * @brief 硬件接口层一般都需要初始化方法  => 对应GPIO引脚初始化
 * 
 */
void Inf_key_init(void);

/**
 * @brief 读取当前按键的值 => 一次读取当前是否有按键被按键
 *  Key_type_value: 返回对应被按下的按键值  NONE 表示没有按键被按下
 */
Key_type_value Inf_get_key_value(void);

/**
 * @brief 获取拨动开关当前的值
 * 
 * @return LED_ON_Type_value 
 */
LED_ON_Type_value Inf_get_led_on_value(void);

/**
 * @brief 获取当前是否为常量的值
 * 
 * @return Light_Type_value 
 */
Light_Type_value Inf_get_light_value(void);

#endif // __INF_KEY__
