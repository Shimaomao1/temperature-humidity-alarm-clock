#ifndef __COM_DEBUG_H
#define __COM_DEBUG_H

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"

#define DEBUG_ENABLE 1

#if DEBUG_ENABLE

// 统一使用通用的宏去做日志输出
#define debug_printf(format, ...) printf("[%s:%d]" format "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)

#else
#define debug_printf(...)

#endif // DEBUG_ENABLE

#endif // !__COM_DEBUG_H
