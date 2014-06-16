/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	main_debug.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief      Debugging main function for development
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef _MAIN_DEBUG_H_
#define _MAIN_DEBUG_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes

#include "robot_config.h"
/*
#include <libopencm3/stm32/gpio.h>

#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <sys/types.h>

#include <stdio.h>
#include <stddef.h>

//#include <string.h>

#include "defines.h"
#include "robot_config.h"

#include "led_f4.h"
#include "dev_serial.h"

// LCD
#include "dev_LCD_HD44780.h"
#include "sensor_ultrasonic.h"
#include "LCD_HD44780.h"

#include "sensor_ultrasonic.h"
#include "waitin.h"
*/

//#include "main.h" // externs & macros of LCD, USART

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//____________________________________________________
//constants (do not change)
//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
typedef struct _S_robot S_robot; // forward declaration
int main_debug(S_robot* r);
/****************
 @brief
 ****************/
// actuators
void DBG_testActuators(S_robot*r, uint32_t reps);
// infra
void DBG_adc_finish(void);
void DBG_tryADC(S_robot* r);
void DBG_tryCNY70(S_robot* r);
// waitin
void DBG_ticTocSumoWait(S_robot* r);
// button
void DBG_touchD7blink(void);
void DBG_testButtonState(S_robot* r, uint32_t repeats,uint32_t ms);
//ultras
void DBG_testAllUltraDistance(S_robot*r, uint32_t reps);
void DBG_testUltraDistanceOld2(S_robot* r,uint32_t repeats);
void DBG_testUltraDistanceOld(S_robot* r,uint32_t repeats);
void DBG_testUltraDistance(S_robot* r,uint32_t repeats);
void DBG_ultraTest(void);
// other
void DBG_flash(void);

/****************
 @brief Shows digital clock on external character lcd in top-right corner
 -> actualizes them only every second
 ****************/
void CLOCK_digiPrint(S_robot* r);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



#endif // _MAIN_DEBUG_H_
