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
int main_debug(S_robot* r)
{
    INIT_leds();
    //ROBOT_initLcd(r);
    ROBOT_initAll(r);
    //S_robot_buttons * b = &(r->btns);
	while (1)
    {
        DBG_testButtonState(r,100,10);
        DBG_testUltraDistance(r,100);

        gpio_toggle(PLED,LED0);


        //TRY_buzzer();
/*
        LCD_displayWriteCheck(r->lcd);
        dev_LCD_checkSeek(flcd);
        */
	}

	return 0;
}

void DBG_adc_finish(uint16_t values[])
{
    UNUSED(values);
}

void DBG_testButtonState(S_robot* r, uint32_t repeats,uint32_t ms)
{
    while(repeats>1)
    {
        repeats--;
        S_robot_buttons * b = &(r->btns);

        REFRESH_buttonState(b->bStart);
        REFRESH_buttonState(b->line_btn);
        REFRESH_buttonState(b->sumo_btn);
        gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);

        LCD_clear(r->lcd);
        LCD_gotoxy(r->lcd,0,0);
        fprintf(R.flcd, "%x|%x|%x",
                b->bStart->state,
                b->line_btn->state,
                b->sumo_btn->state
                 );

        if(b->bStart->state) gpio_set(PLED,LEDGREEN0);
        if(b->line_btn->state) gpio_set(PLED,LEDBLUE3);
        if(b->sumo_btn->state) gpio_set(PLED,LEDRED2);
        mswait(ms);
    }
    gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);
}

void DBG_testUltraDistance(S_robot* r,uint32_t repeats)
{
/*
    uint8_t i_ultra = 0;
    INIT_ultra(i_ultra ,0);
*/
    //S_dev_lcd* lcd_dev = &(lcds_predef[ilcd]);
    S_sensor_ultra* ultra = r->ults.uL;

    while(repeats>1)
    {
        repeats--;

        ULTRA_signalSend(r->ults.uL);
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

        LCD_clear(r->lcd);
        fprintf(r->flcd, "dist[cm]=%.2f", ultra->dist/100);//ULTRA_getDist(i_ultra));
        mswait(500);
    }
}

#if __NOT_IMPLEMENTED_YET
void DBG_debug_try(void)
{

    uint8_t i_ultra = 0;
    INIT_ultra(i_ultra ,0);
    uint8_t ilcd = 0;

    S_dev_lcd* lcd_dev = &(lcds_predef[ilcd]);

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

        LCD_clear(r->lcd);
        fprintf(flcd, "dist[cm]=%.2f", ultra->dist/100);//ULTRA_getDist(i_ultra));
        mswait(500);
    }

}
#endif // __NOT_IMPLEMENTED_YET

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



