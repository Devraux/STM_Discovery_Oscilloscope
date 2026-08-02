/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stm32f7xx_it.h"
#include "stdbool.h"
#include "arm_math.h"


/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/* USER CODE BEGIN Private defines */
#define ADC_DATA_BUFFER_SIZE 		400
#define ADC_BYTE_DATA_BUFFER_SIZE 	ADC_DATA_BUFFER_SIZE * sizeof(uint32_t) // ADC_DATA_BUFFER_SIZE expressed in bytes
#define ADC_RESOLUTION 	0.00080566406 //this value is shorter due to float casting

typedef enum adc_active_buffer_t{
	active_buffer_1 = 1,
	active_buffer_2 = 2,
}adc_active_buffer_t;

/* USER CODE END Private defines */

void MX_ADC1_Init(void);
void MX_ADC3_Init(void);

/* USER CODE BEGIN Prototypes */
// Double buffering DMA & ADC
uint32_t *adc_get_data_buffer_1(void);
uint32_t *adc_get_data_buffer_2(void);
uint32_t *adc_get_current_buffer(void);


int32_t array_get_min(uint32_t *adc_buffer);
int32_t array_get_max(uint32_t *adc_buffer);

/* USER CODE END Prototypes */
 
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

