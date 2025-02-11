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
#include "bsp.h"

/*=== DEFINITIONS GLOBALES ==============================================================================*/
/* Adresse Zone read/write specifique SIGMA */
#define FIRST_ADRESS_RRW_SPEC     RRW_SAT1_SC_DEBUT
#define LAST_ADRESS_RRW_SPEC      RRW_SAT1_SC_FIN
//#define _NO_MATCH_ERR_ /* Avec anti-erreur de matching sur 5B ald 4 */
/*=== PROTOTYPES DES FONCTIONS ==========================================================================*/
extern void RF_Keep_RXLV(void); /* Conditions de maintien en RXLV */
void MODBUS_Set_Registers(void);
extern uint16 uifnMEMFAV_CS(void);
extern void RF_WEB2THM(void);
extern void UPDT_SYS2BUF(void);

/*=== VARIABLES EXTERNES ================================================================================*/
extern uint8 L3_Status;
extern uint8 ucTimerTX2TX;
extern uint8 ucMEMFAV_SA;
extern uint8 ucMEMFAV_SM;
extern uint8 ucMEMFAV_SJ;
extern uint16 uiMEMFAV_CS;
/*=== VARIABLES GLOBALES ================================================================================*/
uint8 Langue;  /* attention variable à peut-etre sauvegarder en virtual eeprom */
uint8 Rf_Level_Quality; /* niveau de réception radio de la face avant */
REGRW_FAV SRegistresRW_FAV; /* Registres read/write spécifiques FAV */
REGRW_COM SRegistresRW_COM; /* Registres communs */
REGR_COM  SRegistresR_COM;  /* Registres communs */
REGRW_THM SRegistresRW_THM; /* Registres read/write spécifiques THM */
REGRW_SAT_SGC SRegistresRW_SAT_SGC; /* Registres read/write spécifiques RW_SAT_SGC */
REGRW_SAT_LED SRegistresRW_SAT_LED; /* Registres read/write spécifiques RW_SAT_LED */
REGRW_SGC_LED SRegistresRW_SGC_LED; /* Registres read/write spécifiques RW_SGC_LED */
/*=== FONCTIONS =========================================================================================*/
/**********************************************************************************************************
* NOM:uint16 uifnMEMFAV_CS(void)
*
* DESCRIPTION      : Calcul de la somme de la zone de RAM.
* PARAMETRES       :
* VALEUR RETOURNEE : la somme.
* MAINTENANCE      :
* 06/06/2013  NA: Creation
*********************************************************************************************************/
uint16 uifnMEMFAV_CS(void)
{
  uint8  i;
  uint16 j=0;
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
* 23/02/2010  NA: Opt de code, Suppression des case non utilisés.
* 25/10/2011 BM/NA: Ajout gestion de IDKEYX.
* 25/10/2011  NA: Supp des 3 ecritures successives en NVM lors du Matching.
* 15/11/2011  NA: Supp de la gestion de réponse au matching.
* 06/03/2012  NA: Ajout demande de reset.
* 09/10/2012 BM/NA: Ajout verif. sur SET_NETID que le periph. souhaite par la FAV correspond au NodeId courant.
* 09/02/2016 BM/NA: Ajout RD_WR_MULTIPLE_REG en version light (zone dediee only).
* 18/05/2016 JLD/NA: Ajout version du THM transmise à la FAV sur l'ack du matching.
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
        UPDT_SYS2BUF();/* Maj des données du systeme vers le buffer d'échange.*/
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

        /* On propose un backup à la FAV si disponible */
        if((uifnMEMFAV_CS() + uiMEMFAV_CS) == 0xFFFF){
          /* [3][AA][MM][JJ] */
          pModbusRsp->Modbus.Datas[taille++] = 3;
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SA);
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SM);
          pModbusRsp->Modbus.Datas[taille++] = Dec2Bcd(ucMEMFAV_SJ);
          FLAG_ACT_TIM5msRUN_BY_MATF = TRUE;
          uiTIM5msRUN = TIMEOUT_TIM5msRUN_ACT_BY_MATF;
        }
        else{
          FLAG_ACT_TIM5msRUN_BY_MATF = TRUE;
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
    { /* données présentes */
      RF_Keep_RXLV(); /* Conditions de maintien en RXLV */
      Rf_Level_Quality = pModbusReq->Modbus.Datas[1];; /* qualité de réception de la face avant : de 00 à 05 */
      SRegistresR_COM.Reg_30020 = Rf_Level_Quality; /* niveau de réception radio de la face avant */
      /* on acquitte avec une trame d'au moins 20ms pour permettre à la face avant de mesurer son niveau de réception radio */
      pModbusRsp->Modbus.FunctionCode = pModbusReq->Modbus.FunctionCode;
      pModbusRsp->Adresse = pModbusReq->Adresse;
      /* On remplie les 58 octets de données */
      pModbusRsp->Modbus.Datas[0] = SX1211_FIFO_SIZE-SIZEOF_HEADER-2; /* nombre d'octets de données 56 */
      q = RF_GetRSSI();
      memset(&pModbusRsp->Modbus.Datas[1],q,(SX1211_FIFO_SIZE-SIZEOF_HEADER-2)); /* remplie les 56 octets de données avec mon niveau de réception radio */
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
      if(first_address == RRW_THM_Magic)FLAG_READEND = TRUE;
      if(first_address == RRW_THM_Backup_48)FLAG_READCHK = TRUE;
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
* DESCRIPTION      :Traitement de la trame de réponse à une requête ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
* 12/07/2010 BM/NA: Modification il faut diviser par 2 la valeur de byte_count.
* 09/11/2011  NA: Mise à jour de la variable de reception des Flags de la FAV.
* 16/11/2011  NA: Ajout pour occulter le FLAG_ALREP du msg pas de sat. initialisé.
* 11/01/2012  NA: Ajout L3_TX_ACKNOWLEDGED sur case ALARM manquant.
* 21/02/2012  NA: Ajout RESTART_CALENDAR_COUNTER, pour une raz complete de la quantification de la seconde.
* 26/06/2012  NA: Ajout DEF_LANGUE, pour la map avec une seule langue imposée.
* 15/02/2016  NA: Ajout filtrage avant maj des variables et supp du Baro.
* 18/05/2016  NA: Ajout langue ALL et controle avec par defaut FRA si inconnue.
* 16/02/2018  NA: Modif maj de la RTC hard ald soft.
* 22/02/2018  NA: Ajout com vers FAV_LED.
*********************************************************************************************************/
void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp)
{
#ifndef DEF_LANGUE
  uint8  Langue_Check;
#endif
  uint8  decalage = (THMZX * OF7_THM_Zx)*2;
  /* On suit le document V1.1 du protocole  */
  /* Fonction code supported ? */
  switch(pModbusRsp->Modbus.FunctionCode)
  {
    case RD_WR_MULTIPLE_REG:
        if(FLAG_MSGAmbi||(!FLAG_MSGAlrm && FLAG_MSGACFM)){/* pas de mise à jour des valeurs sinon */
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
          FLAG_ACT_WEB = FALSE; /* pas de Connect pdt la map !!! */
/*----------------------------------------------------------------------------*/
#endif/*WO_ACT_WEB*/
          
          if(FLAG_NOALREP){
            FLAG_NOALREP = FALSE;
            FLAG_ALREP = FALSE;
            FLAG_ACT_WEB = FALSE;
          }

  #ifdef DEF_LANGUE
          Langue = DEF_LANGUE;  /* pour la map avec une seule langue imposée */
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
        else if(FLAG_MSGPreg){/* pas de mise à jour des valeurs sinon */
          BUF_LED_SREGUL0 = (((uint16_t)pModbusRsp->Modbus.Datas[1] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[2]));
          BUF_LED_SREGUL1 = (((uint16_t)pModbusRsp->Modbus.Datas[3] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[4]));
          BUF_LED_SREGUL2 = (((uint16_t)pModbusRsp->Modbus.Datas[5] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[6]));
          if(IS_BUFF_NOTEMPTY){/* si la FAV a initialisé le buffer d'echange */
            memcpy(&sbREGUL,&BUF_LED_sbREGUL,sizeof(REGULATOR_BLOCK)); /* Buffer -> System */
            FLAG_REFRESH_EEP=TRUE;
          }
        }
        else if(FLAG_MSGPclk){/* pas de mise à jour des valeurs sinon */
          if(IS_MSGCLK_RD){/* maj seulement sur la reponse de la demande de lecture */
            BUF_LED_HEURE_AUTO = (((uint16_t)pModbusRsp->Modbus.Datas[1] << 8) + ((uint16_t)pModbusRsp->Modbus.Datas[2]));
          }
        }
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK reçu */
      break;
    
    case ALARM:
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK reçu */
      break;
    
    case READ_HOLDING_REG:
        if     (FLAG_MSGBkp0)memcpy(&SRegistresRW_THM.Reg_41502[0],&pModbusRsp->Modbus.Datas[1],48);
        else if(FLAG_MSGBkp1)memcpy(&SRegistresRW_THM.Reg_41502[24],&pModbusRsp->Modbus.Datas[1],48);
        else if(FLAG_MSGBkp2)memcpy(&SRegistresRW_THM.Reg_41502[48],&pModbusRsp->Modbus.Datas[1],48);
        L3_Status = L3_TX_ACKNOWLEDGED; /* ACK reçu */
      break;
    
    default:
       /* Code Erreur : le message envoyé est erroné */
        L3_Status = L3_TX_NACK; /* NACK reçu */
      
      break;
  }
} /* void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp) */

/**********************************************************************************************************
* NOM:void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
*
* DESCRIPTION      :Traitement de la trame de réponse à une requête ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
*********************************************************************************************************/
void MODBUS_sendrw_req_pdu(uint16 StartAdr_R, uint16 StartAdr_W, uint16 Qty_R, uint16 Qty_W, uint8 W_Byte_Count, uint16_t* Datas, APU_SX1211* pModbusReq)
{
  uint8 taille = 0;
  uint8 byte_count;
  pModbusReq->Modbus.FunctionCode = RD_WR_MULTIPLE_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R >> 8); /* Adresse début lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R);      /* Adresse début lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(Qty_R >> 8); /* Nombre de mots à lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)Qty_R;        /* Nombre de mots à lire LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_W >> 8);  /* Adresse début écriture */
  pModbusReq->Modbus.Datas[taille++] = (uint8)StartAdr_W;         /* Adresse début écriture */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(Qty_W >> 8); /* Nombre de mot à écrire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)Qty_W; /* Nombre de mot à écrire LSB */
  pModbusReq->Modbus.Datas[taille++] = W_Byte_Count; /* Nombre d'octets de données */
  byte_count = W_Byte_Count;
  while(byte_count)
  {
    pModbusReq->Modbus.Datas[taille++] = (*Datas >> 8); /* données */
    byte_count--;
    pModbusReq->Modbus.Datas[taille++] = (uint8) *Datas; /* données */
    Datas++;
    byte_count--;   
  }
  pModbusReq->Taille = taille;

} /* void MODBUS_sendrw_req_pdu(uint16 StartAdr_R, uint16 StartAdr_W, uint16 Qty_R, uint16 Qty_W, APU_SX1211* pModbusReq) */

void MODBUS_sendrw_req_pd2(uint16 StartAdr_R, uint16 StartAdr_W, uint16 Qty_R, uint16 Qty_W, uint8 W_Byte_Count, uint8_t*  Datas, APU_SX1211* pModbusReq)
{
  uint8 taille = 0;
  uint8 byte_count;
  pModbusReq->Modbus.FunctionCode = RD_WR_MULTIPLE_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R >> 8); /* Adresse début lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R);      /* Adresse début lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(Qty_R >> 8); /* Nombre de mots à lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)Qty_R;        /* Nombre de mots à lire LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_W >> 8);  /* Adresse début écriture */
  pModbusReq->Modbus.Datas[taille++] = (uint8)StartAdr_W;         /* Adresse début écriture */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(Qty_W >> 8); /* Nombre de mot à écrire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)Qty_W; /* Nombre de mot à écrire LSB */
  pModbusReq->Modbus.Datas[taille++] = W_Byte_Count; /* Nombre d'octets de données */
  byte_count = W_Byte_Count;
  while(byte_count)
  {
    pModbusReq->Modbus.Datas[taille++] = (uint8) *Datas; /* données */
    Datas++;
    byte_count--;   
  }
  pModbusReq->Taille = taille;

} /* void MODBUS_sendrw_req_pd2(uint16 StartAdr_R, uint16 StartAdr_W, uint16 Qty_R, uint16 Qty_W, uint8 W_Byte_Count, uint8_t*  Datas, APU_SX1211* pModbusReq) */

/**********************************************************************************************************
* NOM:void MODBUS_mb_rsp_pdu(APU_SX1211* pModbusRsp,APU_SX1211* pModbusReq)
*
* DESCRIPTION      :Traitement de la trame de réponse à une requête ModBus
* PARAMETRES       :Ptr sur le buffer de Reponse, Ptr sur le buffer de requete.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 19/10/2009 JLD: Creation
*********************************************************************************************************/
void MODBUS_sendr_req_pdu(uint16 StartAdr_R, uint16 Qty_R, APU_SX1211* pModbusReq)
{
  uint8 taille = 0;
  pModbusReq->Modbus.FunctionCode = READ_HOLDING_REG; /* Code de fonction */
  taille = 0;        
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R >> 8); /* Adresse début lecture MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(StartAdr_R);      /* Adresse début lecture LSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)(Qty_R >> 8); /* Nombre de mots à lire MSB */
  pModbusReq->Modbus.Datas[taille++] = (uint8)Qty_R;        /* Nombre de mots à lire LSB */
  pModbusReq->Taille = taille;

} /* void MODBUS_sendr_req_pdu(uint16 StartAdr_R, , uint16 Qty_R, APU_SX1211* pModbusReq)*/

/**********************************************************************************************************
* NOM:void MODBUS_sendAl_req_pdu(uint8 NumeroAlarme, uint8 Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq)
*
* DESCRIPTION      :prépare une requete alarme 
* PARAMETRES       :chaines de caractère à envoyer(nom du periph et alarme), pointeur sur le buffer de requete et le code alarme.
* VALEUR RETOURNEE :Sans
* MAINTENANCE      :
* 07/06/2011 BM: Creation
* 03/10/2011 BM: Ajout parametre code alarme et chaine de caractères du nom de l'appareil qui transmet l'al
* 27/10/2011 NA: Mise en place dans le THM.
* 09/05/2012 NA: Modif, le num. de Z(x) est ajoute au texte pdt la transmision.
*********************************************************************************************************/
void MODBUS_sendAl_req_pdu(uint8 NumeroAlarme, uint8 Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq)
{
  uint8 taille = 0;
  uint8 ucNbrChar;

  if(Bitnewstate)
  { /* si TRUE apparition */
    /* Definition du Code de fonction */
    pModbusReq->Modbus.FunctionCode = ALARM;    /* Code de fonction, 0x43 */
    pModbusReq->Modbus.Datas[taille++] = NumeroAlarme; /* données */
    taille++;
    
    /* Ajoute le texte sur la Ligne1, cad l'identifiant */
    ucNbrChar = 0;
    while(*StringLine1)
    {
      pModbusReq->Modbus.Datas[taille++] = (*StringLine1); /* données */
      StringLine1++;
      if(++ucNbrChar >= MAX_NBR_CHAR-1) break; /* On vérifie si la chaine de caractères fait au max 13 caractères. si >, on coupe le reste de la chaine */
    }
    pModbusReq->Modbus.Datas[taille++] = ((THMZX+1)+'0'); /* Ajoute le numero de zone sur la Ligne1 */
    pModbusReq->Modbus.Datas[taille++] = (*"\0"); /* Termine le texte sur la Ligne1 */
    
    /* Ajoute le texte sur la Ligne2, cad l'alarme */
    ucNbrChar = 0;
    while(*StringLine2)
    {
      pModbusReq->Modbus.Datas[taille++] = (*StringLine2); /* données */
      StringLine2++;
      if(++ucNbrChar >= MAX_NBR_CHAR) break; /* On vérifie si la chaine de caractères fait au max 14 caractères. si >, on coupe le reste de la chaine */
    }
    pModbusReq->Modbus.Datas[taille++] = (*"\0"); /* Termine le texte sur la Ligne2 */
    
    /* Declare au final le nbre d'octets à transmettre */
    pModbusReq->Modbus.Datas[1] = (taille - 2); /* Nombre d'octet de données */
    pModbusReq->Taille = taille;
  }
  else
  { /* si FALSE disparition */
    pModbusReq->Modbus.FunctionCode = ALARM;    /* Code de fonction, 0x43 */
    pModbusReq->Modbus.Datas[0] = NumeroAlarme; /* Numéro de l'alarme, 0 à 127 */
    pModbusReq->Modbus.Datas[1] = 0;            /* Nombre d'octets de données, Toujours 0 */
    pModbusReq->Taille = 2;
  }
}/* void MODBUS_sendAl_req_pdu(uint8 NumeroAlarme, uint8 Bitnewstate, const char* StringLine1, const char* StringLine2, APU_SX1211* pModbusReq) */
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
