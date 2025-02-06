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
#include <stdio.h>   // Pour printf
#include <string.h>  // Pour strlen si besoin

/* Init en mode packet (mode de fonctionnement normal) */
const uint8_t RegistersCfg[] = {
/*======================================== Main parameters common to receive & transmit modes ===================================================*/
/* 0:7-5 Chip_mode: Standby mode 4-3 Freq_band: 863-870 band 2-1 VCO_trim: Vtune no correction 0 RPS_select: R1P1S1 group */
DEF_MCPARAM1 | RF_MC1_STANDBY | RF_MC1_BAND_868 | RF_MC1_VCO_TRIM_00 | RF_MC1_RPS_SELECT_1,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 1:7-6 Modul_select:FSK 5 Data_mode_0:Packet mode 4-3 OOK_thresh_type: OOK peak mode 2 Data_mode_1:Packet mode 1-0 IF_gain: IF Chain gain 0dB */
DEF_MCPARAM2 | RF_MC2_MODULATION_FSK | RF_MC2_DATA_MODE_PACKET | RF_MC2_OOK_THRESH_TYPE_PEAK | RF_MC2_GAIN_IF_00,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 2:7-0 Freq_dev: Fdev=Fxtal / [32x(Freq_dev+1)] +/- 50kHz */
DEF_FDEV | RF_FDEV_50,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 3:7 Res 6-0 BR: BitRate=Fxtal/(64(BR+1)) Bitrate = 25kbauds */
DEF_BITRATE | RF_BITRATE_25000,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 4: 7-0 OOK_floor_thres: OOK floor threshold detect 12*0,5=6dB */
DEF_OOKFLOORTHRESH | RF_OOKFLOORTHRESH_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 5:7-6 Fifo_size:FIFO size 64 bytes 5-0 Fifo_thres: FIFO interrupt threshold = 15 bytes */
DEF_MCPARAM6 | RF_MC6_FIFO_SIZE_64 | RF_MC6_FIFO_THRESH_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 6,7,8 : R1,P1,S1 */
DEF_R1 | RF_R1_VALUE,
DEF_P1 | RF_P1_VALUE,
DEF_S1 | RF_S1_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 9,10,11 : R2,P2,S2 */
DEF_R2 | RF_R2_VALUE,
DEF_P2 | RF_P2_VALUE,
DEF_S2 | RF_S2_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 12: 7-5 Res 4-3 PA_ramp PA ramp 23us 2-0 Res */
DEF_PARAMP | RF_PARAMP_11,
/*======================================== Interrupt registers ==================================================================================*/
/* 13:Packet mode:
                  IRQ_1                   IRQ_0
RX & Standby      CRC OK                  FIFO_EMPTY
TX                TX Done                 /FIFO_EMPTY */
/* 7-6 Rx_stby_irq_0: IRQ0 source in RX & Standby FifoEmpty 5-4 Rx_stby_irq_1: IRQ1 in RX & Standby mode CRC OK */
/* 3 Tx_irq_1:IRQ1 in TX mode Fifo full 2 Fifofull read only 1 /Fifoempty read only 0 Fifo_overrun_clr Fifo overrun clear */
DEF_IRQPARAM1 | RF_IRQ0_RX_STDBY_FIFOEMPTY | RF_IRQ1_RX_STDBY_CRCOK | RF_IRQ1_TX_TXDONE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 14:7 Fifo_filling_method: Don't care, Buffered mode only 6 Fifo_fill: Don't care, Buffered mode only */
/* 5 Tx_done Tx_done: IRQ source read only 4 Tx_start_irq_0: In Packet mode Tx starts if FIFO is not empty. IRQ_0 mapped to /Fifoempty */
/* 3 Res 2 RSSI_irq: Goes high when a signal above RSSI_irq threshold is detected. Writing '1' clears the bit */
/* 1 PLL_locked: PLL status 1 locked. Writing '1' clear the bit 0 PLL_lock_en: PLL_locked mapped to pin 23 */
DEF_IRQPARAM2 | RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY | RF_IRQ2_PLL_LOCK_PIN_ON,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 15:	7-0 RSSI_irq_threshold: RSSI threshold for interrupt */
DEF_RSSIIRQTHRESH | RF_RSSIIRQTHRESH_VALUE,
/*============================================== Receiver parameters ============================================================================*/
/* 16:7-4 PassiveFilt: §3.4.4.1 3xFc ButterFilt < PassiveFilt < 4 x Fc ButterFilt */
/* 3-0 ButterFilt: set the receiver bandwidth BW99%=2x[Fdev+(BR/2)] et 2fc > BW99%+LO drifts See §3.4.5. */
DEF_RXPARAM1 | RF_RX1_PASSIVEFILT_676 | RF_RX1_FC_FOPLUS100,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 17:7-4 PolypFilt_center: 0011 default value 100kHz 3-0 Res */
DEF_RXPARAM2 | RF_RX2_FO_100,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 18:7 PolypFilt_on: 0 polyphase filter disabled 6 Bitsync_off: 0 Bit synchronizer on in Continous Rx mode */
/* 5 Sync_on: 1 Sync word recognition on 4-3 Sync_size: 11 32bits 1-0 Sync_tol: 00 0 errors tolerance */
DEF_RXPARAM3 | RF_RX3_POLYPFILT_OFF | RF_RX3_SYNC_SIZE_32 | RF_RX3_SYNC_TOL_0,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 19:Reserved */
DEF_RES19,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*RSSI Value (Read only) */
0,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/* 21:7-5 OOK_thresh_step: 000-> 0.5 db 4-2 OOK_thresh_dec_period: 000->once in each chip period 1-0 OOK_avg_thresh_cutoff: 00 fc=BR/ 8 PI */
DEF_RXPARAM6 | RF_RX6_OOK_THRESH_DECSTEP_000 | RF_RX6_OOK_THRESH_DECPERIOD_000 | RF_RX6_OOK_THRESH_AVERAGING_00,
/*=========================================== Pattern ===========================================================================================*/
/* 22,23,24,25:	7-0 Sync_value_ */
DEF_SYNCBYTE1 | 0x12, /* 1st byte of Sync word */
DEF_SYNCBYTE2 | 0x34, /* 2nd byte of Sync word */
DEF_SYNCBYTE3 | 0x56, /* 3rd byte of Sync word */
DEF_SYNCBYTE4 | 0x78, /* 4th byte of Sync word*/
/*=========================================== Transmitter =======================================================================================*/
/* 26:7-4 InterpFilt:TX Interpolation filter cutoff frequency §3.3.5 fc~3x[Fdev + BR/2)   3-1 Pout: Tx Ouput power */
DEF_TXPARAM | RF_TX_FC_150 | RF_TX_POWER_MAX,
/*=========================================== Crystal oscillator ================================================================================*/
/*27: 7 Clkout_on: 1--> Clkout pin 6-2 Clkout_freq: Fclkout=Fxtal/30 1-0 Res */
DEF_OSCPARAM | RF_OSC_CLKOUT_OFF | RF_OSC_CLKOUT_427,
/*============================================= Packet handler ==================================================================================*/
/*28:7 Manchester_on: 0 off 6-0 Payload_length = 64 bytes */
DEF_PKTPARAM1 | RF_PKT1_MANCHESTER_OFF | 64,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*29: 7-0 Node_adrs: Node local address 255 */
DEF_NODEADRS  | RF_NODEADRS_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*30:7 Pkt_format: 1-> variable length 6-5 Preamble size: 11 -> 4 bytes 4 Whitening_on: 0->off 3 CRC_on: 1-> on 2-1 Adrs_filt: 10->NID + 0x00 */
/* 0 CRC_status: read only 1:OK */
DEF_PKTPARAM3 | RF_PKT3_FORMAT_VARIABLE | RF_PKT3_PREAMBLE_SIZE_32 | RF_PKT3_WHITENING_OFF | RF_PKT3_CRC_ON | RF_PKT3_ADRSFILT_10,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*31:7 CRC_autoclr: 0->fifo auto cleared if CRC failed for current packet 6 Fifo_stby_access: 0->FIFO acces in stanby mode is Write 5-0 Res */
DEF_PKTPARAM4 | RF_PKT4_AUTOCLEAR_ON | RF_PKT4_FIFO_STANDBY_WRITE
};


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

    uint8_t i;

       /* Initializes SX1211 en sautant le registre d'adresse 20 */
    for(i = 0; i <= REG_PKTPARAM4; i++)
    {
      if(i==REG_RSSIVALUE) continue;
      SX1211_WriteRegister(i, RegistersCfg[i]);
    }
  

    while (1)
    {
      uint8_t receivedData = SX1211_ReadData();

      char buffer[50];
      sprintf(buffer, "response : 0x%02X\r\n", receivedData);
      SendUART(buffer);

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
