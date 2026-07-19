# 第三方代码与资料说明

本仓库除了项目本身的应用代码，还保留了构建固件所需的第三方代码。下面的目录不适用仓库根目录 MIT 声明的替代关系，使用时请保留原始版权和许可证文本。

| 目录 | 来源/说明 |
| --- | --- |
| `software/Drivers/CMSIS/` | Arm CMSIS 与 ST 设备支持文件，文件中保留 Apache-2.0、Arm 和 ST 的原始声明。 |
| `software/Drivers/STM32F1xx_HAL_Driver/` | STMicroelectronics STM32F1 HAL，文件中保留 ST 的 BSD 风格许可证声明。 |
| `software/MDK-ARM/freeRTOS/` | FreeRTOS 内核和 ARMCM3 移植层，文件中保留 Amazon FreeRTOS MIT SPDX 声明。 |

三款外设的通信时序说明引用了器件厂商或分销商公开资料，链接列在 `docs/protocols/` 对应文件末尾。
