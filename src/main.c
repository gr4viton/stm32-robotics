/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */
/*
TODO:
[x] udelat jmennou konvenci a coding style
[] rozchodit interrupt
- od tlacitka
- od timeru
- od adc
[] rename main_line na robot_line a to same sumo
[] ultras serials_predef
[] rozchodit pwm
[] vytvorit mu projekt na gitu
[] dat ho jako subprojekt
[] pridat output do repraku
[] lcd
- dodelat seek
- vynechat mswait a dat to na pocet asmwait podle clocku procesoru!
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "main.h"
// move to headerfile


#define LCD_DBUFSZ 1024
#define RBUFSZ 1024
#define TBUFSZ 1024
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DEFINITIONS
//____________________________________________________
// static variables
//____________________________________________________
// other variables


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
int main(void)
{
    S_robot* r = &R; // global= extern S_robot R;
    INIT_clk();
    ROBOT_initButtons(r);

    // Chose a program to run
    E_lifeStyleSelector life = ROBOT_getLifeStyle(&R);
    switch(life)
    {
        case(IAM_BUGGED_ROBOT):
            main_debug(r); break;
        case(IAM_SUMO_WARRIOR):
            //main_sumo(); break;
        case(IAM_SHEEP_FOLLOWING_THE_LINE):
            //main_line(); break;
        default:
            main_debug(r);
    }

    // followin' code could not ever been executed
    INIT_leds();
	while (1) {
        gpio_toggle(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);
        mswait(500);
	}

	return 0;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



