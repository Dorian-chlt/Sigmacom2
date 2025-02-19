/************************************************************************************
*                                                                                   *
*   File Name   : main.c                                                            *
*   Contents    : Main program of the EW1 mode sample program R8C/13 Group          *
*   Copyright(C)2004, Renesas Technology Corp.                                      *
*   Copyright(C)2004, Renesas Solutions Corp.                                       *
*   All rights reserved.                                                            *
*   Version     : 1.20                                                              *
*   note        : 0.01  : First version                                             *
*               : 1.10(2004.08.23): Comment is revised                              *
*               : 1.20(2010.04.22): NA.Initial Read of flash to Ram added           *
************************************************************************************/
#include "EEPROM.h"
#include "stm_eeprom.h"
#include "ERS2_Messages.h"
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

extern RAM2EEP SRAM2EEP;
/* Definition of RAM area */
uint16_t VarDataTab[NB_OF_VAR];
/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[NB_OF_VAR] = {
1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,
33,34,35	
}; /* 0x0000 value is prohibited in en.x-cube-eeprom */

EE_Status EEP_Status;
uint8_t EEP_Error;
    /* Declaration of function prototype */
void EEP_Update(void); /* Update virtual EEPROM contents from RAM */
void RAM_Update(void); /* Update RAM contents from virtual EEPROM */
extern void WEB_Update(void); /* Update WEB contents */
void EEP_Autoformat(void);


/*
Table de const
en Debug, non utilisee par le programme.
en Release, pour la charger Options/Linker/Input/Keep symbols.
*/
const uint64_t pEEP_Preload[NB_OF_VAR+1] @ 0x0801F000 = 
{
0,((uint64_t)(((EEP_DEF_IDKEYX << 24)|(EEP_DEF_SMMODE << 16)) |  1) << 32), /* EEP Factory Preload */ /* 0, <- for EE_PAGESTAT_VALID */
  ((uint64_t)(((EEP_DEF_IDLAN1 << 24)|(EEP_DEF_IDLAN0 << 16)) |  2) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_IDLAN3 << 24)|(EEP_DEF_IDLAN2 << 16)) |  3) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_IDPART << 24)|(EEP_DEF_IDNODE << 16)) |  4) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_OF7PRO << 24)|(EEP_DEF_OF7CTN << 16)) |  5) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_CSELUN << 24)|(EEP_DEF_CSESOL << 16)) |  6) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_LVLCTR << 24)|(EEP_DEF_CSEGEL << 16)) |  7) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_FREE02 << 24)|(EEP_DEF_LUGDAY << 16)) |  8) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |  9) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	10) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	11) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	12) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	13) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	14) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	15) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	16) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	17) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	18) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	19) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	20) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	21) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	22) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	23) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	24) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	25) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	26) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE1 << 24)|(EEP_DEF_PLAGE0 << 16)) |	27) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE3 << 24)|(EEP_DEF_PLAGE2 << 16)) |	28) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_PLAGE5 << 24)|(EEP_DEF_PLAGE4 << 16)) |	29) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_FREE04 << 24)|(EEP_DEF_FREE03 << 16)) |	30) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_FREE06 << 24)|(EEP_DEF_FREE05 << 16)) |	31) << 32), /* EEP Factory Preload */
  ((uint64_t)(((EEP_DEF_FREE08 << 24)|(EEP_DEF_FREE07 << 16)) |	32) << 32), /* EEP Factory Preload */
  ((uint64_t)((                       (EEP_DEF_REGUL1 << 16)) |	33) << 32), /* EEP Factory Preload */
  ((uint64_t)((                       (EEP_DEF_REGUL2 << 16)) |	34) << 32), /* EEP Factory Preload */
  ((uint64_t)((                       (EEP_DEF_REGUL3 << 16)) |	35) << 32), /* EEP Factory Preload */
};
/************************************************************************************
Name:		 RAM_Update
Parameters:  None
Returns:     None
Description: Read the Virtual EEPROM and update the associated RAM variables.
************************************************************************************/
void RAM_Update(void){
uint8_t checksum = 0;
  /* read the last stored NB_OF_VAR variables from EEPROM to RAM */
  for(uint8_t tabidx = 0; tabidx < NB_OF_VAR; tabidx++)
  {
    EE_ReadVariable(VirtAddVarTab[tabidx], &VarDataTab[tabidx]);
    checksum += VarDataTab[tabidx];
  }
  /* Checksum des variables pour l'initialisation par defaut si l'eeprom est vierge */
  if(checksum == 0)
  {
    EEP_Autoformat();
  }
  else
  {
    memcpy(&SRAM2EEP,&VarDataTab[0],(NB_OF_VAR*2)); /* Buffer -> System */
  }
}
/**********************************************************************************************************
* NOM:void EEP_Autoformat(void)
*
* DESCRIPTION      :dans le cas d'une eep vierge le checksum = 0, init des valeurs par defaut.
* PARAMETRES       :sans
* VALEUR RETOURNEE :sans
* MAINTENANCE      :
* 30/04/2010 NA: Creation
* 16/11/2017 NA: Adaptation au STM32L433.
* 12/02/2018 NA: Modif methode de chrgt des val. par defaut de EEP_DEF_SREGUL.
*********************************************************************************************************/
void EEP_Autoformat(void)
{
/* initialisation de EEP_DEF_HHDAYx avec la plage horaire generique */
HH48SEG EEP_DEF_HHDAYx = {
    EEP_DEF_PLAGE0,
    EEP_DEF_PLAGE1,
    EEP_DEF_PLAGE2,
    EEP_DEF_PLAGE3,
    EEP_DEF_PLAGE4,
    EEP_DEF_PLAGE5,
 };
/* initialisation des valeurs par defaut */
  ucSMMODE      = SMMOD_PERM_AUTO; /* RAM load if Auto-Format specific */
  ucIDKEYX      = EEP_DEF_IDKEYX; /* RAM load if Auto-Format */
  ucIDLAN0      = EEP_DEF_IDLAN0; /* RAM load if Auto-Format */
  ucIDLAN1      = EEP_DEF_IDLAN1; /* RAM load if Auto-Format */
  ucIDLAN2      = EEP_DEF_IDLAN2; /* RAM load if Auto-Format */
  ucIDLAN3      = EEP_DEF_IDLAN3; /* RAM load if Auto-Format */
  ucIDNODE      = EEP_DEF_IDNODE; /* RAM load if Auto-Format */
  ucIDPART      = EEP_DEF_IDPART; /* RAM load if Auto-Format */
  scOF7CTN      = EEP_DEF_OF7CTN; /* RAM load if Auto-Format */
  scOF7PRO      = EEP_DEF_OF7PRO; /* RAM load if Auto-Format */
  ucCSESOL      = EEP_DEF_CSESOL; /* RAM load if Auto-Format */
  ucCSELUN      = EEP_DEF_CSELUN; /* RAM load if Auto-Format */
  ucCSEGEL      = EEP_DEF_CSEGEL; /* RAM load if Auto-Format */
  ucLVLCTR      = TDEFCTR;        /* RAM load if Auto-Format specific */
  ucLUGDAY      = EEP_DEF_LUGDAY; /* RAM load if Auto-Format */
  ucFREE02      = EEP_DEF_FREE02; /* RAM load if Auto-Format */
 /* recopie de EEP_DEF_HHDAYx sur les 7 jours de la semaine */
  SHH_DAY0      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY1      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY2      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY3      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY4      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY5      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  SHH_DAY6      = EEP_DEF_HHDAYx; /* RAM load if Auto-Format */
  /* initialisation de l'eeprom vierge */
  ucFREE03      = EEP_DEF_FREE03; /* RAM load if Auto-Format */
  ucFREE04      = EEP_DEF_FREE04; /* RAM load if Auto-Format */
  ucFREE05      = EEP_DEF_FREE05; /* RAM load if Auto-Format */
  ucFREE06      = EEP_DEF_FREE06; /* RAM load if Auto-Format */
  ucFREE07      = EEP_DEF_FREE07; /* RAM load if Auto-Format */
  ucFREE08      = EEP_DEF_FREE08; /* RAM load if Auto-Format */
/* initialisation de EEP_DEF_SREGUL avec les parametres generiques */
REGULATOR_BLOCK EEP_DEF_SREGUL;
/* 0x4B0F */
  EEP_DEF_SREGUL.TypeDeRegulation = 7;
  EEP_DEF_SREGUL.PlancherRadiateur = 1;
  EEP_DEF_SREGUL.Pente = PENT_DEFR;
  EEP_DEF_SREGUL.Ambiance = AMBI_DEF;
  EEP_DEF_SREGUL.Hysteresis = HYST_DEF;
/* 0x1455 */
  EEP_DEF_SREGUL.Umax = SATU_MAXZ1R;
  EEP_DEF_SREGUL.Umin = SATU_MINZx;
/* 0x0001 */
  EEP_DEF_SREGUL.Nk = 1;
  EEP_DEF_SREGUL.Inertie = 0;
  EEP_DEF_SREGUL.Filaire = 0;
  EEP_DEF_SREGUL.Libre2 = 0;
  sbREGUL      = EEP_DEF_SREGUL; /* RAM load if Auto-Format */

  EEP_Update(); /* Update Ram contents to virtual EEPROM */

} /* void EEP_Autoformat(void) */

/************************************************************************************
Name:		 EEP_Update
Parameters:  None
Returns:     result
Description: Update Ram contents to virtual EEPROM
************************************************************************************/

void EEP_Update(void){
  memcpy(&VarDataTab[0],&SRAM2EEP,(NB_OF_VAR*2)); /* System -> Buffer */
  /* Store successively NB_OF_VAR variables from RAM to EEPROM */
  for(uint16_t tabidx = 0; tabidx < NB_OF_VAR; tabidx++)
  {
    /* Unlock the Flash Program Erase controller */
    HAL_FLASH_Unlock();
    /* Writes/updates variable data in EEPROM */
    EEP_Status = EE_WriteVariable(VirtAddVarTab[tabidx], VarDataTab[tabidx]);
    /* Lock the Flash Program Erase controller */
    HAL_FLASH_Lock();
    if(EEP_Status != EE_OK) EEP_Error++;
  }
}

/**********************************************************************************************************
* NOM:uint8 read_eeprom(uint8 *Ptr, uint8 Count, uint8 *pBuff);
*
* DESCRIPTION      : Lecture dans la virtual EEPROM.
* PARAMETRES       : Ptr sur adresse source en Eeprom, Nbre d'octets, Ptr sur adresse destination en RAM
* VALEUR RETOURNEE : 1 si OK, 0 sinon.
* MAINTENANCE      :
* 29/04/2010 NA: Creation
*********************************************************************************************************/
uint8_t read_eeprom(uint8_t *Ptr, uint8_t Count, uint8_t *pBuff) 
{
  uint8_t i;
  for (i = 0; i < Count ;  i++, pBuff++, Ptr++) 
  {
    *pBuff = *Ptr;
  }
  return 1;
}
/**********************************************************************************************************
* NOM:uint8 write_eeprom(uint8 *Ptr, uint8 Count, uint8 *pBuff)
*
* DESCRIPTION      : Ecriture dans la virtual EEPROM.
* PARAMETRES       : Ptr sur la destination en Eeprom, Nbre d'octets, Ptr sur la source en RAM.
* VALEUR RETOURNEE : Nbre d'octets ecrits si ok   0 sinon
* MAINTENANCE      :
* 29/04/2010 NA: Creation
*********************************************************************************************************/
uint8_t write_eeprom(uint8_t *Ptr, uint8_t Count, uint8_t *pBuff)
{
  uint8_t i;

  for (i = 0; i < Count ;  i++, pBuff++, Ptr++) 
  {
    *Ptr = *pBuff;
  }
  EEP_Update(); /* Update Ram contents to virtual EEPROM */
  
  return Count;
} /* uint8 write_eeprom(uint8 *Ptr, uint8 Count, uint8 *pBuff) */

/**********************************************************************************************************
* NOM:void NVM_Process(void)
*
* DESCRIPTION      : Mise à jour de la virtual EEPROM, appelée dans la boucle main.
* PARAMETRES       : 
* VALEUR RETOURNEE : 
* MAINTENANCE      :
* 29/04/2010 NA: Creation
* 17/10/2011 NA: Ajout decl d'un reset par ILAD, necessairement à la suite d'une ecriture en NVM.
* 09/03/2016 NA: Ajout Web update.
*********************************************************************************************************/
void NVM_Process(void)
{
  if(FLAG_REFRESH_EEP){
    EEP_Update(); /* Update Ram contents to virtual EEPROM */
    FLAG_REFRESH_EEP = false;
    WEB_Update(); /* Update WEB contents */
  }
  if(FLAG_RESETMCU){
/* ILAD ie (Illegal Address) Reset was caused by an attempt to access either data or
  an instruction at an unimplemented memory address.*/
    BSP_RST_Request();  /* Declenchement d'un Reset */
  }
} /* void NVM_Process(void) */


