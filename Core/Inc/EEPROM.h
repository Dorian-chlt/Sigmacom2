/*****************************************************************************************************
* Module definition against multiple inclusion
*****************************************************************************************************/
#ifndef __EEPROM__
#define __EEPROM__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>   // Pour printf
#include <string.h> 

/* Virtual EEPROM mapping */
#define EEP_ADR_USRMOD  &VarDataTab[0]               
#define EEP_ADR_IDKEYX  EEP_ADR_USRMOD + 1  
#define EEP_ADR_IDLANX  EEP_ADR_IDKEYX + 1  
#define EEP_ADR_IDNODE  EEP_ADR_IDLANX + 4
#define EEP_ADR_IDPART  EEP_ADR_IDNODE + 1

/* Virtual EEPROM initial default datas preset */
#define EEP_DEF_SMMODE  EEP_PRS_SMMODE
#define EEP_DEF_IDKEYX  EEP_PRS_IDKEYX
#define EEP_DEF_IDLANX  EEP_PRS_IDLANX
#define EEP_DEF_IDLAN0  ((uint8_t)(EEP_DEF_IDLANX >> 24))
#define EEP_DEF_IDLAN1  ((uint8_t)(EEP_DEF_IDLANX >> 16))
#define EEP_DEF_IDLAN2  ((uint8_t)(EEP_DEF_IDLANX >>  8))
#define EEP_DEF_IDLAN3  ((uint8_t)(EEP_DEF_IDLANX    ))
#define EEP_DEF_IDNODE  NID_Thermostat_LCD_Zone_1
#define EEP_DEF_IDPART  EEP_PRS_IDPART
#define EEP_DEF_OF7CTN  0
#define EEP_DEF_OF7PRO  0
#define EEP_DEF_CSESOL  TDEFSOL
#define EEP_DEF_CSELUN  TDEFLUN
#define EEP_DEF_CSEGEL  TDEFGEL
#define EEP_DEF_LVLCTR  EEP_PRS_LVLCTR
#define EEP_DEF_LUGDAY  TMINLUG
#define EEP_DEF_FREE02  0xFF
/* definition des valeurs pour la plage generique 05h00 à 23h00 */
#define EEP_DEF_PLAGE0  0x00
#define EEP_DEF_PLAGE1  0xFC
#define EEP_DEF_PLAGE2  0xFF
#define EEP_DEF_PLAGE3  0xFF
#define EEP_DEF_PLAGE4  0xFF
#define EEP_DEF_PLAGE5  0x3F
/* disponible */
#define EEP_DEF_FREE03  0xFF
#define EEP_DEF_FREE04  0xFF
#define EEP_DEF_FREE05  0xFF
#define EEP_DEF_FREE06  0xFF
#define EEP_DEF_FREE07  0xFF
#define EEP_DEF_FREE08  0x00 /* init à 00 pour la reconnaitre dans la memoire.*/

/* Paramètres de la programmation du chgt des consignes */
#define STEP         5 /* Step d'inc/dec */
#define OF7CSE5DEG  50 /* decalage de la Temp de cse de 5°C cad: (TMAXSOL)+50 = 300 en 1/10°C*/
                       /* decalage de la Temp de cse de 5°C cad: (TMINGEL)+50 =  50 en 1/10°C*/

#define CMAXSOL  300 /* consigne maxi à 30.0°C */
#define TMAXSOL  CMAXSOL-OF7CSE5DEG

#define CDEFSOL  190 /* consigne par defaut à 19.0°C */
#define TDEFSOL  CDEFSOL-OF7CSE5DEG

#define CDEFLUN  170 /* consigne par defaut à 17.0°C */
#define TDEFLUN  CDEFLUN-OF7CSE5DEG

#define CDEFGEL   80 /* consigne par defaut à  8.0°C */
#define TDEFGEL  CDEFGEL-OF7CSE5DEG

#define CMINGEL   50 /* consigne mini à 5°C */
#define TMINGEL  CMINGEL-OF7CSE5DEG

#define HYST_HGEL 20 /* Hysterisis Horsgel à 2°C */

/*  Functions */
extern void EEP_Update(void); /* Update virtual EEPROM contents from RAM */
extern void RAM_Update(void); /* Update RAM contents from virtual EEPROM */
extern uint8_t read_eeprom(uint8_t *Ptr, uint8_t Count, uint8_t *pBuff);
extern uint8_t write_eeprom(uint8_t *Ptr, uint8_t Count, uint8_t *pBuff);

/* Paramètres de la programmation valeur du contraste */
#define TMAXCTR   7     /* sur STM32 maxi 7 ald 15 */
#define TDEFCTR   4     /* contraste moyen */
#define TMINCTR   0     /* contraste mini */

/* Paramètres de la programmation du nbre de jours d'absence */
#define TMAXLUG   99    /* jours d'absence maxi */
#define TMINLUG   1     /* jours d'absence mini */

/* initialisation des paramètres de sbREGUL */
//REGULATOR_BLOCK Pregul = {0x4B0F,0x1455,0x0001};
#define EEP_DEF_REGUL1  0x4B0F
#define EEP_DEF_REGUL2  0x1455
#define EEP_DEF_REGUL3  0x0001

/* I Ambiance defaut 5 [0 - 10] */
#define AMBI_MAX                10
#define AMBI_DEF                5
#define AMBI_MIN                0

 /* H Hysteresis defaut 2 [0 - 7] mais affichage [1 - 8] */
#define HYST_MAX                7
#define HYST_DEF                2
#define HYST_MIN                0

/* K Pente si RAD defaut 1.6 [0.8 - 3.0] si PLA defaut 0.5 [0.3 - 1.3] */
#define PENT_MINR               8
#define PENT_DEFR               16
#define PENT_MAXR               30
#define PENT_MINP               3
//#define PENT_DEFP             5 /* Ready but unused */
#define PENT_MAXP               13

/* Saturations depart chaudiere par steps de 5°C */
#define SATU_MINZx              20 /* defaut Zx 20 */
#define SATU_MAXZ1R             85 /* defaut Z1 85 */
#define SATU_MAXZ1P             45 /* defaut Z1 45 */
//#define SATU_MAXZ2R             80 /* defaut Z2 80 *//* Ready but unused */
//#define SATU_MAXZ2P             45 /* defaut Z2 45 *//* Ready but unused */

/* Types de regulations */   
#define REGULATION_AMBIANCE           (0)   /* Ambiance */
#define REGULATION_AMBIANCE_ET_EXT    (1)   /* Ambiance et temp exterieure */  
#define REGULATION_EXT_SEULE          (2)   /* Temp. exterieure seule */
#define REGULATION_FILAIRE            (3)   /* Thermostat filaire */
#define REGULATION_OFF                (7)   /* Satellite non gere */

#endif