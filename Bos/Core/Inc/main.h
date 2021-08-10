/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BAT_VOLT_Pin GPIO_PIN_3
#define BAT_VOLT_GPIO_Port GPIOC
#define ACS71240_Pin GPIO_PIN_2
#define ACS71240_GPIO_Port GPIOA
#define LM35_Pin GPIO_PIN_3
#define LM35_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOC
#define MOTOR_TIM1_CH1_Pin GPIO_PIN_9
#define MOTOR_TIM1_CH1_GPIO_Port GPIOE
#define SERVO_TIM1_CH2_Pin GPIO_PIN_11
#define SERVO_TIM1_CH2_GPIO_Port GPIOE
#define SERVO_TIM1_CH3_Pin GPIO_PIN_13
#define SERVO_TIM1_CH3_GPIO_Port GPIOE
#define SERVO_TIM1_CH4_Pin GPIO_PIN_14
#define SERVO_TIM1_CH4_GPIO_Port GPIOE
#define HMC_SDA_Pin GPIO_PIN_9
#define HMC_SDA_GPIO_Port GPIOC
#define HMC_SCL_Pin GPIO_PIN_8
#define HMC_SCL_GPIO_Port GPIOA
#define ESP_RX_STM_TX_Pin GPIO_PIN_12
#define ESP_RX_STM_TX_GPIO_Port GPIOC
#define ESP_TX_STM_RX_Pin GPIO_PIN_2
#define ESP_TX_STM_RX_GPIO_Port GPIOD
#define LPS_SCL_Pin GPIO_PIN_6
#define LPS_SCL_GPIO_Port GPIOB
#define LPS_SDA_Pin GPIO_PIN_7
#define LPS_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

typedef struct{
		float    Barometer_Pressure;
		float	 First_Barometer_Pressure;
		float	 Barometer_Altitude;
		float    Barometer_Speed;
}Barometer;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
