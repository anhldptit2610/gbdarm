/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32h7xx_hal.h"

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
#define E3_Pin GPIO_PIN_3
#define E3_GPIO_Port GPIOE
#define KEY_Pin GPIO_PIN_13
#define KEY_GPIO_Port GPIOC
#define JOYPAD_SELECT_Pin GPIO_PIN_0
#define JOYPAD_SELECT_GPIO_Port GPIOA
#define JOYPAD_START_Pin GPIO_PIN_1
#define JOYPAD_START_GPIO_Port GPIOA
#define JOYPAD_B_Pin GPIO_PIN_2
#define JOYPAD_B_GPIO_Port GPIOA
#define JOYPAD_A_Pin GPIO_PIN_3
#define JOYPAD_A_GPIO_Port GPIOA
#define JOYPAD_LEFT_Pin GPIO_PIN_4
#define JOYPAD_LEFT_GPIO_Port GPIOA
#define JOYPAD_RIGHT_Pin GPIO_PIN_5
#define JOYPAD_RIGHT_GPIO_Port GPIOA
#define JOYPAD_UP_Pin GPIO_PIN_6
#define JOYPAD_UP_GPIO_Port GPIOA
#define JOYPAD_DOWN_Pin GPIO_PIN_7
#define JOYPAD_DOWN_GPIO_Port GPIOA
#define ILI9225_DC_Pin GPIO_PIN_7
#define ILI9225_DC_GPIO_Port GPIOE
#define ILI9225_RST_Pin GPIO_PIN_8
#define ILI9225_RST_GPIO_Port GPIOE
#define ILI9225_CS_Pin GPIO_PIN_9
#define ILI9225_CS_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
