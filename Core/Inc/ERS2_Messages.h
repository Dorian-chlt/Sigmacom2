/* A.THEOBALD ERS2_Messages.h     15/2/2018 10:40    */
/* 13/01/15 JLD Creation FAVSIGCOM  R41212 */
/* 27/01/15 R41002 & R41028  Barometre remplace par Flags Web */
/* 03/12/15 JLD Modifs SIGCOM R31200,R31339 */
/* 11/12/15 JLD Suite a ajout routage, R41300-R41327 ne sont plus dans la face avant */
/* 14/12/15 JLD Ajout code de fonction ALARM_TAG 0x45 */
/* 17/02/16 JLD Reprise des tableaux lies a FRISQUET Connect: FAV, SC et SAT. Suppression R31501-31503 RO Thermostat inutilise */
/* Suppression registres COILS inutilises. Ajout R41107. Ajout NodeId Rif et Chaudiere maitre leds. Modif flags R41002 & R41028*/
/* 17/02/16 JLD Ajout R41323 qui manquait */
/* 19/02/16 JLD Ajout MODE_VACANCES en R31238, SigmaCom matchee en R41006 & R41032. Ajout b6: si 1, BOOST On dans flags SAT  */
/* 29/02/16 JLD ALARM_TAG remplace par CHUNK. Ajout enum _CHUNK_ */
/* 22/03/16 JLD Ajout R41213-R41220: ID_CHAUDIERE */
/* 01/04/16 JLD Ajout R41400 et Poisson d'Avril ! */
/*21/09/16 JLD Ajout Registres R31272 à R31279 */
/*07/10/16 JLD  Ajout flags TC_VEILLE et AUTO_MANU dans Registres R41212 */
/*23/11/16 JLD  R41312-R41315 KWh mensuels */
/*09/02/17 JLD  Ajout NID Rif_Sext */
/*08/09/17 JLD  NID Rif supprime, Deplacement NID Rif_Sext */
/*15/02/18 JLD  Ajout R41108-R41115 dans Face avant */
/* Codes de fonctions */
enum _MODBUS_FUNCTION_
{
  READ_HOLDING_REG      = 0x03,
  WRITE_MULTIPLE_REG   = 0x10,
  RD_WR_MULTIPLE_REG  = 0x17,
  SET_NETID                      = 0x41,
  RF_RX_LEVEL                 = 0x42,
  ALARM                            = 0x43,
  ALARM_UTF8           = 0x44,
CHUNK                        = 0x45
};
/*!< Types de chunk */
enum _CHUNK_
{
  NO_MORE_CHUNK = 0,       /*!< Indique la fin */
  ALARM_SET,
  ALARM_CLEAR ,
  ACCOUNT_DELETE
};
/* Codes d'erreurs */
enum _MODBUS_ERRORS_
{
  ILLEGAL_FUNCTION = 0x01,
  ILLEGAL_DATA_ADDRESS,
  ILLEGAL_DATA_VALUE,
  SLAVE_DEVICE_FAILURE,
  ACKNOWLEDGE,
  SLAVE_DEVICE_BUSY,
  MEMORY_PARITY_ERROR = 0x08,
  GATEWAY_PATH_UNAVAILABLE = 0x0A,
  GATEWAY_FAILED_TO_RESPOND
};
/* Registres */
#define RRO_COM_DEBUT       (30001)        /*COM DEBUT*/
#define RRO_COM_Contenu_Tracker_01       (30001)        /*COM Contenu_Tracker_01*/
#define RRO_COM_Contenu_Tracker_02       (30002)        /*COM Contenu_Tracker_02*/
#define RRO_COM_Contenu_Tracker_03       (30003)        /*COM Contenu_Tracker_03*/
#define RRO_COM_Contenu_Tracker_04       (30004)        /*COM Contenu_Tracker_04*/
#define RRO_COM_Contenu_Tracker_05       (30005)        /*COM Contenu_Tracker_05*/
#define RRO_COM_Contenu_Tracker_06       (30006)        /*COM Contenu_Tracker_06*/
#define RRO_COM_Contenu_Tracker_07       (30007)        /*COM Contenu_Tracker_07*/
#define RRO_COM_Contenu_Tracker_08       (30008)        /*COM Contenu_Tracker_08*/
#define RRO_COM_Contenu_Tracker_09       (30009)        /*COM Contenu_Tracker_09*/
#define RRO_COM_Contenu_Tracker_10       (30010)        /*COM Contenu_Tracker_10*/
#define RRO_COM_Contenu_Tracker_11       (30011)        /*COM Contenu_Tracker_11*/
#define RRO_COM_Contenu_Tracker_12       (30012)        /*COM Contenu_Tracker_12*/
#define RRO_COM_Contenu_Tracker_13       (30013)        /*COM Contenu_Tracker_13*/
#define RRO_COM_Contenu_Tracker_14       (30014)        /*COM Contenu_Tracker_14*/
#define RRO_COM_Contenu_Tracker_15       (30015)        /*COM Contenu_Tracker_15*/
#define RRO_COM_Contenu_Tracker_16       (30016)        /*COM Contenu_Tracker_16*/
#define RRO_COM_COMPIL1       (30017)        /*COM TYPE DE CARTE (10)*/
#define RRO_COM_COMPIL2       (30018)        /*COM RELEASE (01)*/
#define RRO_COM_COMPIL3       (30019)        /*COM DIZ MINUTES de compil*/
#define RRO_COM_Radio_RSSI       (30020)        /*COM Radio_RSSI*/
#define RRO_COM_FIN       (30020)        /*COM FIN*/
#define RRO_FAV_DEBUT       (31001)        /*FAV DEBUT*/
#define RRO_FAV_FAV_Flags_interne       (31001)        /*FAV FAV_Flags_interne*/
#define RRO_FAV_Backup_Netid_HI       (31002)        /*FAV Backup_Netid_HI*/
#define RRO_FAV_Backup_Netid_LO       (31003)        /*FAV Backup_Netid_LO*/
#define RRO_FAV_Backup_KEYZ1       (31004)        /*FAV Backup_KEYZ1*/
#define RRO_FAV_Backup_3       (31005)        /*FAV Backup_3*/
#define RRO_FAV_Backup_4       (31006)        /*FAV Backup_4*/
#define RRO_FAV_Backup_5       (31007)        /*FAV Backup_5*/
#define RRO_FAV_Backup_6       (31008)        /*FAV Backup_6*/
#define RRO_FAV_Backup_7       (31009)        /*FAV Backup_7*/
#define RRO_FAV_Backup_8       (31010)        /*FAV Backup_8*/
#define RRO_FAV_Backup_9       (31011)        /*FAV Backup_9*/
#define RRO_FAV_Backup_10       (31012)        /*FAV Backup_10*/
#define RRO_FAV_Backup_11       (31013)        /*FAV Backup_11*/
#define RRO_FAV_Backup_12       (31014)        /*FAV Backup_12*/
#define RRO_FAV_Backup_13       (31015)        /*FAV Backup_13*/
#define RRO_FAV_Backup_14       (31016)        /*FAV Backup_14*/
#define RRO_FAV_Backup_15       (31017)        /*FAV Backup_15*/
#define RRO_FAV_Backup_16       (31018)        /*FAV Backup_16*/
#define RRO_FAV_Backup_17       (31019)        /*FAV Backup_17*/
#define RRO_FAV_Backup_18       (31020)        /*FAV Backup_18*/
#define RRO_FAV_Backup_19       (31021)        /*FAV Backup_19*/
#define RRO_FAV_Backup_20       (31022)        /*FAV Backup_20*/
#define RRO_FAV_Backup_21       (31023)        /*FAV Backup_21*/
#define RRO_FAV_Backup_22       (31024)        /*FAV Backup_22*/
#define RRO_FAV_Backup_23       (31025)        /*FAV Backup_23*/
#define RRO_FAV_Backup_24       (31026)        /*FAV Backup_24*/
#define RRO_FAV_Backup_25       (31027)        /*FAV Backup_25*/
#define RRO_FAV_Backup_26       (31028)        /*FAV Backup_26*/
#define RRO_FAV_Backup_27       (31029)        /*FAV Backup_27*/
#define RRO_FAV_Backup_28       (31030)        /*FAV Backup_28*/
#define RRO_FAV_Backup_29       (31031)        /*FAV Backup_29*/
#define RRO_FAV_Backup_30       (31032)        /*FAV Backup_30*/
#define RRO_FAV_Backup_31       (31033)        /*FAV Backup_31*/
#define RRO_FAV_Backup_32       (31034)        /*FAV Backup_32*/
#define RRO_FAV_Backup_33       (31035)        /*FAV Backup_33*/
#define RRO_FAV_Backup_34       (31036)        /*FAV Backup_34*/
#define RRO_FAV_Backup_35       (31037)        /*FAV Backup_35*/
#define RRO_FAV_Backup_36       (31038)        /*FAV Backup_36*/
#define RRO_FAV_Backup_37       (31039)        /*FAV Backup_37*/
#define RRO_FAV_Backup_38       (31040)        /*FAV Backup_38*/
#define RRO_FAV_Backup_39       (31041)        /*FAV Backup_39*/
#define RRO_FAV_Backup_40       (31042)        /*FAV Backup_40*/
#define RRO_FAV_Backup_41       (31043)        /*FAV Backup_41*/
#define RRO_FAV_Backup_42       (31044)        /*FAV Backup_42*/
#define RRO_FAV_Backup_43       (31045)        /*FAV Backup_43*/
#define RRO_FAV_Backup_44       (31046)        /*FAV Backup_44*/
#define RRO_FAV_Backup_45       (31047)        /*FAV Backup_45*/
#define RRO_FAV_Backup_46       (31048)        /*FAV Backup_46*/
#define RRO_FAV_Backup_47       (31049)        /*FAV Backup_47*/
#define RRO_FAV_Backup_48       (31050)        /*FAV Backup_48*/
#define RRO_FAV_Backup_49       (31051)        /*FAV Backup_49*/
#define RRO_FAV_Backup_50       (31052)        /*FAV Backup_50*/
#define RRO_FAV_Backup_51       (31053)        /*FAV Backup_51*/
#define RRO_FAV_Backup_52       (31054)        /*FAV Backup_52*/
#define RRO_FAV_Backup_53       (31055)        /*FAV Backup_53*/
#define RRO_FAV_Backup_54       (31056)        /*FAV Backup_54*/
#define RRO_FAV_Backup_55       (31057)        /*FAV Backup_55*/
#define RRO_FAV_Backup_56       (31058)        /*FAV Backup_56*/
#define RRO_FAV_Backup_57       (31059)        /*FAV Backup_57*/
#define RRO_FAV_Backup_58       (31060)        /*FAV Backup_58*/
#define RRO_FAV_Backup_59       (31061)        /*FAV Backup_59*/
#define RRO_FAV_Backup_60       (31062)        /*FAV Backup_60*/
#define RRO_FAV_Backup_61       (31063)        /*FAV Backup_61*/
#define RRO_FAV_Backup_62       (31064)        /*FAV Backup_62*/
#define RRO_FAV_Backup_63       (31065)        /*FAV Backup_63*/
#define RRO_FAV_Backup_64       (31066)        /*FAV Backup_64*/
#define RRO_FAV_Backup_65       (31067)        /*FAV Backup_65*/
#define RRO_FAV_Backup_66       (31068)        /*FAV Backup_66*/
#define RRO_FAV_Backup_67       (31069)        /*FAV Backup_67*/
#define RRO_FAV_Backup_68       (31070)        /*FAV Backup_68*/
#define RRO_FAV_Backup_69       (31071)        /*FAV Backup_69*/
#define RRO_FAV_Backup_70       (31072)        /*FAV Backup_70*/
#define RRO_FAV_Backup_71       (31073)        /*FAV Backup_71*/
#define RRO_FAV_FIN       (31073)        /*FAV FIN*/

#define RRO_FAV_DEBUT_SIGCOM        (31200)        /*FAV DEBUT_SIGCOM */
#define RRO_FAV_G1_TEMP_T1       (31200)        /*FAV_G1 TEMP_T1*/
#define RRO_FAV_G1_TEMP_T2       (31201)        /*FAV_G1 TEMP_T2*/
#define RRO_FAV_G1_TD_Z1       (31202)        /*FAV_G1 TD_Z1*/
#define RRO_FAV_G1_TD_Z2       (31203)        /*FAV_G1 TD_Z2*/
#define RRO_FAV_G1_TD_Z3       (31204)        /*FAV_G1 TD_Z3*/
#define RRO_FAV_G1_TEMP_T4       (31205)        /*FAV_G1 TEMP_T4*/
#define RRO_FAV_G1_VITESSE_TURBINE       (31206)        /*FAV_G1 VITESSE_TURBINE*/
#define RRO_FAV_G1_KWH_SAN_5min       (31207)        /*FAV_G1 KWH_SAN_5min*/
#define RRO_FAV_G1_KWH_CHF_5min       (31208)        /*FAV_G1 KWH_CHF_5min*/
#define RRO_FAV_G1_KWH_ENT_5min       (31209)        /*FAV_G1 KWH_ENT_5min*/
#define RRO_FAV_G1_PUISSANCE_INSTANT_BRULEUR       (31210)        /*FAV_G1 PUISSANCE_INSTANT_BRULEUR*/
#define RRO_FAV_G1_AUTO_MANU       (31211)        /*FAV_G1 AUTO_MANU*/
#define RRO_FAV_G1_LIBRE_31212       (31212)        /*FAV_G1 LIBRE_31212*/
#define RRO_FAV_G1_LIBRE_31213       (31213)        /*FAV_G1 LIBRE_31213*/
#define RRO_FAV_G1_LIBRE_31214       (31214)        /*FAV_G1 LIBRE_31214*/
#define RRO_FAV_G1_LIBRE_31215       (31215)        /*FAV_G1 LIBRE_31215*/
#define RRO_FAV_G1_LIBRE_31216       (31216)        /*FAV_G1 LIBRE_31216*/
#define RRO_FAV_G1_LIBRE_31217       (31217)        /*FAV_G1 LIBRE_31217*/
#define RRO_FAV_G1_TAMB_Z1       (31218)        /*FAV_G1 TAMB_Z1*/
#define RRO_FAV_G1_TAMB_Z2       (31219)        /*FAV_G1 TAMB_Z2*/
#define RRO_FAV_G1_TAMB_Z3       (31220)        /*FAV_G1 TAMB_Z3*/
#define RRO_FAV_G1_CD_Z1       (31221)        /*FAV_G1 CD_Z1*/
#define RRO_FAV_G1_CD_Z2       (31222)        /*FAV_G1 CD_Z2*/
#define RRO_FAV_G1_CD_Z3       (31223)        /*FAV_G1 CD_Z3*/
#define RRO_FAV_G1_CAMB_Z1       (31224)        /*FAV_G1 0*/
#define RRO_FAV_G1_CAMB_Z2       (31225)        /*FAV_G1 CAMB_Z2*/
#define RRO_FAV_G1_CAMB_Z3       (31226)        /*FAV_G1 CAMB_Z3*/
#define RRO_FAV_G1_T_EXT       (31227)        /*FAV_G1 T_EXT*/
#define RRO_FAV_G2_TEMP_T1       (31228)        /*FAV_G2 TEMP_T1*/
#define RRO_FAV_G2_TEMP_T2       (31229)        /*FAV_G2 TEMP_T2*/
#define RRO_FAV_G2_TD_Z1       (31230)        /*FAV_G2 TD_Z1*/
#define RRO_FAV_G2_TD_Z2       (31231)        /*FAV_G2 TD_Z2*/
#define RRO_FAV_G2_TD_Z3       (31232)        /*FAV_G2 TD_Z3*/
#define RRO_FAV_G2_TEMP_T4       (31233)        /*FAV_G2 TEMP_T4*/
#define RRO_FAV_G2_VITESSE_TURBINE       (31234)        /*FAV_G2 VITESSE_TURBINE*/
#define RRO_FAV_G2_KWH_SAN_5min       (31235)        /*FAV_G2 KWH_SAN_5min*/
#define RRO_FAV_G2_KWH_CHF_5min       (31236)        /*FAV_G2 KWH_CHF_5min*/
#define RRO_FAV_G2_KWH_ENT_5min       (31237)        /*FAV_G2 KWH_ENT_5min*/
#define RRO_FAV_G2_PUISSANCE_INSTANT_BRULEUR       (31238)        /*FAV_G2 PUISSANCE_INSTANT_BRULEUR*/
#define RRO_FAV_G2_AUTO_MANU       (31239)        /*FAV_G2 AUTO_MANU*/
#define RRO_FAV_G2_LIBRE_31240       (31240)        /*FAV_G2 LIBRE_31240*/
#define RRO_FAV_G2_LIBRE_31241       (31241)        /*FAV_G2 LIBRE_31241*/
#define RRO_FAV_G2_LIBRE_31242       (31242)        /*FAV_G2 LIBRE_31242*/
#define RRO_FAV_G2_LIBRE_31243       (31243)        /*FAV_G2 LIBRE_31243*/
#define RRO_FAV_G2_LIBRE_31244       (31244)        /*FAV_G2 LIBRE_31244*/
#define RRO_FAV_G2_LIBRE_31245       (31245)        /*FAV_G2 LIBRE_31245*/
#define RRO_FAV_G2_LIBRE_31246       (31246)        /*FAV_G2 LIBRE_31246*/
#define RRO_FAV_G2_LIBRE_31247       (31247)        /*FAV_G2 LIBRE_31247*/
#define RRO_FAV_G2_LIBRE_31248       (31248)        /*FAV_G2 LIBRE_31248*/
#define RRO_FAV_G2_LIBRE_31249       (31249)        /*FAV_G2 LIBRE_31249*/
#define RRO_FAV_G2_LIBRE_31250       (31250)        /*FAV_G2 LIBRE_31250*/
#define RRO_FAV_G2_LIBRE_31251       (31251)        /*FAV_G2 LIBRE_31251*/
#define RRO_FAV_G2_LIBRE_31252       (31252)        /*FAV_G2 LIBRE_31252*/
#define RRO_FAV_G2_LIBRE_31253       (31253)        /*FAV_G2 LIBRE_31253*/
#define RRO_FAV_G2_DATE_HEURE_CHAUDIERE_LO       (31254)        /*FAV_G2 DATE_HEURE_CHAUDIERE_LO*/
#define RRO_FAV_G2_DATE_HEURE_CHAUDIERE_HI       (31255)        /*FAV_G2 DATE_HEURE_CHAUDIERE_HI*/
#define RRO_FAV_GA_ACTION_CONTACT       (31256)        /*FAV_GA ACTION_CONTACT*/
#define RRO_FAV_GA_H_Z1       (31257)        /*FAV_GA H_Z1*/
#define RRO_FAV_GA_TD_MAXI_Z1       (31258)        /*FAV_GA TD_MAXI_Z1*/
#define RRO_FAV_GA_H_Z2       (31259)        /*FAV_GA H_Z2*/
#define RRO_FAV_GA_TD_MAXI_Z2       (31260)        /*FAV_GA TD_MAXI_Z2*/
#define RRO_FAV_GA_H_Z3       (31261)        /*FAV_GA H_Z3*/
#define RRO_FAV_GA_TD_MAXI_Z3       (31262)        /*FAV_GA TD_MAXI_Z3*/
#define RRO_FAV_GA_CONFIG_CHAUDIERE       (31263)        /*FAV_GA CONFIG_CHAUDIERE*/
#define RRO_FAV_GA_COEFF_CONSO       (31264)        /*FAV_GA COEFF_CONSO*/
#define RRO_FAV_GA_KWH_SAN_24H       (31265)        /*FAV_GA KWH_SAN_24H*/
#define RRO_FAV_GA_KWH_CHF_24H       (31266)        /*FAV_GA KWH_CHF_24H*/
#define RRO_FAV_GA_MINUTES_SOL_24H       (31267)        /*FAV_GA MINUTES_SOL_24H*/
#define RRO_FAV_GA_KWH_PAC_24H       (31268)        /*FAV_GA KWH_PAC_24H*/
#define RRO_FAV_GA_KWH_PAC_ELEC_24H       (31269)        /*FAV_GA KWH_PAC_ELEC_24H*/
#define RRO_FAV_GA_KWH_BOIS_24H       (31270)        /*FAV_GA KWH_BOIS_24H*/
#define RRO_FAV_GA_KWH_AUTRE_24H       (31271)        /*FAV_GA KWH_AUTRE_24H*/
#define RRO_FAV_GA_ALARMES_NODEID_0       (31272)        /*FAV_GA ALARMES_NODEID_0*/
#define RRO_FAV_GA_ALARMES_NODEID_1       (31273)        /*FAV_GA ALARMES_NODEID_1*/
#define RRO_FAV_GA_ALARMES_NODEID_2       (31274)        /*FAV_GA ALARMES_NODEID_2*/
#define RRO_FAV_GA_ALARMES_NODEID_3       (31275)        /*FAV_GA ALARMES_NODEID_3*/
#define RRO_FAV_GA_ALARMES_NODEID_4       (31276)        /*FAV_GA ALARMES_NODEID_4*/
#define RRO_FAV_GA_ALARMES_NODEID_5       (31277)        /*FAV_GA ALARMES_NODEID_5*/
#define RRO_FAV_GA_ALARMES_NODEID_6       (31278)        /*FAV_GA ALARMES_NODEID_6*/
#define RRO_FAV_GA_ALARMES_NODEID_7       (31279)        /*FAV_GA ALARMES_NODEID_7*/
#define RRO_FAV_GA_VERSION_L_0       (31280)        /*FAV_GA VERSION_L_0*/
#define RRO_FAV_GA_VERSION_H_0       (31281)        /*FAV_GA VERSION_H_0*/
#define RRO_FAV_GA_VERSION_L_1       (31282)        /*FAV_GA VERSION_L_1*/
#define RRO_FAV_GA_VERSION_H_1       (31283)        /*FAV_GA VERSION_H_1*/
#define RRO_FAV_GB_VERSION_L_2       (31284)        /*FAV_GB VERSION_L_2*/
#define RRO_FAV_GB_VERSION_H_2       (31285)        /*FAV_GB VERSION_H_2*/
#define RRO_FAV_GB_VERSION_L_3       (31286)        /*FAV_GB VERSION_L_3*/
#define RRO_FAV_GB_VERSION_H_3       (31287)        /*FAV_GB VERSION_H_3*/
#define RRO_FAV_GB_VERSION_L_4       (31288)        /*FAV_GB VERSION_L_4*/
#define RRO_FAV_GB_VERSION_H_4       (31289)        /*FAV_GB VERSION_H_4*/
#define RRO_FAV_GB_VERSION_L_5       (31290)        /*FAV_GB VERSION_L_5*/
#define RRO_FAV_GB_VERSION_H_5       (31291)        /*FAV_GB VERSION_H_5*/
#define RRO_FAV_GB_VERSION_L_6       (31292)        /*FAV_GB VERSION_L_6*/
#define RRO_FAV_GB_VERSION_H_6       (31293)        /*FAV_GB VERSION_H_6*/
#define RRO_FAV_GB_VERSION_L_7       (31294)        /*FAV_GB VERSION_L_7*/
#define RRO_FAV_GB_VERSION_H_7       (31295)        /*FAV_GB VERSION_H_7*/
#define RRO_FAV_GB_VERSION_L_8       (31296)        /*FAV_GB VERSION_L_8*/
#define RRO_FAV_GB_VERSION_H_8       (31297)        /*FAV_GB VERSION_H_8*/
#define RRO_FAV_GB_VERSION_L_9       (31298)        /*FAV_GB VERSION_L_9*/
#define RRO_FAV_GB_VERSION_H_9       (31299)        /*FAV_GB VERSION_H_9*/
#define RRO_FAV_GB_VERSION_L_10       (31300)        /*FAV_GB VERSION_L_10*/
#define RRO_FAV_GB_VERSION_H_10       (31301)        /*FAV_GB VERSION_H_10*/
#define RRO_FAV_GB_VERSION_L_11       (31302)        /*FAV_GB VERSION_L_11*/
#define RRO_FAV_GB_VERSION_H_11       (31303)        /*FAV_GB VERSION_H_11*/
#define RRO_FAV_GB_VERSION_L_12       (31304)        /*FAV_GB VERSION_L_12*/
#define RRO_FAV_GB_VERSION_H_12       (31305)        /*FAV_GB VERSION_H_12*/
#define RRO_FAV_GB_VERSION_L_13       (31306)        /*FAV_GB VERSION_L_13*/
#define RRO_FAV_GB_VERSION_H_13       (31307)        /*FAV_GB VERSION_H_13*/
#define RRO_FAV_GB_VERSION_L_14       (31308)        /*FAV_GB VERSION_L_14*/
#define RRO_FAV_GB_VERSION_H_14       (31309)        /*FAV_GB VERSION_H_14*/
#define RRO_FAV_GB_VERSION_L_15       (31310)        /*FAV_GB VERSION_L_15*/
#define RRO_FAV_GB_VERSION_H_15       (31311)        /*FAV_GB VERSION_H_15*/
#define RRO_FAV_GC_KWH_MENSUEL_CHF_HI       (31312)        /*FAV_GC KWH_MENSUEL_CHF_HI*/
#define RRO_FAV_GC_KWH_MENSUEL_CHF_LO       (31313)        /*FAV_GC KWH_MENSUEL_CHF_LO*/
#define RRO_FAV_GC_KWH_MENSUEL_SAN_HI       (31314)        /*FAV_GC KWH_MENSUEL_SAN_HI*/
#define RRO_FAV_GC_KWH_MENSUEL_SAN_LO       (31315)        /*FAV_GC KWH_MENSUEL_SAN_LO*/
#define RRO_FAV_GC_LIBRE_31316       (31316)        /*FAV_GC LIBRE_31316*/
#define RRO_FAV_GC_LIBRE_31317       (31317)        /*FAV_GC LIBRE_31317*/
#define RRO_FAV_GC_LIBRE_31318       (31318)        /*FAV_GC LIBRE_31318*/
#define RRO_FAV_GC_LIBRE_31319       (31319)        /*FAV_GC LIBRE_31319*/
#define RRO_FAV_GC_LIBRE_31320       (31320)        /*FAV_GC LIBRE_31320*/
#define RRO_FAV_GC_LIBRE_31321       (31321)        /*FAV_GC LIBRE_31321*/
#define RRO_FAV_GC_LIBRE_31322       (31322)        /*FAV_GC LIBRE_31322*/
#define RRO_FAV_GC_LIBRE_31323       (31323)        /*FAV_GC LIBRE_31323*/
#define RRO_FAV_GC_LIBRE_31324       (31324)        /*FAV_GC LIBRE_31324*/
#define RRO_FAV_GC_LIBRE_31325       (31325)        /*FAV_GC LIBRE_31325*/
#define RRO_FAV_GC_LIBRE_31326       (31326)        /*FAV_GC LIBRE_31326*/
#define RRO_FAV_GC_LIBRE_31327       (31327)        /*FAV_GC LIBRE_31327*/
#define RRO_FAV_GC_LIBRE_31328       (31328)        /*FAV_GC LIBRE_31328*/
#define RRO_FAV_GC_LIBRE_31329       (31329)        /*FAV_GC LIBRE_31329*/
#define RRO_FAV_GC_LIBRE_31330       (31330)        /*FAV_GC LIBRE_31330*/
#define RRO_FAV_GC_LIBRE_31331       (31331)        /*FAV_GC LIBRE_31331*/
#define RRO_FAV_GC_LIBRE_31332       (31332)        /*FAV_GC LIBRE_31332*/
#define RRO_FAV_GC_LIBRE_31333       (31333)        /*FAV_GC LIBRE_31333*/
#define RRO_FAV_GC_LIBRE_31334       (31334)        /*FAV_GC LIBRE_31334*/
#define RRO_FAV_GC_LIBRE_31335       (31335)        /*FAV_GC LIBRE_31335*/
#define RRO_FAV_GC_LIBRE_31336       (31336)        /*FAV_GC LIBRE_31336*/
#define RRO_FAV_GC_LIBRE_31337       (31337)        /*FAV_GC LIBRE_31337*/
#define RRO_FAV_GC_LIBRE_31338       (31338)        /*FAV_GC LIBRE_31338*/
#define RRO_FAV_GC_LIBRE_31339       (31339)        /*FAV_GC LIBRE_31339*/
#define RRO_FAV_FIN_SIGCOM        (31339)        /*FAV FIN_SIGCOM */

#define RRO_RAG2_DEBUT       (32001)        /*RAG2 DEBUT*/
#define RRO_RAG2_g_T1       (32001)        /*RAG2 g_T1*/
#define RRO_RAG2_g_T2       (32002)        /*RAG2 g_T2*/
#define RRO_RAG2_g_T3       (32003)        /*RAG2 g_T3*/
#define RRO_RAG2_g_T4       (32004)        /*RAG2 g_T4*/
#define RRO_RAG2_g_CT1       (32005)        /*RAG2 g_CT1*/
#define RRO_RAG2_g_CT2       (32006)        /*RAG2 g_CT2*/
#define RRO_RAG2_g_CT3       (32007)        /*RAG2 g_CT3*/
#define RRO_RAG2_g_COMPTETOURS       (32008)        /*RAG2 g_COMPTETOURS*/
#define RRO_RAG2_g_Debitair       (32009)        /*RAG2 g_Debitair*/
#define RRO_RAG2_g_Dpairgaz       (32010)        /*RAG2 g_Dpairgaz*/
#define RRO_RAG2_uiConsigneCptrs       (32011)        /*RAG2 uiConsigneCptrs*/
#define RRO_RAG2_g_ConsigneAir       (32012)        /*RAG2 g_ConsigneAir*/
#define RRO_RAG2_g_CSPOW_g_CDPSTA       (32013)        /*RAG2 g_CSPOW g_CDPSTA*/
#define RRO_RAG2_SAlarm2_SAlarm1       (32014)        /*RAG2 SAlarm2 SAlarm1*/
#define RRO_RAG2_SAlarm4_SAlarm3       (32015)        /*RAG2 SAlarm4 SAlarm3*/
#define RRO_RAG2_SFlagsFonction1       (32016)        /*RAG2 giEAU SFlagsFonction1*/
#define RRO_RAG2_g_EAU       (32017)        /*RAG2 g_EAU*/
#define RRO_RAG2_MidFlValue       (32018)        /*RAG2 MidFlValue*/
#define RRO_RAG2_g_Mode2_g_Mode       (32019)        /*RAG2 g_Mode2 g_Mode*/
#define RRO_RAG2_ucTypeBruleur       (32020)        /*RAG2     ucTypeBruleur*/
#define RRO_RAG2_FIN       (32020)        /*RAG2 FIN*/
#define RRO_PACSSC_DEBUT       (33001)        /*PACSSC DEBUT*/
#define RRO_PACSSC_g_T1       (33001)        /*PACSSC g_T1*/
#define RRO_PACSSC_g_T2       (33002)        /*PACSSC g_T2*/
#define RRO_PACSSC_g_T3       (33003)        /*PACSSC g_T3*/
#define RRO_PACSSC_g_T4       (33004)        /*PACSSC g_T4*/
#define RRO_PACSSC_g_T5       (33005)        /*PACSSC g_T5*/
#define RRO_PACSSC_g_T6       (33006)        /*PACSSC g_T6*/
#define RRO_PACSSC_g_T7       (33007)        /*PACSSC g_T7*/
#define RRO_PACSSC_g_T8       (33008)        /*PACSSC g_T8*/
#define RRO_PACSSC_g_T9       (33009)        /*PACSSC g_T9*/
#define RRO_PACSSC_g_T10       (33010)        /*PACSSC g_T10*/
#define RRO_PACSSC_g_EAU       (33011)        /*PACSSC g_EAU*/
#define RRO_PACSSC_g_EAUG       (33012)        /*PACSSC g_EAUG*/
#define RRO_PACSSC_g_QEAU       (33013)        /*PACSSC g_QEAU*/
#define RRO_PACSSC_g_QEAUG       (33014)        /*PACSSC g_QEAUG*/
#define RRO_PACSSC_g_Text       (33015)        /*PACSSC g_Text*/
#define RRO_PACSSC_g_CT6       (33016)        /*PACSSC g_CT6*/
#define RRO_PACSSC_SAlarm1       (33017)        /*PACSSC SAlarm1*/
#define RRO_PACSSC_SAlarm2       (33018)        /*PACSSC SAlarm2*/
#define RRO_PACSSC_SAlarm3       (33019)        /*PACSSC SAlarm3*/
#define RRO_PACSSC_SAlarm4       (33020)        /*PACSSC SAlarm4*/
#define RRO_PACSSC_ucDil_Energie       (33021)        /*PACSSC ucDil_Energie*/
#define RRO_PACSSC_ucDil_Auxiliaire       (33022)        /*PACSSC ucDil_Auxiliaire*/
#define RRO_PACSSC_Puissance_instantanee_en_kW       (33023)        /*PACSSC Puissance_instantanee_en_kW*/
#define RRO_PACSSC_FIN       (33023)        /*PACSSC FIN*/
#define RRO_SIGMA_DEBUT       (33501)        /*SIGMA DEBUT*/
#define RRO_SIGMA_Temperature_de_Depart       (33501)        /*SIGMA Temperature_de_Depart*/
#define RRO_SIGMA_Temperature_Exterieure       (33502)        /*SIGMA Temperature_Exterieure*/
#define RRO_SIGMA_Sonde ext       (33503)        /*SIGMA 0*/
#define RRO_SIGMA_Dil_Config       (33504)        /*SIGMA 0*/
#define RRO_SIGMA_FIN       (33504)        /*SIGMA FIN*/
#define RRO_SIG_SOL_DEBUT       (33601)        /*SIG_SOL DEBUT*/
#define RRO_SIG_SOL_Alarmes       (33601)        /*SIG_SOL Alarmes*/
#define RRO_SIG_SOL_Dil_Config       (33602)        /*SIG_SOL Dil_Config*/
#define RRO_SIG_SOL_T_Ballon_Milieu       (33603)        /*SIG_SOL T_Ballon_Milieu*/
#define RRO_SIG_SOL_T_Ballon_Haut       (33604)        /*SIG_SOL T_Ballon_Haut*/
#define RRO_SIG_SOL_T_Ballon_Bas       (33605)        /*SIG_SOL T_Ballon_Bas*/
#define RRO_SIG_SOL_T_Capteur       (33606)        /*SIG_SOL T_Capteur*/
#define RRO_SIG_SOL_T_Ext       (33607)        /*SIG_SOL T_Ext*/
#define RRO_SIG_SOL_Modulation_pompe       (33608)        /*SIG_SOL Modulation_pompe*/
#define RRO_SIG_SOL_FIN       (33608)        /*SIG_SOL FIN*/
#define RRO_SIG_COLL_DEBUT       (33701)        /*SIG_COLL DEBUT*/
#define RRO_SIG_COLL_Alarmes       (33701)        /*SIG_COLL Alarmes*/
#define RRO_SIG_COLL_Dil_Config       (33702)        /*SIG_COLL Dil_Config*/
#define RRO_SIG_COLL_Temperature_de_Depart       (33703)        /*SIG_COLL Temperature_de_Depart*/
#define RRO_SIG_COLL_Temperature_Ballon       (33704)        /*SIG_COLL Temperature_Ballon*/
#define RRO_SIG_COLL_Temperature_exterieure       (33705)        /*SIG_COLL Temperature_exterieure*/
#define RRO_SIG_COLL_Temperature_Bouteille       (33706)        /*SIG_COLL Temperature_Bouteille*/
#define RRO_SIG_COLL_FIN       (33706)        /*SIG_COLL FIN*/

#define RRW_Dummy     (40000) /*COM DEBUT*/
#define RRW_Dummy     (40000) /*COM Dummy pour lire si on n'a rien a ecrire dans cette zone*/
#define RRW_COM_Adresse_Tracker_01       (40001)        /*COM Adresse_Tracker_01*/
#define RRW_COM_Adresse_Tracker_02       (40002)        /*COM Adresse_Tracker_02*/
#define RRW_COM_Adresse_Tracker_03       (40003)        /*COM Adresse_Tracker_03*/
#define RRW_COM_Adresse_Tracker_04       (40004)        /*COM Adresse_Tracker_04*/
#define RRW_COM_Adresse_Tracker_05       (40005)        /*COM Adresse_Tracker_05*/
#define RRW_COM_Adresse_Tracker_06       (40006)        /*COM Adresse_Tracker_06*/
#define RRW_COM_Adresse_Tracker_07       (40007)        /*COM Adresse_Tracker_07*/
#define RRW_COM_Adresse_Tracker_08       (40008)        /*COM Adresse_Tracker_08*/
#define RRW_COM_Adresse_Tracker_09       (40009)        /*COM Adresse_Tracker_09*/
#define RRW_COM_Adresse_Tracker_10       (40010)        /*COM Adresse_Tracker_10*/
#define RRW_COM_Adresse_Tracker_11       (40011)        /*COM Adresse_Tracker_11*/
#define RRW_COM_Adresse_Tracker_12       (40012)        /*COM Adresse_Tracker_12*/
#define RRW_COM_Adresse_Tracker_13       (40013)        /*COM Adresse_Tracker_13*/
#define RRW_COM_Adresse_Tracker_14       (40014)        /*COM Adresse_Tracker_14*/
#define RRW_COM_Adresse_Tracker_15       (40015)        /*COM Adresse_Tracker_15*/
#define RRW_COM_Adresse_Tracker_16       (40016)        /*COM Adresse_Tracker_16*/
#define RRW_COM_TEST1       (40017)        /*COM DIZ ANNEE du test*/
#define RRW_COM_TEST2       (40018)        /*COM DIZ JOUR du test*/
#define RRW_COM_TEST3       (40019)        /*COM DIZ MINUTES du test*/
#define RRW_COM_RTC_ANNEE_MOIS       (40020)        /*COM DIZ ANNEE*/
#define RRW_COM_RTC_JOUR_HEURE       (40021)        /*COM DIZ JOUR*/
#define RRW_COM_RTC_MINUTES_SECONDES       (40022)        /*COM DIZ MINUTES*/
#define RRW_COM_RTC_WEEKDAY       (40023)        /*COMLangue 0:FR,1:IT,2:GB,3:RU,4:SP,5:PRC Jour semaine 0:DIMANCHE*/
#define RRW_COM_NETID_MSB       (40024)        /*COM NETID_MSB*/
#define RRW_COM_NETID_LSB       (40025)        /*COM NETID_LSB*/
#define RRW_COM_NODEID       (40026)        /*COM NODEID*/
#define RRW_COM_FIN       (40026)        /*COM FIN*/
#define RRW_FAV_DEBUT       (41001)        /*FAV DEBUT*/
#define RRW_FAV_Temperature_Exterieure       (41001)        /*FAV Temperature_Exterieure*/
#define RRW_FAV_Libre_41002       (41002)        /*FAV Libre_41002*/
#define RRW_FAV_RTC_ANNEE_MOIS       (41003)        /*FAV DIZ ANNEE*/
#define RRW_FAV_RTC_JOUR_HEURE       (41004)        /*FAV DIZ JOUR*/
#define RRW_FAV_RTC_MINUTES_SECONDES       (41005)        /*FAV DIZ MINUTES*/
#define RRW_FAV_RTC_WEEKDAY       (41006)        /*FAVLangue 0:FR,1:IT,2:GB,3:RU,4:SP,5:PRC Jour semaine 0:DIMANCHE*/
#define RRW_FAV_Thermostat_LCD_Zone_1_Ambiance       (41007)        /*FAV Thermostat_LCD_Zone_1_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_1_Consigne       (41008)        /*FAV Thermostat_LCD_Zone_1_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_1_Flags       (41009)        /*FAV Thermostat_LCD_Zone_1_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_1_CdZ_write       (41010)        /*FAV Thermostat_LCD_Zone_1_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_1_CdZ_read       (41011)        /*FAV Thermostat_LCD_Zone_1_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_2_Ambiance       (41012)        /*FAV Thermostat_LCD_Zone_2_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_2_Consigne       (41013)        /*FAV Thermostat_LCD_Zone_2_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_2_Flags       (41014)        /*FAV Thermostat_LCD_Zone_2_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_2_CdZ_write       (41015)        /*FAV Thermostat_LCD_Zone_2_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_2_CdZ_read       (41016)        /*FAV Thermostat_LCD_Zone_2_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_3_Ambiance       (41017)        /*FAV Thermostat_LCD_Zone_3_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_3_Consigne       (41018)        /*FAV Thermostat_LCD_Zone_3_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_3_Flags       (41019)        /*FAV Thermostat_LCD_Zone_3_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_3_CdZ_write       (41020)        /*FAV Thermostat_LCD_Zone_3_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_3_CdZ_read       (41021)        /*FAV Thermostat_LCD_Zone_3_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_4_Ambiance       (41022)        /*FAV Thermostat_LCD_Zone_4_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_4_Consigne       (41023)        /*FAV Thermostat_LCD_Zone_4_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_4_Flags       (41024)        /*FAV Thermostat_LCD_Zone_4_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_4_CdZ_write       (41025)        /*FAV Thermostat_LCD_Zone_4_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_4_CdZ_read       (41026)        /*FAV Thermostat_LCD_Zone_4_CdZ_read*/
#define RRW_FAV_Temperature_Exterieure_5A8       (41027)        /*FAV Temperature_Exterieure_5A8*/
#define RRW_FAV_Libre_41028       (41028)        /*FAV Libre_41028*/
#define RRW_FAV_RTC_ANNEE_MOIS_5A8       (41029)        /*FAV ,*/
#define RRW_FAV_RTC_JOUR_HEURE_5A8       (41030)        /*FAV DIZ JOUR*/
#define RRW_FAV_RTC_MINUTES_SECONDES_5A8       (41031)        /*FAV DIZ MINUTES*/
#define RRW_FAV_RTC_WEEKDAY_5A8       (41032)        /*FAVLangue 0:FR,1:IT,2:GB,3:RU,4:SP,5:PRC Jour semaine 0:DIMANCHE*/
#define RRW_FAV_Thermostat_LCD_Zone_5_Ambiance       (41033)        /*FAV Thermostat_LCD_Zone_5_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_5_Consigne       (41034)        /*FAV Thermostat_LCD_Zone_5_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_5_Flags       (41035)        /*FAV Thermostat_LCD_Zone_5_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_5_CdZ_write       (41036)        /*FAV Thermostat_LCD_Zone_5_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_5_CdZ_read       (41037)        /*FAV Thermostat_LCD_Zone_5_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_6_Ambiance       (41038)        /*FAV Thermostat_LCD_Zone_6_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_6_Consigne       (41039)        /*FAV Thermostat_LCD_Zone_6_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_6_Flags       (41040)        /*FAV Thermostat_LCD_Zone_6_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_6_CdZ_write       (41041)        /*FAV Thermostat_LCD_Zone_6_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_6_CdZ_read       (41042)        /*FAV Thermostat_LCD_Zone_6_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_7_Ambiance       (41043)        /*FAV Thermostat_LCD_Zone_7_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_7_Consigne       (41044)        /*FAV Thermostat_LCD_Zone_7_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_7_Flags       (41045)        /*FAV Thermostat_LCD_Zone_7_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_7_CdZ_write       (41046)        /*FAV Thermostat_LCD_Zone_7_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_7_CdZ_read       (41047)        /*FAV Thermostat_LCD_Zone_7_CdZ_read*/
#define RRW_FAV_Thermostat_LCD_Zone_8_Ambiance       (41048)        /*FAV Thermostat_LCD_Zone_8_Ambiance*/
#define RRW_FAV_Thermostat_LCD_Zone_8_Consigne       (41049)        /*FAV Thermostat_LCD_Zone_8_Consigne*/
#define RRW_FAV_Thermostat_LCD_Zone_8_Flags       (41050)        /*FAV Thermostat_LCD_Zone_8_Flags*/
#define RRW_FAV_Thermostat_LCD_Zone_8_CdZ_write       (41051)        /*FAV Thermostat_LCD_Zone_8_CdZ_write*/
#define RRW_FAV_Thermostat_LCD_Zone_8_CdZ_read       (41052)        /*FAV Thermostat_LCD_Zone_8_CdZ_read*/
#define RRW_FAV_Piscine_CT2       (41053)        /*FAV Piscine_CT2*/
#define RRW_FAV_RAG_g_T2       (41054)        /*FAV RAG_g_T2*/
#define RRW_FAV_Flags_Solaire       (41055)        /*FAV Flags_Solaire*/
#define RRW_FAV_Piscine_2_CT2       (41056)        /*FAV Piscine_2_CT2*/
#define RRW_FAV_Charge_Maitre_CT2       (41057)        /*FAV Charge_Maitre_CT2*/
#define RRW_FAV_Charge_2_CT2       (41058)        /*FAV Charge_2_CT2*/
#define RRW_FAV_kWh_N_Chf_1       (41059)        /*FAV kWh_N_Chf_1*/
#define RRW_FAV_kWh_N_San_1       (41060)        /*FAV kWh_N_San_1*/
#define RRW_FAV_kWh_N_Chf_2       (41061)        /*FAV kWh_N_Chf_2*/
#define RRW_FAV_kWh_N_San_2       (41062)        /*FAV kWh_N_San_2*/
#define RRW_FAV_kWh_N_Chf_3       (41063)        /*FAV kWh_N_Chf_3*/
#define RRW_FAV_kWh_N_San_3       (41064)        /*FAV kWh_N_San_3*/
#define RRW_FAV_kWh_N_Chf_4       (41065)        /*FAV kWh_N_Chf_4*/
#define RRW_FAV_kWh_N_San_4       (41066)        /*FAV kWh_N_San_4*/
#define RRW_FAV_kWh_N_Chf_5       (41067)        /*FAV kWh_N_Chf_5*/
#define RRW_FAV_kWh_N_San_5       (41068)        /*FAV kWh_N_San_5*/
#define RRW_FAV_kWh_N_Chf_6       (41069)        /*FAV kWh_N_Chf_6*/
#define RRW_FAV_kWh_N_San_6       (41070)        /*FAV kWh_N_San_6*/
#define RRW_FAV_kWh_N_Chf_7       (41071)        /*FAV kWh_N_Chf_7*/
#define RRW_FAV_kWh_N_San_7       (41072)        /*FAV kWh_N_San_7*/
#define RRW_FAV_kWh_N_Chf_8       (41073)        /*FAV kWh_N_Chf_8*/
#define RRW_FAV_kWh_N_San_8       (41074)        /*FAV kWh_N_San_8*/
#define RRW_FAV_kWh_N_Chf_9       (41075)        /*FAV kWh_N_Chf_9*/
#define RRW_FAV_kWh_N_San_9       (41076)        /*FAV kWh_N_San_9*/
#define RRW_FAV_kWh_N_Chf_10       (41077)        /*FAV kWh_N_Chf_10*/
#define RRW_FAV_kWh_N_San_10       (41078)        /*FAV kWh_N_San_10*/
#define RRW_FAV_kWh_N_Chf_11       (41079)        /*FAV kWh_N_Chf_11*/
#define RRW_FAV_kWh_N_San_11       (41080)        /*FAV kWh_N_San_11*/
#define RRW_FAV_kWh_N_Chf_12       (41081)        /*FAV kWh_N_Chf_12*/
#define RRW_FAV_kWh_N_San_12       (41082)        /*FAV kWh_N_San_12*/
#define RRW_FAV_kWh_N_1_Chf_1       (41083)        /*FAV kWh_N_1_Chf_1*/
#define RRW_FAV_kWh_N_1_San_1       (41084)        /*FAV kWh_N_1_San_1*/
#define RRW_FAV_kWh_N_1_Chf_2       (41085)        /*FAV kWh_N_1_Chf_2*/
#define RRW_FAV_kWh_N_1_San_2       (41086)        /*FAV kWh_N_1_San_2*/
#define RRW_FAV_kWh_N_1_Chf_3       (41087)        /*FAV kWh_N_1_Chf_3*/
#define RRW_FAV_kWh_N_1_San_3       (41088)        /*FAV kWh_N_1_San_3*/
#define RRW_FAV_kWh_N_1_Chf_4       (41089)        /*FAV kWh_N_1_Chf_4*/
#define RRW_FAV_kWh_N_1_San_4       (41090)        /*FAV kWh_N_1_San_4*/
#define RRW_FAV_kWh_N_1_Chf_5       (41091)        /*FAV kWh_N_1_Chf_5*/
#define RRW_FAV_kWh_N_1_San_5       (41092)        /*FAV kWh_N_1_San_5*/
#define RRW_FAV_kWh_N_1_Chf_6       (41093)        /*FAV kWh_N_1_Chf_6*/
#define RRW_FAV_kWh_N_1_San_6       (41094)        /*FAV kWh_N_1_San_6*/
#define RRW_FAV_kWh_N_1_Chf_7       (41095)        /*FAV kWh_N_1_Chf_7*/
#define RRW_FAV_kWh_N_1_San_7       (41096)        /*FAV kWh_N_1_San_7*/
#define RRW_FAV_kWh_N_1_Chf_8       (41097)        /*FAV kWh_N_1_Chf_8*/
#define RRW_FAV_kWh_N_1_San_8       (41098)        /*FAV kWh_N_1_San_8*/
#define RRW_FAV_kWh_N_1_Chf_9       (41099)        /*FAV kWh_N_1_Chf_9*/
#define RRW_FAV_kWh_N_1_San_9       (41100)        /*FAV kWh_N_1_San_9*/
#define RRW_FAV_kWh_N_1_Chf_10       (41101)        /*FAV kWh_N_1_Chf_10*/
#define RRW_FAV_kWh_N_1_San_10       (41102)        /*FAV kWh_N_1_San_10*/
#define RRW_FAV_kWh_N_1_Chf_11       (41103)        /*FAV kWh_N_1_Chf_11*/
#define RRW_FAV_kWh_N_1_San_11       (41104)        /*FAV kWh_N_1_San_11*/
#define RRW_FAV_kWh_N_1_Chf_12       (41105)        /*FAV kWh_N_1_Chf_12*/
#define RRW_FAV_kWh_N_1_San_12       (41106)        /*FAV kWh_N_1_San_12*/
#define RRW_FAV_KWH_SOL_24H       (41107)        /*FAV KWH_SOL_24H*/
#define RRW_FAV_MISE_A_L_HEURE1       (41108)        /*FAV MISE_A_L_HEURE1*/
#define RRW_FAV_MISE_A_L_HEURE2       (41109)        /*FAV MISE_A_L_HEURE2*/
#define RRW_FAV_MISE_A_L_HEURE3       (41110)        /*FAV MISE_A_L_HEURE3*/
#define RRW_FAV_MISE_A_L_HEURE4       (41111)        /*FAV MISE_A_L_HEURE4*/
#define RRW_FAV_HEURE_AUTO       (41112)        /*FAV HEURE_AUTO*/
#define RRW_FAV_HYSTERESIS       (41113)        /*FAV HYSTERESIS*/
#define RRW_FAV_TD_MINI       (41114)        /*FAV TD_MINI*/
#define RRW_FAV_Nk       (41115)        /*FAV Nk*/
#define RRW_FAV_FIN       (41115)        /*FAV FIN*/

#define RRW_FAV_DEBUT_SIGCOM       (41200)        /*FAV DEBUT_SIGCOM*/
#define RRW_FAV_DATE_DEP_VACANCES1       (41200)        /*FAV DATE_DEP_VACANCES1*/
#define RRW_FAV_DATE_DEP_VACANCES2       (41201)        /*FAV DATE_DEP_VACANCES2*/
#define RRW_FAV_DATE_DEP_VACANCES3       (41202)        /*FAV DATE_DEP_VACANCES3*/
#define RRW_FAV_DATE_DEP_VACANCES4       (41203)        /*FAV DATE_DEP_VACANCES4*/
#define RRW_FAV_DATE_RET_VACANCES1       (41204)        /*FAV DATE_RET_VACANCES1*/
#define RRW_FAV_DATE_RET_VACANCES2       (41205)        /*FAV DATE_RET_VACANCES2*/
#define RRW_FAV_DATE_RET_VACANCES3       (41206)        /*FAV DATE_RET_VACANCES3*/
#define RRW_FAV_DATE_RET_VACANCES4       (41207)        /*FAV DATE_RET_VACANCES4*/
#define RRW_FAV_DATE_ENTRETIEN1       (41208)        /*FAV DATE_ENTRETIEN1*/
#define RRW_FAV_DATE_ENTRETIEN2       (41209)        /*FAV DATE_ENTRETIEN2*/
#define RRW_FAV_DATE_ENTRETIEN3       (41210)        /*FAV DATE_ENTRETIEN3*/
#define RRW_FAV_DATE_ENTRETIEN4       (41211)        /*FAV DATE_ENTRETIEN4*/
#define RRW_FAV_MODE_ECS       (41212)        /*FAV MODE_ECS*/
#define RRW_FAV_Id_chaudiere_2       (41213)        /*FAV Id_chaudiere_2*/
#define RRW_FAV_Id_chaudiere_4       (41214)        /*FAV Id_chaudiere_4*/
#define RRW_FAV_Id_chaudiere_6       (41215)        /*FAV Id_chaudiere_6*/
#define RRW_FAV_Id_chaudiere_8       (41216)        /*FAV Id_chaudiere_8*/
#define RRW_FAV_Id_chaudiere_10       (41217)        /*FAV Id_chaudiere_10*/
#define RRW_FAV_Id_chaudiere_12       (41218)        /*FAV Id_chaudiere_12*/
#define RRW_FAV_Id_chaudiere_14       (41219)        /*FAV Id_chaudiere_14*/
#define RRW_FAV_Id_chaudiere_16       (41220)        /*FAV Id_chaudiere_16*/
#define RRW_FAV_FIN_SIGCOM       (41220)        /*FAV FIN_SIGCOM*/

#define RRW_SAT1_SC_DEBUT       (41300)        /*SAT1_SC DEBUT*/
#define RRW_SAT1_SC_CONS_CONF_Z1       (41300)        /*SAT1_SC CONS_CONF_Z1*/
#define RRW_SAT1_SC_CONS_HG_Z1       (41301)        /*SAT1_SC CONS_HG_Z1*/
#define RRW_SAT1_SC_b0       (41302)        /*SAT1_SC 0*/
#define RRW_SAT1_SC_SAT1_Prog1       (41303)        /*SAT1_SC SAT1_Prog1*/
#define RRW_SAT1_SC_SAT1_Prog2       (41304)        /*SAT1_SC SAT1_Prog2*/
#define RRW_SAT1_SC_SAT1_Prog3       (41305)        /*SAT1_SC SAT1_Prog3*/
#define RRW_SAT1_SC_SAT1_Prog4       (41306)        /*SAT1_SC SAT1_Prog4*/
#define RRW_SAT1_SC_SAT1_Prog5       (41307)        /*SAT1_SC SAT1_Prog5*/
#define RRW_SAT1_SC_SAT1_Prog6       (41308)        /*SAT1_SC SAT1_Prog6*/
#define RRW_SAT1_SC_SAT1_Prog7       (41309)        /*SAT1_SC SAT1_Prog7*/
#define RRW_SAT1_SC_SAT1_Prog8       (41310)        /*SAT1_SC SAT1_Prog8*/
#define RRW_SAT1_SC_SAT1_Prog9       (41311)        /*SAT1_SC SAT1_Prog9*/
#define RRW_SAT1_SC_SAT1_Prog10       (41312)        /*SAT1_SC SAT1_Prog10*/
#define RRW_SAT1_SC_SAT1_Prog11       (41313)        /*SAT1_SC SAT1_Prog11*/
#define RRW_SAT1_SC_SAT1_Prog12       (41314)        /*SAT1_SC SAT1_Prog12*/
#define RRW_SAT1_SC_SAT1_Prog13       (41315)        /*SAT1_SC SAT1_Prog13*/
#define RRW_SAT1_SC_SAT1_Prog14       (41316)        /*SAT1_SC SAT1_Prog14*/
#define RRW_SAT1_SC_SAT1_Prog15       (41317)        /*SAT1_SC SAT1_Prog15*/
#define RRW_SAT1_SC_SAT1_Prog16       (41318)        /*SAT1_SC SAT1_Prog16*/
#define RRW_SAT1_SC_SAT1_Prog17       (41319)        /*SAT1_SC SAT1_Prog17*/
#define RRW_SAT1_SC_SAT1_Prog18       (41320)        /*SAT1_SC SAT1_Prog18*/
#define RRW_SAT1_SC_SAT1_Prog19       (41321)        /*SAT1_SC SAT1_Prog19*/
#define RRW_SAT1_SC_SAT1_Prog20       (41322)        /*SAT1_SC SAT1_Prog20*/
#define RRW_SAT1_SC_SAT1_Prog21       (41323)        /*SAT1_SC SAT1_Prog21*/
#define RRW_SAT1_SC_FIN       (41323)        /*SAT1_SC FIN*/

#define RRW_FAV_SOL_DEBUT       (41400)        /*FAV_SOL DEBUT*/
#define RRW_FAV_SOL_MODE_ECS       (41400)        /*FAV_SOL MODE_ECS*/
#define RRW_FAV_SOL_FIN       (41400)        /*FAV_SOL FIN*/

#define RRW_THM_DEBUT       (41501)        /*THM DEBUT*/
#define RRW_THM_Magic       (41501)        /*THM Magic*/
#define RRW_THM_Backup_0       (41502)        /*THM Backup_0*/
#define RRW_THM_Backup_1       (41503)        /*THM Backup_1*/
#define RRW_THM_Backup_2       (41504)        /*THM Backup_2*/
#define RRW_THM_Backup_3       (41505)        /*THM Backup_3*/
#define RRW_THM_Backup_4       (41506)        /*THM Backup_4*/
#define RRW_THM_Backup_5       (41507)        /*THM Backup_5*/
#define RRW_THM_Backup_6       (41508)        /*THM Backup_6*/
#define RRW_THM_Backup_7       (41509)        /*THM Backup_7*/
#define RRW_THM_Backup_8       (41510)        /*THM Backup_8*/
#define RRW_THM_Backup_9       (41511)        /*THM Backup_9*/
#define RRW_THM_Backup_10       (41512)        /*THM Backup_10*/
#define RRW_THM_Backup_11       (41513)        /*THM Backup_11*/
#define RRW_THM_Backup_12       (41514)        /*THM Backup_12*/
#define RRW_THM_Backup_13       (41515)        /*THM Backup_13*/
#define RRW_THM_Backup_14       (41516)        /*THM Backup_14*/
#define RRW_THM_Backup_15       (41517)        /*THM Backup_15*/
#define RRW_THM_Backup_16       (41518)        /*THM Backup_16*/
#define RRW_THM_Backup_17       (41519)        /*THM Backup_17*/
#define RRW_THM_Backup_18       (41520)        /*THM Backup_18*/
#define RRW_THM_Backup_19       (41521)        /*THM Backup_19*/
#define RRW_THM_Backup_20       (41522)        /*THM Backup_20*/
#define RRW_THM_Backup_21       (41523)        /*THM Backup_21*/
#define RRW_THM_Backup_22       (41524)        /*THM Backup_22*/
#define RRW_THM_Backup_23       (41525)        /*THM Backup_23*/
#define RRW_THM_Backup_24       (41526)        /*THM Backup_24*/
#define RRW_THM_Backup_25       (41527)        /*THM Backup_25*/
#define RRW_THM_Backup_26       (41528)        /*THM Backup_26*/
#define RRW_THM_Backup_27       (41529)        /*THM Backup_27*/
#define RRW_THM_Backup_28       (41530)        /*THM Backup_28*/
#define RRW_THM_Backup_29       (41531)        /*THM Backup_29*/
#define RRW_THM_Backup_30       (41532)        /*THM Backup_30*/
#define RRW_THM_Backup_31       (41533)        /*THM Backup_31*/
#define RRW_THM_Backup_32       (41534)        /*THM Backup_32*/
#define RRW_THM_Backup_33       (41535)        /*THM Backup_33*/
#define RRW_THM_Backup_34       (41536)        /*THM Backup_34*/
#define RRW_THM_Backup_35       (41537)        /*THM Backup_35*/
#define RRW_THM_Backup_36       (41538)        /*THM Backup_36*/
#define RRW_THM_Backup_37       (41539)        /*THM Backup_37*/
#define RRW_THM_Backup_38       (41540)        /*THM Backup_38*/
#define RRW_THM_Backup_39       (41541)        /*THM Backup_39*/
#define RRW_THM_Backup_40       (41542)        /*THM Backup_40*/
#define RRW_THM_Backup_41       (41543)        /*THM Backup_41*/
#define RRW_THM_Backup_42       (41544)        /*THM Backup_42*/
#define RRW_THM_Backup_43       (41545)        /*THM Backup_43*/
#define RRW_THM_Backup_44       (41546)        /*THM Backup_44*/
#define RRW_THM_Backup_45       (41547)        /*THM Backup_45*/
#define RRW_THM_Backup_46       (41548)        /*THM Backup_46*/
#define RRW_THM_Backup_47       (41549)        /*THM Backup_47*/
#define RRW_THM_Backup_48       (41550)        /*THM Backup_48*/
#define RRW_THM_Backup_49       (41551)        /*THM Backup_49*/
#define RRW_THM_Backup_50       (41552)        /*THM Backup_50*/
#define RRW_THM_Backup_51       (41553)        /*THM Backup_51*/
#define RRW_THM_Backup_52       (41554)        /*THM Backup_52*/
#define RRW_THM_Backup_53       (41555)        /*THM Backup_53*/
#define RRW_THM_Backup_54       (41556)        /*THM Backup_54*/
#define RRW_THM_Backup_55       (41557)        /*THM Backup_55*/
#define RRW_THM_Backup_56       (41558)        /*THM Backup_56*/
#define RRW_THM_Backup_57       (41559)        /*THM Backup_57*/
#define RRW_THM_Backup_58       (41560)        /*THM Backup_58*/
#define RRW_THM_Backup_59       (41561)        /*THM Backup_59*/
#define RRW_THM_Backup_60       (41562)        /*THM Backup_60*/
#define RRW_THM_Backup_61       (41563)        /*THM Backup_61*/
#define RRW_THM_Backup_62       (41564)        /*THM Backup_62*/
#define RRW_THM_Backup_63       (41565)        /*THM Backup_63*/
#define RRW_THM_Backup_64       (41566)        /*THM Backup_64*/
#define RRW_THM_Backup_65       (41567)        /*THM Backup_65*/
#define RRW_THM_Backup_66       (41568)        /*THM Backup_66*/
#define RRW_THM_Backup_67       (41569)        /*THM Backup_67*/
#define RRW_THM_Backup_68       (41570)        /*THM Backup_68*/
#define RRW_THM_Backup_69       (41571)        /*THM Backup_69*/
#define RRW_THM_Backup_70       (41572)        /*THM Backup_70*/
#define RRW_THM_Backup_71       (41573)        /*THM Backup_71*/
#define RRW_THM_FIN       (41573)        /*THM FIN*/
#define RRW_RAG2_DEBUT       (42002)        /*RAG2 DEBUT*/
#define RRW_RAG2_Consigne_Zone1_10EME       (42002)        /*RAG2 Consigne_Zone1_10EME*/
#define RRW_RAG2_Consigne_Zone2_10EME       (42003)        /*RAG2 Consigne_Zone2_10EME*/
#define RRW_RAG2_Plus_haute_consigne_1_10EME       (42004)        /*RAG2 Plus_haute_consigne_1_10EME*/
#define RRW_RAG2_Telecommande_Rag       (42005)        /*RAG2 Telecommande_Rag*/
#define RRW_RAG2_Temp_T1_FAV       (42006)        /*RAG2 Temp_T1_FAV*/
#define RRW_RAG2_FIN       (42006)        /*RAG2 FIN*/
#define RRW_PACSSC_DEBUT       (43001)        /*PACSSC DEBUT*/
#define RRW_PACSSC_g_CT3_Man       (43001)        /*PACSSC g_CT3_Man*/
#define RRW_PACSSC_g_CT3_v4v_Z1       (43002)        /*PACSSC g_CT3_v4v_Z1*/
#define RRW_PACSSC_g_CT3_radio_Z2       (43003)        /*PACSSC g_CT3_radio_Z2*/
#define RRW_PACSSC_g_CT3_la_plus_haute_ZX       (43004)        /*PACSSC g_CT3_la_plus_haute_ZX*/
#define RRW_PACSSC_ucDilSwitch       (43005)        /*PACSSC ucDilSwitch*/
#define RRW_PACSSC_g_ModeX       (43006)        /*PACSSC g_ModeX*/
#define RRW_PACSSC_FIN       (43006)        /*PACSSC FIN*/
#define RRW_SIGMA_DEBUT       (43501)        /*SIGMA DEBUT*/
#define RRW_SIGMA_Mod_Hyd_Consigne_de_Depart       (43501)        /*SIGMA Mod_Hyd_Consigne_de_Depart*/
#define RRW_SIGMA_Mod_Hyd_Flags       (43502)        /*SIGMA Mod_Hyd_Flags*/
#define RRW_SIGMA_FIN       (43502)        /*SIGMA FIN*/
#define RRW_SIG_SOL_DEBUT       (43601)        /*SIG_SOL DEBUT*/
#define RRW_SIG_SOL_Mode_Manuel       (43601)        /*SIG_SOL Mode_Manuel*/
#define RRW_SIG_SOL_Dils_Fav       (43602)        /*SIG_SOL Dils_Fav*/
#define RRW_SIG_SOL_Dt_On       (43602)        /*SIG_SOL Dt_On*/
#define RRW_SIG_SOL_Dt_Off       (43604)        /*SIG_SOL Dt_Off*/
#define RRW_SIG_SOL_Lt_Capteur       (43605)        /*SIG_SOL Lt_Capteur*/
#define RRW_SIG_SOL_Ct_Ballon       (43606)        /*SIG_SOL Ct_Ballon*/
#define RRW_SIG_SOL_Lt_Ballon       (43607)        /*SIG_SOL Lt_Ballon*/
#define RRW_SIG_SOL_FIN       (43607)        /*SIG_SOL FIN*/
#define RRW_SIG_COLL_DEBUT       (43701)        /*SIG_COLL DEBUT*/
#define RRW_SIG_COLL_Mode_Manuel       (43701)        /*SIG_COLL Mode_Manuel*/
#define RRW_SIG_COLL_Dils_Fav       (43702)        /*SIG_COLL Dils_Fav*/
#define RRW_SIG_COLL_Mod_Col_Consigne_de_Depart       (43703)        /*SIG_COLL Mod_Col_Consigne_de_Depart*/
#define RRW_SIG_COLL_Mod_Col_Flags       (43704)        /*SIG_COLL Mod_Col_Flags*/
#define RRW_SIG_COLL_Temp_T1_FAV       (43705)        /*SIG_COLL Temp_T1_FAV*/
#define RRW_SIG_COLL_Thermostat_LCD_Zone_1_Ambiance       (43706)        /*SIG_COLL Thermostat_LCD_Zone_1_Ambiance*/
#define RRW_SIG_COLL_Magic       (43707)        /*SIG_COLL Magic*/
#define RRW_SIG_COLL_Backup_0       (43708)        /*SIG_COLL Backup_0*/
#define RRW_SIG_COLL_Backup_1       (43709)        /*SIG_COLL Backup_1*/
#define RRW_SIG_COLL_Backup_2       (43710)        /*SIG_COLL Backup_2*/
#define RRW_SIG_COLL_Backup_3       (43711)        /*SIG_COLL Backup_3*/
#define RRW_SIG_COLL_Backup_4       (43712)        /*SIG_COLL Backup_4*/
#define RRW_SIG_COLL_Backup_5       (43713)        /*SIG_COLL Backup_5*/
#define RRW_SIG_COLL_Backup_6       (43714)        /*SIG_COLL Backup_6*/
#define RRW_SIG_COLL_Backup_7       (43715)        /*SIG_COLL Backup_7*/
#define RRW_SIG_COLL_Backup_8       (43716)        /*SIG_COLL Backup_8*/
#define RRW_SIG_COLL_Backup_9       (43717)        /*SIG_COLL Backup_9*/
#define RRW_SIG_COLL_Backup_10       (43718)        /*SIG_COLL Backup_10*/
#define RRW_SIG_COLL_Backup_11       (43719)        /*SIG_COLL Backup_11*/
#define RRW_SIG_COLL_Backup_12       (43720)        /*SIG_COLL Backup_12*/
#define RRW_SIG_COLL_Backup_13       (43721)        /*SIG_COLL Backup_13*/
#define RRW_SIG_COLL_Backup_14       (43722)        /*SIG_COLL Backup_14*/
#define RRW_SIG_COLL_Backup_15       (43723)        /*SIG_COLL Backup_15*/
#define RRW_SIG_COLL_Backup_16       (43724)        /*SIG_COLL Backup_16*/
#define RRW_SIG_COLL_Backup_17       (43725)        /*SIG_COLL Backup_17*/
#define RRW_SIG_COLL_Backup_18       (43726)        /*SIG_COLL Backup_18*/
#define RRW_SIG_COLL_Backup_19       (43727)        /*SIG_COLL Backup_19*/
#define RRW_SIG_COLL_Backup_20       (43728)        /*SIG_COLL Backup_20*/
#define RRW_SIG_COLL_Backup_21       (43729)        /*SIG_COLL Backup_21*/
#define RRW_SIG_COLL_Backup_22       (43730)        /*SIG_COLL Backup_22*/
#define RRW_SIG_COLL_Backup_23       (43731)        /*SIG_COLL Backup_23*/
#define RRW_SIG_COLL_Backup_24       (43732)        /*SIG_COLL Backup_24*/
#define RRW_SIG_COLL_Backup_25       (43733)        /*SIG_COLL Backup_25*/
#define RRW_SIG_COLL_Backup_26       (43734)        /*SIG_COLL Backup_26*/
#define RRW_SIG_COLL_Backup_27       (43735)        /*SIG_COLL Backup_27*/
#define RRW_SIG_COLL_Backup_28       (43736)        /*SIG_COLL Backup_28*/
#define RRW_SIG_COLL_Backup_29       (43737)        /*SIG_COLL Backup_29*/
#define RRW_SIG_COLL_Backup_30       (43738)        /*SIG_COLL Backup_30*/
#define RRW_SIG_COLL_Backup_31       (43739)        /*SIG_COLL Backup_31*/
#define RRW_SIG_COLL_Backup_32       (43740)        /*SIG_COLL Backup_32*/
#define RRW_SIG_COLL_Backup_33       (43741)        /*SIG_COLL Backup_33*/
#define RRW_SIG_COLL_Backup_34       (43742)        /*SIG_COLL Backup_34*/
#define RRW_SIG_COLL_Backup_35       (43743)        /*SIG_COLL Backup_35*/
#define RRW_SIG_COLL_Backup_36       (43744)        /*SIG_COLL Backup_36*/
#define RRW_SIG_COLL_Backup_37       (43745)        /*SIG_COLL Backup_37*/
#define RRW_SIG_COLL_Backup_38       (43746)        /*SIG_COLL Backup_38*/
#define RRW_SIG_COLL_Backup_39       (43747)        /*SIG_COLL Backup_39*/
#define RRW_SIG_COLL_Backup_40       (43748)        /*SIG_COLL Backup_40*/
#define RRW_SIG_COLL_Backup_41       (43749)        /*SIG_COLL Backup_41*/
#define RRW_SIG_COLL_Backup_42       (43750)        /*SIG_COLL Backup_42*/
#define RRW_SIG_COLL_Backup_43       (43751)        /*SIG_COLL Backup_43*/
#define RRW_SIG_COLL_Backup_44       (43752)        /*SIG_COLL Backup_44*/
#define RRW_SIG_COLL_Backup_45       (43753)        /*SIG_COLL Backup_45*/
#define RRW_SIG_COLL_Backup_46       (43754)        /*SIG_COLL Backup_46*/
#define RRW_SIG_COLL_Backup_47       (43755)        /*SIG_COLL Backup_47*/
#define RRW_SIG_COLL_Backup_48       (43756)        /*SIG_COLL Backup_48*/
#define RRW_SIG_COLL_Backup_49       (43757)        /*SIG_COLL Backup_49*/
#define RRW_SIG_COLL_Backup_50       (43758)        /*SIG_COLL Backup_50*/
#define RRW_SIG_COLL_Backup_51       (43759)        /*SIG_COLL Backup_51*/
#define RRW_SIG_COLL_Backup_52       (43760)        /*SIG_COLL Backup_52*/
#define RRW_SIG_COLL_Backup_53       (43761)        /*SIG_COLL Backup_53*/
#define RRW_SIG_COLL_Backup_54       (43762)        /*SIG_COLL Backup_54*/
#define RRW_SIG_COLL_Backup_55       (43763)        /*SIG_COLL Backup_55*/
#define RRW_SIG_COLL_Backup_56       (43764)        /*SIG_COLL Backup_56*/
#define RRW_SIG_COLL_Backup_57       (43765)        /*SIG_COLL Backup_57*/
#define RRW_SIG_COLL_Backup_58       (43766)        /*SIG_COLL Backup_58*/
#define RRW_SIG_COLL_Backup_59       (43767)        /*SIG_COLL Backup_59*/
#define RRW_SIG_COLL_Backup_60       (43768)        /*SIG_COLL Backup_60*/
#define RRW_SIG_COLL_Backup_61       (43769)        /*SIG_COLL Backup_61*/
#define RRW_SIG_COLL_Backup_62       (43770)        /*SIG_COLL Backup_62*/
#define RRW_SIG_COLL_Backup_63       (43771)        /*SIG_COLL Backup_63*/
#define RRW_SIG_COLL_Backup_64       (43772)        /*SIG_COLL Backup_64*/
#define RRW_SIG_COLL_Backup_65       (43773)        /*SIG_COLL Backup_65*/
#define RRW_SIG_COLL_Backup_66       (43774)        /*SIG_COLL Backup_66*/
#define RRW_SIG_COLL_Backup_67       (43775)        /*SIG_COLL Backup_67*/
#define RRW_SIG_COLL_Backup_68       (43776)        /*SIG_COLL Backup_68*/
#define RRW_SIG_COLL_Backup_69       (43777)        /*SIG_COLL Backup_69*/
#define RRW_SIG_COLL_Backup_70       (43778)        /*SIG_COLL Backup_70*/
#define RRW_SIG_COLL_Backup_71       (43779)        /*SIG_COLL Backup_71*/
#define RRW_SIG_COLL_FIN       (43779)        /*SIG_COLL FIN*/

/* Liste des NodeId */
#define NID_Broadcast_0      (0)
#define NID_Carte_En_Test      (1)
#define NID_Libre_2      (2)
#define NID_Thermostat_LCD_Zone_1      (8)
#define NID_Thermostat_LCD_Zone_2      (9)
#define NID_Thermostat_LCD_Zone_3      (10)
#define NID_Thermostat_LCD_Zone_4      (11)
#define NID_Thermostat_LCD_Zone_5      (12)
#define NID_Thermostat_LCD_Zone_6      (13)
#define NID_Thermostat_LCD_Zone_7      (14)
#define NID_Thermostat_LCD_Zone_8      (15)
#define NID_Thermostat_sans_LCD_Zone_1      (16)
#define NID_Thermostat_sans_LCD_Zone_2      (17)
#define NID_Thermostat_sans_LCD_Zone_3      (18)
#define NID_Thermostat_sans_LCD_Zone_4      (19)
#define NID_Report_Alarme      (20)
#define NID_Report_Pompe      (21)
#define NID_Module_Piscine      (22)
#define NID_Module_Piscine_2      (23)
#define NID_Unite_ext_PAC_1      (24)
#define NID_Unite_ext_PAC_2      (25)
#define NID_Unite_ext_PAC_3      (26)
#define NID_Unite_ext_PAC_4      (27)
#define NID_Sonde_Zone_1      (32)
#define NID_Sonde_Zone_2      (33)
#define NID_Sonde_Zone_3      (34)
#define NID_Sonde_Zone_4      (35)
#define NID_Module_hyd_temp_ext_zone_1      (36)
#define NID_Module_hyd_temp_ext_zone_2      (37)
#define NID_Module_hyd_temp_ext_zone_3      (38)
#define NID_Module_hyd_temp_ext_zone_4      (39)
#define NID_Module_hydraulique_zone_1      (40)
#define NID_Module_hydraulique_zone_2      (41)
#define NID_Module_hydraulique_zone_3      (42)
#define NID_Module_hydraulique_zone_4      (43)
#define NID_Module_hydraulique_zone_5      (44)
#define NID_Module_hydraulique_zone_6      (45)
#define NID_Module_hydraulique_zone_7      (46)
#define NID_Module_hydraulique_zone_8      (47)
#define NID_Charge_Maitre      (48)
#define NID_Charge_2      (49)
#define NID_Rif_Sext      (50)
#define NID_Libre_51      (51)
#define NID_Libre_52      (52)
#define NID_Libre_53      (53)
#define NID_Libre_54      (54)
#define NID_Libre_55      (55)
#define NID_FAV_Solaire      (56)
#define NID_Chaudiere_esclave_1      (57)
#define NID_Chaudiere_esclave_2      (58)
#define NID_Chaudiere_esclave_3      (59)
#define NID_Chaudiere_esclave_4      (60)
#define NID_Chaudiere_esclave_5      (61)
#define NID_Chaudiere_esclave_6      (62)
#define NID_Chaudiere_esclave_7      (63)
#define NID_Chaudiere_esclave_8      (64)
#define NID_Chaudiere_esclave_9      (65)
#define NID_Module_collectif_1      (66)
#define NID_Module_collectif_2      (67)
#define NID_Module_collectif_3      (68)
#define NID_Module_collectif_4      (69)
#define NID_Module_collectif_5      (70)
#define NID_Module_collectif_6      (71)
#define NID_Module_collectif_7      (72)
#define NID_Module_collectif_8      (73)
#define NID_FAV_Collective_1      (74)
#define NID_FAV_Collective_2      (75)
#define NID_FAV_Collective_3      (76)
#define NID_FAV_Collective_4      (77)
#define NID_FAV_Collective_5      (78)
#define NID_FAV_Collective_6      (79)
#define NID_FAV_Collective_7      (80)
#define NID_FAV_Collective_8      (81)
#define NID_Pont_IP      (126)
#define NID_Tracker      (127)
#define NID_Chaudiere_maitre      (128)
#define NID_Chaufferie_maitre_1      (129)
#define NID_Chaufferie_maitre_2      (130)
#define NID_Chaudiere_maitre_Leds      (131)
#define NID_Banc_test      (253)
#define NID_Espion      (254)
#define NID_Broadcast_255      (255)

#define NID_MAITRE (0x80)       /* MSB a 1 definit un maitre */

/* Structures des messages */
#define SX1211_FIFO_SIZE  64    /* Taille en octets du message complet y compris sa longueur  */
#define SIZEOF_HEADER     6     /* Taille de la partie fixe entre l'octet taille et les donnees ModBus */
typedef struct {
uint8_t FunctionCode;
uint8_t Datas[SX1211_FIFO_SIZE - SIZEOF_HEADER - 1];  /* Soustrait la partie fixe du message emis et l'octet taille */
} PDU_MODBUS;
/* Structure de l'Application Protocol Unit pour un SX1211 */
typedef struct {
uint8_t Retries;    /* Nombre de tentatives de re emissions (non gere par le SX1211) */
uint8_t Taille;     /* Taille du message recu/emis par le SX1211 */
uint8_t Dest;       /* NodeId du destinataire (celui gere par le SX1211) */
uint8_t Exp;        /* NodeId de l'expediteur (celui gere par le layer 3) */
uint8_t Cle;        /* Cle de matching */
uint8_t Numero;     /* Numero d'ordre du message */
uint8_t Adresse;     /* Adresse ModBus */
PDU_MODBUS Modbus;  /* Message ModBus (58 octets) */
} APU_SX1211;

/* NETID reserves */
#define NETID_ZERO ((uint32_t)0x0)            /* Interdit par le SX1211 */
#define TESTING_NETID     ((uint32_t)0xFFFFFFFE)                      /* NETID utilise lors du test en prod */
/* NETID utilise lors du matching */
#ifdef SIMULATEUR
  #if(SIMULATEUR==1)   
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFFD)
  #elif(SIMULATEUR==2)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFFC)
  #elif(SIMULATEUR==3)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFFB)
  #elif(SIMULATEUR==4)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFFA)
  #elif(SIMULATEUR==5)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF9)
  #elif(SIMULATEUR==6)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF8)
  #elif(SIMULATEUR==7)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF7)
  #elif(SIMULATEUR==8)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF6)
  #elif(SIMULATEUR==9)    
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF5)
  #else
    #define MATCHING_NETID    ((uint32_t)0xFFFFFFF4)
  #endif
#else
  #define MATCHING_NETID    ((uint32_t)0xFFFFFFFF)                      
#endif

/*==================== FIN DU FICHIER  =========================*/
