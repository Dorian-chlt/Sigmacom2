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

#define SIZEOF_HEADER     6 
uint32_t netid = 0xFFFFFFFF;
void SystemClock_Config(void);
uint8_t newnetid;

RAM2EEP SRAM2EEP;

APU_SX1211 TrameRx;
APU_SX1211 TrameEnvoie;
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

void ReadAndPrintIRQs(void)
{
    char buffer[64];

    uint8_t irq0_val = IRQ_0;  // Lire IRQ_0 (GPIOB, PIN_6)
    uint8_t irq1_val = IRQ_1;  // Lire IRQ_1 (GPIOA, PIN_8)

    snprintf(buffer, sizeof(buffer), "IRQ_0: %d, IRQ_1: %d\r\n", irq0_val, irq1_val);
    SendUART(buffer);  // Envoie la valeur des IRQs sur UART
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

    HAL_Delay(1000);

    l3_set_netid(MATCHING_NETID); /* Ecriture de l'adresse de matching dans le sx1211 */

    SetRFMode(RF_RECEIVER);

    while (1)
    {
       ReadAndPrintIRQs();  // Lire et afficher les IRQs
       HAL_Delay(200);  // Pause de 500 ms

      
          if (l3_net_to_appli(&TrameRx))  // Si un message RF est reçu
          {
            SendUART("Message RF reçu !\r\n");
            if(!(TrameRx.Adresse & 0x80)){
         
              if(true)
                { // On vérifie la clé sauf dans le cas du banc test        
                  MODBUS_mb_req_pdu(&TrameEnvoie,&TrameRx); //Traitement de la trame recu 
                  // On répond. On ajoute la partie fixe layer3 
                  TrameEnvoie.Taille += SIZEOF_HEADER;
                  TrameEnvoie.Dest = TrameRx.Exp; // A destination de celui qui a envoyé la question 
                  TrameEnvoie.Exp = 8;      //Mon Node ID 
                  TrameEnvoie.Cle = ucIDKEYX;     // Clé du périphérique 
                  TrameEnvoie.Numero = TrameRx.Numero; // On répond au numéro de message de la requête 
                  TrameEnvoie.Adresse = TrameRx.Adresse | 0x80 ; // on met MSB à 1 pour indiquer qu'on envoie une réponse 
                 
                  RF_TransmitFrame((uint8_t *)&TrameEnvoie.Taille);  
                  
                  uint32_t combinedData = (TrameRx.Modbus.Datas[1] << 24) |
                        (TrameRx.Modbus.Datas[2] << 16) |
                        (TrameRx.Modbus.Datas[3] << 8)  |
                        (TrameRx.Modbus.Datas[4]);
                  
                  char buffer[64];
                  snprintf(buffer, sizeof(buffer), "Valeur combinée: 0x%08X\r\n", combinedData);
                  SendUART(buffer);
         
                  SendUART("Message RF envoyé !\r\n");
                  
                  RF_MatchingEnd();                     
                }
             }
          
     
  
  /*    

uint8_t rfBuffer[SX1211_FIFO_SIZE];  // Taille max de la FIFO
 
if (RF_ReceiveFrame(rfBuffer))  // Si un message RF est reçu
{
    SendUART("📡 Message RF reçu !\r\n");

    char buffer[256] = {0};  // Buffer pour l'affichage
    sprintf(buffer, "Données RF : ");

    // Construire la trame complète sous forme hexadécimale
    for (uint8_t i = 0; i < SX1211_FIFO_SIZE; i++) {
        char byte_str[4];  // Buffer pour "0xXX "
        sprintf(byte_str, "%02X ", rfBuffer[i]);  // Convertit chaque octet en hexadécimal
        strcat(buffer, byte_str);
    }
    strcat(buffer, "\r\n");

    SendUART(buffer); // Envoie la trame complète en UART

    // ============================
    // EXTRACTION DES 6 PREMIERS OCTETS
    // ============================
    uint8_t taille   = rfBuffer[0]; // Taille du message
    uint8_t source   = rfBuffer[1]; // Source du message
    uint8_t exp      = rfBuffer[2]; // Expéditeur
    uint8_t cle      = rfBuffer[3]; // Clé de validation
    uint8_t numero   = rfBuffer[4]; // Numéro du message
    uint8_t adresse  = rfBuffer[5]; // Adresse cible

    // ============================
    // EXTRACTION DU RESTE DE LA TRAME (58 OCTETS)
    // ============================
    uint8_t data[SX1211_FIFO_SIZE - 6];  // Stocker le payload
    memcpy(data, &rfBuffer[6], SX1211_FIFO_SIZE - 6);  // Copie du payload

    // ============================
    // AFFICHAGE DES VALEURS EXTRAITES
    // ============================
    char infoBuffer[200]; // Buffer pour afficher les détails
    snprintf(infoBuffer, sizeof(infoBuffer),
        "📡 Trame RF analysée :\r\n"
        "Taille   : 0x%02X (%d)\r\n"
        "Source   : 0x%02X\r\n"
        "Exp      : 0x%02X\r\n"
        "Clé      : 0x%02X\r\n"
        "Numéro   : 0x%02X\r\n"
        "Adresse  : 0x%02X\r\n",
        taille, taille, source, exp, cle, numero, adresse
    );
    SendUART(infoBuffer);
    // ============================
    // AFFICHAGE DU PAYLOAD (RESTE DES DONNÉES)
    // ============================
    SendUART("Données (payload) : ");
    for (uint8_t i = 0; i < SX1211_FIFO_SIZE - 6; i++) {
        snprintf(infoBuffer, sizeof(infoBuffer), "%02X ", data[i]);
        SendUART(infoBuffer);
    }
    SendUART("\r\n");
    
    
    SendUART("Enoie d'une reponse ");
    
    MODBUS_mb_req_pdu(&TrameEnvoie,&TrameRx);
    
    TrameEnvoie.Taille += SIZEOF_HEADER;
    TrameEnvoie.Dest = exp; // A destination de celui qui a envoyé la question 
    TrameEnvoie.Exp = 8;      //Mon Node ID 
    TrameEnvoie.Cle = cle;     // Clé du périphérique 
    TrameEnvoie.Numero = numero; // On répond au numéro de message de la requête 
    TrameEnvoie.Adresse = adresse | 0x80 ; // on met MSB à 1 pour indiquer qu'on envoie une réponse 
   
    RF_TransmitFrame((uint8_t *)&TrameEnvoie.Taille); 
    
    SendUART("Reponse envoyé ");
    */

      }
      else
      {
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
