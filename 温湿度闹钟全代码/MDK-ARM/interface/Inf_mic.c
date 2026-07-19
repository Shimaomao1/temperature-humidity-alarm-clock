#include "Inf_mic.h"


/**
 * @brief 根据MIC_IN的值判断是否有人说话
 * 
 * @return Mic_value 
 */
Mic_value Inf_get_mic_value(void)
{
    // 读取值为0的时候 有人大声说话
    // 读取值为1  说明没有
    if (HAL_GPIO_ReadPin(MIC_IN_GPIO_Port, MIC_IN_Pin) == GPIO_PIN_RESET)
    {
        return MIC_ON;
    }
    
    return MIC_OFF;
}
