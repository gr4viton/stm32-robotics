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
    _tic();
    INIT_leds();
    ROBOT_initAll(r);
    _tocPrint(r->flcd);
	while (1)
    {
        /**
            DO not debug inside this while!
            Make a separate function like DBG_tryADC(void),
            and call it from here :)
        */
//        DBG_testButtonState(r,100,10);
//        DBG_testUltraDistance(r,100);
        //TRY_buzzer();
        //LCD_displayWriteCheck(r->lcd);
        //dev_LCD_checkSeek(flcd);
        //DBG_tryADC(r);
        DBG_tryCNY70(r);

	}

	return 0;
}


void DBG_tryCNY70(S_robot* r)
{
    LCD_clear(r->lcd);
    fprintf(r->flcd,"PC0=%7.2f \n", r->infs.i[0]->val);


    gpio_toggle(PLED,LED0);
    mswait(500);
}
void DBG_tryADC(S_robot* r)
{
    //uint16_t input_adc0 = INFRA_readNaiive(10);

    //uint16_t input_adc1 = INFRA_readNaiive(11);
//    uint16_t input_adc1 = r->infs.iFL->val;

    LCD_clear(r->lcd);
    fprintf(r->flcd,"%7.2f  %7.2f\n%7.2f  %7.2f\n",
            r->infs.iFL->val,
            r->infs.iFR->val,
            r->infs.iBL->val,
            r->infs.iBR->val
            );


    gpio_toggle(PLED,LED0);
    mswait(500);

    //fprintf(r->fus,"ADC=%u\n\r", r->infs.iFL->val);
    //LCD_clear(r->lcd);
    //fprintf(r->flcd,"ADC=%u", r->infs.iFL->val);
}

void DBG_adc_finish(void)
{
    //R.infs.iFL->val=values[0];
}

void DBG_testButtonState(S_robot* r, uint32_t repeats,uint32_t ms)
{
    while(repeats>1)
    {
        repeats--;
        S_robot_buttons * b = &(r->btns);

        REFRESH_buttonState(b->bStart);
        REFRESH_buttonState(b->bLine);
        REFRESH_buttonState(b->bSumo);
        gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);

        LCD_clear(r->lcd);
        LCD_gotoxy(r->lcd,0,0);
        fprintf(R.flcd, "%x|%x|%x",
                b->bStart->state,
                b->bLine->state,
                b->bSumo->state
                 );

        if(b->bStart->state) gpio_set(PLED,LEDGREEN0);
        if(b->bLine->state) gpio_set(PLED,LEDBLUE3);
        if(b->bSumo->state) gpio_set(PLED,LEDRED2);
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
    INIT_ultraPredef(i_ultra ,0);
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



