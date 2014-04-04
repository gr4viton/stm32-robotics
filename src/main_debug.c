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

    INIT_leds();
    S_dev_lcd* lcd_dev = INIT_dev_LCD();
	while (1)
    {
        int a = 10;
        while(1)
        {
            a=a-1;
            REFRESH_buttonState(&(btns.start_btn));
            REFRESH_buttonState(&(btns.line_btn));
            REFRESH_buttonState(&(btns.sumo_btn));
            gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);

            LCD_clear(lcd_dev);
            LCD_gotoxy(lcd_dev,0,0);
            fprintf(flcd, "%x|%x|%x",
                    btns.start_btn.state,
                    btns.line_btn.state,
                    btns.sumo_btn.state
                     );
            LCD_gotoxy(lcd_dev,0,1);
            fprintf(flcd, "%lx|%lx|%lx",
                    GPIO_IDR(btns.start_btn.port) ,
                    GPIO_IDR(btns.line_btn.port) ,
                    GPIO_IDR(btns.sumo_btn.port)
                     );

            if(btns.start_btn.state)
                gpio_set(PLED,LEDGREEN0);
            if(btns.line_btn.state)
                gpio_set(PLED,LEDBLUE3);
            if(btns.sumo_btn.state)
                gpio_set(PLED,LEDRED2);
            mswait(666);

        }
        gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);

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
#if __NOT_IMPLEMENTED_YET
void DBG_debug_try(void)
{

    uint8_t i_ultra = 0;
    INIT_ultra(i_ultra ,0);
    uint8_t ilcd = 0;

    S_dev_lcd* lcd_dev = &(lcds[ilcd]);

    S_sensor_ultra* ultra = &(ultras_predef[i_ultra]);
    ULTRA_signalSend(ultra);
    while(1)
    {
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
    }

}
#endif // __NOT_IMPLEMENTED_YET

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



