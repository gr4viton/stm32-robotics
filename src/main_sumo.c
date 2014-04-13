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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "main_sumo.h"

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
int main_sumo(S_robot* r)
{
    _tic();
    ROBOT_initAll(r);
    _tocPrint(r->flcd);
    // waitin for button
    while(r->STARTED==0) __asm__("nop");
/*
    while(1)
    {
        _tic();
        uint8_t a=0;
        for(a=0; a<5; a++) SUMO_waitSec(970, a);
        _tocPrint(r->flcd);
        mswait(300);
    }*/

    uint32_t here = _tic();
    // 5sec waitin
    SUMO_wait5secAndSample(r);
    _tocPrintFrom(r->flcd, here );
    mswait(500);

	while (1) {
        //gpio_toggle(PLED,LED1);
        LCD_clear(r->lcd);
        fprintf(r->flcd, "%d|%.1f\n", r->infs.iFR->free, r->infs.iFR->val );
        fprintf(r->flcd, "%.1f|%.1f", r->infs.iFR->triggerVal, r->infs.iFR->nStds * r->infs.iFR->stdev);
        mswait(300);
	}

	return 0;
}

void SUMO_wait5secAndSample(S_robot* r)
{
    uint8_t a=0;
    uint16_t one_sec = 970;
    SUMO_waitSec(one_sec, 0);
    SUMO_waitSec(one_sec, 1);
    SUMO_waitSec(one_sec, 2);
    /*
    - sampling is done in the background --> interrupts of adc
    it is not mean from whole 5 seconds but last [INFRA_SAMPLES] samples
    so we only need to grab the final value
    - in SUMO, the interrupt should be triggered on white line around the arena
    */
    double trigger_add = 666; // value offset from measured free space voltage
    // = used in case that standard deviation is too small (<100)
    for(a=0; a<r->infs.nInfs; a++)
        INFRA_setTresholdLastValue(r->infs.i[a], trigger_add);

    SUMO_waitSec(one_sec, 3);
    SUMO_waitSec(one_sec, 4);
}

void SUMO_wait5sec(uint16_t one_sec )
{
    uint8_t a=0;
    for(a=0; a<5; a++) SUMO_waitSec(one_sec, a);
}

void SUMO_waitSec(uint16_t one_sec, uint8_t index)
{
    gpio_clear(PLED, LEDS);
    switch(index)
    {
        default:
        case(0): gpio_set(PLED, LED0|LED1); break;
        case(1): gpio_set(PLED, LED1|LED2); break;
        case(2): gpio_set(PLED, LED2|LED3); break;
        case(3): gpio_set(PLED, LED2|LED3|LED0); break;
        case(4): gpio_set(PLED, LEDS); break;
    }
    mswait(one_sec);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

