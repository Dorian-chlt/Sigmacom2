/*---------------------------------------------------------------------------------------------------------
                   A.THEOBALD SA CONTROLE ET REGULATION
-----------------------------------------------------------------------------------------------------------

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

 Projet:  THM 947.
 Fichier:  layer3.h
 Description: Fichier de definition du layer 3.
 Auteur:      Beno�t MARQUANT
 Version:     XX-00
 CPU: R8C24 / 20 MHz
 Modifications:
 02/11/09: Creation.
 12/07/10: NA : Mise � jour pour le THM.
-----------------------------------------------------------------------------------------------------------
                Propriete de A. THEOBALD SA
         Ne peut etre copie ou diffuse sans son accord prealable
---------------------------------------------------------------------------------------------------------*/
/*=== INCLUDE FILES =====================================================================================*/
/*=== DEFINITIONS GLOBALES ==============================================================================*/
/* Read communs */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>   // Pour printf
#include <string.h> 
#include "ERS2_Messages.h"
typedef struct  {
  uint16_t Reg_30001;   /* Donnee tracker #1 */
  uint16_t Reg_30002;
  uint16_t Reg_30003;
  uint16_t Reg_30004;
  uint16_t Reg_30005;
  uint16_t Reg_30006;
  uint16_t Reg_30007;
  uint16_t Reg_30008;
  uint16_t Reg_30009;
  uint16_t Reg_30010;
  uint16_t Reg_30011;
  uint16_t Reg_30012;
  uint16_t Reg_30013;
  uint16_t Reg_30014;
  uint16_t Reg_30015;
  uint16_t Reg_30016;   /* Donnee tracker #16 */
  uint16_t Reg_30017;   /* Type carte / CDC */
  uint16_t Reg_30018;   /* Release / Heure compil */
  uint16_t Reg_30019;   /* Minutes / secondes */
  uint16_t Reg_30020;   /* Libre / RSSI */
} REGR_COM;

/* Read write communs */
typedef struct  {
  uint16_t Reg_40000;   /* Dummy */
  uint16_t Reg_40001;   /* Adresse tracker #1 */
  uint16_t Reg_40002;
  uint16_t Reg_40003;
  uint16_t Reg_40004;
  uint16_t Reg_40005;
  uint16_t Reg_40006;
  uint16_t Reg_40007;
  uint16_t Reg_40008;
  uint16_t Reg_40009;
  uint16_t Reg_40010;
  uint16_t Reg_40011;
  uint16_t Reg_40012;
  uint16_t Reg_40013;
  uint16_t Reg_40014;
  uint16_t Reg_40015;
  uint16_t Reg_40016;   /* Adresse tracker #16 */
  uint16_t Reg_40017;   /* Date/heure test */
  uint16_t Reg_40018;
  uint16_t Reg_40019;
  uint16_t Reg_40020;   /* RTC [Annee][mois] */
  uint16_t Reg_40021;   /* RTC [Jour][heure] */
  uint16_t Reg_40022;   /* RTC [Minutes][secondes] */
  uint16_t Reg_40023;   /* Flags_alarme	/ RTC    [][Jour semaine] */
  uint16_t Reg_40024;   /* NetId  [MSB] */
  uint16_t Reg_40025;   /* NetId  [LSB] */
  uint16_t Reg_40026;   /*        [   ][NodeId] */
} REGRW_COM;

/* RW particuliers de THM */
typedef struct{
  uint16_t Reg_41501; /* THM DEBUT */
  uint16_t Reg_41502[72]; /* THM BACKUP */
} REGRW_THM;

/* RW particuliers de REGRW_SAT_LED */
typedef struct{
  uint16_t Reg_41108;
  uint16_t Reg_41109;
  uint16_t Reg_41110;
  uint16_t Reg_41111;
  union{
    struct{
      uint8_t b0:1;     /* 0 */
      uint8_t b1:1;     /* 1 */
      uint8_t b2:1;     /* 2 */
      uint8_t b3:1;     /* 3 */
      uint8_t b4:1;     /* 4 */
      uint8_t b5:1;     /* 5 */
      uint8_t b6:1;     /* 6 */
      uint8_t b7:1;     /* 7 */
      uint8_t b8:1;     /* 8 */
      uint8_t b9:1;     /* 9 */
      uint8_t b10:1;    /* 10 */
      uint8_t b11:1;    /* 11 */
      uint8_t b12:1;    /* 12 */
      uint8_t b13:1;    /* 13 */
      uint8_t b14:1;    /* 14 */
      uint8_t b15:1;    /* 15 */
    }bit;
    uint16_t word;
  }Reg_41112;
  uint16_t Reg_41113;
  uint16_t Reg_41114;
  uint16_t Reg_41115;
} REGRW_SAT_LED;

#define ADR_DEBUT_SAT_FAVLED    SRegistresRW_SAT_LED.Reg_41108  /*FAV DEBUT_SAT_FAVLED*/
#define BUF_LED_MISE_A_L_HEURE1 SRegistresRW_SAT_LED.Reg_41108
#define BUF_LED_MISE_A_L_HEURE2 SRegistresRW_SAT_LED.Reg_41109
#define BUF_LED_MISE_A_L_HEURE3 SRegistresRW_SAT_LED.Reg_41110
#define BUF_LED_MISE_A_L_HEURE4 SRegistresRW_SAT_LED.Reg_41111
#define BUF_LED_HEURE_AUTO      SRegistresRW_SAT_LED.Reg_41112.word
#define BUF_LED_FLAG_DST        SRegistresRW_SAT_LED.Reg_41112.bit.b0
#define BUF_LED_SREGUL0         SRegistresRW_SAT_LED.Reg_41113
#define BUF_LED_SREGUL1         SRegistresRW_SAT_LED.Reg_41114
#define BUF_LED_SREGUL2         SRegistresRW_SAT_LED.Reg_41115
#define BUF_LED_sCLCK           BUF_LED_MISE_A_L_HEURE1
#define BUF_LED_sbREGUL         BUF_LED_SREGUL0

/* RW particuliers de REGRW_SGC_LED */
typedef struct{
  uint16_t Reg_41200;
  uint16_t Reg_41201;
  uint16_t Reg_41202;
  uint16_t Reg_41203;
  uint16_t Reg_41204;
  uint16_t Reg_41205;
  uint16_t Reg_41206;
  uint16_t Reg_41207;
} REGRW_SGC_LED;

#define BUF_LED_DEBUT_SIGCOM       SRegistresRW_SGC_LED.Reg_41200        /*FAV DEBUT_SIGCOM*/
#define BUF_LED_DATE_DEP_VACANCES1       SRegistresRW_SGC_LED.Reg_41200        /*FAV DATE_DEP_VACANCES1*/
#define BUF_LED_DATE_DEP_VACANCES2       SRegistresRW_SGC_LED.Reg_41201        /*FAV DATE_DEP_VACANCES2*/
#define BUF_LED_DATE_DEP_VACANCES3       SRegistresRW_SGC_LED.Reg_41202        /*FAV DATE_DEP_VACANCES3*/
#define BUF_LED_DATE_DEP_VACANCES4       SRegistresRW_SGC_LED.Reg_41203        /*FAV DATE_DEP_VACANCES4*/
#define BUF_LED_DATE_RET_VACANCES1       SRegistresRW_SGC_LED.Reg_41204        /*FAV DATE_RET_VACANCES1*/
#define BUF_LED_DATE_RET_VACANCES2       SRegistresRW_SGC_LED.Reg_41205        /*FAV DATE_RET_VACANCES2*/
#define BUF_LED_DATE_RET_VACANCES3       SRegistresRW_SGC_LED.Reg_41206        /*FAV DATE_RET_VACANCES3*/
#define BUF_LED_DATE_RET_VACANCES4       SRegistresRW_SGC_LED.Reg_41207        /*FAV DATE_RET_VACANCES4*/

/* RW particuliers de REGRW_SAT_SGC */
typedef struct{
  uint8_t Reg_41300[48]; /* FAV SAT_SIGCOM */
} REGRW_SAT_SGC;

#define ADR_DEBUT_SAT_SIGCOM    SRegistresRW_SAT_SGC.Reg_41300[0]  /*FAV DEBUT_SAT_SIGCOM*/
#define BUF_WEB_ucCSESOL        SRegistresRW_SAT_SGC.Reg_41300[0]  /* RRW_FAV_SAT1_Camb_C */
#define BUF_WEB_ucCSELUN        SRegistresRW_SAT_SGC.Reg_41300[1]  /* RRW_FAV_SAT1_Camb_R */
#define BUF_WEB_ucCSEGEL        SRegistresRW_SAT_SGC.Reg_41300[2]  /* RRW_FAV_SAT1_Camb_HG */
#define BUF_WEB_ucSMMODE        SRegistresRW_SAT_SGC.Reg_41300[3]  /* RRW_FAV_SAT1_SMMODE */
#define BUF_WEB_SFlagsWValueLSB SRegistresRW_SAT_SGC.Reg_41300[4]  /* RRW_FAV_SAT1_FlagsWLSB */
#define BUF_WEB_SFlagsWValueMSB SRegistresRW_SAT_SGC.Reg_41300[5]  /* RRW_FAV_SAT1_FlagsWMSB */
#define BUF_WEB_SHH_DAY0        SRegistresRW_SAT_SGC.Reg_41300[6]  /* RRW_FAV_SAT1_SHH_DAY0 */

/* offset d'adressage RRW dans la FAV pour l'acc�s individuel des THM_Zx */
#define OF7_THM_Zx 5   

/* RW particuliers de FAV */
typedef struct{          
  uint16_t Reg_41XX[OF7_THM_Zx]; 
} REGRW_FAV;

#define BUF_FAV_Tamb            SRegistresRW_FAV.Reg_41XX[0]  /* RRW_FAV_Zone_x_Ambiance */
#define BUF_FAV_Tcse            SRegistresRW_FAV.Reg_41XX[1]  /* RRW_FAV_Zone_x_Consigne */
#define BUF_FAV_Flgs            SRegistresRW_FAV.Reg_41XX[2]  /* RRW_FAV_Zone_x_Flags */
#define BUF_FAV_CdzW            SRegistresRW_FAV.Reg_41XX[3]  /* RRW_FAV_Zone_x_Commande_rst_write */
#define BUF_FAV_CdzR            SRegistresRW_FAV.Reg_41XX[4]  /* RRW_FAV_Zone_x_Commande_rst_read */

/* Nombre de caract�res affichables par ligne sur la FAV */
#define MAX_NBR_CHAR              26

/* Ordre et appelation des langues */
typedef enum
{
  FRA,
  ITA,
  ENG,
  ESP,
  RUS,
  CHN
}Langue_Enum;

#define LANGUE_LAST CHN   /* definit la dimension maximum des tables */

//#define DEF_LANGUE FRA  /* pour la map avec une seule langue impos�e */

/*====================================== EXTERNES =========================================================*/
void MODBUS_sendAl_req_pdu(uint8_t NumeroAlarme, uint8_t Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq);
void MODBUS_mb_req_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq);   /* traitement buffer d'entree */
void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp);
void MODBUS_sendrw_req_pdu(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, uint8_t W_Byte_Count, uint16_t* Datas, APU_SX1211* pModbusReq);
void MODBUS_sendrw_req_pd2(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, uint8_t W_Byte_Count, uint8_t*  Datas, APU_SX1211* pModbusReq);
void MODBUS_sendr_req_pdu(uint16_t StartAdr_R, uint16_t Qty_R, APU_SX1211* pModbusReq);
void MODBUS_Set_Registers(void);

extern REGRW_COM SRegistresRW_COM; /* Registres communs */
extern REGR_COM  SRegistresR_COM;  /* Registres communs */
extern REGRW_FAV SRegistresRW_FAV; /* Registres read/write sp�cifiques FAV */
extern REGRW_THM SRegistresRW_THM; /* Registres read/write sp�cifiques THM */
extern REGRW_SAT_SGC SRegistresRW_SAT_SGC; /* Registres read/write sp�cifiques RW_SAT_SGC */
extern REGRW_SAT_LED SRegistresRW_SAT_LED; /* Registres read/write sp�cifiques RW_SAT_LED */
extern REGRW_SGC_LED SRegistresRW_SGC_LED; /* Registres read/write sp�cifiques RW_SGC_LED */
