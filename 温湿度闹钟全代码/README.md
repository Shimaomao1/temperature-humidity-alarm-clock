# 固件目录说明

该目录是 STM32F103C8T6 的 Keil MDK-ARM 工程。打开 `MDK-ARM/P01_CLOCK_HAL.uvprojx` 即可查看 target 和源文件分组。

## 代码分层

- `Core/`：CubeMX 生成的芯片启动、时钟、GPIO、USART 和中断框架；只在 `USER CODE` 区域加入应用入口。
- `MDK-ARM/application/`：FreeRTOS 任务、时间/闹钟状态和页面显示逻辑。
- `MDK-ARM/interface/`：DHT11、DS1302Z、NV020D、LED、按键、触摸和麦克风接口实现。
- `MDK-ARM/common/`：调试输出封装。
- `MDK-ARM/freeRTOS/`：FreeRTOS 内核和 Cortex-M3 移植层。
- `Drivers/`：CMSIS、STM32F1 HAL 和芯片设备头文件。

## 入口和任务

`Core/Src/main.c` 完成 HAL 初始化后调用 `App_freeRTOS_start()`。调度器启动后创建五个任务：

| 任务 | 主要工作 |
| --- | --- |
| `collect` | 每秒读取一次 DS1302Z，约每 10 秒读取一次 DHT11。 |
| `show` | 刷新时间、温湿度和设置页面。 |
| `switch` | 扫描按键、拨动开关和触摸输入。 |
| `alarm` | 判断闹钟时间并控制 NV020D 播放/停止。 |
| `volume` | 调节音量页面和播放试听。 |

构建产生的中间文件和固件产物不纳入版本库，详见仓库根目录 `.gitignore`。
