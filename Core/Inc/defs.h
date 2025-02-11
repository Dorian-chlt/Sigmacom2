
/*--------------------------------------------------------------------------------------------------------------------------------------------
                   A.THEOBALD SA CONTROLE ET REGULATION
----------------------------------------------------------------------------------------------------------------------------------------------

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

 Projet:  Radio : carte THM.
 Fichier:  defs.h
 Description: Fichier include principal.
 Auteur:      NA
 Version:     XX-00
 CPU: MC9S08LL16 
 Modifications:
 29/07/09: Creation.
-----------------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------------------------
                Propriete de A. THEOBALD SA
         Ne peut etre copie ou diffuse sans son accord prealable
--------------------------------------------------------------------------------------------------------------------------------------------*/

/*=== INCLUDE FILES ========================================================================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEFS_H
#define __DEFS_H

/*=== DEFINITIONS SPECIFIQUES ==============================================================================================================*/
#define _TYPE_		0x27	    /* Type de produit */
#define _CDC_	  	0x00	    /* Version de cdc */
#define _RELEASE_	0x04	    /* Version de soft */
#define _DTCR_     "27;00;04"       /* MST display char ASCII */
//#define DEBUG /* Definition manuelle si l'environnement ne gère pas l'option */
//#define WO_APP_RESET
//#define WO_ADC_AVERAGE
//#define WO_ACT_WEB     /* RAZ de l'info equipement Visio Connect */
/*----------------------------------------------------------------------------*/
#ifndef DEBUG
/*----------------------------------------------------------------------------*/
#define _PRD_   /* Production avec passage aux bancs tests RF et Vision */
//#define _VAL_   /* Validation sans passage aux bancs tests (avec des cartes parfaites) */
//#define _LAB_   /* Laboratoire avec equipements RF non matchés ---> LABO */
//#define _RND_   /* R&D speciale avec equipements RF pre-matchés ----> NA */
/*----------------------------------------------------------------------------*/
#endif/*DEBUG*/

/* ATTENTION pdt la map avec equipements RF pre-matchés un chgt de FAV
necessite un full erase, pour forcer l'auto-init avec preset de la FLASH */
#define _FAVLED_   /* Compil pour map avec Fav LED type ERS */
//#define _FAVOLD_   /* Compil pour map avec Fav LCD à icones semi-graphique */

#define _VERSDISPLAY_       /* Dir. de compil. pour afficher la version du THM */
//#define _TESTDISPLAY_
#define MSG2SCROLL {DRLVP _DTCR_}          /* char ASCII */
//#define MSG2SCROLL { __TIME__ __DATE__ }  /* char ASCII */

/*=== DEFINITIONS GLOBALES =================================================================================================================*/
#define NODEID_MAXI NID_Thermostat_LCD_Zone_3 /* NID_Thermostat_LCD_Zone_3 */
#define NODEID_INIT NID_Thermostat_LCD_Zone_1 /* NID_Thermostat_LCD_Zone_1 */

#define THMZX (ucIDNODE - NID_Thermostat_LCD_Zone_1) /* numero de zone Z1=0,.., Z8=7 */
#define THMCX (ucIDPART - NID_FAV_Collective_1) /* numero de collectif C1=0,.., C4=3 */

#define TIM01sRST_REPEAT  610   /* Effectue une TX RF vers la FAV toutes les 10mns */
#define TOP_TIME_NDOFSWREL  15  /* XXs apres le dernier relaché */

/* Envoie une trame vers le PART 03s après le dernier relaché */
#define RF_REFRESH03s  uiTIM01sRST_SAMPLE = TIM01sRST_REPEAT-3; 

/* Envoie une trame vers le PART XXs après le dernier relaché */
#define RF_REFRESHXXs  uiTIM01sRST_SAMPLE = TIM01sRST_REPEAT-TOP_TIME_NDOFSWREL;

/* Timeout */
#define TIMEOUT_REDSUPxH            18 /* car 18 x 10mn = 180mn = 3h */
#define TIMEOUT_TIM01sACT_INFO       5 // Timeout chargé à 5s.
#define TIMEOUT_TIM01sMST_UPDT      30 // Timeout chargé à 30s.
#define TIMEOUT_TIM01sWEB_UPDT       4 // Timeout chargé à 4s.
#define TIMEOUT_TIM1TSWEBMODIF       1 // Timeout chargé à 1TimeSlot.
#define SW_MATCHING_AXS              13     //   13 x  DELAY_REPEAT_250ms.
#define SW_AXS_REGL_LVL0              2     //    2 x  DELAY_REPEAT 02s
#define SW_AXS_REGL_LVL1              5     //    5 x  DELAY_REPEAT_FIRST 05s
#define SW_AXS_REGL_LVL2              1     //    1 x  DELAY_REPEAT_NEXT 01s.
#define TIMEOUT_TIM5msRUN_ACT_BY_RXTX           60000 //60000 x  5ms   300s.soit 5mn de CTRL_RF_RXTX
#define TIMEOUT_TIM5msRUN_ACT_BY_MATC             200 //  200 x  5ms 1000ms.
#define TIMEOUT_TIM5msRUN_ACT_BY_MATF            6000 // 6000 x  5ms    30s.
#define TIMEOUT_TIM5msRUN_ACT_BY_RXLV            2000 // 2000 x  5ms    10s.
#define TIMEOUT_TIM5msRUN_ACT_BY_RXGC             400 //  400 x  5ms     2s.
#define TIMEOUT_TIM5msRUN_ACT_BY_AUTO TO_T5msRUNAUTOMAX // 3776 x  5ms 18,88s, pour 2x(4TX-RX + alea non acquittés).
#define TIMEOUT_TIM5msRUN_ACT_BY_INFO   200 //  200 x  5ms 1000ms.
#define TIMEOUT_TIM5msRUN_ACT_BY_WAIT    20 //   20 x  5ms  100ms.
#define TIMEOUT_TIM01sLIM_MODE       600 /* Timeout chargé à 600 */
#define RTC_HBKP      1  /* à 01hXX */
#define RTC_MBKP      5  /* à XXh05 */
#define RTC_SBKP      36 /* à XXhXX36s */

/* Paramètres installateur de la mesure de la CTN */
#define STEP01C      1 /* Step d'inc/dec à 0.1C° */
#define TABSOF7PRO  20 /* Temperature absolue maxi d'offset cad (-2.0°C à +2.0°C) */

/* valeur initiale de g_Text pour ne pas l'afficher */
#define SANS_Text 1290 

/* ===================================== HARDWARE ================================================ */
/* Voltage level are in Volts x 100 */
#define BATT_WHEEL_MAX  310
#define BATT_WHEEL_MIN  (BATT_WHEEL_MAX-96)
/* Seuils d'alarme */
#define BATT_LEVEL_LOW  215 /* batterie clignote */
#define BATT_LEVEL_NOP  210 /* batterie & defaut clignotent */
#define BATT_LEVEL_RES  190 /* RESET */
#define BATT_SUCC_CONF    2 /* pour avoir 3 confirmations successives avant definition du FLAG */

#define LEDRADR   V_LEDRADR
#define LEDRADV   V_LEDRADV

#define LED_LOW(num)  LED_##num = FALSE
#define LED_HIGH(num) LED_##num = TRUE
#define LED_TOGL(num) LED_##num = ~LED_##num

//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#ifdef  _TRACE_SCOPE_
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#define vfnMapLED_(num,act) LED_##act(num) /* PTCD_PTCDX */
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#else// _TRACE_SCOPE_
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#define vfnMapLED_(num,act) LED_##act(4)   /* V_LED4 */
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
#endif//_TRACE_SCOPE_

#define BLINK	2

/* ====================================== SOFTWARE ============================================= */
/* State Machine */
enum _SMRAD_ITEM_
{                    
  SMRAD_SLEEP,
  SMRAD_SLAVE, 
  SMRAD_MASTER_TRANSMIT, 
  SMRAD_MASTER_RXACKWAIT, 
  SMRAD_MASTER_GOTOSLEEP, 
};

/* State Machine */
enum _SMMODE_ITEM_
{
  SMMOD_TEST_INIT,     /* Test fabrication poste RF */
  SMMOD_TEST_RXTX,     /* Test fabrication poste RF */
  SMMOD_TEST_SLCD,     /* Test fabrication poste VISION */
  SMMOD_TEST_OF7C,     /* Test fabrication poste VISION */
  SMMOD_REGL_PROG,     /* Mode reglage a duree limitee */
  SMMOD_PERM_AUTO,     /* Mode permanent */
  SMMOD_PERM_ASOL,     /* Mode permanent */
  SMMOD_PERM_ALUN,     /* Mode permanent */
  SMMOD_PERM_AGEL,     /* Mode permanent */
  SMMOD_REGL_ACLK,     /* Mode reglage a duree limitee */
  SMMOD_REGL_ALUG,     /* Mode reglage a duree limitee */
  SMMOD_REGL_LCTR,     /* Mode reglage a duree limitee */
  SMMOD_REGL_ZONE,     /* Mode reglage a duree limitee */
  SMMOD_REGL_OF7P,     /* Mode reglage a duree limitee */
  SMMOD_REGL_VALH,     /* Mode reglage a duree limitee */
  SMMOD_REGL_VMAX,     /* Mode reglage a duree limitee */
  SMMOD_REGL_VALI,     /* Mode reglage a duree limitee */
  SMMOD_REGL_VMIN,     /* Mode reglage a duree limitee */
  SMMOD_REGL_VALK,     /* Mode reglage a duree limitee */
};

/* State Machine */
enum _SMLCD_ITEM_
{
  SMLCD_0,
  SMLCD_1,
  SMLCD_2,
  SMLCD_3,
  SMLCD_4,
  SMLCD_5,
  SMLCD_6,
};

/* State Machine */
enum _SMINF_ITEM_
{
  SMINF_0,      /* Menu info premier cas : en position initiale d'attente */
  SMINF_1,      /* Menu info g_Text */
  SMINF_2,      /* Menu info g_Tamb */
  SMINF_3,      /* Menu info _Error_Handler, nombre de Reset*/
  SMINF_4,      /* Menu info _Error_Handler, code erreur */
  SMINF_5,      /* Menu info _Error_Handler, nom de fichier */
  SMINF_6,      /* Menu info _Error_Handler, numero de ligne */
  SMINF_7,      /* Menu info nombre de Reset Hardware */
  SMINF_8,      /* Menu info variable de test */
  SMINF_9,      /* Menu info dernier cas : version courante en scroll */
};

/* State Machine */
enum _SMCLK_ITEM_
{
  SMCLK_0,      /* Menu clk premier cas : en position initiale d'attente annee */
  SMCLK_1,      /* Menu clk mois */
  SMCLK_2,      /* Menu clk date */
  SMCLK_3,      /* Menu clk heure ete/hiv automatique */
  SMCLK_4,      /* Menu clk heure */
  SMCLK_5,      /* Menu clk dernier cas : minute */
};
#define SMCLK_X SMCLK_5 /* affichage des fenetres de SMCLK_0 à SMCLK_X */

typedef struct{
  uint16_t  TypeDeRegulation:3;   /* REGULATION_XXX */
  uint16_t  PlancherRadiateur:1;  /* 1: radiateurs */
  uint16_t  Pente:5;              /* Pente 0-31 */
  uint16_t  Ambiance:4;           /* Ambiance 0-15 */
  uint16_t  Hysteresis:3;         /* Valeur de l'hysteresis 0-7 */
  uint8_t   Umax;                 /* Saturation commande maxi */
  uint8_t   Umin;                 /* Saturation commande mini */
  uint8_t   Nk;                   /* N pour calcul du k */
  uint8_t   Inertie:2;            /* Inertie 0-3 */
  uint8_t   Filaire:2;            /* Regul. filaireInertie 0-3 */  
  uint8_t   Libre2:4;             /* Libre */
                                  /*!< Total 3 words */
}REGULATOR_BLOCK;
#define Par_TypeDeRegulation      SRAM2EEP.Pregul.TypeDeRegulation
#define Par_PlancherRadiateur     SRAM2EEP.Pregul.PlancherRadiateur
#define Par_Pente                 SRAM2EEP.Pregul.Pente
#define Par_Ambiance              SRAM2EEP.Pregul.Ambiance
#define Par_Hysteresis            SRAM2EEP.Pregul.Hysteresis
#define Par_Umax                  SRAM2EEP.Pregul.Umax
#define Par_Umin                  SRAM2EEP.Pregul.Umin
#define Par_Nk                    SRAM2EEP.Pregul.Nk
#define Par_Inertie               SRAM2EEP.Pregul.Inertie
#define Par_Filaire               SRAM2EEP.Pregul.Filaire
#define Par_Libre2                SRAM2EEP.Pregul.Libre2

/* ====================================== BIT FIELDS ============================================= */
/* generic bit management structure */
union byte_def{
  struct{
    char	b0:1;
    char	b1:1;
    char	b2:1;
    char	b3:1;
    char	b4:1;
    char	b5:1;
    char	b6:1;
    char	b7:1;
  }bit;
  uint8	byte;
};

union word_def{
  struct{
    char	b0:1;
    char	b1:1;
    char	b2:1;
    char	b3:1;
    char	b4:1;
    char	b5:1;
    char	b6:1;
    char	b7:1;
    char	b8:1;
    char	b9:1;
    char	b10:1;
    char	b11:1;
    char	b12:1;
    char	b13:1;
    char	b14:1;
    char	b15:1;
  }bit;
  uint16 word;
};

/* Structure de HH48SEG */
typedef struct
{
  UINT8 RAM[6];    
}HH48SEG;

/* Flags Transmis à la FAV et le WEB */
extern union word_def SFlags0;      /* Structure */
#define SFlags0Value	SFlags0.word	/* all bits value */
#define FLAG_SUN_STATE            SFlags0.bit.b0
#define FLAG_USER_DERO            SFlags0.bit.b1
#define FLAG_REDSUPxH             SFlags0.bit.b2
#define FLAG_ACT_HGEL             SFlags0.bit.b3
#define FLAG_GEL_STATE            SFlags0.bit.b4
#define FLAG_OTO_STATE            SFlags0.bit.b5
#define FLAG_WEB_BOOST            SFlags0.bit.b6
#define FLAG_007                  SFlags0.bit.b7
#define FLAG_008                  SFlags0.bit.b8
#define FLAG_009                  SFlags0.bit.b9
#define FLAG_010                  SFlags0.bit.b10
#define FLAG_011                  SFlags0.bit.b11
#define FLAG_012                  SFlags0.bit.b12
#define FLAG_013                  SFlags0.bit.b13
#define FLAG_014                  SFlags0.bit.b14
#define FLAG_BATT_LOW             SFlags0.bit.b15

/* Flags Recus de la FAV */
extern union byte_def SFlags1;      /* Structure */
#define SFlags1Value	SFlags1.byte	/* all bits value */
#define FLAG_ALREP                SFlags1.bit.b0
#define FLAG_ACT_VAC              SFlags1.bit.b1
#define FLAG_ACT_ARR              SFlags1.bit.b2
#define FLAG_POMPE1               SFlags1.bit.b3
#define FLAG_EXTECSSTOP           SFlags1.bit.b4
#define FLAG_ACT_WEB              SFlags1.bit.b5
#define FLAG_106                  SFlags1.bit.b6
#define FLAG_107                  SFlags1.bit.b7

/* Flags generaux */
extern union byte_def SFlags2;      /* Structure */
#define SFlags2Value	SFlags2.byte	/* all bits value */
#define FLAG_ACT_TIM5msRUN_BY_KEYB     SFlags2.bit.b0
#define FLAG_ACT_TIM5msRUN_BY_AUTO     SFlags2.bit.b1
#define FLAG_ACT_TIM5msRUN_BY_RXLV     SFlags2.bit.b2
#define FLAG_ACT_TIM5msRUN_BY_MATC     SFlags2.bit.b3
#define FLAG_ACT_TIM5msRUN_BY_MATF     SFlags2.bit.b4
#define FLAG_REF_EEP_RF_SWREL     SFlags2.bit.b5
#define FLAG_REFRESH_EEP          SFlags2.bit.b6
#define FLAG_REFRESH_LCD          SFlags2.bit.b7

/* Flags generaux */
extern union byte_def SFlags3;      /* Structure */
#define SFlags3Value	SFlags3.byte	/* all bits value */
#define FLAG_ACT_INFO             SFlags3.bit.b0
#define FLAG_NEW_CSE              SFlags3.bit.b1
#define FLAG_RESETMCU             SFlags3.bit.b2
#define FLAG_MATCHED              SFlags3.bit.b3
#define FLAG_ACT_QSEC             SFlags3.bit.b4
#define FLAG_VER_DISP             SFlags3.bit.b5
#define FLAG_DIST_LOST            SFlags3.bit.b6
#define FLAG_BATT_NOP             SFlags3.bit.b7

/* Flags generaux */
extern union byte_def SFlags4;      /* Structure */
#define SFlags4Value	SFlags4.byte	/* all bits value */
#define FLAG_SX1211_WAKEUP        SFlags4.bit.b0
#define FLAG_SWACT                SFlags4.bit.b1
#define FLAG_CDZMEMAVRED          SFlags4.bit.b2
#define FLAG_HH_EEP               SFlags4.bit.b3
#define FLAG_HH_RAM               SFlags4.bit.b4
#define FLAG_SX1211_READY         SFlags4.bit.b5
#define FLAG_SWPRESSED            SFlags4.bit.b6
#define FLAG_DUMMY                SFlags4.bit.b7

/* Flags generaux */
extern union byte_def SFlags5;     /* Structure */
#define SFlags5Value	SFlags5.byte /* all bits value */
#define FLAG_NOALREP              SFlags5.bit.b0
#define FLAG_ACT_MATC             SFlags5.bit.b1
#define FLAG_SND_MSGI_AMB         SFlags5.bit.b2
#define FLAG_SND_MSGI_CSE         SFlags5.bit.b3
#define FLAG_TXRX_LIVE            SFlags5.bit.b4
#define FLAG_TS_FIRE              SFlags5.bit.b5
#define FLAG_TS_AGAIN             SFlags5.bit.b6
#define TOP_SCI                   SFlags5.bit.b7

/* Flags generaux */
extern union byte_def SFlags6;     /* Structure */
#define SFlags6Value	SFlags6.byte /* all bits value */
#define FLAG_600                  SFlags6.bit.b0
#define FLAG_COLLECTIF            SFlags6.bit.b1
#define FLAG_READEND              SFlags6.bit.b2
#define FLAG_MODBUS_NACK          SFlags6.bit.b3
#define FLAG_READCHK              SFlags6.bit.b4
#define FLAG_UPDT_THM2WEB         SFlags6.bit.b5
#define FLAG_UPDT_WEB2THM         SFlags6.bit.b6
#define FLAG_WEB_MODIF            SFlags6.bit.b7

/* Flags generaux */
extern union byte_def SFlags7;     /* Structure */
#define SFlags7Value	SFlags7.byte /* all bits value */
#define FLAG_REG_MSG              SFlags7.bit.b0
#define FLAG_SND_MSGIPreg         SFlags7.bit.b1
#define FLAG_CLK_MSG              SFlags7.bit.b2
#define FLAG_SND_MSGIPclk         SFlags7.bit.b3
#define FLAG_LUG_MSG              SFlags7.bit.b4
#define FLAG_SND_MSGIPlug         SFlags7.bit.b5
#define FLAG_TOUCH                SFlags7.bit.b6
#define FLAG_WITHFAVLED           SFlags7.bit.b7

/* Flags Msg2Go */
extern union word_def SMsg2Go;     /* Structure */
#define SMsg2GoValue	SMsg2Go.word /* all bits value */
#define FLAG_MSGACFM              SMsg2Go.bit.b0
#define FLAG_MSGAlrm              SMsg2Go.bit.b1
#define FLAG_MSGAmbi              SMsg2Go.bit.b2
#define FLAG_MSGBkp0              SMsg2Go.bit.b3
#define FLAG_MSGBkp1              SMsg2Go.bit.b4
#define FLAG_MSGBkp2              SMsg2Go.bit.b5
#define FLAG_MSGPreg              SMsg2Go.bit.b6
#define FLAG_MSGPclk              SMsg2Go.bit.b7
#define FLAG_MSGPlug              SMsg2Go.bit.b8
#define FLAG_S09                  SMsg2Go.bit.b9
#define FLAG_S10                  SMsg2Go.bit.b10
#define FLAG_S11                  SMsg2Go.bit.b11
#define FLAG_S12                  SMsg2Go.bit.b12
#define FLAG_S13                  SMsg2Go.bit.b13
#define FLAG_S14                  SMsg2Go.bit.b14
#define FLAG_MSGSX2W              SMsg2Go.bit.b15

/* Flags define */
#define IS_ACT_VAC              (FLAG_ACT_VAC == TRUE)
#define IS_ACT_ARR              (FLAG_ACT_ARR == TRUE)
#define FLAG_FAV_HGL_ACT        (IS_ACT_VAC || IS_ACT_ARR)
#define FLAG_FAV_HGL_END        (!FLAG_FAV_HGL_ACT)
#define FLAGd_UPDKO             ((IS_FAVNORM && FLAG_FAV_HGL_ACT)||FLAG_ACT_INFO||(ucSMMODE == SMMOD_TEST_RXTX))
#define V_LED4                  FLAG_DUMMY
#define V_LEDRADR               FLAG_DUMMY
#define V_LEDRADV               FLAG_DUMMY
#define FLAG_ACT_TIM5msRUN_BY_ALL0  (!(FLAG_ACT_TIM5msRUN_BY_KEYB)&&\
                                     !(FLAG_ACT_TIM5msRUN_BY_AUTO)&&\
                                     !(FLAG_ACT_TIM5msRUN_BY_RXLV)&&\
                                     !(FLAG_ACT_TIM5msRUN_BY_MATF)&&\
                                     !(FLAG_ACT_QSEC)&&\
                                     !(FLAG_ACT_TIM5msRUN_BY_MATC))
#define IS_FAVNORM              (FLAG_WITHFAVLED == FALSE)
#define IS_FAVLEDS              (FLAG_WITHFAVLED == TRUE)
#define IS_PARTLED              (ucIDPART == NID_Chaudiere_maitre_Leds)
#define IS_NEXTOVER_FAVNORM     (++ucSMMODE > SMMOD_PERM_AGEL)
#define IS_NEXTOVER_FAVLEDS     (++ucSMMODE > SMMOD_REGL_ALUG)
#define IS_NEXTOVER_FAVTYPE     ((IS_FAVNORM && IS_NEXTOVER_FAVNORM) || (IS_FAVLEDS && IS_NEXTOVER_FAVLEDS))
#define IS_THMZ1                (ucIDNODE == NID_Thermostat_LCD_Zone_1)
#define IS_THMZ2                (ucIDNODE == NID_Thermostat_LCD_Zone_2)
#define IS_RADIATEUR            (Par_PlancherRadiateur == 1)
#define IS_AMBI_NUL             (Par_Ambiance == 0)
#define IS_AMBI_OTH             (!IS_AMBI_NUL)
#define IS_TEXT_ABS             (g_Text == SANS_Text)
#define IS_REGULEXT             ((Par_TypeDeRegulation == REGULATION_AMBIANCE_ET_EXT)||(Par_TypeDeRegulation == REGULATION_EXT_SEULE))
#define MSG_RD false
#define MSG_WR true
#define IS_MSGCLK_RD            (FLAG_CLK_MSG == MSG_RD)
#define IS_MSGREG_RD            (FLAG_REG_MSG == MSG_RD)
#define IS_BUFF_EMPTY           ((BUF_LED_SREGUL0 == 0) && (BUF_LED_SREGUL1 == 0) && (BUF_LED_SREGUL2 == 0))
#define IS_BUFF_NOTEMPTY        (!IS_BUFF_EMPTY)
#define MSG_LUGEND false
#define MSG_LUGBEG true
#define IS_ACTLUG_END           (FLAG_LUG_MSG == MSG_LUGEND)
#define IS_ACTLUG_BEG           (!IS_ACTLUG_END)
#define IS_LUG_THM_OR_LED       (IS_FAVLEDS && (IS_ACTLUG_BEG || FLAG_FAV_HGL_ACT))
#define IS_ERR_HANDLER_RD       ((ulResetSoft != 0)&&(!FLAG_TOUCH))
/*
Exemple de define d'un bit pour la trace au scope à action double INT/EXT ald d'un bit INT seul.
#define FLAG_ACT_MATC             LED_2//SFlags5.bit.b1
permet l'affichage au scope de l'activité de la phase de matching.
*/


//#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
//    /* Select calibration mode single ended or differential ended */
//    MODIFY_REG(hadc->Instance->CR, ADC_CR_ADCALDIF, SingleDiff);
//    /* Start ADC calibration */
//    SET_BIT(hadc->Instance->CR, ADC_CR_ADCAL);

/*=== VARIABLES EXTERNES ==================================================*/
/* globales utilisées par tout le monde */
typedef struct {
  uint8_t Var_01;
  #define ucSMMODE              SRAM2EEP.Var_01
  uint8_t Var_02;
  #define ucIDKEYX              SRAM2EEP.Var_02
  uint8_t Var_03;
  #define ucIDLAN0              SRAM2EEP.Var_03
  uint8_t Var_04;
  #define ucIDLAN1              SRAM2EEP.Var_04
  uint8_t Var_05;
  #define ucIDLAN2              SRAM2EEP.Var_05
  uint8_t Var_06;
  #define ucIDLAN3              SRAM2EEP.Var_06
  uint8_t Var_07;
  #define ucIDNODE              SRAM2EEP.Var_07
  uint8_t Var_08;
  #define ucIDPART              SRAM2EEP.Var_08
  int8_t  Var_09;
  #define scOF7CTN              SRAM2EEP.Var_09
  int8_t  Var_10;
  #define scOF7PRO              SRAM2EEP.Var_10
  uint8_t Var_11;
  #define ucCSESOL              SRAM2EEP.Var_11
  uint8_t Var_12;
  #define ucCSELUN              SRAM2EEP.Var_12
  uint8_t Var_13;
  #define ucCSEGEL              SRAM2EEP.Var_13
  uint8_t Var_14;
  #define ucLVLCTR              SRAM2EEP.Var_14
  uint8_t Var_15;
  #define ucLUGDAY              SRAM2EEP.Var_15
  uint8_t Var_16;
  #define ucFREE02              SRAM2EEP.Var_16
  HH48SEG Var_d0;
  #define SHH_DAY0              SRAM2EEP.Var_d0
  HH48SEG Var_d1;
  #define SHH_DAY1              SRAM2EEP.Var_d1
  HH48SEG Var_d2;
  #define SHH_DAY2              SRAM2EEP.Var_d2
  HH48SEG Var_d3;
  #define SHH_DAY3              SRAM2EEP.Var_d3
  HH48SEG Var_d4;
  #define SHH_DAY4              SRAM2EEP.Var_d4
  HH48SEG Var_d5;
  #define SHH_DAY5              SRAM2EEP.Var_d5
  HH48SEG Var_d6;
  #define SHH_DAY6              SRAM2EEP.Var_d6
  uint8_t Var_59;
  #define ucFREE03              SRAM2EEP.Var_59
  uint8_t Var_60;
  #define ucFREE04              SRAM2EEP.Var_60
  uint8_t Var_61;
  #define ucFREE05              SRAM2EEP.Var_61
  uint8_t Var_62;
  #define ucFREE06              SRAM2EEP.Var_62
  uint8_t Var_63;
  #define ucFREE07              SRAM2EEP.Var_63
  uint8_t Var_64;
  #define ucFREE08              SRAM2EEP.Var_64
  REGULATOR_BLOCK Pregul;
  #define sbREGUL               SRAM2EEP.Pregul
}RAM2EEP;
extern RAM2EEP SRAM2EEP;

extern uint8 ucSMLCD,ucSMINF,ucSMRAD,ucSMCLK;
#define RAM_SHH_DAY7A_INIT   ((uint8_t)0xF0) /* SHH_DAYxx init */
#define RAM_SHH_DAY7B_INIT   ((uint8_t)0x0F) /* SHH_DAYxx init */
extern HH48SEG SHH_DAY7;
extern uint16 uiVar_Check;
extern uint8  ucTIM01sMST_UPDT;
extern uint16 uiTIM01sLIM_MODE;

extern uint8 ucTIM01sACT_INFO;
extern uint16 uiTIM01sRST_SAMPLE;
extern uint16 uiTIM5msRUN;

extern int16 g_Text; /* Temperature exterieure initialisée à SANS_Text */
extern int16 g_Tamb; /* Thermostat_LCD_Zone_1_Ambiance */
extern int16 g_Tcse; /* Thermostat_LCD_Zone_1_Consigne */
extern int16 g_Toto; /* Thermostat_LCD_Zone_1_Consigne */
extern int16 g_Temp; /* Thermostat_LCD_Zone_1_Consigne */
extern uint16 g_Batt;

extern uint8 ucProg_Curs;
extern uint8 Rf_Level_Quality; /* niveau de réception radio de la face avant */
extern uint8 Langue;  /* attention variable à peut-etre sauvegarder en virtual eeprom */
extern uint16 uiCDZMEMAVRED;

/* fonctions globales utilisable par tout le monde */
extern uint8 Bcd2Dec(uint8 bcd);
extern uint8 Dec2Bcd(uint8 dec);

#endif /* __DEFS_H */