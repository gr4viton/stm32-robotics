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
    ROBOT_initLifeDebug(r);
    fprintf(r->flcd, "Init=");
    _tocPrint(r->flcd);
    mswait(400);
    LCD_clear(r->lcd);

    ROBOT_START(r);

    uint32_t period = 10;
    uint32_t prStart = _tic();
    uint8_t q=0;
    //uint32_t ocval = 0;

    S_robot_dcmotors* ds = 0;
    ds = &(r->dcs);
    S_actuator_dcmotor* m = 0;
    m = ds->m[q];

    float dcadd = 0.00001;
    mswait(1000);
    while(1)
    {
        CLOCK_digiPrint(r);
        if(m->dutyCycle >= 1.0) m->dutyCycle = 0.0;
        DCMOT_SET_dutyCycle(m, m->dutyCycle+dcadd);
    }

	while (1)
    {
        /**
            DO not debug inside this while!
            Make a separate function like DBG_tryADC(void),
            and call it from here :)
        */

//        DBG_testButtonState(r,100,10);
//        DBG_testUltraDistanceOld(r,100);
        //TRY_buzzer();
        //LCD_displayWriteCheck(r->lcd);
        //dev_LCD_checkSeek(flcd);
        //DBG_tryADC(r);
        //DBG_tryCNY70(r
        //DBG_testUltraDistance(r,0xFFFF);
        //DBG_testAllUltraDistance(r,0xFFFF);
        DBG_testActuators(r,0xFFFFFF);

        //INIT_tim(r);
	}

	return 0;
}


void DBG_testActuators_wInit(S_robot*r, uint32_t reps)
{
    uint32_t period = 100;
    uint32_t prStart = _tic();
    uint8_t q=0;
    //uint32_t ocval = 0;
    //init
     rcc_clock_setup_in_hse_8mhz_out_72mhz();

     /* enable GPIOC clock */
     rcc_periph_clock_enable(RCC_GPIOC);

     /*
      * set GPIO12 at PORTC (led) to 'output alternate function push-pull'.
      */
    // gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);

    gpio_mode_setup(GPIOA,GPIO_MODE_AF,GPIO_PUPD_NONE,GPIO1);
    gpio_set_af(GPIOA,GPIO_AF1,GPIO1);
    uint32_t *p = TIM2_CCMR1;
    *p = 0x6800;
    p = TIM2_CR1 ;
    *p |= 1<<7; // ARPE
    p = TIM2_EGR ;
    *p |= 1<<0;

    p = TIM2_CCER ;    *p |= 1<<4; //enable output



    while(reps>1)
    {
        CLOCK_digiPrint(r);
        if( _tocFrom(prStart) > period )
        {

            //TIM_CCR1(m->TIMX) = m->OCval;
            //ocval += 10;
            //model_timer_SET_outputCompare(TIM2,TIM_OC2,ocval);

            // printit
            LCD_gotoxy(r->lcd,0,0);
            LCD_gotoxy(r->lcd,0,1);
            //fprintf(r->flcd, "");
                //for(q=0;q<ROB_MOTOR_COUNT;q++)
                    //fprintf(r->flcd, "%.0f%%=%lu/%lu", ds->m[q]->dutyCycle*100, ds->m[q]->OCval, ds->m[q]->tim_s->period );
                    //fprintf(r->flcd, "oc=%lu=%lu", ocval, m->OCval );

            prStart = _tic();

        }
        reps--;
    }
}


void DBG_testActuators(S_robot*r, uint32_t reps)
{
    uint32_t period = 10;
    uint32_t prStart = _tic();
    uint8_t q=0;
    //uint32_t ocval = 0;

    S_robot_dcmotors* ds = 0;
    ds = &(r->dcs);
    S_actuator_dcmotor* m = 0;
    m = ds->m[q];

    float dcadd = 0.00001;
    mswait(1000);
    while(1)
    {
        CLOCK_digiPrint(r);
        if(m->dutyCycle >= 1.0) m->dutyCycle = 0.0;
        DCMOT_SET_dutyCycle(m, m->dutyCycle+dcadd);
    }
    while(reps>1)
    {
        CLOCK_digiPrint(r);
        if( _tocFrom(prStart) > period )
        {

            // cycle dutyCycle
            if(m->dutyCycle >= 1.0) m->dutyCycle = 0.0;

            DCMOT_SET_dutyCycle(m, m->dutyCycle+0.02);

            TIM_CCR1(m->TIMX) = m->OCval;
            //ocval += 10;
            //model_timer_SET_outputCompare(TIM2,TIM_OC2,ocval);

            // printit
            LCD_gotoxy(r->lcd,0,0);
                    fprintf(r->flcd, "%.0f%%", ds->m[q]->dutyCycle*100);
            LCD_gotoxy(r->lcd,0,1);
            //fprintf(r->flcd, "");
                //for(q=0;q<ROB_MOTOR_COUNT;q++)
                    fprintf(r->flcd, "%lu/%lu|", ds->m[q]->OCval, ds->m[q]->tim_s->period );
                    //fprintf(r->flcd, "oc=%lu=%lu", ocval, m->OCval );

            prStart = _tic();

        }
        reps--;
    }
    gpio_toggle(PLED,LEDORANGE1);
}


void DBG_touchD7blink(void)
{
    // D7 - touch blink
	rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO7);
    model_gpioPin_INIT_exti2(GPIOD,EXTI7,NVIC_EXTI9_5_IRQ,10,EXTI_TRIGGER_BOTH);
}

void DBG_tryCNY70(S_robot* r)
{
    LCD_clear(r->lcd);
    fprintf(r->flcd,"PC0=%7.2f \r\n", r->infs.i[0]->val);


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

void SUMO_wait5secAndSample(S_robot* r);

void DBG_ticTocSumoWait(S_robot* r)
{
    while(1)
    {
        _tic();
        uint8_t a=0;
        for(a=0; a<5; a++) SUMO_wait5secAndSample(r);
        _tocPrint(r->flcd);
        mswait(300);
    }
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



void DBG_testAllUltraDistance(S_robot*r, uint32_t reps)
{

    uint32_t period = 100;
    uint32_t prStart = _tic();
    uint8_t q=0;
    S_robot_ultras* us = 0;
    us = &(r->ults);
    while(reps>1)
    {
        CLOCK_digiPrint(r);

        if( _tocFrom(prStart) > period )
        {
            gpio_toggle(PLED,LEDBLUE3);
            LCD_gotoxy(r->lcd,0,0);
            fprintf(r->flcd, "s");
                for(q=0;q<ROB_ULTRA_COUNT;q++)
                    fprintf(r->flcd, "%u", us->u[q]->state );
            fprintf(r->flcd, "p");
                for(q=0;q<ROB_ULTRA_COUNT;q++)
                    fprintf(r->flcd, "%u", us->u[q]->nOwerflow);
            fprintf(r->flcd, "  ");
            //fprintf(r->flcd, "p=");
            //u->nOwerflow

            LCD_gotoxy(r->lcd,0,1);
            //fprintf(r->flcd, "a");
                for(q=0;q<ROB_ULTRA_COUNT;q++)
                {
                    fprintf(r->flcd, "%3.0f|", us->u[q]->dist );
                    //fprintf(r->flcd, "%3.0f", us->u[q]->dist );
                    //if(q<3) fprintf(r->flcd, "|");
                }

            //fprintf(r->flcd, "[s=%u][%4u]", u->state, cnt);

            //cnt = timer_get_counter(u->TIMX);
            prStart = _tic();
        }
        reps--;
    }
}


void DBG_testUltraDistance(S_robot*r, uint32_t reps)
{

    uint32_t period = 100;
    uint32_t prStart = _tic();
    S_sensor_ultra* u = 0;
    u = r->ults.u[0];
    while(reps>1)
    {
        CLOCK_digiPrint(r);

        if( _tocFrom(prStart) > period )
        {
            gpio_toggle(PLED,LEDBLUE3);
            LCD_gotoxy(r->lcd,0,0);
            fprintf(r->flcd, "=%0.5f|s=%u",  u->dist, u->state  );
            LCD_gotoxy(r->lcd,0,1);
            fprintf(r->flcd, "=%6lu |p=%u ",  u->nTicks, u->nOwerflow);
            //fprintf(r->flcd, "[s=%u][%4u]", u->state, cnt);

            //cnt = timer_get_counter(u->TIMX);
            prStart = _tic();
        }
        reps--;
    }
}


void DBG_testUltraDistanceOld2(S_robot* r,uint32_t repeats)
{
    S_sensor_ultra* u ;
    uint8_t a =0;
    FILE* f = r->fus; // for output on uart
    //FILE* f = r->flcd; // for output on lcd

//    INIT_ultraTimer(r,0);

    while(repeats>1)
    {
        repeats--;
        fprintf(f,"ULTRAS[0-3]:");

        for(a=0;a<ROB_ULTRA_COUNT;a++)
        {
            u = r->ults.u[a];
            if(u->ticksStart == 0)
                ULTRA_signalSend(u);
            fprintf(f, "[%u]=%3lu|%5.2f[cm]", a, u->nTicks, u->dist);
        }
        fprintf(f, "\n");


        //LCD_clear(r->lcd);
        //fprintf(f, "%lu|%.2f[cm]", u->nTicks, u->dist);
        //fprintf(f, "%lu|%.2f[cm]\n", u->nTicks, u->dist);
        mswait(200);
    }
}


void DBG_flash(void)
{
    gpio_toggle(PLED,LEDS);
}


void CLOCK_digiPrint(S_robot* r)
{
    static uint32_t world_time = 0;
    static uint32_t last_wtime = 0;
    world_time = _tic()/1000;
    if(world_time != last_wtime)
    {
        char str_hhmmss[] = "%2lu:%02lu:%02lu";
        char str_mss[] = "%2lu:%02lu";
        char str_s[] = "%2lu";
        uint32_t hh = (world_time/3600);
        uint32_t mm = (world_time/60)%60;
        uint32_t ss = world_time%60;
        if(mm > 0)
        {
            if(hh>0)
            {
                LCD_gotoxy(r->lcd,8,0);
                fprintf(r->flcd, str_hhmmss, hh, mm, ss);
            }
            else
            {
                LCD_gotoxy(r->lcd,11,0);
                fprintf(r->flcd, str_mss, mm, ss);
            }
        }
        else
        {
            LCD_gotoxy(r->lcd,14,0);
            fprintf(r->flcd, str_s, ss);

        }
        last_wtime = world_time;
    }

}

#if __NOT_IMPLEMENTED_YET
void DBG_debug_try(void)
{

    uint8_t i_ultra = 0;
    INIT_ultraPredef(i_ultra ,0);
    uint8_t ilcd = 0;

    S_dev_lcd* lcd_dev = &(predef_lcds[ilcd]);

    S_sensor_ultra* ultra = &(predef_ultras[i_ultra]);
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



