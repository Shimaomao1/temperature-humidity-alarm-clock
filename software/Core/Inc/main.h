/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DS_CLK_Pin GPIO_PIN_0
#define DS_CLK_GPIO_Port GPIOA
#define DS_IO_Pin GPIO_PIN_1
#define DS_IO_GPIO_Port GPIOA
#define DS_RST_Pin GPIO_PIN_2
#define DS_RST_GPIO_Port GPIOA
#define NVD_BUSY_Pin GPIO_PIN_3
#define NVD_BUSY_GPIO_Port GPIOA
#define NVD_CLK_Pin GPIO_PIN_4
#define NVD_CLK_GPIO_Port GPIOA
#define NVD_SDA_Pin GPIO_PIN_5
#define NVD_SDA_GPIO_Port GPIOA
#define ALARM_SET_Pin GPIO_PIN_6
#define ALARM_SET_GPIO_Port GPIOA
#define ALARM_EN_Pin GPIO_PIN_7
#define ALARM_EN_GPIO_Port GPIOA
#define UP_Pin GPIO_PIN_0
#define UP_GPIO_Port GPIOB
#define DOWN_Pin GPIO_PIN_1
#define DOWN_GPIO_Port GPIOB
#define TIME_SET_Pin GPIO_PIN_2
#define TIME_SET_GPIO_Port GPIOB
#define LED_ON_Pin GPIO_PIN_10
#define LED_ON_GPIO_Port GPIOB
#define LIGHT_Pin GPIO_PIN_11
#define LIGHT_GPIO_Port GPIOB
#define MIC_VCC_Pin GPIO_PIN_12
#define MIC_VCC_GPIO_Port GPIOB
#define MIC_IN_Pin GPIO_PIN_13
#define MIC_IN_GPIO_Port GPIOB
#define TOUCH_Pin GPIO_PIN_14
#define TOUCH_GPIO_Port GPIOB
#define DHT11_DATA_Pin GPIO_PIN_15
#define DHT11_DATA_GPIO_Port GPIOB
#define ALARM_5_Pin GPIO_PIN_8
#define ALARM_5_GPIO_Port GPIOA
#define VCCP00_Pin GPIO_PIN_11
#define VCCP00_GPIO_Port GPIOA
#define VCCP01_Pin GPIO_PIN_12
#define VCCP01_GPIO_Port GPIOA
#define VCCP02_Pin GPIO_PIN_15
#define VCCP02_GPIO_Port GPIOA
#define LED_OEA_Pin GPIO_PIN_3
#define LED_OEA_GPIO_Port GPIOB
#define LED_OEB_Pin GPIO_PIN_4
#define LED_OEB_GPIO_Port GPIOB
#define LED_LE_Pin GPIO_PIN_5
#define LED_LE_GPIO_Port GPIOB
#define LED_CLK_Pin GPIO_PIN_6
#define LED_CLK_GPIO_Port GPIOB
#define LED_SDI_Pin GPIO_PIN_7
#define LED_SDI_GPIO_Port GPIOB
#define P03_Pin GPIO_PIN_8
#define P03_GPIO_Port GPIOB
#define P04_Pin GPIO_PIN_9
#define P04_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
