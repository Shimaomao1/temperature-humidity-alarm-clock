#include "App_dateTime.h"
#include <stddef.h>

static uint8_t App_dateTime_bcd_to_dec(uint8_t value)
{
    return (uint8_t)(((value >> 4U) * 10U) + (value & 0x0FU));
}

static uint8_t App_dateTime_dec_to_bcd(uint8_t value)
{
    return (uint8_t)(((value / 10U) << 4U) | (value % 10U));
}

/**
 * @brief Read the calendar fields used by the display task.
 *
 * The application stores hours in 24-hour format even if the RTC was
 * configured for 12-hour display mode.
 */
void App_dateTime_get(Clock_Date_time_type *date_time)
{
    uint8_t hour_data;

    if (date_time == NULL)
    {
        return;
    }

    date_time->year = (uint16_t)(2000U +
        App_dateTime_bcd_to_dec(Inf_DS1302Z_Read_Byte(DS1302Z_YEAR_REG)));
    date_time->month = App_dateTime_bcd_to_dec(
        Inf_DS1302Z_Read_Byte(DS1302Z_MONTH_REG));
    date_time->day = App_dateTime_bcd_to_dec(
        Inf_DS1302Z_Read_Byte(DS1302Z_DAY_REG));
    date_time->week = (uint8_t)(Inf_DS1302Z_Read_Byte(DS1302Z_WEEK_REG) & 0x0FU);

    hour_data = Inf_DS1302Z_Read_Byte(DS1302Z_HOUR_REG);
    if ((hour_data & 0x80U) != 0U)
    {
        /* 12-hour mode: bit 5 marks PM and 12 AM maps to 00:00. */
        uint8_t hour_12 = App_dateTime_bcd_to_dec(hour_data & 0x1FU);
        date_time->is_12_hour = 1U;
        date_time->is_pm = (uint8_t)((hour_data >> 5U) & 0x01U);
        if (hour_12 == 12U)
        {
            hour_12 = 0U;
        }
        date_time->hour = (int8_t)(hour_12 +
            (date_time->is_pm ? 12U : 0U));
    }
    else
    {
        /* 24-hour mode uses bits 5..4 for the tens digit. */
        date_time->is_12_hour = 0U;
        date_time->is_pm = 0U;
        date_time->hour = (int8_t)App_dateTime_bcd_to_dec(hour_data & 0x3FU);
    }

    date_time->is_12_hour = 0U;
    date_time->min = (int8_t)App_dateTime_bcd_to_dec(
        Inf_DS1302Z_Read_Byte(DS1302Z_MINUTE_REG));
}

/**
 * @brief Write the calendar fields and restart the RTC at 00 seconds.
 *
 * The RTC is always written in 24-hour BCD format. This avoids the noon and
 * midnight corner cases that occur when a 24-hour application value is sent
 * through the chip's 12-hour encoding.
 */
void App_dateTime_set(Clock_Date_time_type *date_time)
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t century;
    uint8_t month_for_formula;
    int16_t weekday;

    if (date_time == NULL)
    {
        return;
    }

    /* Unlock the RTC before changing the calendar. */
    Inf_DS1302Z_Write_Byte(DS1302Z_CONTROL_REG, 0x00U);

    Inf_DS1302Z_Write_Byte(DS1302Z_YEAR_REG,
        App_dateTime_dec_to_bcd((uint8_t)(date_time->year % 100U)));
    Inf_DS1302Z_Write_Byte(DS1302Z_MONTH_REG,
        App_dateTime_dec_to_bcd(date_time->month));
    Inf_DS1302Z_Write_Byte(DS1302Z_DAY_REG,
        App_dateTime_dec_to_bcd(date_time->day));
    Inf_DS1302Z_Write_Byte(DS1302Z_HOUR_REG,
        (uint8_t)(App_dateTime_dec_to_bcd((uint8_t)date_time->hour) & 0x3FU));
    Inf_DS1302Z_Write_Byte(DS1302Z_MINUTE_REG,
        App_dateTime_dec_to_bcd((uint8_t)date_time->min));
    Inf_DS1302Z_Write_Byte(DS1302Z_SECOND_REG, 0x00U);

    /* Zeller's congruence gives the weekday value expected by the display. */
    year = date_time->year;
    month = date_time->month;
    century = (uint8_t)(year / 100U);
    month_for_formula = month;
    if (month_for_formula <= 2U)
    {
        year--;
        month_for_formula = (uint8_t)(month_for_formula + 12U);
    }

    weekday = (int16_t)(year + year / 4U + century / 4U -
        2 * century + 26 * (month_for_formula + 1U) / 10U +
        date_time->day - 1);
    weekday = (int16_t)(weekday % 7);
    if (weekday <= 0)
    {
        weekday = (int16_t)(weekday + 7);
    }
    Inf_DS1302Z_Write_Byte(DS1302Z_WEEK_REG, (uint8_t)weekday);
}
