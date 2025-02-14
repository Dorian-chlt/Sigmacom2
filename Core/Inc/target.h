#ifndef __TARGET__
#define __TARGET__

/* Definitions selon les cibles globales */
    #define EEP_IDKEYX_PROD (0)
    #define EEP_IDLANX_PROD (0)
    #define EEP_IDPART_PROD NID_Chaudiere_maitre

  #if   defined(_FAVOLD_) /* Fav LCD à icones semi-graphique */
/*----------------------------------------------------------------------------*/
    #define EEP_IDKEYX_LABO (0x67)      /* SB 27/11/17 pour map du THM2 05/01/18 */
    #define EEP_IDLANX_LABO (0x066E2370)/* SB 27/11/17 pour map du THM2 05/01/18 */
    #define EEP_IDPART_LABO NID_Chaudiere_maitre
/*----------------------------------------------------------------------------*/
  #elif defined(_FAVLED_) /* Fav LED type ERS */
/*----------------------------------------------------------------------------*/
    #define EEP_IDKEYX_LABO (0xCA)
    #define EEP_IDLANX_LABO (0x2DEDD05A)
    #define EEP_IDPART_LABO NID_Chaudiere_maitre_Leds
/*----------------------------------------------------------------------------*/
  #else      /*(_FAVSTD_)    Fav LCD standard full-graphique */
/*----------------------------------------------------------------------------*/
    #define EEP_IDKEYX_LABO (0x3A)
    #define EEP_IDLANX_LABO (0x05da0297)
    #define EEP_IDPART_LABO NID_Chaudiere_maitre
/*----------------------------------------------------------------------------*/
  #endif

/* Definitions selon les cibles globales */
  #if   defined(_PRD_)
/*----------------------------------------------------------------------------*/
    #define DRLVP               "P"     /* affichage du type de compilation */
    #define SMINF_X             SMINF_2 /* affichage des fenetres de SMINF_1 à SMINF_X */
    #define EEP_PRS_LVLCTR      TMAXCTR /* contraste maximal retenu pour cette cible */
    #define EEP_PRS_IDKEYX      EEP_IDKEYX_PROD /* IDKEYX retenu pour cette cible */
    #define EEP_PRS_IDLANX      EEP_IDLANX_PROD /* IDLANX retenu pour cette cible */
    #define EEP_PRS_IDPART      EEP_IDPART_PROD /* IDPART retenu pour cette cible */
    #define EEP_PRS_SMMODE      SMMOD_TEST_INIT /* on démarre dans ce mode */
/*----------------------------------------------------------------------------*/
  #elif defined(_VAL_)
/*----------------------------------------------------------------------------*/
    #define DRLVP               "V"     /* affichage du type de compilation */
    #define SMINF_X             SMINF_2 /* affichage des fenetres de SMINF_1 à SMINF_X */
    #define EEP_PRS_LVLCTR      TDEFCTR /* contraste moyen retenu pour cette cible */
    #define EEP_PRS_IDKEYX      EEP_IDKEYX_PROD /* IDKEYX retenu pour cette cible */
    #define EEP_PRS_IDLANX      EEP_IDLANX_PROD /* IDLANX retenu pour cette cible */
    #define EEP_PRS_IDPART      EEP_IDPART_PROD /* IDPART retenu pour cette cible */
    #define EEP_PRS_SMMODE      SMMOD_PERM_AUTO /* on démarre dans ce mode */
/*----------------------------------------------------------------------------*/
  #elif defined(_LAB_)
/*----------------------------------------------------------------------------*/
    #define DRLVP               "L"     /* affichage du type de compilation */
    #define SMINF_X             SMINF_7 /* affichage des fenetres de SMINF_1 à SMINF_X */
    #define EEP_PRS_LVLCTR      TDEFCTR /* contraste moyen retenu pour cette cible */
    #define EEP_PRS_IDKEYX      EEP_IDKEYX_PROD /* IDKEYX retenu pour cette cible */
    #define EEP_PRS_IDLANX      EEP_IDLANX_PROD /* IDLANX retenu pour cette cible */
    #define EEP_PRS_IDPART      EEP_IDPART_PROD /* IDPART retenu pour cette cible */
    #define EEP_PRS_SMMODE      SMMOD_PERM_AUTO /* on démarre dans ce mode */
/*----------------------------------------------------------------------------*/
  #elif defined(_RND_)
/*----------------------------------------------------------------------------*/
    #define DRLVP               "r"     /* affichage du type de compilation */
    #define SMINF_X             SMINF_7 /* affichage des fenetres de SMINF_1 à SMINF_X */
    #define EEP_PRS_LVLCTR      TDEFCTR /* contraste moyen retenu pour cette cible */
    #define EEP_PRS_IDKEYX      EEP_IDKEYX_LABO /* IDKEYX retenu pour cette cible */
    #define EEP_PRS_IDLANX      EEP_IDLANX_LABO /* IDLANX retenu pour cette cible */
    #define EEP_PRS_IDPART      EEP_IDPART_LABO /* IDPART retenu pour cette cible */
    #define EEP_PRS_SMMODE      SMMOD_PERM_AUTO /* on démarre dans ce mode */
/*----------------------------------------------------------------------------*/
  #else      /*(_DBG_)*/
/*----------------------------------------------------------------------------*/
    #define DRLVP               "d"     /* affichage du type de compilation */
    #define SMINF_X             SMINF_7 /* affichage des fenetres de SMINF_1 à SMINF_X */
    #define EEP_PRS_LVLCTR      TDEFCTR /* contraste moyen retenu pour cette cible */
    #define EEP_PRS_IDKEYX      EEP_IDKEYX_LABO /* IDKEYX retenu pour cette cible */
    #define EEP_PRS_IDLANX      EEP_IDLANX_LABO /* IDLANX retenu pour cette cible */
    #define EEP_PRS_IDPART      EEP_IDPART_LABO /* IDPART retenu pour cette cible */
    #define EEP_PRS_SMMODE      SMMOD_PERM_AUTO /* on démarre dans ce mode */
/*----------------------------------------------------------------------------*/
  #endif

/* Definitions selon les cibles specifiques */
  #if   defined(_PRD_)
/*----------------------------------------------------------------------------*/
//    #define _PROD_END_EMS_        /* Dir. de compil. pour le _PROD_END_EMS_ uniquement */
/*----------------------------------------------------------------------------*/
  #elif defined(_VAL_)
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
  #elif defined(_LAB_)
/*----------------------------------------------------------------------------*/
    #define CHECK_ERRHAND       IS_ERR_HANDLER_RD
/*----------------------------------------------------------------------------*/
  #elif defined(_RND_)
/*----------------------------------------------------------------------------*/
    #define CHECK_ERRHAND       IS_ERR_HANDLER_RD
/*----------------------------------------------------------------------------*/
  #else      /*(_DBG_)*/
/*----------------------------------------------------------------------------*/
    #define CHECK_ERRHAND       IS_ERR_HANDLER_RD
    #define _SWO_DBG_             /* Dir. de compil. pour le dbg avec trace SWO */
//    #define _TESTDISPLAY_         /* Dir. de compil. pour l'aff de map du THM */
//    #define _TRACE_SCOPE_         /* Dir. de compil. pour le fct en map de la trace avec des sorties */
//    #define _SWITCH_UPD_          /* Dir. de compil. pour le fct en update par les switchs */
//    #define SIMULATEUR _RELEASE_  /* = de 1 jusque A soit 10 adresses de MATCHING_NETID réservés pour ce type de fct */
/*----------------------------------------------------------------------------*/
  #endif

/* Definitions specifiques pour la trace de variables par SWO */
#ifdef  _SWO_DBG_
/*----------------------------------------------------------------------------*/
#define SWO_RF_VAR /* uiVar_Test = valeur de SetRFMode(uint8_t mode)*/
/*----------------------------------------------------------------------------*/
#endif/*_SWO_DBG_*/

#endif


