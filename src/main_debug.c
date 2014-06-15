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
    R.STARTED = 1;
    ROBOT_initLifeDebug(r);
    fprintf(r->flcd, "Init=");
    _tocPrint(r->flcd);
    mswait(400);
    LCD_clear(r->lcd);

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
        //DBG_tryCNY70(r);
        DBG_testUltraDistance(r,100);

        //INIT_tim(r);

        while(1)
        {

        }
	}

	return 0;
}

void DBG_touchD7blink(void)
{
    // D7 - touch blink
	rcc_periph_clock_enable(RCC_GPIOD);
    gpio_mode_setup(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO7);
    ROBOT_initIsr(GPIOD,EXTI7,NVIC_EXTI9_5_IRQ,10,EXTI_TRIGGER_BOTH);
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

struct _S_periph_timer
{
    uint32_t tim; // timer address
    uint32_t period;

} S_periph_timer;


//extern uint32_t world_time;
void timtick_setup(S_robot* r);
void timtick_setup(S_robot* r)
{
    // initialize timer X for 1ns ticking
    uint32_t t;
	t   = TIM3;

    //____________________________________________________
    // clock initialization
	rcc_periph_clock_enable(RCC_APB1ENR_TIM3EN);
	rcc_periph_clock_enable(RCC_TIM3);
    timer_reset(t); /* Time Base configuration */

    // CK_INT = f_periph(TIM3=APB1) = 30[MHz]??          //RM0090.pdf
    //____________________________________________________
    // MODE SETTING
    // - use internal clock as a trigger
    timer_set_mode(t, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // TIM_CR1_CKD_CK_INT = Clock Division Ratio
    //  - set CK_PSC = CK_INT = f_periph = 84[MHz]
    // TIM_CR1_CMS_EDGE = Center-aligned Mode Selection
    //  - edge..??
    // TIM_CR1_DIR_UP = counting direction up

    // continuous
    timer_continuous_mode(t);
    //timer_get
    // 84Mhz / 8400 = 10kHz
    // [prsc=30, period=65536-1] == <0.37; 24186.27> [ms] == <0.006; 417> [cm]
    // [prsc=8400, period=10k  ] == <0.1 ; 840> [ms]

    timer_set_prescaler(t, 30);

    // Input Filter clock prescaler -
    timer_set_clock_division(t, 0);

    // TIMx_ARR - Period in counter clock ticks.
    timer_set_period(t, 65536-1);

    /* Generate TRGO on every update. */
    timer_set_master_mode(t, TIM_CR2_MMS_UPDATE);


    S_sensor_ultra* u = 0;
    uint8_t q = 0;
    uint8_t qmax = ROB_ULTRA_MAX_COUNT;
    uint8_t timDIERccXie = 0;
    for(q=0;q<qmax;q++)
    {

        u = R.ults.u[q];

        /* Disable outputs. */
        timer_disable_oc_output(t, u->timOCX);

        /* -- OC1 configuration -- */

        /* Configure global mode of line 1. */
        timer_disable_oc_clear(t, u->timOCX);
        timer_disable_oc_preload(t, u->timOCX);
        timer_set_oc_slow_mode(t, u->timOCX);
        timer_set_oc_mode(t, u->timOCX, TIM_OCM_FROZEN);

        /* Set the capture compare value for OC1. */
        timer_set_oc_value(t, u->timOCX, u->interval_trigger);

        /* Enable commutation interrupt. */
        timDIERccXie = TIM_DIER_CC1IE<<q;
        timer_enable_irq(t, timDIERccXie);
        /* ---- */
    }

    // start
    timer_enable_counter(t);
    //uint16_t irqs = 0 ;

    timer_enable_irq(t, TIM_DIER_UIE);
    //irqs = TIM_DIER_CC1IE | TIM_DIER_BIE ;
    //irqs = TIM_DIER_BIE;
    //irqs = TIM_DIER_TIE;
    //irqs = TIM_DIER_UIE; // update is the best :)
    //irqs = 0xFFFF;
    //timer_enable_irq(t,irqs);

    //____________________________________________________
    // NVIC isr setting

	//exti_select_source(exti, port);
	//exti_set_trigger(exti, trig);
	//exti_enable_request(exti);

    // enable interrupt in NestedVectorInterrupt
    // -> if some of the timer interrupts is enabled -> it will call the tim isr function
	nvic_enable_irq(NVIC_TIM3_IRQ);
    uint32_t last_wtime = 0;
    uint16_t cnt = 0;

    uint32_t world_time = 0;

    uint32_t period = 100;
    uint32_t prStart = _tic();
    //S_sensor_ultra* u
    u = r->ults.u[0];
	while(1)
    {
        world_time = _tocFrom(0)/1000;
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

        if( _tocFrom(prStart) > period )
        {
            gpio_toggle(PLED,LEDBLUE3);
            LCD_gotoxy(r->lcd,0,0);
            fprintf(r->flcd, "=%0.5f|s=%u",  u->dist, u->state  );
            LCD_gotoxy(r->lcd,0,1);
            fprintf(r->flcd, "=%6lu |p=%u ",  u->nTicks, u->nOwerflow);
            //fprintf(r->flcd, "[s=%u][%4u]", u->state, cnt);

            cnt = timer_get_counter(t);
            prStart = _tic();
        }
    }
}

void DBG_testUltraDistance(S_robot* r,uint32_t repeats)
{
    S_sensor_ultra* u ;
    uint8_t a =0;
    FILE* f = r->fus; // for output on uart
    //FILE* f = r->flcd; // for output on lcd

    timtick_setup(r);

    while(repeats>1)
    {
        repeats--;
        fprintf(f,"ULTRAS[0-3]:");

        for(a=0;a<ROB_ULTRA_MAX_COUNT;a++)
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

void DBG_testUltraDistanceOld(S_robot* r,uint32_t repeats)
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

void DBG_flash(void)
{
    gpio_toggle(PLED,LEDRED2);
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



