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
/****************
 @brief FILE USART - pointer to usart device to write strings to
 ****************/
FILE *fus;
uint8_t rbuf[RBUFSZ]; // recieve buffer (using ring buffer logic)
uint8_t tbuf[TBUFSZ]; // transmission buffer (using ring buffer logic)

/****************
 @brief FILE LCD - pointer to lcd device display to write strings to
 ****************/
FILE *flcd;
uint8_t lcd_dbuf[LCD_DBUFSZ]; // lcd data buffer (will use ring buffer logic)


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
    INIT_clk();
    INIT_buttons();

    // Chose a program to run
    E_lifeStyleSelector life = GET_lifeStyle();
    switch(life)
    {
        case(IAM_BUGGED_ROBOT):
            main_debug(); break;
        case(IAM_SUMO_WARRIOR):
            //main_sumo(); break;
        case(IAM_SHEEP_FOLLOWING_THE_LINE):
            //main_line(); break;
        default:
            main_debug();
    }

    // followin' code could not ever been executed
    INIT_leds();
	while (1) {
        gpio_toggle(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);
        mswait(500);
	}

	return 0;
}
S_dev_lcd* INIT_dev_LCD(void)
{
    int ilcd = 0;
    flcd = fopenLCD(ilcd, 16,
                   LCD_C_8BIT_2L_5x7_LIGHT,
                   LCD_C_ENTRY_RIGHT_CMOVE,
                   LCD_C_CUR_VIS_STATIC,
                   lcd_dbuf, LCD_DBUFSZ);
    return &(lcds[ilcd]);
}
void INIT_dev_usart(void)
{
    // uarts[3] = UART4 = tC10,rC11
    fus = fopenserial(3, 9600, tbuf, TBUFSZ, rbuf, RBUFSZ);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



