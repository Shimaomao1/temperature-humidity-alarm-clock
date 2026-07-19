#include "App_show.h"

// ????????1s??1??
uint8_t clock_flag = 0;
uint32_t clock_last_time = 0;

// ???time_set_type
extern Show_Time_Set show_time_set;

// ???show_alarm_set
extern Show_alarm_Set show_alarm_set;

// ?????????????
extern Alarm_Time_Type alarm_time1;
extern Alarm_Time_Type alarm_time2;
/**
 * @brief ????? ????LED??
 *
 */
void App_show_start(void)
{
    Inf_led_start();
    // ????????? => ???????1s???§Ø?
    clock_last_time = xTaskGetTickCount();
}

/**
 * @brief ??????????LED???
 *  ????????1s??1??
 *
 */
void App_show_normal(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type)
{
    // 1. ????? => ??????1s?????
    // Inf_led_set_clock(clock_date_time->hour, clock_date_time->min, clock_flag);

    // ???1s?????  => 500ms ?§Ý?flag??  =>  500ms???(??5ms??????)  500ms????????
    if (xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag = (clock_flag + 1) % 2;
        clock_last_time = xTaskGetTickCount();
    }

    // ????????????? ????????????? => ??????????????¦Ë
    // 1. ????§³????  ???24h????????
    uint8_t hour = 0;
    // if (clock_date_time->is_12_hour)
    // {
    //     if (clock_date_time->is_pm)
    //     {
    //         hour = 12 + clock_date_time->hour;
    //     }
    //     else
    //     {
    //         hour = clock_date_time->hour;
    //     }
    // }
    // else
    // {
    //     hour = clock_date_time->hour;
    // }
    if (clock_date_time->is_12_hour && clock_date_time->is_pm)
    {
        hour = 12 + clock_date_time->hour;
    }
    else
    {
        hour = clock_date_time->hour;
    }

    // 2. ?§Ø???????????????
    if (show_type->is_12_hour)
    {
        if (hour >= 12)
        {
            hour = hour - 12;
            show_type->is_pm = 1;
        }
        else
        {
            show_type->is_pm = 0;
        }
    }
    

    // ??????????
    Inf_led_set_all(hour, clock_date_time->min, clock_flag, temperature, humidity, show_type);
}

/**
 * @brief ??????????????LED???
 *
 * @param clock_date_time
 */
void App_show_time_set(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type)
{

    if (xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag = (clock_flag + 1) % 2;
        clock_last_time = xTaskGetTickCount();
    }
    // ?§Ø????????????????????? => ????Inf_led_time_set???????
    // 1.?§Ø???????????
    // ???????????????????
    switch (show_time_set)
    {
    case TIME_SET_YEAR:
        // Inf_led_time_set(TIME_SET_YEAR, clock_date_time->year);
        if (clock_flag)
        {
            // ??
            Inf_led_set_all(clock_date_time->year / 100, clock_date_time->year % 100, 0, temperature, humidity, show_type);
        }
        else
        {
            // ?????????????????
            Inf_led_set_all(clock_date_time->year / 100, 0xff, 0, temperature, humidity, show_type);
        }
        break;
    case TIME_SET_MONTH:
        // Inf_led_time_set(TIME_SET_MONTH, clock_date_time->month);
        if (clock_flag)
        {
            // ??
            Inf_led_set_all(clock_date_time->month, 0xff, 0, temperature, humidity, show_type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 0, temperature, humidity, show_type);
        }
        break;
    case TIME_SET_DAY:
        // Inf_led_time_set(TIME_SET_DAY, clock_date_time->day);
        if (clock_flag)
        {
            Inf_led_set_all(0xff, clock_date_time->day, 0, temperature, humidity, show_type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 0, temperature, humidity, show_type);
        }

        break;
    case TIME_SET_HOUR:
        // Inf_led_time_set(TIME_SET_HOUR, clock_date_time->hour);
        if (clock_flag)
        {
            Inf_led_set_all(clock_date_time->hour, 0xff, 1, temperature, humidity, show_type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, temperature, humidity, show_type);
        }
        break;
    case TIME_SET_MIN:
        // Inf_led_time_set(TIME_SET_MIN, clock_date_time->min);
        if (clock_flag)
        {
            Inf_led_set_all(0xff, clock_date_time->min, 1, temperature, humidity, show_type);
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, temperature, humidity, show_type);
        }
        break;
    default:
        break;
    }
}

/**
 * @brief ???????????????LED???
 *
 */
void App_show_alarm_set(Clock_Date_time_type *clock_date_time, int8_t temperature, int8_t humidity, Show_type_Struct *show_type)
{

    if (xTaskGetTickCount() - clock_last_time >= 500)
    {
        clock_flag = (clock_flag + 1) % 2;
        clock_last_time = xTaskGetTickCount();
    }
    switch (show_alarm_set)
    {
    case ALARM_SET_HOUR:
        if (clock_flag)
        {
            // ??  => ?????????????§³?
            if (show_type->is_alarm_1)
            {
                Inf_led_set_all(alarm_time1.hour, 0xff, 1, temperature, humidity, show_type);
            }
            else if (show_type->is_alarm_2)
            {
                Inf_led_set_all(alarm_time2.hour, 0xff, 1, temperature, humidity, show_type);
            }
        }
        else
        {
            // ????
            Inf_led_set_all(0xff, 0xff, 1, temperature, humidity, show_type);
        }

        break;
    case ALARM_SET_MIN:
        // ?§Ø???
        if (clock_flag)
        {
            // ?§Ø?????????????
            if (show_type->is_alarm_1)
            {
                Inf_led_set_all(0xff, alarm_time1.min, 1, temperature, humidity, show_type);
            }
            else if (show_type->is_alarm_2)
            {
                Inf_led_set_all(0xff, alarm_time2.min, 1, temperature, humidity, show_type);
            }
        }
        else
        {
            Inf_led_set_all(0xff, 0xff, 1, temperature, humidity, show_type);
        }

        break;
    default:
        break;
    }
}
