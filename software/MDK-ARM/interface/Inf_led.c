#include "Inf_led.h"

// 设计数组 保存时钟对应的数字
uint16_t led_clk_h_s[3] = {0x0000, 0x0006, 0x005b};
uint16_t led_clk_h_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

uint16_t led_clk_m_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_clk_m_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

// 温度的数组
uint16_t led_th_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_th_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

// 湿度的数组
uint16_t led_hu_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_hu_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

// 闪烁标签
extern uint8_t clock_flag;

// 引入触摸参数 => 控制不亮的时间  实现亮度控制
extern uint8_t touch_count;

/**
 * @brief 控制总开关
 *
 */
void Inf_led_start(void)
{
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 关闭总开关
 *
 */
void Inf_led_stop(void)
{
    LED_OEA_H;
    LED_OEB_H;
}

/**
 * @brief 写入数据
 *
 * @param p00 VCCP00 电源控制
 * @param p01 VCCP01 电源控制
 * @param p02 VCCP02 电源控制
 * @param data 两个驱动芯片的数据
 */
void Inf_led_write(uint8_t p00, uint8_t p01, uint8_t p02, uint32_t data)
{

    // 传输数据高位在前  先发的是后一个芯片 right
    for (uint8_t i = 0; i < 32; i++)
    {
        if (data & (0x80000000 >> i))
        {
            LED_SDI_H;
        }
        else
        {
            LED_SDI_L;
        }

        LED_CLK_H;
        LED_CLK_L;
    }

    // 关闭掉所有的灯   理论上彻底不会亮错误的灯
    // LED_OEA_H;
    // LED_OEB_H;
    // 锁存数据  => 执行锁存之后  数据在芯片中生效 修改了out
    LED_LE_H;
    LED_LE_L;
    // 打开所有的灯
    // 切换VCCP00-02供电
    HAL_GPIO_WritePin(VCCP00_GPIO_Port, VCCP00_Pin, (GPIO_PinState)p00);
    HAL_GPIO_WritePin(VCCP01_GPIO_Port, VCCP01_Pin, (GPIO_PinState)p01);
    HAL_GPIO_WritePin(VCCP02_GPIO_Port, VCCP02_Pin, (GPIO_PinState)p02);

    // OE 使能
    LED_OEA_L;
    LED_OEB_L;
}

/**
 * @brief 设置时钟
 *
 * @param h 小时
 * @param m 分钟
 */
void Inf_led_set_clock(uint8_t h, uint8_t m, uint8_t flag)
{
    // 先让h亮5ms
    uint8_t h_shi = h / 10;
    uint8_t h_ge = h % 10;
    uint16_t data = 0;
    // 添加点对应的数值
    if (flag)
    {
        data = led_clk_h_s[h_shi] + led_clk_h_g[h_ge] + 0x20;
    }
    else
    {
        data = led_clk_h_s[h_shi] + led_clk_h_g[h_ge];
    }

    Inf_led_write(0, 1, 1, data);

    vTaskDelay(pdMS_TO_TICKS(5U));

    uint8_t m_shi = m / 10;
    uint8_t m_ge = m % 10;

    Inf_led_write(1, 0, 1, led_clk_m_s[m_shi] + led_clk_m_g[m_ge]);
    vTaskDelay(pdMS_TO_TICKS(5U));
}

/**
 * @brief 同时展示温湿度和闹钟
 *
 * @param h 如果值大于99 不显示
 * @param m 如果值大于99 不显示
 * @param flag
 * @param temperature
 * @param humidity
 */
void Inf_led_set_all(uint8_t h, uint8_t m, uint8_t flag, int8_t temperature, int8_t humidity, Show_type_Struct *show_type)
{
    uint32_t vccp02_data = 0x38;

    // 一次性写入32位数据 => 先写高16位 => 先写right的数据
    // 1. 拼接VCCP00导电时候的  32data值
    // 1.1 湿度值=> 添加对应的单位符号0x4000
    uint16_t data_right = led_hu_s[humidity / 10] + led_hu_g[humidity % 10] + 0x4000;
    uint16_t am_pm_data = 0;
    // 1.2 添加AM/PM的灯
    if (show_type->is_12_hour)
    {
        if (show_type->is_pm)
        {
            am_pm_data = 0x8000;
        }
        else
        {
            am_pm_data = 0x4000;
        }
    }

    // 1.3 时钟的小时 添加对应的点
    uint16_t data_left = 0;
    if (flag)
    {
        // 添加逻辑 h > 99 表示不展示
        if (h > 99)
        {
            data_left = am_pm_data + 0x20;
        }
        else
        {
            data_left = led_clk_h_s[h / 10] + led_clk_h_g[h % 10] + am_pm_data + 0x20;
        }
    }
    else
    {
        if (h > 99)
        {
            data_left = am_pm_data;
        }
        else
        {
            data_left = led_clk_h_s[h / 10] + led_clk_h_g[h % 10] + am_pm_data;
        }
    }

    // 1.4 5天闹钟标识
    if (show_type->is_alarm_five_day)
    {
        data_right += 0x8000;
    }

    // 一次写入32位数据 => 两边都会亮
    Inf_led_write(0, 1, 1, (data_right << 16) | data_left);
    vTaskDelay(pdMS_TO_TICKS(5U - touch_count));

    // 2. 拼接VCCP01导电时候的  32data值
    // 2.0 区分摄氏度和华氏度

    // 默认就是摄氏度
    // 修正温度的值
    int8_t temp_now = temperature;
    uint16_t temperature_data = 0;
    if (show_type->is_celsius)
    {
        temperature_data = 0x4000;
    }
    else
    {
        temperature_data = 0x8000;
        // 计算为华氏度
        temp_now = (temperature * 9 / 5) + 32;
    }

    // 2.1 拼接温度 => 添加对应的摄氏度符号
    // 只保留数据的十位和个位  并且是整数
    if (temp_now < 0)
    {
        temp_now = -temp_now;
        // 需要点亮符号
        vccp02_data += 0x02;
    }
    if (temp_now > 99)
    {
        temp_now -= 100;
        // 点亮百位
        vccp02_data += 0x05;
    }

    data_right = led_th_s[temp_now / 10] + led_th_g[temp_now % 10] + temperature_data;

    // 2.2 拼接分钟
    if (m > 99)
    {
        data_left = 0;
    }
    else
    {
        data_left = led_clk_m_s[m / 10] + led_clk_m_g[m % 10];
    }

    // 2.3 添加闹钟标识
    uint16_t alarm_data = 0;
    if (show_type->is_alarm_1)
    {
        alarm_data += 0x4000;
    }
    if (show_type->is_alarm_2)
    {
        alarm_data += 0x8000;
    }
    data_left += alarm_data;
    // 不亮touch_count时间 => 关闭掉VCCP00
    HAL_GPIO_WritePin(VCCP00_GPIO_Port, VCCP00_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(touch_count));
    // 一次性写入32位数据 => 两边都会亮
    Inf_led_write(1, 0, 1, (data_right << 16) | data_left);
    vTaskDelay(pdMS_TO_TICKS(5U - touch_count));
    HAL_GPIO_WritePin(VCCP01_GPIO_Port, VCCP01_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(touch_count));
    // 3. VCCP02供电时电亮的灯
    Inf_led_write(1, 1, 0, (vccp02_data << 16));
    vTaskDelay(pdMS_TO_TICKS(5U - touch_count));
    // 关闭VCCP02一段时间
    HAL_GPIO_WritePin(VCCP02_GPIO_Port, VCCP02_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(touch_count));
    // 特殊灯 充电标识
    if (show_type->is_charging)
    {
        // 拉低P03
        HAL_GPIO_WritePin(P03_GPIO_Port, P03_Pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief 用于时间设置的时候 展示时钟的数字
 *
 * @param num1 大于99  表示不亮
 * @param num2 大于99  表示不亮
 * @param point 1亮   0不亮
 */
void Inf_led_time_set_show_num(uint16_t num1, uint16_t num2, uint8_t point)
{
    uint16_t vcc_p00_data = 0;
    uint16_t vcc_p01_data = 0;
    uint8_t point_data = 0;
    // 标记点是否亮
    if (point)
    {
        point_data = 0x20;
    }

    // 根据第一个数值计算vcc_p00的值
    if (num1 > 99)
    {
        vcc_p00_data = point_data + 0;
    }
    else
    {
        vcc_p00_data = led_clk_h_s[num1 / 10] + led_clk_h_g[num1 % 10] + point_data;
    }

    if (num2 > 99)
    {
        vcc_p01_data = 0;
    }
    else
    {
        vcc_p01_data = led_clk_m_s[num2 / 10] + led_clk_m_g[num2 % 10];
    }

    // 两次时间点亮
    Inf_led_write(0, 1, 1, vcc_p00_data);
    vTaskDelay(pdMS_TO_TICKS(5U));
    Inf_led_write(1, 0, 1, vcc_p01_data);
    vTaskDelay(pdMS_TO_TICKS(5U));
}

uint32_t time_set_last_time = 0;
uint8_t led_flag = 0;
/**
 * @brief 进入到设置时间页面之后  LED显示的内容
 *
 * @param time_set 显示的不同页面
 * @param value 显示的具体值
 */
void Inf_led_time_set(Show_Time_Set time_set, uint16_t value)
{
    // 每过1s闪一次  => 500ms 切换flag状态  =>  500ms时间(以5ms一次闪烁)  500ms根本不亮
    if (xTaskGetTickCount() - time_set_last_time >= 500)
    {
        led_flag = (led_flag + 1) % 2;
        time_set_last_time = xTaskGetTickCount();
    }

    switch (time_set)
    {
    case TIME_SET_YEAR:
        // 显示年份 => 可修改  => 不能再去采集数据
        // 一直显示20   闪烁显示后两位
        if (led_flag)
        {
            Inf_led_time_set_show_num(value / 100, value % 100, 0);
        }
        else
        {
            Inf_led_time_set_show_num(value / 100, 0xff, 0);
        }

        break;
    case TIME_SET_MONTH:
        // 显示月份 => 可修改  => 不能再去采集数据
        // 显示在前两位 闪烁  后两位灭掉
        if (led_flag)
        {
            Inf_led_time_set_show_num(value, 0xff, 0);
        }
        else
        {
            Inf_led_time_set_show_num(0xff, 0xff, 0);
        }

        break;
    case TIME_SET_DAY:
        // 显示天 => 可修改  => 不能再去采集数据
        // 显示后两位  闪烁  前两位灭掉
        if (led_flag)
        {
            Inf_led_time_set_show_num(0xff, value, 0);
        }
        else
        {
            Inf_led_time_set_show_num(0xff, 0xff, 0);
        }
        break;
    case TIME_SET_HOUR:
        // 区分时间设置时候的月份  => 点常亮
        // 显示小时 => 可修改  => 不能再去采集数据
        // 显示在前两位 闪烁  后两位灭掉
        if (led_flag)
        {
            Inf_led_time_set_show_num(value, 0xff, 1);
        }
        else
        {
            Inf_led_time_set_show_num(0xff, 0xff, 1);
        }
        break;
    case TIME_SET_MIN:
        // 显示年份 => 可修改  => 不能再去采集数据
        // 显示后两位  闪烁  前两位灭掉
        if (led_flag)
        {
            Inf_led_time_set_show_num(0xff, value, 1);
        }
        else
        {
            Inf_led_time_set_show_num(0xff, 0xff, 1);
        }
        break;
    default:
        break;
    }
}
