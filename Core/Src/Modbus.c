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

 Projet:  Face avant a LCD 939.
 Fichier:  layer3.c
 Description: Protocole de communication radio niveau 3 (Paquets)
 Auteur:      Jean Luc DUPUIS
 Version:     00
 CPU: R8C24 @ 20MHz interne
 Compilateur IAR ICCM16C
 Modifications:
 12/11/08: Creation.
-----------------------------------------------------------------------------------------------------------
                                     Propriete de A. THEOBALD SA
                          Ne peut etre copie ou diffuse sans son accord prealable
---------------------------------------------------------------------------------------------------------*/

/*=== INCLUDE FILES =====================================================================================*/
#include "Modbus.h"
#include "EEPROM.h"
#include "sx1211.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>   // Pour printf
#include <string.h>

union word_def SFlags0; /* FLAG */
union byte_def SFlags1; /* FLAG */
union byte_def SFlags2; /* FLAG */
union byte_def SFlags3; /* FLAG */
union byte_def SFlags4; /* FLAG */
union byte_def SFlags5; /* FLAG */
union byte_def SFlags6; /* FLAG */
union byte_def SFlags7; /* FLAG */
union word_def SMsg2Go; /* FLAG */
uint8_t ucTIM01sWEB_UPDT,ucTIM1TSWEBMODIF;
uint16_t uiTIM5msRUN;
 // Remplace par la version correspondant à ton MCU

extern RAM2EEP SRAM2EEP;
/*=== DEFINITIONS GLOBALES ==============================================================================*/
/* Adresse Zone read/write specifique SIGMA */
#define FIRST_ADRESS_RRW_SPEC     RRW_SAT1_SC_DEBUT
#define LAST_ADRESS_RRW_SPEC      RRW_SAT1_SC_FIN
#define FLAG_FAV_HGL_ACT        (IS_ACT_VAC || IS_ACT_ARR)
#define DEC2BCD(dec) (((dec / 10) << 4) + (dec % 10))
uint8_t Dec2Bcd(uint8_t dec){
return (DEC2BCD(dec));
}
//#define _NO_MATCH_ERR_ /* Avec anti-erreur de matching sur 5B ald 4 */
/*=== PROTOTYPES DES FONCTIONS ==========================================================================*/

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef   sTime;
extern RTC_DateTypeDef   sDate;


void RF_Keep_RXLV(void); /* Conditions de maintien en RXLV */
void MODBUS_Set_Registers(void);
extern uint16_t uifnMEMFAV_CS(void);

void RF_WEB2THM(void);
void UPDT_SYS2BUF(void);



enum _L3_TX_STATUS_ {
  L3_TX_ACKNOWLEDGED,    /* ACK du distant le message a bien été compris */
  L3_TX_NACK             /* NACK du distant le message envoyé est erroné */
};

/*=== VARIABLES EXTERNES ================================================================================*/
extern uint8_t L3_Status;
extern uint8_t ucTimerTX2TX;
extern uint8_t ucMEMFAV_SA;
extern uint8_t ucMEMFAV_SM;
extern uint8_t ucMEMFAV_SJ;
extern uint16_t uiMEMFAV_CS;
/*=== VARIABLES GLOBALES ================================================================================*/
uint8_t Langue;  /* attention variable � peut-etre sauvegarder en virtual eeprom */
uint8_t Rf_Level_Quality; /* niveau de r�ception radio de la face avant */
REGRW_FAV SRegistresRW_FAV; /* Registres read/write sp�cifiques FAV */
REGRW_COM SRegistresRW_COM; /* Registres communs */
REGR_COM  SRegistresR_COM;  /* Registres communs */
REGRW_THM SRegistresRW_THM; /* Registres read/write sp�cifiques THM */
REGRW_SAT_SGC SRegistresRW_SAT_SGC; /* Registres read/write sp�cifiques RW_SAT_SGC */
REGRW_SAT_LED SRegistresRW_SAT_LED; /* Registres read/write sp�cifiques RW_SAT_LED */
REGRW_SGC_LED SRegistresRW_SGC_LED; /* Registres read/write sp�cifiques RW_SGC_LED */
/*=== FONCTIONS =========================================================================================*/
uint8_t FILT_MODE(uint8_t value){
  switch(value)
  {
    default:
      value = SMMOD_PERM_AUTO;
    break;

    case SMMOD_PERM_ASOL:
      value = SMMOD_PERM_ASOL;
    break;

    case SMMOD_PERM_ALUN:
      value = SMMOD_PERM_ALUN;
    break;

    case SMMOD_PERM_AGEL:
      value = SMMOD_PERM_AGEL;
    break;
  }
  return value;
}/* end of uint8 FILT_MODE(uint8 value) */

void RF_Keep_RXLV (void)
{
  FLAG_ACT_TIM5msRUN_BY_KEYB = false;
  FLAG_ACT_TIM5msRUN_BY_RXLV = true;
  uiTIM5msRUN = TIMEOUT_TIM5msRUN_ACT_BY_RXLV;
}

void UPDT_SYS2BUF(void){

  BUF_WEB_ucCSESOL        = ucCSESOL;                     /* Maj FAV SAT1_Camb_C */  
  BUF_WEB_ucCSELUN        = ucCSELUN;                     /* Maj FAV SAT1_Camb_R */  
  BUF_WEB_ucCSEGEL        = ucCSEGEL;                     /* Maj FAV SAT1_Camb_HG */
  BUF_WEB_ucSMMODE        = FILT_MODE(ucSMMODE);          /* Maj FAV SAT1_SMMODE */  
  BUF_WEB_SFlagsWValueLSB = (uint8_t)(SFlags0Value     ); /* Maj des Flags LSB */  
  BUF_WEB_SFlagsWValueMSB = (uint8_t)(SFlags0Value >> 8); /* Maj des Flags MSB */  
  memcpy(&BUF_WEB_SHH_DAY0,&SHH_DAY0.RAM[0],42);            /* Maj des 7 plages horaires */
}

void UPDT_BUF2SYS(void){
  
  ucCSESOL      = BUF_WEB_ucCSESOL;   /* Maj FAV SAT1_Camb_C */  
  ucCSELUN      = BUF_WEB_ucCSELUN;   /* Maj FAV SAT1_Camb_R */  
  ucCSEGEL      = BUF_WEB_ucCSEGEL;   /* Maj FAV SAT1_Camb_HG */
  ucSMMODE      = FILT_MODE(BUF_WEB_ucSMMODE);   /* Maj FAV SAT1_SMMODE */  
  SFlags0Value  = ((uint16_t)BUF_WEB_SFlagsWValueMSB << 8) + ((uint16_t)BUF_WEB_SFlagsWValueLSB); /* Maj des Flags MSB & LSB */ 
  memcpy(&SHH_DAY0.RAM[0],&BUF_WEB_SHH_DAY0,42);            /* Maj des 7 plages horaires WtoSx */

}

void RF_WEB2THM(void){

  UPDT_BUF2SYS(); /* Maj des données du buffer d'échange vers le systeme */
  FLAG_UPDT_WEB2THM = true;
  FLAG_REFRESH_EEP = true;
  FLAG_WEB_MODIF = true;
  ucTIM1TSWEBMODIF = TIMEOUT_TIM1TSWEBMODIF;
}
/**********************************************************************************************************
* NOM:uint16_t uifnMEMFAV_CS(void)
*
* DESCRIPTION      : Calcul de la somme de la zone de RAM.
* PARAMETRES       :
* VALEUR RETOURNEE : la somme.
* MAINTENANCE      :
* 06/06/2013  NA: Creation
*********************************************************************************************************/
uint16_t uifnMEMFAV_CS(void)
{
  uint8_t  i;
  uint16_t j=0;
  for( i = 0; i < 72; i++ ){
    j=j+SRegistresRW_THM.Reg_41502[i]; /* compute sum for all i values */
  }
  return j;
}
/**********************************************************************************************************
* NOM:void MODBUS_mb_req_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
*
* DESCRIPTION      :Traitement d'une requete ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
* 23/02/2010  NA: Opt de code, Suppression des case non utilis�s.
* 25/10/2011 BM/NA: Ajout gestion de IDKEYX.
* 25/10/2011  NA: Supp des 3 ecritures successives en NVM lors du Matching.
* 15/11/2011  NA: Supp de la gestion de r�ponse au matching.
* 06/03/2012  NA: Ajout demande de reset.
* 09/10/2012 BM/NA: Ajout verif. sur SET_NETID que le periph. souhaite par la FAV correspond au NodeId courant.
* 09/02/2016 BM/NA: Ajout RD_WR_MULTIPLE_REG en version light (zone dediee only).
* 18/05/2016 JLD/NA: Ajout version du THM transmise � la FAV sur l'ack du matching.
*********************************************************************************************************/
void MODBUS_mb_req_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
{
  uint16_t q;
  uint16_t first_address, last_address,starting_address, quantity_read, quantity_write;
  uint8_t taille = 0,byte_count;
  uint16_t* p;
  /* On suit le document V1.1 du protocole  */
  /* Fonction code supported ? */
  switch(pModbusReq->Modbus.FunctionCode)
  {
  case RD_WR_MULTIPLE_REG:
    /* This function code performs a combination of one read operation and one write operation in a single MODBUS transaction.
    The write operation is performed before the read.
    Holding registers are addressed starting at zero. Therefore holding registers 1-16 are addressed in the PDU as 0-15.
    The request specifies the starting address and number of holding registers to be read as well as the starting address,
    number of holding registers, and the data to be written. The byte count specifies the number of bytes to follow in the write data
    field. The normal response contains the data from the group of registers that were read. The byte count field specifies the
    quantity of bytes to follow in the read data field. */
    quantity_read = (uint16_t)pModbusReq->Modbus.Datas[3];
    quantity_write = (uint16_t)pModbusReq->Modbus.Datas[7];
    byte_count = pModbusReq->Modbus.Datas[8];
    if(quantity_read >= 1 && quantity_read <= 28 && quantity_write >= 1 && quantity_write <= 25 &&(byte_count >> 1) == (uint8_t)quantity_write)
    { /* Donnees valides, commence par le write */
      starting_address = ((uint16_t)pModbusReq->Modbus.Datas[4] << 8) + (uint16_t)pModbusReq->Modbus.Datas[5];
      if(starting_address <= RRW_COM_FIN)
      { /* Zone read/write commune */
        first_address = RRW_Dummy;
        last_address = RRW_COM_FIN;
        p = (uint16_t*)&SRegistresRW_COM.Reg_40000 + starting_address - first_address;
        UPDT_SYS2BUF();/* Maj des donn�es du systeme vers le buffer d'�change.*/
      }
      else
      { /* Zone read/write specifique SAT_SGC */
        first_address = FIRST_ADRESS_RRW_SPEC;
        last_address = LAST_ADRESS_RRW_SPEC;
        p = (uint16_t*)&SRegistresRW_SAT_SGC.Reg_41300 + starting_address - first_address;
      }
      if(starting_address >= first_address && (starting_address + quantity_write - 1) <= last_address )
      { /* Valide, ecrit les donnees dans la structure */
        memcpy(p,&pModbusReq->Modbus.Datas[9],quantity_write << 1);
        RF_WEB2THM();
        /* Lecture */
        starting_address = ((uint16_t)pModbusReq->Modbus.Datas[0] << 8) + (uint16_t)pModbusReq->Modbus.Datas[1];
        if(starting_address >= RRW_SAT1_SC_DEBUT && starting_address <= RRW_SAT1_SC_FIN)
        { /* Zone read/write specifique SAT_SGC */
          first_address = FIRST_ADRESS_RRW_SPEC;
          last_address = LAST_ADRESS_RRW_SPEC;
          p = (uint16_t*)&SRegistresRW_SAT_SGC.Reg_41300 + starting_address - first_address;
        }
        if(starting_address >= first_address && (starting_address + quantity_read - 1) <= last_address )
        { /* Valide, lit les donnees dans la structure */
          pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode;
          pModbusRsp->Modbus.Datas[0] = quantity_read << 1;
          memcpy(&pModbusRsp->Modbus.Datas[1],p,pModbusRsp->Modbus.Datas[0]);
          pModbusRsp->Taille = pModbusRsp->Modbus.Datas[0] + 1;
        }
        else
        { /* Erreur 0x02 */
          pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
          pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_ADDRESS;  /* 0x02 */
          pModbusRsp->Taille = taille;
        }
      }
      else
      { /* Erreur 0x02 */
        pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
        pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_ADDRESS;  /* 0x02 */
        pModbusRsp->Taille = taille;
      }
    }
    else
    { /* Erreur 0x03 */
      pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
      pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_VALUE;  /* 0x03 */
      pModbusRsp->Taille = taille;
    }
    break;
    
  case SET_NETID:

      /* Ecrit le NETID recu et le PARTID en eeprom */
      byte_count = pModbusReq->Modbus.Datas[0];
      if(byte_count == 0x04)
      {
        ucIDPART = pModbusReq->Exp; /* maj de la variable courante en RAM avant la sauvegarde en NVM. */
        ucIDKEYX = pModbusReq->Cle; /* maj de la variable courante en RAM avant la sauvegarde en NVM. */
        memcpy(&ucIDLAN0,&pModbusReq->Modbus.Datas[1],byte_count); /* maj du nouveau netid en RAM avant la sauvegarde en NVM */
        EEP_Update(); /* Update Ram contents to virtual EEPROM */
        /* On acquitte */
        /* [ACK] */
        pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode;

        /* On propose un backup � la FAV si disponible */
        if((uifnMEMFAV_CS() + uiMEMFAV_CS) == 0xFFFF){
          /* [3][AA][MM][JJ] */
          pModbusRsp->Modbus.Datas[taille++] = 3;
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SA);
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SM);
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SJ);
          FLAG_ACT_TIM5msRUN_BY_MATF = true;
          uiTIM5msRUN = TIMEOUT_TIM5msRUN_ACT_BY_MATF;
        }
        else{
          FLAG_ACT_TIM5msRUN_BY_MATF = true;
          uiTIM5msRUN = TIMEOUT_TIM5msRUN_ACT_BY_MATC;
        }
        /* On transmet les infos de la version du THM */
        /* [1][_TYPE_][_CDC_][_RELEASE_] */
        pModbusRsp->Modbus.Datas[taille++] = 1; /* <== nouveau type d'info ald du nbre de bytes */
        pModbusRsp->Modbus.Datas[taille++] = _TYPE_;
        pModbusRsp->Modbus.Datas[taille++] = _CDC_;
        pModbusRsp->Modbus.Datas[taille++] = _RELEASE_;
        pModbusRsp->Taille = taille;
       }
      else 
      { /* Erreur 0x02 */
        pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
        pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_ADDRESS;
        pModbusRsp->Taille = taille;
      }
    break;
    
  case RF_RX_LEVEL:
/* +---------------------------------------------  taille fifo SX1211 = 64octets  --------------------------------------------------+
   |               |             |            |             |                |                 |                                    |
   + 1octet taille + 1octet dest + 1octet exp + 1octet mess + 1octet Adresse + 1octet funccode +        58octets data               +
*/
    byte_count = pModbusReq->Modbus.Datas[0];
    if(byte_count != 0x00)
    { /* donn�es pr�sentes */
      RF_Keep_RXLV(); /* Conditions de maintien en RXLV */
      Rf_Level_Quality = pModbusReq->Modbus.Datas[1];; /* qualit� de r�ception de la face avant : de 00 � 05 */
      SRegistresR_COM.Reg_30020 = Rf_Level_Quality; /* niveau de r�ception radio de la face avant */
      /* on acquitte avec une trame d'au moins 20ms pour permettre � la face avant de mesurer son niveau de r�ception radio */
      pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode;
      pModbusRsp->Adresse = pModbusReq->Adresse;
      /* On remplie les 58 octets de donn�es */
      pModbusRsp->Modbus.Datas[0] = SX1211_FIFO_SIZE-SIZEOF_HEADER-2; /* nombre d'octets de donn�es 56 */
      q = RF_GetRSSI();
      memset(&pModbusRsp->Modbus.Datas[1],q,(SX1211_FIFO_SIZE-SIZEOF_HEADER-2)); /* remplie les 56 octets de donn�es avec mon niveau de r�ception radio */
      pModbusRsp->Taille = SX1211_FIFO_SIZE-SIZEOF_HEADER-1; /* 57 */
    }
    else
    { /* Erreur 0x01 */
      pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
      pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_FUNCTION;
      pModbusRsp->Taille = taille;
    }
    break;

  case READ_HOLDING_REG:
  /*                          0                  1             2              3
       >>> [FunctionCode][Adresse debut HI][Adresse debut LO][Reg count HI][Reg count LO]
       <<< [FunctionCode][Nbre octets][Datas..]       
  */
    first_address= (uint16_t)(pModbusReq->Modbus.Datas[0] << 8)+(uint16_t)(pModbusReq->Modbus.Datas[1]);
    quantity_read= (uint16_t)pModbusReq->Modbus.Datas[3];
    byte_count = (uint8_t)(quantity_read << 1);
    if(quantity_read == 0 || quantity_read > 24){
     pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
     pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_VALUE;  
     pModbusRsp->Taille = taille;        
    }
    else if(first_address >= RRW_THM_DEBUT &&
      (first_address + quantity_read)<=(RRW_THM_FIN+1)){
      pModbusRsp->Modbus.Datas[0]= byte_count;
      /* sizeof(char)*byte_count genere un warning multiplication by 1 */
      memcpy(&pModbusRsp->Modbus.Datas[1],&SRegistresRW_THM.Reg_41501+(first_address - RRW_THM_DEBUT),sizeof(char)*byte_count);
      pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode;
      pModbusRsp->Taille = byte_count+1;
      if(first_address == RRW_THM_Magic)FLAG_READEND = true;
      if(first_address == RRW_THM_Backup_48)FLAG_READCHK = true;
    }
    else{
     pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
     pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_DATA_ADDRESS;  
     pModbusRsp->Taille = taille;
    }
    break;

  default:
    /* Pas supporte */
    pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode + 0x80;
    pModbusRsp->Modbus.Datas[taille++] = ILLEGAL_FUNCTION;
    pModbusRsp->Taille = taille;
    break;
  }
} /* void MODBUS_mb_req_pdu(ADU_TRAME* pModbusRsp,ADU_TRAME* pModbusReq) */

/**********************************************************************************************************
* NOM:void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp)
*
* DESCRIPTION      :Traitement de la trame de r�ponse � une requ�te ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
* 12/07/2010 BM/NA: Modification il faut diviser par 2 la valeur de byte_count.
* 09/11/2011  NA: Mise � jour de la variable de reception des Flags de la FAV.
* 16/11/2011  NA: Ajout pour occulter le FLAG_ALREP du msg pas de sat. initialis�.
* 11/01/2012  NA: Ajout L3_TX_ACKNOWLEDGED sur case ALARM manquant.
* 21/02/2012  NA: Ajout RESTART_CALENDAR_COUNTER, pour une raz complete de la quantification de la seconde.
* 26/06/2012  NA: Ajout DEF_LANGUE, pour la map avec une seule langue impos�e.
* 15/02/2016  NA: Ajout filtrage avant maj des variables et supp du Baro.
* 18/05/2016  NA: Ajout langue ALL et controle avec par defaut FRA si inconnue.
* 16/02/2018  NA: Modif maj de la RTC hard ald soft.
* 22/02/2018  NA: Ajout com vers FAV_LED.
*********************************************************************************************************/
void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp)
{
#ifndef DEF_LANGUE
  uint8_t  Langue_Check;
#endif
  uint8_t  decalage = (THMZX * OF7_THM_Zx)*2;
  /* On suit le document V1.1 du protocole  */
  /* Fonction code supported ? */
  switch(pModbusRsp->Modbus.FunctionCode)
  {
    case RD_WR_MULTIPLE_REG:
        if(FLAG_MSGAmbi||(!FLAG_MSGAlrm && FLAG_MSGACFM)){/* pas de mise � jour des valeurs sinon */
          g_Text = ((int16_t)pModbusRsp->Modbus.Datas[1] << 8) + ((int16_t)pModbusRsp->Modbus.Datas[2]);
          /* if the wakeup timer is enabled then deactivate it to disable the wakeup timer interrupt */
          HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
          /*!< Les donnees recues sont en bcd */
          sDate.Year    = Bcd2Dec(pModbusRsp->Modbus.Datas[5]);
          sDate.Month   = Bcd2Dec(pModbusRsp->Modbus.Datas[6]);
          sDate.Date    = Bcd2Dec(pModbusRsp->Modbus.Datas[7]);
          sDate.WeekDay = Bcd2Dec(pModbusRsp->Modbus.Datas[12] & 0x0F); // maj de Jour (quartet bas de l'octet)
//          HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
          sTime.Hours   = Bcd2Dec(pModbusRsp->Modbus.Datas[8]);
          sTime.Minutes = Bcd2Dec(pModbusRsp->Modbus.Datas[9]);
          sTime.Seconds = Bcd2Dec(pModbusRsp->Modbus.Datas[10]);
//          HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
          /* Enable the WakeUp */
          HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);

          SFlags1Value = ((uint8_t)pModbusRsp->Modbus.Datas[11]);
          if(FLAG_FAV_HGL_ACT){
            FLAG_LUG_MSG = MSG_LUGBEG; /* la fav confirme que des vacances sont en cours */
          }
          else{
            FLAG_LUG_MSG = MSG_LUGEND; /* la fav confirme que les vacances sont finies */
          }
/*----------------------------------------------------------------------------*/
#ifdef  WO_ACT_WEB
/*----------------------------------------------------------------------------*/
          FLAG_ACT_WEB = false; /* pas de Connect pdt la map !!! */
/*----------------------------------------------------------------------------*/
#endif/*WO_ACT_WEB*/
          
          if(FLAG_NOALREP){
            FLAG_NOALREP = false;
            FLAG_ALREP = false;
            FLAG_ACT_WEB = false;
          }

  #ifdef DEF_LANGUE
          Langue = DEF_LANGUE;  /* pour la map avec une seule langue impos�e */
  #else
          /* Controle de la langue retournee par la FAV */
          Langue_Check = pModbusRsp->Modbus.Datas[12] >> 4; // maj de Langue (quartet haut de l'octet)
          if(Langue_Check > LANGUE_LAST)
          {
            /* langue inconnue, on reste sur la derniere langue courante */
          }
          else
          {
            Langue = Langue_Check; /* langue reconnue prend cette langue */
          }
  #endif

          /* 091012 ATTENTION decalage sera ok jusque Z4 il faudra revoir le process si > Z4 */
          BUF_FAV_CdzR=((int16_t)pModbusRsp->Modbus.Datas[21 + decalage] << 8) + ((int16_t)pModbusRsp->Modbus.Datas[22 + decalage]);
        }
        else if(FLAG_MSGPreg){/* pas de mise � jour des valeurs sinon */
          BUF_LED_SREGUL0 = (((uint16_t)pModbusRsp->Modbus.Datas[1] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[2]));
          BUF_LED_SREGUL1 = (((uint16_t)pModbusRsp->Modbus.Datas[3] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[4]));
          BUF_LED_SREGUL2 = (((uint16_t)pModbusRsp->Modbus.Datas[5] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[6]));
          if(IS_BUFF_NOTEMPTY){/* si la FAV a initialis� le buffer d'echange */
            memcpy(&sbREGUL,&BUF_LED_sbREGUL,sizeof(REGULATOR_BLOCK)); /* Buffer -> System */
            FLAG_REFRESH_EEP=true;
          }
        }
        else if(FLAG_MSGPclk){/* pas de mise � jour des valeurs sinon */
          if(IS_MSGCLK_RD){/* maj seulement sur la reponse de la demande de lecture */
            BUF_LED_HEURE_AUTO = (((uint16_t)pModbusRsp->Modbus.Datas[1] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[2]));
          }
        }
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK re�u */
      break;
    
    case ALARM:
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK re�u */
      break;
    
    case READ_HOLDING_REG:
        if     (FLAG_MSGBkp0)memcpy(&SRegistresRW_THM.Reg_41502[0],&pModbusRsp->Modbus.Datas[1],48);
        else if(FLAG_MSGBkp1)memcpy(&SRegistresRW_THM.Reg_41502[24],&pModbusRsp->Modbus.Datas[1],48);
        else if(FLAG_MSGBkp2)memcpy(&SRegistresRW_THM.Reg_41502[48],&pModbusRsp->Modbus.Datas[1],48);
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK re�u */
      break;
    
    default:
       /* Code Erreur : le message envoy� est erron� */
        L3_Status = L3_TX_NACK; /* NACK re�u */
      
      break;
  }
} /* void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp) */

/**********************************************************************************************************
* NOM:void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
*
* DESCRIPTION      :Traitement de la trame de r�ponse � une requ�te ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
*********************************************************************************************************/
void MODBUS_sendrw_req_pdu(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, uint8_t W_Byte_Count, uint16_t* Datas, APU_SX1211* pModbusReq)
{
  uint8_t taille = 0;
  uint8_t byte_count;
  pModbusReq->Modbus.FunctionCode = RD_WR_MULTIPLE_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R >> 8); /* Adresse d�but lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R);      /* Adresse d�but lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(Qty_R >> 8); /* Nombre de mots � lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)Qty_R;        /* Nombre de mots � lire LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_W >> 8);  /* Adresse d�but �criture */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)StartAdr_W;         /* Adresse d�but �criture */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(Qty_W >> 8); /* Nombre de mot � �crire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)Qty_W; /* Nombre de mot � �crire LSB */
  pModbusReq->Modbus.Datas[taille++] = W_Byte_Count; /* Nombre d'octets de donn�es */
  byte_count = W_Byte_Count;
  while(byte_count)
  {
    pModbusReq->Modbus.Datas[taille++] = (*Datas >> 8); /* donn�es */
    byte_count--;
    pModbusReq->Modbus.Datas[taille++] = (uint8_t) *Datas; /* donn�es */
    Datas++;
    byte_count--;   
  }
  pModbusReq->Taille = taille;

} /* void MODBUS_sendrw_req_pdu(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, APU_SX1211* pModbusReq) */

void MODBUS_sendrw_req_pd2(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, uint8_t W_Byte_Count, uint8_t*  Datas, APU_SX1211* pModbusReq)
{
  uint8_t taille = 0;
  uint8_t byte_count;
  pModbusReq->Modbus.FunctionCode = RD_WR_MULTIPLE_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R >> 8); /* Adresse d�but lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R);      /* Adresse d�but lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(Qty_R >> 8); /* Nombre de mots � lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)Qty_R;        /* Nombre de mots � lire LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_W >> 8);  /* Adresse d�but �criture */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)StartAdr_W;         /* Adresse d�but �criture */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(Qty_W >> 8); /* Nombre de mot � �crire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)Qty_W; /* Nombre de mot � �crire LSB */
  pModbusReq->Modbus.Datas[taille++] = W_Byte_Count; /* Nombre d'octets de donn�es */
  byte_count = W_Byte_Count;
  while(byte_count)
  {
    pModbusReq->Modbus.Datas[taille++] = (uint8_t) *Datas; /* donn�es */
    Datas++;
    byte_count--;   
  }
  pModbusReq->Taille = taille;

} /* void MODBUS_sendrw_req_pd2(uint16_t StartAdr_R, uint16_t StartAdr_W, uint16_t Qty_R, uint16_t Qty_W, uint8_t W_Byte_Count, uint8_t*  Datas, APU_SX1211* pModbusReq) */

/**********************************************************************************************************
* NOM:void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
*
* DESCRIPTION      :Traitement de la trame de r�ponse � une requ�te ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
*********************************************************************************************************/
void MODBUS_sendr_req_pdu(uint16_t StartAdr_R, uint16_t Qty_R, APU_SX1211* pModbusReq)
{
  uint8_t taille = 0;
  pModbusReq->Modbus.FunctionCode = READ_HOLDING_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R >> 8); /* Adresse d�but lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(StartAdr_R);      /* Adresse d�but lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)(Qty_R >> 8); /* Nombre de mots � lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8_t)Qty_R;        /* Nombre de mots � lire LSB */
  pModbusReq->Taille = taille;

} /* void MODBUS_sendr_req_pdu(uint16_t StartAdr_R, , uint16_t Qty_R, APU_SX1211* pModbusReq)*/

/**********************************************************************************************************
* NOM:void MODBUS_sendAl_req_pdu(uint8_t NumeroAlarme, uint8_t Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq)
*
* DESCRIPTION      :pr�pare une requete alarme 
* PARAMETRES       :chaines de caract�re � envoyer(nom du periph et alarme), pointeur sur le buffer de requete et le code alarme.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 07/06/2011 BM: Creation
* 03/10/2011 BM: Ajout parametre code alarme et chaine de caract�res du nom de l'appareil qui transmet l'al
* 27/10/2011 NA: Mise en place dans le THM.
* 09/05/2012 NA: Modif, le num. de Z(x) est ajoute au texte pdt la transmision.
*********************************************************************************************************/
void MODBUS_sendAl_req_pdu(uint8_t NumeroAlarme, uint8_t Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq)
{
  uint8_t taille = 0;
  uint8_t ucNbrChar;

  if(Bitnewstate)
  { /* si true apparition */
    /* Definition du Code de fonction */
    pModbusReq->Modbus.FunctionCode = ALARM;    /* Code de fonction, 0x43 */
    pModbusReq->Modbus.Datas[taille++] = NumeroAlarme; /* donn�es */
    taille++;
    
    /* Ajoute le texte sur la Ligne1, cad l'identifiant */
    ucNbrChar = 0;
    while(*StringLine1)
    {
      pModbusReq->Modbus.Datas[taille++] = (*StringLine1); /* donn�es */
      StringLine1++;
      if(++ucNbrChar >= MAX_NBR_CHAR-1) break; /* On v�rifie si la chaine de caract�res fait au max 13 caract�res. si >, on coupe le reste de la chaine */
    }
    pModbusReq->Modbus.Datas[taille++] = ((THMZX+1)+'0'); /* Ajoute le numero de zone sur la Ligne1 */
    pModbusReq->Modbus.Datas[taille++] = (*"\0"); /* Termine le texte sur la Ligne1 */
    
    /* Ajoute le texte sur la Ligne2, cad l'alarme */
    ucNbrChar = 0;
    while(*StringLine2)
    {
      pModbusReq->Modbus.Datas[taille++] = (*StringLine2); /* donn�es */
      StringLine2++;
      if(++ucNbrChar >= MAX_NBR_CHAR) break; /* On v�rifie si la chaine de caract�res fait au max 14 caract�res. si >, on coupe le reste de la chaine */
    }
    pModbusReq->Modbus.Datas[taille++] = (*"\0"); /* Termine le texte sur la Ligne2 */
    
    /* Declare au final le nbre d'octets � transmettre */
    pModbusReq->Modbus.Datas[1] = (taille - 2); /* Nombre d'octet de donn�es */
    pModbusReq->Taille = taille;
  }
  else
  { /* si false disparition */
    pModbusReq->Modbus.FunctionCode = ALARM;    /* Code de fonction, 0x43 */
    pModbusReq->Modbus.Datas[0] = NumeroAlarme; /* Num�ro de l'alarme, 0 � 127 */
    pModbusReq->Modbus.Datas[1] = 0;            /* Nombre d'octets de donn�es, Toujours 0 */
    pModbusReq->Taille = 2;
  }
}/* void MODBUS_sendAl_req_pdu(uint8_t NumeroAlarme, uint8_t Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq) */
/**********************************************************************************************************
* NOM:void MODBUS_Set_Registers(void)
*
* DESCRIPTION : Initialise les registres ModBus. Appellee a l'init.
* PARAMETRES :sans
* VALEUR RETOURNEE :sans
* MAINTENANCE :
* 13/01/2009 BM: Creation
*********************************************************************************************************/
void MODBUS_Set_Registers(void)
{
  uint8_t nibble;

  /* Registres read only */
  /* [30001-30016] lecture tracker initialises a 0 par le compilateur */
  /* [30017-30019] */
  SRegistresR_COM.Reg_30017 = (_TYPE_ << 8) + _CDC_;
  SRegistresR_COM.Reg_30018 = _RELEASE_ << 8;
  /* __TIME__  A string that identifies the time of compilation in the form "hh:mm:ss" */
  nibble = __TIME__[0] - '0'; SRegistresR_COM.Reg_30018 += (uint16_t)nibble << 4;
  nibble = __TIME__[1] - '0'; SRegistresR_COM.Reg_30018 += (uint16_t)nibble;
  nibble = __TIME__[3] - '0'; SRegistresR_COM.Reg_30019 = (uint16_t)nibble << 12;
  nibble = __TIME__[4] - '0'; SRegistresR_COM.Reg_30019 += (uint16_t)nibble << 8;
  nibble = __TIME__[6] - '0'; SRegistresR_COM.Reg_30019 += (uint16_t)nibble << 4;
  nibble = __TIME__[7] - '0'; SRegistresR_COM.Reg_30019 += (uint16_t)nibble;
  /* [30020] RSSI initialise a 0 par le compilateur */

} /* void MODBUS_Set_Registers(void) */
