/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes
#include <libopencm3/stm32/gpio.h>

#include "dev_LCD_HD44780.h"
#include "LCD_HD44780.h"

#include "defines.h"
#include "led_f4.h"
#include "waitin.h"
#include "sensor_button.h"

#include "main_debug.h"
#include "main_line.h"
#include "main_sumo.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
// USART recieve buffer size
#define RBUFSZ 1024
// USART transmission buffer size
#define TBUFSZ 1024
// LCD data buffer size
#define LCD_DBUFSZ 1024
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
    //____________________________________________________
    // from main.c
extern FILE *fus;
extern uint8_t rbuf[RBUFSZ];
extern uint8_t tbuf[TBUFSZ];
extern FILE *flcd;
extern uint8_t lcd_dbuf[LCD_DBUFSZ];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS
/****************
 \brief   Selects with of the programs should be started depending on buttons
 ****************/
int main(void);

/****************
 \brief  Initializes lcd display
 \retval pointer to the LCD display devic - maybe for debug only in future
 ****************/
S_dev_lcd* INIT_dev_LCD(void);

/****************
 \brief  Initializes usart communication
 ****************/
void INIT_dev_usart(void);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



#endif // MAIN_H_INCLUDED
