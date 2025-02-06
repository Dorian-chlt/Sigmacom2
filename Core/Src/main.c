/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>   // Pour printf
#include <string.h>  // Pour strlen si besoin

void SystemClock_Config(void);
void SX1211_SelectConfig(void);
void SX1211_DeselectConfig(void);
uint8_t SX1211_ReadRegister(uint8_t reg);

void SendUART(const char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void SX1211_SelectData(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // NSS_DATA = LOW
}

void SX1211_DeselectData(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // NSS_DATA = HIGH
}

void SX1211_SelectConfig(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET); // NSS_CONFIG = LOW
}

void SX1211_DeselectConfig(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // NSS_CONFIG = HIGH
}

uint8_t SX1211_ReadRegister(uint8_t reg) {
    uint8_t data[2] = {reg & 0x7F, 0x00}; // Bit 7 à 0 pour lecture
    uint8_t received[2];

    SX1211_SelectConfig();
    HAL_SPI_TransmitReceive(&hspi1, data, received, 2, HAL_MAX_DELAY);
    SX1211_DeselectConfig();

    return received[1]; // Le 2ème octet contient la réponse
}

void SX1211_WriteRegister(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg | 0x80, value}; // Bit 7 à 1 pour écriture

    SX1211_SelectConfig();
    HAL_SPI_Transmit(&hspi1, data, 2, HAL_MAX_DELAY);
    SX1211_DeselectConfig();
}

uint8_t SX1211_ReadData(void) {
    uint8_t data[2] = {0x16 & 0x7F, 0x00}; // Bit 7 à 0 pour lecture
    uint8_t received[2];

    SX1211_SelectData();
    HAL_SPI_TransmitReceive(&hspi1, data, received, 2, HAL_MAX_DELAY);
    SX1211_DeselectData();

    return received[1]; // Octet reçu depuis la FIFO
}


int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();

    SendUART("Test SX1211 : Lecture du registre 0x01...\r\n");

    SX1211_WriteRegister(0x01, 0x49);
    HAL_Delay(10);

    uint8_t modeRegister = SX1211_ReadRegister(0x01);

    char buffer[50];
    sprintf(buffer, "Valeur lue: 0x%02X\r\n", modeRegister);
    SendUART(buffer);

    while (1)
    {
        HAL_Delay(1000);
    }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
