/*---------------------------------------------------------------------------------------------------
                                   A.THEOBALD SA CONTROLE ET REGULATION
-----------------------------------------------------------------------------------------------------

                                               #
                                              # #
                                             #   #
                                            #     #
                                           #       #
                                          ###########
                                         #     #     #
                                        #      #      #
                                               #
                                               #
                                               #

 Projet:  Face avant a LCD 946.
 Fichier:  sx1211.c
 Description: Module de gestion du chip radio SEMTECH SX1211 en mode packet.
 Auteur:      Jean Luc DUPUIS
 Version:     00
 CPU: STM32F103TB @ 36MHz interne
 Compilateur IAR EWARMv5 Librairies ST V3.0.0
 -D _DEBUG_SX1211_ pour activer le mode trace.
 Modifications:
 01/06/09: Creation.

Parametres de fonctionnement du SX1211:
---------------------------------------- TX ---------------------------------------------------------
- FRf = 868.950 MHz (Milieu de bande G2) Freq_band = 10, FRf = 9/8 . 12.800 / (R + 1) . [ 75.(P+1) + S]
- Pout = max (Adresse 26: RF_TX_POWER_MAX)
- Fdev = �50 kHz (Adresse 2: RF_FDEV_50)
- BR = 25 kbps (Adresse 3: RF_BITRATE_25000) indice modulation = 2Fdev/BR = 4.
- TX filter BW  = 150kHz (Adresse 26: RF_TX_FC_150, BW~3x[FDev + BR/2] = 187.5kHz
- CLKout = OFF (Adresse 27: RF_OSC_CLKOUT_OFF).

---------------------------------------- RX ---------------------------------------------------------
BW99% = 2[FDev+(BR/2)] = 125kHz et 2Fc > BW99% + LO drifts




                              |<--------Low Pass --------------->|
                              |    |<----- Butterworth ---->|    |
                              |    |    |< BW 99%     >|    |    |
                              +----+----+--------------+----+----+
                             /    /     |       |      |    |\    \
                            /    /      |       |      |    | \    \
                           /    /       |       |      |    |  \    \
--------------------------+----+--------+-------0------+----+---+-+--+-------------------------------

- Butterworth filter 2Fc = 200 kHz (Adresse 18: RF_RX3_POLYPFILT_OFF et adresse 16 RF_RX1_FC_FOPLUS100).
- LP filter, 1.5 FcButter < Fc < 2 FcButter, Fc = 676kHz,  Adresse 16: RF_RX1_PASSIVEFILT_676
Note: Cette valeur est a affiner sur un recepteur en mesurant le BER.

Sequence d'initialisation en mode DEBUG (_DEBUG_SX1211_ est defini)
-------------------------------------------------------------------
Mode paquet               Mode buffer
C0:30                     idem
C1:8c                     a8
C2:07                     idem
C3:07                     idem
C4:0c                     idem
C5:cf                     idem
C6:7f                     idem
C7:65                     idem
C8:4a                     idem
C9:5f                     idem
C10:4c                    idem
C11:13                    idem
C12:38                    idem
C13:88                    b0
C14:19                    idem
C15:00                    idem
C16:e3                    idem
C17:38                    idem
C18:38                    idem
C19:07                    idem
C21:00                    idem
C22:12  NodeId 0x12345678 ff  0xFF88B859  (debut de la sequence pseudo aleatoire)
C23:34                    e1
C24:56                    1D
C25:78                    9a
C26:50                    idem
C27:3c                    idem
C28:40                    Non initialise en mode buffered
C29:FF                    Non initialise en mode buffered
C30:ec                    Non initialise en mode buffered
C31:00                    Non initialise en mode buffered

-----------------------------------------------------------------------------------------------------
                                     Propriete de A. THEOBALD SA
                          Ne peut etre copie ou diffuse sans son accord prealable
---------------------------------------------------------------------------------------------------*/
/* Includes ---------------------------------------------------------------------------------------*/

#include <sx1211.h>
#include "spi.h"
#include "main.h"
#include "ERS2_Messages.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>   // Pour printf
#include <string.h>  // Pour strlen si besoin
#include "usart.h"


void SetMyNid(uint8_t Nid);

uint8_t ucCarierDetect;
uint8_t ucAccessFailure;
uint8_t ucMRK_RXOVR; /* marqueur de purge de la fifo */        
#define TX_MAX_CHECK /* Pour la definition de la duree de TX_MAX_TIME */
/*----------------------------------------------------------------------------*/
#ifdef  TX_MAX_CHECK
/*----------------------------------------------------------------------------*/
uint32_t ulmeas1,ulmeas2; /* TX_MAX_TIME checks sur IRQ1 pdt le dbg */
/*----------------------------------------------------------------------------*/
#endif/*TX_MAX_CHECK*/
/*----------------------------------------------------------------------------*/
/* Private typedef --------------------------------------------------------------------------------*/
/* Private define ---------------------------------------------------------------------------------*/
/* CCITT O.153  */
#define CCITT_SEED   (uint16_t)0x1FF    /* Germe du GPA */
/* Private macro ----------------------------------------------------------------------------------*/
/* Private variables ------------------------------------------------------------------------------*/
static uint8_t PreMode = RF_STANDBY;                          /* Etat precedent du chip = RegistersCfg[0][7-5]  */
static uint8_t ucRfValue;                                     /* Derniere valeur de la RSSI */
uint8_t ucRfVal_RSSI_IRQ;                                     /* Derniere valeur de la RSSI */
/* CCITT O.153 */
static uint16_t uiLfsr = CCITT_SEED;

/* Tables -----------------------------------------------------------------------------------------*/
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
/* 16:7-4 PassiveFilt: �3.4.4.1 3xFc ButterFilt < PassiveFilt < 4 x Fc ButterFilt */
/* 3-0 ButterFilt: set the receiver bandwidth BW99%=2x[Fdev+(BR/2)] et 2fc > BW99%+LO drifts See �3.4.5. */
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
/* 26:7-4 InterpFilt:TX Interpolation filter cutoff frequency �3.3.5 fc~3x[Fdev + BR/2)   3-1 Pout: Tx Ouput power */
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

/*--------------------------- Mode CCITT --------------------------------------------------------------------------------------------------------*/
/* Parametres specifiques au test d'homologation */
const uint8_t CcittRegistersCfg[] = {
/* O:7-5 Chip_mode: Standby mode 4-3 Freq_band: 863-870 band 2-1 VCO_trim: Vtune no correction 0 RPS_select: R1P1S1 group */
DEF_MCPARAM1 | RF_MC1_STANDBY | RF_MC1_BAND_868 | RF_MC1_VCO_TRIM_00 | RF_MC1_RPS_SELECT_1,
/*======================================== Main parameters common to receive & transmit modes ===================================================*/
// 1:7-6 Modul_select:FSK 5 Data_mode_0:Buffered mode 4-3 OOK_thresh_type: OOK peak mode 2 Data_mode_1:Buffered mode 1-0 IF_gain: IF Chain gain 0dB
DEF_MCPARAM2 | RF_MC2_MODULATION_FSK | RF_MC2_DATA_MODE_BUFFERED | RF_MC2_OOK_THRESH_TYPE_PEAK | RF_MC2_GAIN_IF_00,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 2:7-0 Freq_dev: Fdev=Fxtal / [32x(Freq_dev+1)] +/- 50kHz
DEF_FDEV | RF_FDEV_50,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 3:7 Res 6-0 BR: BitRate=Fxtal/(64(BR+1)) Bitrate = 25kbauds
DEF_BITRATE | RF_BITRATE_25000,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 4: 7-0 OOK_floor_thres: OOK floor threshold detect 12*0,5=6dB
DEF_OOKFLOORTHRESH | RF_OOKFLOORTHRESH_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 5:7-6 Fifo_size:FIFO size 64 bytes 5-0 Fifo_thres: FIFO interrupt threshold
DEF_MCPARAM6 | RF_MC6_FIFO_SIZE_64 | RF_MC6_FIFO_THRESH_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 6,7,8 : R1,P1,S1
DEF_R1 | RF_R1_VALUE,
DEF_P1 | RF_P1_VALUE,
DEF_S1 | RF_S1_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 9,10,11 : R2,P2,S2
DEF_R2 | RF_R2_VALUE,
DEF_P2 | RF_P2_VALUE,
DEF_S2 | RF_S2_VALUE,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 12: 7-5 Res 4-3 PA_ramp PA ramp 23us 2-0 Res
DEF_PARAMP | RF_PARAMP_11,
/*======================================== Interrupt registers ==================================================================================*/
/*                IRQ_1                   IRQ_0
RX & Standby      FIFO_THRESHOLD          /FIFO_EMPTY
TX                FIFO_FULL               /FIFO_EMPTY */
// 13:Buffered mode: 7-6 Rx_stby_irq_0:IRQ0 source in RX & Standby  5-4 Rx_stby_irq_1: IRQ1 in RX & Standby mode
// 3 Tx_irq_1:IRQ1 in TX mode 2 Fifofull read only 1 /Fifoempty read only 0 Fifo_overrun_clr Fifo overrun clear
DEF_IRQPARAM1 | RF_IRQ0_RX_STDBY_FIFOEMPTY | RF_IRQ1_RX_STDBY_FIFOTHRESH | RF_IRQ1_TX_FIFOFULL,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 14:7 Fifo_filling_method: Automatic on SYNC word  6 Fifo_fill flag
// 5 Tx_done Tx_done: IRQ source read only 4 Tx_start_irq_0: In Packet mode Tx starts if FIFO is not empty. IRQ_0 mapped to /Fifoempty
// 3 Res 2 RSSI_irq: Goes high when a signal above RSSI_irq threshold is detected. Writing '1' clears the bit
// 1 PLL_locked: PLL status 1 locked. Writing '1' clear the bit 0 PLL_lock_en: PLL_locked mapped to pin 23
DEF_IRQPARAM2 | RF_IRQ2_FIFOFILL_METHOD_AUTO | RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY | RF_IRQ2_PLL_LOCK_PIN_ON,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 15:	7-0 RSSI_irq_threshold: RSSI threshold for interrupt
DEF_RSSIIRQTHRESH | RF_RSSIIRQTHRESH_VALUE,
/*============================================== Receiver parameters ============================================================================*/
// 16:7-4 PassiveFilt: �3.4.4.1 3xFc ButterFilt < PassiveFilt < 4 x Fc ButterFilt
// 3-0 ButterFilt: set the receiver bandwidth BW99%=2x[Fdev+(BR/2)] et 2fc > BW99%+LO drifts See �3.4.5.
DEF_RXPARAM1 | RF_RX1_PASSIVEFILT_676 | RF_RX1_FC_FOPLUS100,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 17:7-4 PolypFilt_center: 0011 default value 100kHz 3-0 Res
DEF_RXPARAM2 | RF_RX2_FO_100,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 18:7 PolypFilt_on: 0 polyphase filter disabled 6 Bitsync_off: 0 Bit synchronizer on in Continous Rx mode
// 5 Sync_on: 1 Sync word recognition on 4-3 Sync_size: 10 24bits 1-0 Sync_tol: 00 0 errors tolerance
DEF_RXPARAM3 | RF_RX3_POLYPFILT_OFF | RF_RX3_SYNC_SIZE_32 | RF_RX3_SYNC_TOL_0,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 19:Reserved
DEF_RES19,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
//RSSI Value (Read only)
0,
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// 21:7-5 OOK_thresh_step: 000-> 0.5 db 4-2 OOK_thresh_dec_period: 000->once in each chip period 1-0 OOK_avg_thresh_cutoff: 00 fc=BR/ 8 PI
DEF_RXPARAM6 | RF_RX6_OOK_THRESH_DECSTEP_000 | RF_RX6_OOK_THRESH_DECPERIOD_000 | RF_RX6_OOK_THRESH_AVERAGING_00,
/*=========================================== Pattern ===========================================================================================*/
// 22,23,24,25:	7-0 Sync_values. 4 premiers octets de la sequence CCITT O.153 ff 88 b8 59
DEF_SYNCBYTE1 | 0xFF, // 1st byte of Sync word,
DEF_SYNCBYTE2 | 0x88, // 2nd byte of Sync word,
DEF_SYNCBYTE3 | 0xB8, // 3rd byte of Sync word,
DEF_SYNCBYTE4 | 0x59, // 4th byte of Sync word,
/*=========================================== Transmitter =======================================================================================*/
// 26:7-4 InterpFilt:TX Interpolation filter cutoff frequency �3.3.5 fc~3x[Fdev + BR/2)   3-1 Pout: Tx Ouput power
DEF_TXPARAM | RF_TX_FC_150 | RF_TX_POWER_MAX,
/*=========================================== Crystal oscillator ================================================================================*/
// 27: 7 Clkout_on: 1--> Clkout pin 6-2 Clkout_freq: Fclkout=Fxtal/30 1-0 Res
DEF_OSCPARAM | RF_OSC_CLKOUT_OFF | RF_OSC_CLKOUT_427,
/* Les registres 28 a 31 ne concernent pas le mode buffered */
/*============================================= Packet handler ==================================================================================*/
/* Non initialise en mode buffered */
};

/* Private function prototypes --------------------------------------------------------------------*/
void SetRFMode(uint8_t mode);
void Wait(uint16_t t);                                 /* attend t �s  */
static void WriteRegister(uint8_t address, uint8_t value);    /* Lecture ecriture NSS_CONF  */
static uint8_t ReadRegister(uint8_t address);
static uint8_t RF_CcittO153(void);                     /* GPA */
static void WriteFifo(uint8_t data);                          /* Lecture ecriture NSS_DATA  */
bool RF_CCA_ConfirmIdle(void);                         /* Test canal */

/* Private functions ------------------------------------------------------------------------------*/

/**********************************************************************************************************
* NOM:bool RF_Configuration(void)
*
* DESCRIPTION :This function sends the values contained in RegistersCfg variable to the transceiver.
* It also initializes the StartByte and baud rates variables.
* Packet format, variable length.
*
* +------------------+------------------+------------+-----------------------+------------+
* | Preamble 32 bits |Sync word 32 bits |Length byte | Message (63 bytes max)| CRC 2 bytes|
* +------------------+------------------+------------+-----------------------+------------+
*                                       |<------------------- FIFO --------->|
*
* Length 592 bits max, tmax ~ 24ms  @ 25kbds.
*
*
*
*
* PARAMETRES       : Sans.
* VALEUR RETOURNEE : false si on ne peut pas relire les registres de commande, true sinon.
* MAINTENANCE      :
* 16/04/2008 JLD: Creation
*
*********************************************************************************************************/
bool RF_Configuration(void)
{
   uint8_t i;


   /* Initializes SX1211 en sautant le registre d'adresse 20 */
  for(i = 0; i <= REG_PKTPARAM4; i++)
  {
    if(i==REG_RSSIVALUE) continue;
    WriteRegister(i, RegistersCfg[i]);
  }


  /* Test de la presence du chip sans utiliser le semaphore de la SPI2 */
  if(ReadRegister(REG_SYNCBYTE1) != 0x12 || ReadRegister(REG_SYNCBYTE2) != 0x34 ||
     ReadRegister(REG_SYNCBYTE3) != 0x56 || ReadRegister(REG_SYNCBYTE4) != 0x78) {return false;}
  
  /* Passe en SLEEP. On reviendra automatiquement a ce mode dans le layer 2 */
  /* JLD 04/11/13: Si l'on appelle plus d'une fois RF_Configuration(), le SX1211 reste alors en STANDBY
      car la globale PreMode n'etait pas a jour . */
  PreMode = RF_STANDBY;
/*----------------------------------------------------------------------------*/
#ifndef _MODE_TEST_RF_
/*----------------------------------------------------------------------------*/
  SetRFMode(RF_SLEEP);
/*----------------------------------------------------------------------------*/
#else /*_MODE_TEST_RF_*/  
/*----------------------------------------------------------------------------*/
  SetRFMode(RF_RECEIVER);
/*----------------------------------------------------------------------------*/
#endif/*_MODE_TEST_RF_*/  
/*----------------------------------------------------------------------------*/
  return true;
} /* bool RF_Configuration(void)  */

/**********************************************************************************************************
* NOM:bool RF_ReceiveFrame(uint8_t *buffer)
*
* DESCRIPTION      : Reception  d'une trame RF.
* PARAMETRES       : Pointeur sur le buffer
* VALEUR RETOURNEE : true si trame recue, false sinon
* MAINTENANCE      :
* 16/04/2008 JLD: Creation
* 02/11/2009 JLD: Adaptation R8C-->STM32
*********************************************************************************************************/
bool RF_ReceiveFrame(uint8_t *buffer)
{
  uint8_t SavedMode = PreMode;  /* Sauve le mode courant  */
  uint8_t r;

  /* Acquisition du niveau de reception */
  if(IRQ_0)
  { /* IRQ_0 = /FIFO_EMPTY, Fifo non vide */
    r = ReadRegister(REG_RSSIVALUE);
    if(r>ucRfValue){
      ucRfValue = r;    /* ucRfValue = Valeur crete de la RSSI */
    }
    /* Overrun ? */    
    if(ReadRegister(REG_IRQPARAM1) & RF_IRQ1_FIFO_OVERRUN_CLEAR){
      /* Oui, purge la fifo en ecrivant le flag a 1 */
      WriteRegister(REG_IRQPARAM1, RegistersCfg[REG_IRQPARAM1] | RF_IRQ1_FIFO_OVERRUN_CLEAR);          
      ucMRK_RXOVR++; /* marqueur de purge de la fifo */        
    }
  }
  else
  {
    ucRfValue = 0;
  }
  /* Paquet valide ? */
  if(IRQ_1)
  { /* CRC OK, passe en standby et positionne le flag pour une lecture de fifo */
    uint8_t dummy = 0xFF; // Valeur pour générer l'horloge SPI
    uint8_t receivedByte;
    
    uint8_t len = 0;    /*!< JLD 03/10/16: limite la taille a SX1211_FIFO_SIZE bytes */
    SetRFMode(RF_STANDBY);
    WriteRegister(REG_PKTPARAM4, (RegistersCfg[REG_PKTPARAM4] & 0xBF) | RF_PKT4_FIFO_STANDBY_READ);
    /* Lecture des donnees tant que la fifo n'est pas vide  */
    while(len < SX1211_FIFO_SIZE && IRQ_0)
    { /* Lecture tant que fifo non vide */
      NSS_DATA_LOW();
      HAL_SPI_TransmitReceive(&hspi1, &dummy, &receivedByte, 1, HAL_MAX_DELAY);
      NSS_DATA_HIGH();
      buffer[len++] = r;
    }
    /* Restaure le mode */
    SetRFMode(SavedMode);
    return true;
  } 
  return false;
} /* bool RF_ReceiveFrame(uint8_t *buffer)  */

/**********************************************************************************************************
* NOM:void RF_TransmitFrame(uint8_t * pBuffer)
*
* DESCRIPTION :Transmet le buffer en mode paquet.
* PARAMETRES :ptr sur la taille en octet.
* VALEUR RETOURNEE :sans
* MAINTENANCE :
* 30/11/2009 JLD: Creation
* 15/03/2012  NA: Ajout en mode SPY on reste toujours muet.
* 18/01/2018  NA: Definition TX_MAX_TIME avec ulmeas2 � 0x00011BA7 cycles sur STM32L433 80MHz pdt le test RF.
* 24/04/2018  NA: Definition TX_MAX_TIME avec ulmeas2 � 0x0000388C cycles sur STM32L433 16MHz pdt le test RF.
*********************************************************************************************************/
#define TX_MAX_TIME ((uint16_t)0x8000) /* Cycles Time out sur IRQ1 */
void RF_TransmitFrame(uint8_t * pBuffer)
{
  uint8_t SavedMode = PreMode;          /* Sauve le mode courant  */
  uint8_t sizeplus1 = pBuffer[0]+1;     /* Nombre d'octets a transmettre = taille + 1 (l'octet taille lui-meme) */
  uint16_t tempo = TX_MAX_TIME;                  /* Time out sur IRQ1 */

  if(sizeplus1 > (((RegistersCfg[REG_MCPARAM6])>>6)+1)*16)
  {  /* If size + 1 > FIFO size */
    return;
  }
  /* Passe en standby */
  SetRFMode(RF_STANDBY);
  /* Passe la fifo  en mode 'write' durant le standby. */
  WriteRegister(REG_PKTPARAM4, (RegistersCfg[REG_PKTPARAM4] & 0xBF) | RF_PKT4_FIFO_STANDBY_WRITE);
  /* Remplit la fifo */
  while(sizeplus1--)
  {
    WriteFifo(*pBuffer);
    pBuffer++;
  }

  /* Passe en emission et transmet immediatement car RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY est positionne */
  SetRFMode(RF_TRANSMITTER);
  /* Attend la fin de l'emission en testant la remontee de IRQ1 car RF_IRQ1_TX_TXDONE est positionne */
  do
  {
    if(tempo-- == 0)
    {
      /* Restaure le mode et sort */
      SetRFMode(SavedMode);
//      uiError_Code = 8;/* pdt le test RF avec 0x00003500 qui est trop court */
      return;
    }
  }while(!IRQ_1);
/*----------------------------------------------------------------------------*/
#ifdef  TX_MAX_CHECK
/*----------------------------------------------------------------------------*/
  ulmeas1 = (uint32_t)(TX_MAX_TIME - tempo);/* chronometrage live */
  if(ulmeas1 > ulmeas2)ulmeas2 = ulmeas1;   /* capture de la valeur MAX */
  //uiVar_Check = (uint16_t)ulmeas2; /* affiche la variable dans le menu info */
/*----------------------------------------------------------------------------*/
#endif/*TX_MAX_CHECK*/
/*----------------------------------------------------------------------------*/
  Wait(1000);       /* Wait for last bit to be sent (worst case bitrate) 1/1600 bds = 625us */
  /* Restaure le mode */
  SetRFMode(SavedMode);
  return;

} /* void RF_TransmitFrame(uint8_t * pBuffer) */

/**********************************************************************************************************
* NOM:void SetRFMode(uint8_t mode)
*
* DESCRIPTION      : Gere les mode de fonctionnement du SX1211: emission, reception,standby, etc.
* PARAMETRES       : Mode.
* VALEUR RETOURNEE : Sans.
* MAINTENANCE      :
* 18/04/2008 JLD: Creation, reecriture avec des switches, c'est plus beau...
* 01/09/2009 JLD: Adaptation R8C-->STM32
* 10/09/2018  NA: Ajout SWO_RF_VAR pour suivre l'etat courant du chip RF.
*********************************************************************************************************/
void SetRFMode(uint8_t mode)
{
/*----------------------------------------------------------------------------*/
#ifdef SWO_RF_VAR
/*----------------------------------------------------------------------------*/
  uiVar_Test = mode;
/*----------------------------------------------------------------------------*/
#endif/*SWO_RF_VAR*/
/*----------------------------------------------------------------------------*/
  /*

  SLEEP     STANDBY     SYNTHETIZER   TX          RX
  |           |           |
  |-- TS_OS ->|-- TS_FS ->|-- TS_TR ->|-- TS_RE ->|
  |                       |           |<- TS_TR --|
  |                       |           |           |
  |                       |----------- TS_RE ---->|
  |                       |                       |
  Toutes les autres transisitions  ne necessitent pas d'attente.  */

  /* Suivant la demande */
  switch(mode)
  {
  case RF_TRANSMITTER:
    /* Passe en emission */
    switch(PreMode)
    {
    case  RF_SLEEP:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);        		
      Wait(TS_OS);        		
    case RF_STANDBY:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHETIZER);        		
      Wait(TS_FS);         		
    case RF_SYNTHETIZER:
    case RF_RECEIVER:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_TRANSMITTER);
      Wait(TS_TR);
    default:
      break;
    }
    PreMode = RF_TRANSMITTER;
    break;
  case RF_RECEIVER:
    /* Passe en reception */
    switch(PreMode)
    {
    case RF_SLEEP:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);        		
      Wait(TS_OS);        		
    case RF_STANDBY:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHETIZER);        		
      Wait(TS_FS);
    case RF_SYNTHETIZER:
    case RF_TRANSMITTER:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_RECEIVER);
      Wait(TS_RE);
    default:
      break;
    }
    PreMode = RF_RECEIVER;
    break;
  case RF_STANDBY:
    /* Passe en standby */
    switch(PreMode)
    {
    case RF_SLEEP:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);
      Wait(TS_OS);
      break;
    case RF_SYNTHETIZER:
    case RF_TRANSMITTER:
    case RF_RECEIVER:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);       		
    default:
      break;
    }
    PreMode = RF_STANDBY;
    break;
  case RF_SYNTHETIZER:
    /* Passe en synthetiseur */
    switch(PreMode)
    {
    case RF_SLEEP:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_STANDBY);        		
      Wait(TS_OS);        		
    case RF_STANDBY:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHETIZER);        		
      Wait(TS_FS);
      break;
    case RF_RECEIVER:
    case RF_TRANSMITTER:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SYNTHETIZER);        		
    default:
      break;
    }
    PreMode = RF_SYNTHETIZER;
    break;
  case RF_SLEEP:
    /* Passe en slip */
    switch(PreMode)
    {
    case RF_STANDBY:
    case RF_SYNTHETIZER:
    case RF_RECEIVER:
    case RF_TRANSMITTER:
      WriteRegister(REG_MCPARAM1, (RegistersCfg[REG_MCPARAM1] & 0x1F) | RF_SLEEP);
    default:
      break;
    }
    PreMode = RF_SLEEP;
  default:
    break;
  }
}

/**********************************************************************************************************
* NOM:void SetMyNid(uint8 Nid)
*
* DESCRIPTION      : Postionne le registre 29[7-0] Node_adrs a Nid.
* PARAMETRES       : Nid
* VALEUR RETOURNEE : sans
* MAINTENANCE      :
* 12/11/2008 JLD: Creation
*********************************************************************************************************/
void SetMyNid(uint8_t Nid)
{
  WriteRegister(REG_NODEADRS,Nid);
} /* void SetMyNid(uint8 Nid) */

/**********************************************************************************************************
* NOM:void RF_SetCurrentNetid(uint32_t Netid)
*
* DESCRIPTION      :Ecrit la valeur courante du Netid dans le SX1211.
* PARAMETRES       :netid sur 32 bits
* VALEUR RETOURNEE :sans
* MAINTENANCE      :
* 19/11/2008 JLD: Creation
* 01/09/2009 JLD: Adaptation R8C-->STM32
*********************************************************************************************************/
void RF_SetCurrentNetid(uint32_t Netid)
{
  uint32_t r;
  r = Netid >> 24;
  WriteRegister(REG_SYNCBYTE1,(uint8_t) r); /* MSB */
  r = (Netid >> 16) & 0xFF;
  WriteRegister(REG_SYNCBYTE2,(uint8_t) r);
  r = (Netid >> 8) & 0xFF;
  WriteRegister(REG_SYNCBYTE3,(uint8_t) r);
  r = Netid & 0xFF;
  WriteRegister(REG_SYNCBYTE4,(uint8_t) r); /* LSB */

} /* void RF_SetCurrentNetid(uint32_t Netid) */

/**********************************************************************************************************
* NOM:uint32_t RF_GetCurrentNetid(void)
*
* DESCRIPTION      :Retourne la valeur courante du Netid (sync_value).
* PARAMETRES       :Sans
* VALEUR RETOURNEE :netid sur 32 bits
* MAINTENANCE      :
* 19/11/2008 JLD: Creation
*********************************************************************************************************/
uint32_t RF_GetCurrentNetid(void)
{
  uint32_t r;

  r  = (uint32_t) ReadRegister(REG_SYNCBYTE1) << 24;
  r += (uint32_t) ReadRegister(REG_SYNCBYTE2) << 16;
  r += (uint32_t) ReadRegister(REG_SYNCBYTE3) << 8;
  r += (uint32_t) ReadRegister(REG_SYNCBYTE4);
  return r;
} /* uint32_t RF_GetCurrentNetid(void) */
/***************************************************************************************************/
/**
* @brief Regle le mode de filtrage des paquets.
*
* @param mode RF_PKT3_ADRSFILT_00 ou RF_PKT3_ADRSFILT_01 ou RF_PKT3_ADRSFILT_10 ou RF_PKT3_ADRSFILT_11 
* @retval sans
* @date 23/09/2010
* @author Jean Luc DUPUIS
*/
/***************************************************************************************************/
void RF_SetAddressFiltering(uint8_t mode)
{
  WriteRegister(REG_PKTPARAM3, (RegistersCfg[REG_PKTPARAM3] & 0xF9) | mode);   

} /* void RF_SetAddressFiltering(uint8_t mode) */

/**********************************************************************************************************
* NOM:uint16_t RF_GetRSSI(void)
*
* DESCRIPTION :Retourne la valeur crete de la RSSI.
* PARAMETRES :sans
* VALEUR RETOURNEE :
* MAINTENANCE :
* 09/12/2009 JLD: Creation
*********************************************************************************************************/
uint16_t RF_GetRSSI(void)
{
  return (uint16_t) ucRfValue;

} /* uint16_t RF_GetRSSI(void) */

/**********************************************************************************************************
* NOM:uint8_t RF_GetNodeId(void)
*
* DESCRIPTION :Retourne le nodeid courant
* PARAMETRES :sans
* VALEUR RETOURNEE :nodeid
* MAINTENANCE :
* 16/12/2009 JLD: Creation
*********************************************************************************************************/
uint8_t RF_GetNodeId(void)
{
  return ReadRegister(REG_NODEADRS);
} /* uint8_t RF_GetNodeId(void) */

/**********************************************************************************************************
* NOM:int8_t RF_Rssi_to_dbm(uint16_t rssi)
*
* DESCRIPTION :Transforme une valeur Rssi en dBm.
* PARAMETRES : RSSI
* VALEUR RETOURNEE : [-100,-40] dBm
* MAINTENANCE :
* 11/01/2010 JLD: Creation
*********************************************************************************************************/
int8_t RF_Rssi_to_dbm(uint8_t rssi)
{
  /* Zone non lineaire  */
  if(rssi < 38) return (-100);             // -100 dBm
  if(rssi > 140) return (-40);             // -40  dBm
  return ((rssi >> 1) - 110);   // Niveau dBm = (RSSI_Val x 0.5) - 110

} /* int8_t RF_Rssi_to_dbm(uint16_t rssi) */

/***************************************************************************************************/
/**
* @brief Regle la puissance d'emission.
*
* @param sans
* @retval sans
* @date 26/05/2010
* @author Jean Luc DUPUIS
* @note SPI2 protegee par semaphore.
*/
/***************************************************************************************************/
void RF_SetRfPower(uint8_t pow)
{ 
  uint8_t reg;
  reg = ReadRegister(REG_TXPARAM) & 0xF1; /* Masque bits 3-1 */
  pow &= 0x0E;
  WriteRegister(REG_TXPARAM,reg | pow);
  
} /* void RF_SetRfPower(uint8_t pow) */


/**********************************************************************************************************
* NOM:void WriteRegister(uint8_t address, uint8_t value)
*
* DESCRIPTION      : Ecrit value a l'adresse address avec NSS_CONFIG = 0.
* PARAMETRES       : Adresse, Valeur.
* VALEUR RETOURNEE : Sans.
* MAINTENANCE      :
* 01/09/2009 JLD: Adaptation R8C-->STM32
*********************************************************************************************************/
void WriteRegister(uint8_t address, uint8_t value)
{
    NSS_CONF_LOW();
    address = (address << 1) & 0x3E ;
    HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, &value, 1, HAL_MAX_DELAY);
    NSS_CONF_HIGH();
}

/**********************************************************************************************************
* NOM:uint8_t ReadRegister(uint8_t address)
*
* DESCRIPTION      : Lit la valeur a l'adresse passee en parametre avec NSS_CONFIG = 0.
* PARAMETRES       : Adresse.
* VALEUR RETOURNEE : Valeur lue.
* MAINTENANCE      :
* 22/04/2008 JLD: Creation
* 01/09/2009 JLD: Adaptation R8C-->STM32
*********************************************************************************************************/
uint8_t ReadRegister(uint8_t address)
{
    address = ((address << 1) & 0x3E | 0x40) ;
    uint8_t received[2] = {0x00, 0x00};  // Initialisation pour éviter des valeurs aléatoires

    NSS_CONF_LOW();
    HAL_SPI_TransmitReceive(&hspi1, &address, received, 2, HAL_MAX_DELAY);
    NSS_CONF_HIGH();

    return received[1]; // Le 2ème octet contient la vraie réponse
}


/**********************************************************************************************************
* NOM:void WriteFifo(uint8_t data)
*
* DESCRIPTION :Ecrit un octet dans la fifo.
* PARAMETRES :octet a ecrire
* VALEUR RETOURNEE :sans
* MAINTENANCE :
* 24/11/2009 JLD: Creation
*********************************************************************************************************/
void WriteFifo(uint8_t dataByte)
{
  
  NSS_DATA_LOW();
  HAL_SPI_Transmit(&hspi1, &dataByte, 2, HAL_MAX_DELAY);
  NSS_DATA_HIGH();

} /* void WriteFifo(uint8_t data) */


/**********************************************************************************************************
* NOM:void Wait(uint16 t)
*
* DESCRIPTION      : Attend t us. Temps legerement superieur, teste au scope.
* PARAMETRES       : Sans.
* VALEUR RETOURNEE : Sans.
* MAINTENANCE      :
* 21/04/2008 JLD: Creation
* 28/11/2017 NA: Adaptation STM32L433 scoped 325 pour 320�s ED_TIME, 5071 pour 5000�s TS_OS.
*********************************************************************************************************/
void Wait(uint16_t t)
{
    HAL_Delay(t);  // Attente de 't' millisecondes
}

/*--------------------------------------------------------------------------------------------------------*/
/*------------------------------ CSMA-CA -----------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/

/***************************************************************************************************/
/**
* @brief Teste si le canal est libre durant ED_TIME.
*
* @param sans
* @retval true si canal libre, false sinon
* @date 13/02/2012
* @author Jean Luc DUPUIS
* 23/03/2012  NA; Ajout sortie ucRfVal_RSSI_IRQ.
* 23/05/2012 JLD; Ajout Patch qui efface a coup sur le flag de RSS.
*/
/***************************************************************************************************/
bool RF_CCA_ConfirmIdle(void)
{
  uint8_t i;
  for(i=0;i<10;i++){
    WriteRegister(REG_IRQPARAM2, RegistersCfg[REG_IRQPARAM2] | RF_IRQ2_RSSI_IRQ_CLEAR);  /* Efface le flag */
    if((ReadRegister(REG_IRQPARAM2) & RF_IRQ2_RSSI_IRQ_CLEAR) == 0){break;}
  }
  Wait(ED_TIME);                                                                        /* Attend ED_TIME */  
  if(ReadRegister(REG_IRQPARAM2) & RF_IRQ2_RSSI_IRQ_CLEAR){                             /* Relit le flag */
    ucRfVal_RSSI_IRQ = ReadRegister(REG_RSSIVALUE);
    return false;
  }
  else{
    ucRfVal_RSSI_IRQ = 0;
    return true;
  }  
} /* bool RF_CCA_ConfirmIdle(void) */

/***************************************************************************************************/
/**
* @brief Fonction de CSMA-CA selon la norme 802.15.4.
*
* @param ptr sur la trame a emettre
* @retval SUCCESS ou FAILURE
* @date 13/02/2012
* @author Jean Luc DUPUIS
* 21/02/2012  NA: Au lieu de jeter l'�ponge tire quand m�me.
* 19/03/2012  NA: Retour au on jette l'�ponge + ajout _RF_CSMAFORCED_.
*/
/***************************************************************************************************/
bool RF_CsmaCa(void)
{
//#define _RF_CSMAFORCED_/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
#define macMaxCSMABackoff  4 /* The maximum number of backoffs the CSMA-CA algorithm will attempt before declaring a channel access failure.
                                Range [0-5] Default 4 */
#define macBackoffPeriod  1  /* en ms */

#define macMinBE  3 /*  The minimum value of the backoff exponent in the CSMA-CA algorithm. Note that if this value is set to 0, collision avoidance is 
                        disabled during the first iteration of the algorithm. Also note that for the slotted version of the CSMA-CA algorithm with the 
                        battery life extension enabled, the minimum value of the backoff exponent will be the lesser of 2 and the value of macMinBE.
                          Range [0-3] Default 3 */
#define aMaxBE  6   /*  The maximum value of the backoff exponent in the CSMA-CA algorithm. */
 
  uint8_t ucNb;
  uint8_t ucBe = macMinBE;
  uint16_t uiDelay;
  for(ucNb=0;ucNb<macMaxCSMABackoff;ucNb++){
    /* Calcule delai aleatoire dans [0,2^Be -1] */
    uiDelay = (uint16_t)rand() & (0x00FF >> (8-ucBe));  /* rand retourne un int [0,32767]. */
    Wait(uiDelay * macBackoffPeriod);
    if(RF_CCA_ConfirmIdle()==true){
     return true;
    }
    ucCarierDetect++;
    if(ucBe < aMaxBE){ucBe++;}
  }
  ucAccessFailure++;
#ifndef _RF_CSMAFORCED_/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
  return false;  
#else /*_RF_CSMAFORCED_  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
  return true; // Au lieu de jeter l'�ponge tire quand m�me ald false ; 
#endif/*_RF_CSMAFORCED_  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX*/
  
} /* bool RF_CsmaCa(void) */

/*--------------------------------------------------------------------------------------------------------*/
/*--------------------------- Mode CCITT -----------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------*/

/*
Mode d'emploi:

a l'init de la carte faire:

if(TOUCHE_APPUYEE_AU_RESET)
{
  if(RF_CcittConfiguration() == 0)
  {
    // Erreur de communication SX1211
    while(1);
  }

  // Affichage d'indication du mode test (led qui clignote, texte, etc).

  while(1)
  {
    RF_CcittLoop();
  }
}

*/


/**********************************************************************************************************
* NOM:bool RF_CcittConfiguration(void)
*
* DESCRIPTION :Configure le chip pour emettre en mode buffered. A appeler une fois pour changer de mode.
* PARAMETRES :sans
* VALEUR RETOURNEE :false si chip non detecte, true sinon.
* MAINTENANCE :
* 24/11/2009 JLD: Creation
*********************************************************************************************************/
bool RF_CcittConfiguration(void)
{
  uint8_t i;

  /* Initializes SX1211 en sautant le registre d'adresse 20 */
  for(i = 0; i <= REG_OSCPARAM; i++)
  {
    if(i==REG_RSSIVALUE) continue;
    WriteRegister(i, CcittRegistersCfg[i]);
  }
  /* Test de la presence du chip */
  if(RF_GetCurrentNetid() != 0xFF88B859)
    return false;
  /* Passe en TX ce qui vide la fifo quelque soit l'etat precedent ($5.2.2.4) */
  SetRFMode(RF_TRANSMITTER);
  /* Contrairement a ma 1ere version de test, je n'emets pas de preambule pour avoir un spectre lisse, ce qui interdit */
  /* toute reception de ce message et donc test du taux d'erreur */
  /* Remplit la fifo avec des octets du GPA tout reinitialise de frais  */
  uiLfsr = CCITT_SEED;
  while(!IRQ_1)
  {
    /* Tant que la fifo n'est pas pleine, sort un octet pseudo aleatoire */
   WriteFifo(RF_CcittO153());
  }
  /* Nous sommes a 25kb/s, cela va prendre 64*8/25000=20.48 ms avant d'avoir IRQ0 indiquant la fifo vide */
  return true;

} /* bool RF_CcittConfiguration(void) */

/**********************************************************************************************************
* NOM:void RF_CcittLoop(void)
*
* DESCRIPTION :Emet en continu une sequence pseudo aleatoire. A appeller toutes les 20ms au minimum.
* PARAMETRES :sans
* VALEUR RETOURNEE :sans
* MAINTENANCE :
* 24/11/2009 JLD: Creation
*********************************************************************************************************/
void RF_CcittLoop(void)
{
  if(!IRQ_0)
  { /* Fifo d'emission vide */
    while(!IRQ_1)
    {
      /* Tant que la fifo n'est pas pleine, sort un octet pseudo aleatoire */
      WriteFifo(RF_CcittO153());
    }
  }
} /* void RF_CcittLoop(void) */


/**********************************************************************************************************
* NOM:uint8_t RF_CcittO153(void)
* DESCRIPTION      : Genere une sequence pseudo aleatoire conformement a la recommandation O.153 du CCITT
  qui dit au �2.1:
  511-bit pseudo-random test pattern:
  This pattern is primarily intended for error measurements at bit rates up to 14.4 kbit/s (see � 3.1).
  The pattern may be generated in a nine-stage shift-register whose 5th and 9th stage outputs are added in a
  modulo-two addition stage, and the result is fed back to the input of the first stage. The pattern begins
  with the first ONE  of 9 consecutive ONES.
  2 Recommendation O.153 (10/92)
  Number of shift-register stages 9
  Length of the pseudo-random sequence 2^9 � 1 = 511 bits
  Longest sequence of ZEROs 8 (non-inverted signal)

La sequence n'est pas remise a zero entre chaque appel, la suite de bits est donc continue.
Dans le SX1211, le MSB est transmis en 1er.

                          +-----+
                          |     |<------------------------------------------------------------+
   +----------------------| XOR |                                                             |
   |                      |     |<-----+                                                      |
   |                      +-----+      |                                                      |
   |                                   |                                                      |
   |      +----+   +----+   +----+   +----+   +----+   +----+   +----+   +----+   +----+      |
   +----> + X8 +-->+ X7 +-->+ X6 +-->+ X5 +-->+ X4 +-->+ X3 +-->+ X2 +-->+ X1 +-->+ X0 +------+--> Out
          +----+   +----+   +----+   +----+   +----+   +----+   +----+   +----+   +----+
       x    x
1   111111111 1FF
2   011111111 0FF
3   001111111 07F
4   000111111 03F
5   000011111 01F
6   100001111 10F
7   110000111 187
8   111000011 1C3 -> 0xFF
1   111100001 1E1
2   011110000 0F0
3   101111000 178
4   110111100 1BC
5   111011110 1DE
6   011101111 0EF
7   001110111 077
8   000111011 03B -> 0x87
1   000011101 01D
2   100001110 10E
3   010000111 087
4   101000011 143
5   110100001 1A1
6   011010000 0D0
7   001101000 068
8   100110100 134 -> 0xB8

* PARAMETRES :sans
* VALEUR RETOURNEE :octet a transmettre
* MAINTENANCE :
* 24/11/2009 JLD: Creation
*********************************************************************************************************/
uint8_t RF_CcittO153(void)
{
  uint8_t j,r,m;
  bool bIn;

  r = 0;
  m = 0x80;

  for(j=0;j<8;j++)
  {
    /* Calcul de l'octet a emettre MSB 1st */
    if(uiLfsr & 1) r |= m;
    m = m >> 1;
    /* GPA, le compilo a des problemes avec le ou excusif a^b */
    bIn = (bool)(((uiLfsr & 32) && !(uiLfsr & 1)) || (!(uiLfsr & 32) && (uiLfsr & 1)));
    uiLfsr = uiLfsr >> 1;
    if (bIn) uiLfsr |= 0x100;
  }
  return r;
} /* uint8_t RF_CcittO153(void) */


