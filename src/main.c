/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief      This function only choses which of the 3 possible mains would run

\descrptn
    If you look here and hope to find out how this program works,
    you should look inside headerfiles on function headers to know what they do
    mainly look inside the robot_config.c and inside the struct S_robot.
    S_robot is is the core of everything.
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */
/*
TODO:
[x] udelat jmennou konvenci a coding style
[x] vytvorit mu projekt na gitu
[x] ultras serials_predef
[x] pridat output do repraku
[x] rozchodit interrupt
x] od tlacitka
x] od timeru
x] od adc -> pomoci injected a Timeru
] od adc watchdog - kdy z merenych hodnot budu menit hodnotu watchdogu?

[] rozchodit pwm
[] lcd
- dodelat seek
- vynechat mswait a dat to na pocet asmwait podle clocku procesoru!

[] sensor_battery - ADC-VBAT channel
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "main.h"
// move to headerfile
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
    /** If you look here and hope to find out how this program works,
    you should look inside headerfiles for function headers to know what they do
    mainly look inside the robot_config.c and inside the struct S_robot.
    S_robot is is the core of everything
    */

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
            main_sumo(r); break;
        case(IAM_SHEEP_FOLLOWING_THE_LINE):
            //main_line(r); break;
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



