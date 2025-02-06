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

 Projet:  Face avant a LCD 946-A1
 Fichier:  sx1211.h
 Description: Fonctions du SX1211
 Auteur:      Jean Luc DUPUIS
 Version:     00
 CPU: STM32F207VG @ 120MHz interne
 Compilateur IAR EWARM v7.80 Librairies ST V1.1.2
 Modifications:
 01/09/09: Adaptation au STM32.

-----------------------------------------------------------------------------------------------------
                                     Propriete de A. THEOBALD SA
                          Ne peut etre copie ou diffuse sans son accord prealable
---------------------------------------------------------------------------------------------------*/
/* Public define ----------------------------------------------------------------------------------*/
/*******************************************************************
** SX1211 definitions                                             **
*******************************************************************/

/*******************************************************************
** SX1211 Operating modes definition                              **
*******************************************************************/
#define RF_SLEEP                         0x00   // 0
#define RF_STANDBY                       0x20   // 32d
#define RF_SYNTHETIZER                   0x40   // 64d
#define RF_RECEIVER                      0x60   // 96d
#define RF_TRANSMITTER                   0x80   // 128d


/*******************************************************************
** SX1211 Internal registers Address                              **
*******************************************************************/
#define REG_MCPARAM1                     0x00
#define REG_MCPARAM2                     0x01
#define REG_FDEV                         0x02
#define REG_BITRATE                      0x03
#define REG_OOKFLOORTHRESH               0x04
#define REG_MCPARAM6                     0x05
#define REG_R1                           0x06
#define REG_P1                           0x07
#define REG_S1                           0x08
#define REG_R2                           0x09
#define REG_P2                           0x0A
#define REG_S2                           0x0B
#define REG_PARAMP                       0x0C

#define REG_IRQPARAM1                    0x0D
#define REG_IRQPARAM2                    0x0E
#define REG_RSSIIRQTHRESH                0x0F

#define REG_RXPARAM1                     0x10
#define REG_RXPARAM2                     0x11
#define REG_RXPARAM3                     0x12
#define REG_RES19                        0x13
#define REG_RSSIVALUE                    0x14
#define REG_RXPARAM6                     0x15

#define REG_SYNCBYTE1                    0x16
#define REG_SYNCBYTE2                    0x17
#define REG_SYNCBYTE3                    0x18
#define REG_SYNCBYTE4                    0x19

#define REG_TXPARAM                      0x1A

#define REG_OSCPARAM                     0x1B

#define REG_PKTPARAM1                    0x1C
#define REG_NODEADRS                     0x1D
#define REG_PKTPARAM3                    0x1E
#define REG_PKTPARAM4                    0x1F


/*******************************************************************
** SX1211 initialisation register values definition               **
*******************************************************************/
#define DEF_MCPARAM1                     0x00
#define DEF_MCPARAM2                     0x00
#define DEF_FDEV                         0x00
#define DEF_BITRATE                      0x00
#define DEF_OOKFLOORTHRESH               0x00
#define DEF_MCPARAM6                     0x00
#define DEF_R1                           0x00
#define DEF_P1                           0x00
#define DEF_S1                           0x00
#define DEF_R2                           0x00
#define DEF_P2                           0x00
#define DEF_S2                           0x00
#define DEF_PARAMP                       0x20

#define DEF_IRQPARAM1                    0x00
#define DEF_IRQPARAM2                    0x08
#define DEF_RSSIIRQTHRESH                0x00

#define DEF_RXPARAM1                     0x00
#define DEF_RXPARAM2                     0x08
#define DEF_RXPARAM3                     0x20
#define DEF_RES19                        0x07
#define DEF_RSSIVALUE                    0x00
#define DEF_RXPARAM6                     0x00

#define DEF_SYNCBYTE1                    0x00
#define DEF_SYNCBYTE2                    0x00
#define DEF_SYNCBYTE3                    0x00
#define DEF_SYNCBYTE4                    0x00

#define DEF_TXPARAM                      0x00

#define DEF_OSCPARAM                     0x00

#define DEF_PKTPARAM1                    0x00
#define DEF_NODEADRS                     0x00
#define DEF_PKTPARAM3                    0x00
#define DEF_PKTPARAM4                    0x00

/*******************************************************************
** SX1211 bit control definition                                  **
*******************************************************************/
// MC Param 1
// Chip operating mode
#define RF_MC1_SLEEP                     0x00
#define RF_MC1_STANDBY                   0x20
#define RF_MC1_SYNTHESIZER               0x40
#define RF_MC1_RECEIVER                  0x60
#define RF_MC1_TRANSMITTER               0x80

// Frequency band
#define RF_MC1_BAND_915L                 0x00
#define RF_MC1_BAND_915H                 0x08
#define RF_MC1_BAND_868                  0x10
#define RF_MC1_BAND_950                  0x10

// VCO trimming
#define RF_MC1_VCO_TRIM_00               0x00
#define RF_MC1_VCO_TRIM_01               0x02
#define RF_MC1_VCO_TRIM_10               0x04
#define RF_MC1_VCO_TRIM_11               0x06

// RF frequency selection
#define RF_MC1_RPS_SELECT_1              0x00
#define RF_MC1_RPS_SELECT_2              0x01

// MC Param 2
// Modulation scheme selection
#define RF_MC2_MODULATION_OOK            0x40
#define RF_MC2_MODULATION_FSK            0x80

// Data operation mode
#define RF_MC2_DATA_MODE_CONTINUOUS      0x00
#define RF_MC2_DATA_MODE_BUFFERED        0x20
#define RF_MC2_DATA_MODE_PACKET          0x04

// Rx OOK threshold mode selection
#define RF_MC2_OOK_THRESH_TYPE_FIXED     0x00
#define RF_MC2_OOK_THRESH_TYPE_PEAK      0x08
#define RF_MC2_OOK_THRESH_TYPE_AVERAGE   0x10

// Gain on IF chain
#define RF_MC2_GAIN_IF_00                0x00
#define RF_MC2_GAIN_IF_01                0x01
#define RF_MC2_GAIN_IF_10                0x02
#define RF_MC2_GAIN_IF_11                0x03


// Frequency deviation (kHz)
#define RF_FDEV_33                       0x0B
#define RF_FDEV_40                       0x09
#define RF_FDEV_50                       0x07
#define RF_FDEV_80                       0x04
#define RF_FDEV_100                      0x03
#define RF_FDEV_133                      0x02
#define RF_FDEV_200                      0x01


// Bitrate (bit/sec)
#define RF_BITRATE_1600                  0x7C
#define RF_BITRATE_2000                  0x63
#define RF_BITRATE_2500                  0x4F
#define RF_BITRATE_5000                  0x27
#define RF_BITRATE_8000                  0x18
#define RF_BITRATE_10000                 0x13
#define RF_BITRATE_20000                 0x09
#define RF_BITRATE_25000                 0x07
#define RF_BITRATE_40000                 0x04
#define RF_BITRATE_50000                 0x03
#define RF_BITRATE_100000                0x01


// OOK threshold
#define RF_OOKFLOORTHRESH_VALUE          0x0C


// MC Param 6
// FIFO size
#define RF_MC6_FIFO_SIZE_16              0x00
#define RF_MC6_FIFO_SIZE_32              0x40
#define RF_MC6_FIFO_SIZE_48              0x80
#define RF_MC6_FIFO_SIZE_64              0xC0

// FIFO threshold
#define RF_MC6_FIFO_THRESH_VALUE         0x0F

// Freq1 868.950 MHz (Milieu de bande G2)
// SynthR1
#define RF_R1_VALUE                      127
// SynthP1
#define RF_P1_VALUE                      101
// SynthS1
#define RF_S1_VALUE                      74

// Freq2 867.00 MHz ( Milieu de bande SX1211 863-870 pour test RF)
// SynthR2
#define RF_R2_VALUE                      119
// SynthP2
#define RF_P2_VALUE                      95
// SynthS2
#define RF_S2_VALUE                      25

// PA ramp times in OOK
#define RF_PARAMP_00                     0x00
#define RF_PARAMP_01                     0x08
#define RF_PARAMP_10                     0x10
#define RF_PARAMP_11                     0x18


// IRQ Param 1
// Select RX&STDBY IRQ_0 sources (Packet mode)
#define RF_IRQ0_RX_STDBY_PAYLOADREADY    0x00
#define RF_IRQ0_RX_STDBY_WRITEBYTE       0x40
#define RF_IRQ0_RX_STDBY_FIFOEMPTY       0x80
#define RF_IRQ0_RX_STDBY_SYNCADRS        0xC0

// Select RX&STDBY IRQ_1 sources (Packet mode)
#define RF_IRQ1_RX_STDBY_CRCOK           0x00
#define RF_IRQ1_RX_STDBY_FIFOFULL        0x10
#define RF_IRQ1_RX_STDBY_RSSI            0x20
#define RF_IRQ1_RX_STDBY_FIFOTHRESH      0x30

// Select TX IRQ_1 sources (Packet mode)
#define RF_IRQ1_TX_FIFOFULL              0x00
#define RF_IRQ1_TX_TXDONE                0x08

// FIFO overrun/clear
#define RF_IRQ1_FIFO_OVERRUN_CLEAR       0x01


// IRQ Param 2
// Select TX start condition and IRQ_0 source (Packet mode)
#define RF_IRQ0_TX_FIFOTHRESH_START_FIFOTHRESH     0x00
#define RF_IRQ0_TX_FIFOEMPTY_START_FIFONOTEMPTY    0x10
//FIFO filling method (buffered mode)
#define RF_IRQ2_FIFOFILL_METHOD_AUTO     0x00
#define RF_IRQ2_FIFOFILL_METHOD_MANUAL   0x80

// RSSI IRQ flag
#define RF_IRQ2_RSSI_IRQ_CLEAR           0x04

// PLL_Locked flag
#define RF_IRQ2_PLL_LOCK_CLEAR           0x02

// PLL_Locked pin
#define RF_IRQ2_PLL_LOCK_PIN_OFF         0x00
#define RF_IRQ2_PLL_LOCK_PIN_ON          0x01

// RSSI threshold for interrupt 
// 802.15.4 dit CCA thresold = Sensitivity @BER=1% + 10dB. Sensibilite mesuree par Semtech @BER=0.1% : -104dBm
// -104 + 10 =-94dBm ou RSSI=50.
//#define RF_RSSIIRQTHRESH_VALUE           50 /* -90dBm */
//#define RF_RSSIIRQTHRESH_VALUE           70 /* -80dBm qualif */
//#define RF_RSSIIRQTHRESH_VALUE           130 /* -50dBm qualif */
#define RF_RSSIIRQTHRESH_VALUE           140 /* -50dBm qualif */

// RX Param 1
// Passive filter (kHz)
#define RF_RX1_PASSIVEFILT_65            0x00
#define RF_RX1_PASSIVEFILT_82            0x10
#define RF_RX1_PASSIVEFILT_109           0x20
#define RF_RX1_PASSIVEFILT_137           0x30
#define RF_RX1_PASSIVEFILT_157           0x40
#define RF_RX1_PASSIVEFILT_184           0x50
#define RF_RX1_PASSIVEFILT_211           0x60
#define RF_RX1_PASSIVEFILT_234           0x70
#define RF_RX1_PASSIVEFILT_262           0x80
#define RF_RX1_PASSIVEFILT_321           0x90
#define RF_RX1_PASSIVEFILT_378           0xA0
#define RF_RX1_PASSIVEFILT_414           0xB0
#define RF_RX1_PASSIVEFILT_458           0xC0
#define RF_RX1_PASSIVEFILT_514           0xD0
#define RF_RX1_PASSIVEFILT_676           0xE0
#define RF_RX1_PASSIVEFILT_987           0xF0

// Butterworth filter (kHz)
#define RF_RX1_FC_VALUE                  0x03
// !!! Values defined below only apply if RFCLKREF = DEFAULT VALUE = 0x07 !!!
#define RF_RX1_FC_FOPLUS25               0x00
#define RF_RX1_FC_FOPLUS50               0x01
#define RF_RX1_FC_FOPLUS75               0x02
#define RF_RX1_FC_FOPLUS100              0x03
#define RF_RX1_FC_FOPLUS125              0x04
#define RF_RX1_FC_FOPLUS150              0x05
#define RF_RX1_FC_FOPLUS175              0x06
#define RF_RX1_FC_FOPLUS200              0x07
#define RF_RX1_FC_FOPLUS225              0x08
#define RF_RX1_FC_FOPLUS250              0x09



// RX Param 2
// Polyphase filter center value (kHz)
#define RF_RX2_FO_VALUE                  0x03
// !!! Values defined below only apply if RFCLKREF = DEFAULT VALUE = 0x07 !!!
#define RF_RX2_FO_50                     0x10
#define RF_RX2_FO_75                     0x20
#define RF_RX2_FO_100                    0x30
#define RF_RX2_FO_125                    0x40
#define RF_RX2_FO_150                    0x50
#define RF_RX2_FO_175                    0x60
#define RF_RX2_FO_200                    0x70
#define RF_RX2_FO_225                    0x80
#define RF_RX2_FO_250                    0x90
#define RF_RX2_FO_275                    0xA0
#define RF_RX2_FO_300                    0xB0
#define RF_RX2_FO_325                    0xC0
#define RF_RX2_FO_350                    0xD0
#define RF_RX2_FO_375                    0xE0
#define RF_RX2_FO_400                    0xF0


// Rx Param 3
// Polyphase filter enable
#define RF_RX3_POLYPFILT_ON              0x80
#define RF_RX3_POLYPFILT_OFF             0x00

// Bit synchronizer
// Automatically activated in Packet mode

// Sync word recognition
// Automatically activated in Packet mode

// Size of the reference sync word
#define RF_RX3_SYNC_SIZE_8               0x00
#define RF_RX3_SYNC_SIZE_16              0x08
#define RF_RX3_SYNC_SIZE_24              0x10
#define RF_RX3_SYNC_SIZE_32              0x18

// Number of tolerated errors for the sync word recognition
#define RF_RX3_SYNC_TOL_0                0x00
#define RF_RX3_SYNC_TOL_1                0x02
#define RF_RX3_SYNC_TOL_2                0x04
#define RF_RX3_SYNC_TOL_3                0x06


// RSSI Value (Read only)


// Rx Param 6
// Decrement step of RSSI threshold in OOK demodulator (peak mode)
#define RF_RX6_OOK_THRESH_DECSTEP_000    0x00
#define RF_RX6_OOK_THRESH_DECSTEP_001    0x20
#define RF_RX6_OOK_THRESH_DECSTEP_010    0x40
#define RF_RX6_OOK_THRESH_DECSTEP_011    0x60
#define RF_RX6_OOK_THRESH_DECSTEP_100    0x80
#define RF_RX6_OOK_THRESH_DECSTEP_101    0xA0
#define RF_RX6_OOK_THRESH_DECSTEP_110    0xC0
#define RF_RX6_OOK_THRESH_DECSTEP_111    0xE0

// Decrement period of RSSI threshold in OOK demodulator (peak mode)
#define RF_RX6_OOK_THRESH_DECPERIOD_000  0x00
#define RF_RX6_OOK_THRESH_DECPERIOD_001  0x04
#define RF_RX6_OOK_THRESH_DECPERIOD_010  0x08
#define RF_RX6_OOK_THRESH_DECPERIOD_011  0x0C
#define RF_RX6_OOK_THRESH_DECPERIOD_100  0x10
#define RF_RX6_OOK_THRESH_DECPERIOD_101  0x14
#define RF_RX6_OOK_THRESH_DECPERIOD_110  0x18
#define RF_RX6_OOK_THRESH_DECPERIOD_111  0x1C

// Cutoff freq of average function of RSSI threshold in OOK demodulator (average mode)
#define RF_RX6_OOK_THRESH_AVERAGING_00   0x00
#define RF_RX6_OOK_THRESH_AVERAGING_11   0x03


// TX Param
// Interpolator filter Tx (kHz)
#define RF_TX_FC_VALUE                   0x70
// !!! Values defined below only apply if RFCLKREF = DEFAULT VALUE = 0x07 !!!
#define RF_TX_FC_25                      0x00
#define RF_TX_FC_50                      0x10
#define RF_TX_FC_75                      0x20
#define RF_TX_FC_100                     0x30
#define RF_TX_FC_125                     0x40
#define RF_TX_FC_150                     0x50
#define RF_TX_FC_175                     0x60
#define RF_TX_FC_200                     0x70
#define RF_TX_FC_225                     0x80
#define RF_TX_FC_250                     0x90
#define RF_TX_FC_275                     0xA0
#define RF_TX_FC_300                     0xB0
#define RF_TX_FC_325                     0xC0
#define RF_TX_FC_350                     0xD0
#define RF_TX_FC_375                     0xE0
#define RF_TX_FC_400                     0xF0

// Tx Output power (dBm)
#define RF_TX_POWER_MAX                  0x00
#define RF_TX_POWER_PLUS10               0x02
#define RF_TX_POWER_PLUS7                0x04
#define RF_TX_POWER_PLUS4                0x06
#define RF_TX_POWER_PLUS1                0x08
#define RF_TX_POWER_MINUS2               0x0A
#define RF_TX_POWER_MINUS5               0x0C
#define RF_TX_POWER_MINUS8               0x0E

// OSC Param
// CLKOUT enable
#define RF_OSC_CLKOUT_ON                 0x80
#define RF_OSC_CLKOUT_OFF                0x00

// CLKOUT frequency (kHz)
#define RF_OSC_CLKOUT_12800              0x00
#define RF_OSC_CLKOUT_6400               0x04
#define RF_OSC_CLKOUT_3200               0x08
#define RF_OSC_CLKOUT_2133               0x0C
#define RF_OSC_CLKOUT_1600               0x10
#define RF_OSC_CLKOUT_1280               0x14
#define RF_OSC_CLKOUT_1067               0x18
#define RF_OSC_CLKOUT_914                0x1C
#define RF_OSC_CLKOUT_800                0x20
#define RF_OSC_CLKOUT_711                0x24
#define RF_OSC_CLKOUT_640                0x28
#define RF_OSC_CLKOUT_582                0x2C
#define RF_OSC_CLKOUT_533                0x30
#define RF_OSC_CLKOUT_492                0x34
#define RF_OSC_CLKOUT_457                0x38
#define RF_OSC_CLKOUT_427                0x3C


// PKT Param 1
// Manchester enable
#define RF_PKT1_MANCHESTER_ON            0x80
#define RF_PKT1_MANCHESTER_OFF           0x00

// Payload length
#define RF_PKT1_LENGTH_VALUE             0x00


// Node Address
#define RF_NODEADRS_VALUE                0xFF


// PKT Param 3
//Packet format
#define RF_PKT3_FORMAT_FIXED             0x00
#define RF_PKT3_FORMAT_VARIABLE          0x80

// Preamble size
#define RF_PKT3_PREAMBLE_SIZE_8          0x00
#define RF_PKT3_PREAMBLE_SIZE_16         0x20
#define RF_PKT3_PREAMBLE_SIZE_24         0x40
#define RF_PKT3_PREAMBLE_SIZE_32         0x60

// Whitening enable
#define RF_PKT3_WHITENING_ON             0x10
#define RF_PKT3_WHITENING_OFF            0x00

// CRC enable
#define RF_PKT3_CRC_ON                   0x08
#define RF_PKT3_CRC_OFF                  0x00

// Address filtering
#define RF_PKT3_ADRSFILT_00              0x00   // off
#define RF_PKT3_ADRSFILT_01              0x02   // Nid seul
#define RF_PKT3_ADRSFILT_10              0x04   // Nid + 0x00
#define RF_PKT3_ADRSFILT_11              0x06   // Nid + 0x00 + 0xFF

//CRC status (Read only)
#define RF_PKT3_CRC_STATUS               0x01


// PKT Param 4
// FIFO autoclear if CRC failed for current packet
#define RF_PKT4_AUTOCLEAR_ON             0x00
#define RF_PKT4_AUTOCLEAR_OFF            0x80

// Select FIFO access in standby mode (read or write)
#define RF_PKT4_FIFO_STANDBY_WRITE       0x00
#define RF_PKT4_FIFO_STANDBY_READ        0x40


// Timings
#define TS_OS          5000 // Quartz Osc wake up time, max 5 ms
#define TS_FS          800  // Freq Synth wake-up time from OS, max 800 us
#define TS_RE          500  // Receiver wake-up time from FS, max 500 us
#define TS_TR          500 // Transmitter wake-up time from FS, max 500 us

#define ED_TIME        320 /* 802.15.4 dit 8 symboles, a 25ksymboles/sec, 320�s */ 

/* Private macro ----------------------------------------------------------------------------------*/
#define RF_FIFO_EMPTY   (IRQ_0 == 0)      /* Fifo SX1211 */

/* Private variables ------------------------------------------------------------------------------*/
