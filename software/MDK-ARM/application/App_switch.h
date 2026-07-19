#ifndef __APP_SWITCH__
#define __APP_SWITCH__

#include "Inf_key.h"
#include "Inf_led.h"
#include "Inf_mic.h"
#include "App_show.h"
/**
 * @brief 处理开关的按键事件
 * 
 */
void App_switch_key_process(void);

/**
 * @brief 处理两个拨动开关
 * 
 */
void App_switch_rotary_process(void);

/**
 * @brief 处理时间设置的按键事件
 * 
 */
void App_switch_time_set_process(void);


/**
 * @brief 处理闹钟设置的按键事件
 * 
 */
void App_switch_alarm_set_process(void);

#endif // __APP_SWITCH__
