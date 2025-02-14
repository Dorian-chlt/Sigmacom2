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
#include "sx1211.h"
#include "defs.h"
#include "Modbus.h"
#include "EEPROM.h"
#include "ERS2_Messages.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>   // Pour printf
#include <string.h>  // Pour strlen si besoin

uint32_t netid = 0xFFFFFFFF;
void SystemClock_Config(void);

RAM2EEP SRAM2EEP;

APU_SX1211 TrameRx;
APU_SX1211 TrameTx;
uint8_t ucMEMFAV_SA;
uint8_t ucMEMFAV_SM;
uint8_t ucMEMFAV_SJ;
uint16_t uiMEMFAV_CS;

bool l3_net_to_appli(APU_SX1211* p)
{
  return(RF_ReceiveFrame((uint8_t *)&p->Taille));
}

void l3_set_netid(uint32_t netid)
{

  RF_SetCurrentNetid(netid);  /* Appel a la couche 2 du SX1211 */

}
void RF_MatchingEnd(void)
{
  if(FLAG_READEND){
    if(FLAG_READCHK){
      ucIDKEYX = (uint8_t)(SRegistresRW_THM.Reg_41502[2]);/* maj de la variable courante en RAM avant la sauvegarde en NVM. */
      read_eeprom((uint8_t *)&SRegistresRW_THM.Reg_41502[0], 4, &ucIDLAN0); /* lecture de l'eeprom */
    }
    EEP_Update(); /* Update Ram contents to virtual EEPROM */
    FLAG_RESETMCU = true;
  }
}

void SendUART(const char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}



int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init();
    
    if(RF_Configuration()){
      SendUART("configue reussi\r\n");
    }else{
      SendUART("configue echouee\r\n");
    }

    HAL_Delay(3000);

    l3_set_netid(MATCHING_NETID); /* Ecriture de l'adresse de matching dans le sx1211 */

    SetRFMode(RF_RECEIVER);

    while (1)
    {
  
          if (l3_net_to_appli(&TrameRx))  // Si un message RF est reçu
          {
            SendUART("Message RF reçu !\r\n");
            if(!(TrameRx.Adresse & 0x80)){
          /* MSB de Adresse à 0 -> le distant a envoyé une question */
              if(FLAG_ACT_TIM5msRUN_BY_MATF || FLAG_ACT_TIM5msRUN_BY_MATC || (!(FLAG_ACT_MATC)&&(TrameRx.Cle == ucIDKEYX)) || TrameRx.Exp == NID_Banc_test)
                { /* On vérifie la clé sauf dans le cas du banc test */         
                MODBUS_mb_req_pdu(&TrameTx,&TrameRx); /* Traitement de la trame recu */
                /* On répond. On ajoute la partie fixe layer3 */
                TrameTx.Taille += SIZEOF_HEADER;
                TrameTx.Dest = TrameRx.Exp; /* A destination de celui qui a envoyé la question */
                TrameTx.Exp = ucIDNODE;     /* Mon Node ID */
                TrameTx.Cle = ucIDKEYX;     /* Clé du périphérique */
                TrameTx.Numero = TrameRx.Numero; /* On répond au numéro de message de la requête */
                TrameTx.Adresse = TrameRx.Adresse | 0x80 ; /* on met MSB à 1 pour indiquer qu'on envoie une réponse */
               
                RF_TransmitFrame((uint8_t *)&TrameTx.Taille);  /* C'est parti !! */
       
                SendUART("Message RF envoyé !\r\n");
                
                  RF_MatchingEnd();                     /* dans le cas d'un matching */
                }
             }
          }else{
            SendUART("Aucun message RF reçu.\r\n");
          }
     
  
      HAL_Delay(720);
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
