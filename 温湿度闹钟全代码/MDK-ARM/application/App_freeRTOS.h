#ifndef __APP_FREERTOS__
#define __APP_FREERTOS__

#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "Inf_key.h"
#include "Inf_touch.h"
#include "Inf_mic.h"
#include "Inf_dht11.h"
#include "Inf_nv020d.h"


#include "App_dateTime.h"
#include "App_show.h"
#include "App_switch.h"
/**
 * @brief 启动FreeRTOS => 在main方法中调用  后面的代码不再生效
 * 
 */
void App_freeRTOS_start(void);

#endif // __APP_FREERTOS__
