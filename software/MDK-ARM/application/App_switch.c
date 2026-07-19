#include "App_switch.h"

// 展示模式
extern Show_Mode_type show_mode;
extern Show_Time_Set show_time_set;
extern Show_alarm_Set show_alarm_set;
extern Clock_Date_time_type clock_date_time;
// 展示类型
extern Show_type_Struct show_type;
// led灯总开关
extern uint8_t led_en;

// 切换闹钟状态
uint8_t switch_alarn_en = 0;

// 设置的两个闹钟
extern Alarm_Time_Type alarm_time1;
extern Alarm_Time_Type alarm_time2;

// 声控标签
uint8_t sound_control_flag = 0;
uint32_t sound_control_last_time = 0;
/**
 * @brief 处理开关的按键事件
 *
 */
void App_switch_key_process(void)
{
    // 1. 周期读取按键开关的值
    Key_type_value key_value = Inf_get_key_value();

    // 2. 处理按键开关的触发
    switch (key_value)
    {
    case KEY_TIME_SET:
        // 短按时间设置 => 切换12/24h
        show_type.is_12_hour = (show_type.is_12_hour + 1) % 2;
        break;
    case KEY_TIME_SET_LONG:
        // TODO: 进入到时间设置模块
        // 切换模式到时间设置
        show_mode = SHOW_MODE_TIME_SET;
        // 时间设置统一使用24h制
        show_type.is_12_hour = 0;
        if (clock_date_time.is_12_hour == 1 && clock_date_time.is_pm == 1)
        {
            clock_date_time.hour += 12;
            clock_date_time.is_12_hour = 0;
        }

        break;
    case KEY_UP:
        show_type.is_celsius = (show_type.is_celsius + 1) % 2;
        break;
    case KEY_DOWN:
        // TODO: 播放音频 调节音量模式
        show_mode = SHOW_MODE_VOLUME;
        break;
    case KEY_ALARM_SET:
        // TODO: 播放音频 调节音量模式
        show_mode = SHOW_MODE_VOLUME;
        break;
    case KEY_ALARM_SET_LONG:
        // TODO: 进入到闹钟设置模块
        show_mode = SHOW_MODE_ALARM_SET;
        // 闹钟设置也统一使用24h制  => 设置闹钟的时候强制切换为24h
        show_type.is_12_hour = 0;
        // 默认设置的就是闹钟1
        show_type.is_alarm_1 = 1;
        show_type.is_alarm_2 = 0;
        break;
    case KEY_ALARM_EN:
        switch_alarn_en++;
        if (switch_alarn_en % 2 == 0)
        {
            show_type.is_alarm_1 = 0;
            show_type.is_alarm_2 = 0;
        }
        else if (switch_alarn_en == 1)
        {
            show_type.is_alarm_1 = 1;
        }
        else if (switch_alarn_en == 3)
        {
            show_type.is_alarm_2 = 1;
        }
        else if (switch_alarn_en == 5)
        {
            show_type.is_alarm_1 = 1;
            show_type.is_alarm_2 = 1;
        }
        if (switch_alarn_en == 6)
        {
            switch_alarn_en = 0;
        }

        break;
    case KEY_ALARM_5:
        show_type.is_alarm_five_day = (show_type.is_alarm_five_day + 1) % 2;
        break;
    default:
        break;
    }
}

/**
 * @brief 处理两个拨动开关
 *
 */
void App_switch_rotary_process(void)
{
    // 1. 读取拨动开关当前的值
    LED_ON_Type_value led_on_value = Inf_get_led_on_value();

    // 当为light_off时  表示声控模式   =>   连接声控开关 => 一次接受到声控值 亮5s
    Light_Type_value light_type_value = Inf_get_light_value();

    // 读取声控开关的值
    Mic_value mic_value = Inf_get_mic_value();
    if (mic_value == MIC_ON)
    {
        // 说明有人说话  => 5s之后变成0
        sound_control_flag = 1;
        sound_control_last_time = xTaskGetTickCount();
    }

    // 先判断总开关
    if (led_on_value == LED_ON)
    {
        // 判断常量或者声控模式
        if (light_type_value == LIGHT_ON)
        {
            // 常量模式 => 直接打开
            led_en = 1;
        }
        // 声控模式
        else if (sound_control_flag == 1)
        {

            // 亮的5s
            if (xTaskGetTickCount() - sound_control_last_time <= 5000)
            {
                // 5s内  亮
                led_en = 1;
            }
            else
            {
                led_en = 0;
                sound_control_flag = 0;
            }
        }
        else
        {
            led_en = 0;
        }
    }
    else
    {
        led_en = 0;
    }
}

void App_switch_time_set_process(void)
{
    // 获取当前的按键值
    Key_type_value key_value = Inf_get_key_value();

    // 根据按键值进行处理
    switch (key_value)
    {
    case KEY_UP:
        // 当前页面值+1
        // 1. 判断当前页面
        if (show_time_set == TIME_SET_YEAR)
        {
            // 当前页面是年设置 => 按下up
            clock_date_time.year++;
            if (clock_date_time.year > 2099)
            {
                clock_date_time.year = 2000;
            }
        }
        else if (show_time_set == TIME_SET_MONTH)
        {
            // 当前页面是月设置
            clock_date_time.month++;
            if (clock_date_time.month > 12)
            {
                clock_date_time.month = 1;
            }
        }
        else if (show_time_set == TIME_SET_DAY)
        {
            // 当前页面是日设置
            clock_date_time.day++;
            if (clock_date_time.day > 31)
            {
                clock_date_time.day = 1;
            }
        }
        else if (show_time_set == TIME_SET_HOUR)
        {
            clock_date_time.hour++;
            if (clock_date_time.hour > 23)
            {
                clock_date_time.hour = 0;
            }
        }
        else if (show_time_set == TIME_SET_MIN)
        {
            clock_date_time.min++;
            if (clock_date_time.min > 59)
            {
                clock_date_time.min = 0;
            }
        }

        break;
    case KEY_DOWN:
        // 当前页面值-1
        if (show_time_set == TIME_SET_YEAR)
        {
            clock_date_time.year--;
            if (clock_date_time.year < 2000)
            {
                clock_date_time.year = 2099;
            }
        }
        else if (show_time_set == TIME_SET_MONTH)
        {
            clock_date_time.month--;
            if (clock_date_time.month < 1)
            {
                clock_date_time.month = 12;
            }
        }
        else if (show_time_set == TIME_SET_DAY)
        {
            clock_date_time.day--;
            if (clock_date_time.day < 1)
            {
                clock_date_time.day = 31;
            }
        }
        else if (show_time_set == TIME_SET_HOUR)
        {
            clock_date_time.hour--;
            if (clock_date_time.hour < 0)
            {
                clock_date_time.hour = 23;
            }
        }
        else if (show_time_set == TIME_SET_MIN)
        {
            clock_date_time.min--;
            if (clock_date_time.min < 0)
            {
                clock_date_time.min = 59;
            }
        }
        break;
    case KEY_TIME_SET:
        // 进入到下一个页面的设置  => 如果已经是最后一个页面 => 再按退出时间设置模式
        show_time_set++;
        if (show_time_set == TIME_SET_END)
        {
            // 已经到最后页面 要退出 => 将时间设置页面退出到正常页面
            show_mode = SHOW_MODE_NORMAL;

            // 设置时间
            clock_date_time.is_12_hour = show_type.is_12_hour;
            clock_date_time.is_pm = show_type.is_pm;
            App_dateTime_set(&clock_date_time);
            show_time_set = TIME_SET_YEAR;
        }

        break;
    default:
        break;
    }
}

/**
 * @brief 处理闹钟设置的按键事件
 *
 */
void App_switch_alarm_set_process(void)
{
    // 有四个按键可以使用
    Key_type_value key_value = Inf_get_key_value();

    switch (key_value)
    {
    case KEY_UP:
        // 设置闹钟的时间 +1
        if (show_alarm_set == ALARM_SET_HOUR)
        {
            if (show_type.is_alarm_1 == 1)
            {
                // 设置闹钟的时间即可
                alarm_time1.hour++;
                if (alarm_time1.hour > 23)
                {
                    alarm_time1.hour = 0;
                }
            }
            else if (show_type.is_alarm_2 == 1)
            {
                alarm_time2.hour++;
                if (alarm_time2.hour > 23)
                {
                    alarm_time2.hour = 0;
                }
            }
        }
        else if (show_alarm_set == ALARM_SET_MIN)
        {
            if (show_type.is_alarm_1 == 1)
            {
                alarm_time1.min++;
                if (alarm_time1.min > 59)
                {
                    alarm_time1.min = 0;
                }
            }
            else if (show_type.is_alarm_2 == 1)
            {
                alarm_time2.min++;
                if (alarm_time2.min > 59)
                {
                    alarm_time2.min = 0;
                }
            }
        }

        break;
    case KEY_DOWN:
        // 设置闹钟的时间 -1
        // 判断不同的当前页面
        if (show_alarm_set == ALARM_SET_HOUR)
        {
            if (show_type.is_alarm_1 == 1)
            {
                alarm_time1.hour--;
                if (alarm_time1.hour < 0)
                {
                    alarm_time1.hour = 23;
                }
            }
            else if (show_type.is_alarm_2 == 1)
            {
                alarm_time2.hour--;
                if (alarm_time2.hour < 0)
                {
                    alarm_time2.hour = 23;
                }
            }
        }
        else if (show_alarm_set == ALARM_SET_MIN)
        {
            if (show_type.is_alarm_1 == 1)
            {
                alarm_time1.min--;
                if (alarm_time1.min < 0)
                {
                    alarm_time1.min = 59;
                }
            }
            else if (show_type.is_alarm_2 == 1)
            {
                alarm_time2.min--;
                if (alarm_time2.min < 0)
                {
                    alarm_time2.min = 59;
                }
            }
        }

        break;
    case KEY_ALARM_SET:
        // 确认页面到下一级配置  => 页面设置完成之后 记得保存闹钟时间到全局变量
        show_alarm_set++;
        if (show_alarm_set == ALARM_SET_END)
        {
            // 退出闹钟设置页面 保存好闹钟的时间
            // 设置完闹钟之后 要让switch_alarn_en值对应当前的闹钟设置
            if (switch_alarn_en % 2 == 0)
            {
                // 此时没有打开闹钟 => 设置的哪个闹钟  打开哪个闹钟
                if (show_type.is_alarm_1 == 1)
                {
                    switch_alarn_en = 1;
                }
                else if (show_type.is_alarm_2 == 1)
                {
                    switch_alarn_en = 3;
                }
            }
            else if (switch_alarn_en == 1)
            {
                // 闹钟1已经打开
                if (show_type.is_alarm_2 == 1)
                {
                    // 将两个闹钟都打开
                    switch_alarn_en = 5;
                    show_type.is_alarm_1 = 1;
                }
            }
            else if (switch_alarn_en == 3)
            {
                // 闹钟2已经打开
                if (show_type.is_alarm_1 == 1)
                {
                    // 将两个闹钟都打开
                    switch_alarn_en = 5;
                    show_type.is_alarm_2 = 1;
                }
            }
            // 状态切换回normal
            show_mode = SHOW_MODE_NORMAL;
            show_alarm_set = ALARM_SET_HOUR;
        }

        break;
    case KEY_ALARM_EN:
        // 随时切换是闹钟1-2
        if (show_type.is_alarm_1 == 1)
        {
            show_type.is_alarm_1 = 0;
            show_type.is_alarm_2 = 1;
        }
        else if (show_type.is_alarm_2 == 1)
        {
            show_type.is_alarm_2 = 0;
            show_type.is_alarm_1 = 1;
        }

        break;
    default:
        break;
    }
}
