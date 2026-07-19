# software 目录说明

这里记录本项目自己编写的应用层和接口层代码，便于按模块阅读和移植。

## 自写代码分层

- `MDK-ARM/application/`：FreeRTOS 任务、时间/闹钟状态和页面显示逻辑。
- `MDK-ARM/interface/`：DHT11、DS1302Z、NV020D、LED、按键、触摸和麦克风接口实现。
- `MDK-ARM/common/`：调试输出封装。

## 入口和任务

启动流程进入 `App_freeRTOS_start()` 后创建五个任务：

| 任务 | 主要工作 |
| --- | --- |
| `collect` | 每秒读取一次 DS1302Z，约每 10 秒读取一次 DHT11。 |
| `show` | 刷新时间、温湿度和设置页面。 |
| `switch` | 扫描按键、拨动开关和触摸输入。 |
| `alarm` | 判断闹钟时间并控制 NV020D 播放/停止。 |
| `volume` | 调节音量页面和播放试听。 |
