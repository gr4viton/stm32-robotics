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
#include "main_line.h"
// move to headerfile

#if __NOT_IMPLEMENTED_YET

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
static uint8_t rbuf[RBUFSZ];
static uint8_t tbuf[TBUFSZ];
static uint8_t lcd_dbuf[LCD_DBUFSZ];
//____________________________________________________
// other variables
FILE *fus;
FILE *flcd;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_dev_lcd lcds[];
extern S_sensor_ultra ultras_predef[];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
int main_line(void)
{
    uint8_t i_ultra = 0;
    uint8_t ilcd = 0;

    INIT_clk();

    fus = fopenserial(3, 9600, tbuf, TBUFSZ, rbuf, RBUFSZ); // uarts[3] = UART4 = tC10,rC11
    //lcd = fopenlcd(1, 9600, tbuf,1024,rbuf,1024);

    flcd = fopenLCD(ilcd, 16,
                   LCD_C_8BIT_2L_5x7_LIGHT, //0x38,//0x30, //LCD_C_8BIT_2L_5x7_LIGHT, //LCD_C_8BIT_1L_5x7_LIGHT,//
                   LCD_C_ENTRY_RIGHT_CMOVE,
                   LCD_C_CUR_VIS_STATIC,
                   lcd_dbuf, LCD_DBUFSZ);

    //ultra_sensor_t* ultra1 = 0;
    INIT_leds();
    INIT_ultra(i_ultra ,0);


    S_dev_lcd* lcd_dev = &(lcds[ilcd]);
    S_sensor_ultra* ultra = &(ultras_predef[i_ultra]);
	while (1) {
        gpio_toggle(PLED,LED0);

        ULTRA_signalSend(ultra);

        ultra->dist = 0;
        while( !gpio_get(ultra->rxport,ultra->rxpin) )
        {
            __asm__("nop");
        }
        while( gpio_get(ultra->rxport,ultra->rxpin) )
        {
            (ultra->dist)++;
        }

        //u30+4;u10+1
        ultra->dist *= 1.1;//1.111;//1.33333333;//(1 + 4.0/30);

        LCD_clear(lcd_dev);
        fprintf(flcd, "dist[cm]=%.2f", ultra->dist/100);//ULTRA_getDist(i_ultra));
        mswait(500);

        //TRY_buzzer();
/*
        LCD_displayWriteCheck(lcd_dev);
        dev_LCD_checkSeek(flcd);
        */
	}

	return 0;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES


#endif // __NOT_IMPLEMENTED_YET

