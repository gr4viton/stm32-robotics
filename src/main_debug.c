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
#include "main_debug.h"
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
int main_debug(void)
{
    uint8_t ilcd = 0;

    INIT_leds();
    //INIT_

    S_dev_lcd* lcd_dev = &(lcds[ilcd]);
	while (1)
    {
        int a = 10;
        while(a-->0)
        {
            REFRESH_buttonState(&(btns.start_btn));
            REFRESH_buttonState(&(btns.line_btn));
            REFRESH_buttonState(&(btns.sumo_btn));
            gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);

            LCD_clear(lcd_dev);
            fprintf(flcd, "%x|%x|%x",
                    btns.start_btn.state,
                    btns.line_btn.state,
                    btns.sumo_btn.state
                     );
            if(btns.start_btn.state)
                gpio_set(PLED,LEDGREEN0);
            if(btns.line_btn.state)
                gpio_set(PLED,LEDBLUE3);
            if(btns.sumo_btn.state)
                gpio_set(PLED,LEDRED2);
            mswait(500);

        }
        ULTRA_debug_try();

        gpio_toggle(PLED,LED0);


        //TRY_buzzer();
/*
        LCD_displayWriteCheck(lcd_dev);
        dev_LCD_checkSeek(flcd);
        */
	}

	return 0;
}
void DBG_adc_finish(uint16_t values[])
{
    UNUSED(values);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



