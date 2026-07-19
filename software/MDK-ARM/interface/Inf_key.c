#include "Inf_key.h"

/**
 * @brief 硬件接口层一般都需要初始化方法  => 对应GPIO引脚初始化
 *
 */
void Inf_key_init(void)
{
    // MX_GPIO_Init();  => 在main.c中已经调用过了
}

/**
 * @brief 获取指定按键是否被按下
 *
 * @param GPIOx
 * @param GPIO_Pin
 * @return uint8_t 1 表示被按下  0 表示没有被按下
 */
uint8_t Inf_get_key_press(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
    {
        // 1.1  消抖判断  按下按钮了  =>  需要等待抬起 => 物理按键需要有延时消抖逻辑
        vTaskDelay(pdMS_TO_TICKS(10U));
        if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
        {
            // 1.2 消抖完成  用户确实按下  => 等待用户抬起引脚
            while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(pdMS_TO_TICKS(1U));
            }
            // 1.3 用户按下抬起了
            return 1;
        }
    }
    return 0;
}

/**
 * @brief 读取当前按键的值 => 一次读取当前是否有按键被按键
 *  Key_type_value: 返回对应被按下的按键值  NONE 表示没有按键被按下
 */
Key_type_value Inf_get_key_value(void)
{
    // 确认短按逻辑  => 按键的所有值默认是1   用户按下之后显示为0  => 抬起生效
    // 1. 确定用户按下了对应按钮
    if (Inf_get_key_press(UP_GPIO_Port, UP_Pin) == 1)
    {
        return KEY_UP;
    }
    else if (Inf_get_key_press(DOWN_GPIO_Port, DOWN_Pin) == 1)
    {
        return KEY_DOWN;
    }
    else if (Inf_get_key_press(ALARM_EN_GPIO_Port, ALARM_EN_Pin) == 1)
    {
        return KEY_ALARM_EN;
    }
    else if (Inf_get_key_press(ALARM_5_GPIO_Port, ALARM_5_Pin) == 1)
    {
        return KEY_ALARM_5;
    }

    // 添加长按逻辑  时间设置
    if (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
    {
        // 1.1 消抖判断  按下按钮了  =>  需要等待抬起 => 物理按键需要有延时消抖逻辑
        vTaskDelay(pdMS_TO_TICKS(10U));
        if (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
        {
            // 1.2 真的按下按键 => 计时  判断是否为长按
            TickType_t start_tick = xTaskGetTickCount(); // 获取当前时间

            // 1.3 等待抬起
            while (HAL_GPIO_ReadPin(TIME_SET_GPIO_Port, TIME_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(pdMS_TO_TICKS(1U));
            }
            // 1.4 已经抬起  => 计算按下的时间
            TickType_t end_tick = xTaskGetTickCount(); // 获取当前时间

            // 1.5 判断时长是否超过3s
            if (end_tick - start_tick >= 3000)
            {
                return KEY_TIME_SET_LONG;
            }
            else
            {
                return KEY_TIME_SET;
            }
        }
    }

    // 闹钟设置
    if (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
    {
        // 1.1 消抖判断  按下按钮了  =>  需要等待抬起 => 物理按键需要有延时消抖逻辑
        vTaskDelay(pdMS_TO_TICKS(10U));
        if (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
        {
            // 1.2 真的按下按键 => 计时  判断是否为长按
            TickType_t start_tick = xTaskGetTickCount(); // 获取当前时间

            // 1.3 等待抬起
            while (HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port, ALARM_SET_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(pdMS_TO_TICKS(1U));
            }
            // 1.4 已经抬起  => 计算按下的时间
            TickType_t end_tick = xTaskGetTickCount(); // 获取当前时间

            // 1.5 判断时长是否超过3s
            if (end_tick - start_tick >= 3000)
            {
                return KEY_ALARM_SET_LONG;
            }
            else
            {
                return KEY_ALARM_SET;
            }
        }
    }

    // 最后返回的是没有按键按下
    return KEY_NONE;
}

/**
 * @brief 获取拨动开关当前的值
 *
 * @return LED_ON_Type_value
 */
LED_ON_Type_value Inf_get_led_on_value(void)
{
    if (HAL_GPIO_ReadPin(LED_ON_GPIO_Port, LED_ON_Pin) == GPIO_PIN_RESET)
    {
        // 对应开
        return LED_OFF;
    }
    return LED_ON;
}

/**
 * @brief 获取当前是否为常量的值
 *
 * @return Light_Type_value.
 */
Light_Type_value Inf_get_light_value(void)
{
    if (HAL_GPIO_ReadPin(LIGHT_GPIO_Port, LIGHT_Pin) == GPIO_PIN_RESET)
    {
        return LIGHT_OFF;
    }

    return LIGHT_ON;
}
