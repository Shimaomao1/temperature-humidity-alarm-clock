#ifndef __INF_TOUCH__
#define __INF_TOUCH__

#include "gpio.h"

typedef enum
{
    TOUCH,
    NO_TOUCH
}Touch_value;

/**
 * @brief ÅÐ¶ÏÊÇ·ñ±»´¥Ãþ
 * 
 * @return Touch_value 
 */
Touch_value Inf_get_touch_value(void);

#endif // __INF_TOUCH__
