/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_ultrasonic.c
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
#include "sensor_ultrasonic.h"

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
 @brief connection array
 pins[x] == predef_pins[dcmotor_conarr[x]]
 order of pins of actuator in conarr (as in INIT_dcmotorPredef) is:
 { .echo, .trig }
 ****************/
uint32_t ultra_conarr[][SENSOR_ULTRA_PINS_COUNT] =
{
/*<0;3>*/{PA3,PA1},{PD6,PB7},{PA7,PA5},{PE5,PE1}
};

/****************
 \brief Predefined ultrasonic sensors ports & clocks [tx=out;rx=in]
 ****************/
S_sensor_ultra predef_ultras[] =
{
//2014_06_16 - board v1
// with changes:  E3-->E5; C13-->H1
/*0*/ {.conarr=ultra_conarr[0],.timOCX=TIM_OC1}
/*1*/,{.conarr=ultra_conarr[1],.timOCX=TIM_OC2}
/*2*/,{.conarr=ultra_conarr[2],.timOCX=TIM_OC3}
/*3*/,{.conarr=ultra_conarr[3],.timOCX=TIM_OC4}
///*1*/,{.priority=0,.clk=RCC_GPIOB,.rxport=GPIOB,.txport=GPIOB,.rxpin=GPIO9,.txpin=GPIO7,.exti=EXTI9,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC2} // TAK KURVA DO PICE CO JE UZ!!
///*1*/,{.priority=0,.clk=RCC_GPIOC,.rxport=GPIOC,.txport=GPIOC,.rxpin=GPIO15,.txpin=GPIO11,.exti=EXTI15,.irq=NVIC_EXTI15_10_IRQ,.timOCX=TIM_OC2} // just nope strope
///*1*/,{.priority=0,.clk=RCC_GPIOC|RCC_GPIOH,.rxport=GPIOC,.txport=GPIOH,.rxpin=GPIO15,.txpin=GPIO1,.exti=EXTI15,.irq=NVIC_EXTI15_10_IRQ,.timOCX=TIM_OC2} // does not work either
///*1*/,{.priority=0,.clk=RCC_GPIOC,.rxport=GPIOC,.txport=GPIOC,.rxpin=GPIO15,.txpin=GPIO13,.exti=EXTI15,.irq=NVIC_EXTI15_10_IRQ,.timOCX=TIM_OC2} // nope because only one pin from PC13-PC15 can be configured as GPIO at a time (BAT)
///*3*/,{.priority=0,.clk=RCC_GPIOE,.rxport=GPIOE,.txport=GPIOE,.rxpin=GPIO5, .txpin=GPIO1, .exti=EXTI5, .irq=NVIC_EXTI9_5_IRQ,  .timOCX=TIM_OC4} // nope because two GPIO3's are selected as exti source = not possible (EXTI)
//2014_06_16 - debug
/*4*/,{.priority=0,.clk=RCC_GPIOD,.rxport=GPIOD,.txport=GPIOD,.rxpin=GPIO2,.txpin=GPIO0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ,  .timOCX=TIM_OC1}
/*5*/,{.priority=0,.clk=RCC_GPIOD,.rxport=GPIOD,.txport=GPIOD,.rxpin=GPIO3,.txpin=GPIO1,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ,  .timOCX=TIM_OC2}
/*6*/,{.priority=0,.clk=RCC_GPIOD,.rxport=GPIOD,.txport=GPIOD,.rxpin=GPIO6,.txpin=GPIO4,.exti=EXTI6,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC3}
/*7*/,{.priority=0,.clk=RCC_GPIOD,.rxport=GPIOD,.txport=GPIOD,.rxpin=GPIO7,.txpin=GPIO5,.exti=EXTI7,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC4}
//,{.clk=RCC_GPIOE, .::rxport=GPIOE, .txport=GPIOE, .rxpin=GPIO5, .txpin=GPIO4, .irq=NVIC_EXTI4_IRQ, .exti=EXTI5}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE


S_sensor_ultra* INIT_ultraPredef(uint8_t index, S_timer_setup* a_tim_s)
{
    S_sensor_ultra* u = &predef_ultras[index];
    // index of ultrasound
    //u->indx = index;

    // states
    u->echoState = 0;
    u->state = s0_idle_before_trigger;

    // tick counting
    u->ticksStart = 0;
    u->ticksEnd = 0;
    u->nTicks = 0;
    u->nOwerflow = 0;

    // timer setup
    //ult->timOCX = TIM_OC1<<index;
    u->tim_s = a_tim_s;
    u->TIMX = a_tim_s->TIMX;

    // count from frequency of timer and its period
    // should be at least 10us
    uint32_t f10us = 10000; // == 1/t == 1/(100[us])
    u->nTriggerTicks = a_tim_s->full_freq / (a_tim_s->prsc) / f10us;
    //u->nTriggerTicks = 1000;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // pin settings new
    uint8_t q=0;
    uint8_t qmax = SENSOR_ULTRA_PINS_COUNT;
    //____________________________________________________
    // fill the pins array from predef_gpioPin array and initialize the pins

    // connection
    for(q=0;q<qmax;q++)
    {
        u->pins[q] = &( predef_gpioPin[ u->conarr[q] ] );
        // need some mechanism of collision detection! -> everything should be cli -> do not use GUI!
    }

    // map pointers of pins
    // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // will be in an array too

    u->rx = u->pins[0];
    //u->rx = &(predef_gpioPin[2]);
    u->tx = u->pins[1];
//    u->pins[0]->mode = 14;

    // create configuration
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // LATER this will not be here as the pins in predef_gpioPins will be configured as wanted already -> so only
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    u->rx->mode = GPIO_MODE_INPUT;
    u->rx->mode = GPIO_PUPD_NONE;
    u->rx->exti_enabled = true;

    u->tx->mode = GPIO_MODE_OUTPUT;
    u->tx->mode = GPIO_PUPD_PULLDOWN;

    // insert configuration
    for(q=0;q<qmax;q++)
    {
        model_gpioPin_INIT(u->pins[q]);
    }

    /*
    // pin settings old

	rcc_periph_clock_enable(u->clk);
	gpio_mode_setup(u->txport, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, u->txpin);
	gpio_mode_setup(u->rxport, GPIO_MODE_INPUT, GPIO_PUPD_NONE, u->rxpin);
	gpio_clear(u->txport, u->txpin);
    */
/* setup timer:
    - external trigger start rising edge
    - upcounting
    - external trigger stop falling edge::
    - good prescalers for measurining signal between <0.2; 12> [ms]
*/
    /* -------- timer settings -------- */
    q = index;
    uint32_t t = u->tim_s->TIMX;

    /* Disable outputs. */
    timer_disable_oc_output(t, u->timOCX);

    /* -- OCX configuration -- */

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(t, u->timOCX);
    timer_disable_oc_preload(t, u->timOCX);
    timer_set_oc_slow_mode(t, u->timOCX);
    timer_set_oc_mode(t, u->timOCX, TIM_OCM_FROZEN);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(t, u->timOCX, u->nTriggerTicks);

    /* Enable commutation interrupt. */
    timer_enable_irq(t, TIM_DIER_CC1IE<<q);
    /* ---- */

    return u;
}

S_timer_setup* INIT_ultraTimer(uint8_t indx)
{
    // initialize timer X for 1ns ticking
    S_timer_setup* tim_s = &(predef_timers[indx]);
    uint32_t t = tim_s->TIMX;
    //____________________________________________________
    // clock initialization
	rcc_periph_clock_enable(tim_s->apbclk);
	rcc_periph_clock_enable(tim_s->clk);
    timer_reset(t);
    /* Time Base configuration */
    //____________________________________________________
    // MODE SETTING
    // - use internal clock as a trigger
    timer_set_mode(t, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // TIM_CR1_CKD_CK_INT = Clock Division Ratio - set CK_PSC = CK_INT = f_periph = 84[MHz]
    // TIM_CR1_CMS_EDGE = Center-aligned Mode Selection  - edge..??
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
    timer_set_period(t, 0xFFFF-1);
    /* Generate TRGO on every update. */
    timer_set_master_mode(t, TIM_CR2_MMS_UPDATE);

    timer_enable_irq(t, TIM_DIER_UIE);
    //irqs = TIM_DIER_CC1IE | TIM_DIER_BIE ;
    //irqs = TIM_DIER_BIE;
    //irqs = TIM_DIER_TIE;
    //irqs = TIM_DIER_UIE; // update is the best :)
    //irqs = 0xFFFF;
    //timer_enable_irq(t,irqs);

    // NVIC isr setting
    // enable interrupt in NestedVectorInterrupt
    // -> if some of the timer interrupts is enabled -> it will call the tim isr function
	nvic_enable_irq(tim_s->nvic);

    return tim_s;
}

void ULTRA_setCoefs(S_sensor_ultra* ult, double coef[])
{
    uint8_t a=0;
    for(a=0;a<SENSOR_ULTRA_COEF_COUNT;a++)
        ult->coef[a] = coef[a];
}

double ULTRA_calcDist(S_sensor_ultra* ult)
{
    // in future there may be linearization or preprocessing
    uint8_t a=0;
    ult->dist = ult->coef[0];
    for(a=1;a<SENSOR_ULTRA_COEF_COUNT;a++)
    {
        ult->dist += ult->coef[a] * pow((double)(ult->nTicks), a);
    }
    return ult->dist;
}

void ULTRA_tiggerStart(S_sensor_ultra *u)
{
    // send trigger signal
    /* enable compare interrupt */
    timer_enable_irq(u->TIMX, ((TIM_DIER_CC1IE)<<(u->indx)) );
    /* set value of compare register to produce trigger signal of given interval */
    timer_set_oc_value(u->TIMX, u->timOCX, u->nTriggerTicks);

    gpio_set(u->txport, u->txpin);

    u->nOwerflow = 0;
    u->state = s1_sending_trigger;
}

void ULTRA_tiggerEnd(S_sensor_ultra *u)
{
    gpio_clear(u->txport, u->txpin);
    u->state = s2_waiting_for_echo;
    u->nOwerflow = 0;
    /* disable compare interrupt */
    timer_disable_irq(u->TIMX, ((TIM_DIER_CC1IE)<<(u->indx)) );
    //timer_disable_oc_clear(t,tim10_isr)
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/* deprecated */
void ULTRA_signalSend(S_sensor_ultra *u)
{
    gpio_set(u->txport, u->txpin);
    //u->ticksStart = timer5;
    // wait
    mswait(1);
    gpio_clear(u->txport, u->txpin);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void ULTRA_echoStarted(S_sensor_ultra *u)
{
    u->ticksStart = timer_get_counter(u->TIMX);
    u->state = s3_waiting_for_echo_end;
    u->nOwerflow = 0;
}

void ULTRA_echoEnded(S_sensor_ultra *u)
{
    u->ticksEnd = timer_get_counter(u->TIMX);
    uint32_t sum_tick_in_period = 0xFFFF;

    /* the echo signal should always come inside one period - that is cross maximally one timer overflow*/
    if(u->nOwerflow <= 1)
    {
        u->nTicks = (u->nOwerflow * sum_tick_in_period) + u->ticksEnd - u->ticksStart;
        ULTRA_calcDist(u);
    }
    else
    {
        /* if it doesnt - it is probably bad measurement = lost connection or distance too big*/
        /* so skip the waiting for the lost signal and send another trigger */
        u->state = s0_idle_before_trigger;
    }

    //u->nTicks = (u->cnt_period * sum_tick_in_period) + u->ticksEnd - u->ticksStart;

    u->ticksStart = 0; // for interrupt handler distinction between echoStart and echoEnd
    u->nOwerflow = 0;
    u->state = s0_idle_before_trigger;
}

void ULTRA_handleEcho(S_sensor_ultra* u)
{
    if( exti_get_flag_status(u->exti) ) // this is second checking - but harmless
    { // some of the ultras responded with up or down edge
        u->echoState = BIT_TOGGLE(u->echoState, 0);

        if((u->state == s2_waiting_for_echo)||(u->state == s3_waiting_for_echo_end))
        //if(u->state == s2_waiting_for_echo)
        if(u->echoState != 0)
        { // start of measuring period
             ULTRA_echoStarted(u);


        }
        //else if(u->state == s3_waiting_for_echo_end)
        if(u->echoState == 0)
        { // end of measuring period
             ULTRA_echoEnded(u);
        }
    }
}

#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void gpiod_isr(void)
{
    S_sensor_ultra *dev;
    //switch(INTERRUPTED_PIN){
    //case(ultras[0]):
        dev=&predef_ultras[0];
    //}

    //!!!!
    //dev->ticksEnd = timer5a;
    ULTRA_signalAcquired(dev);
}

void tim5_isr(void)
{
    if (timer_interrupt_source(TIM5, TIM_SR_CC4IF))
    {
        //if (++counter == 0)
        //{
            //gpio_toggle(PLED,LEDBLUE3);
        //}
    }
}
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING



#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void timer3a_isr(){
    // find out which counting is active and increment overflow
    //dev->overflow++
}
#endif


#if __NOT_IMPORTANT
//gpio_enable_interrupts(ult->rxport,ult->rxpin);
	// txport interrupt enable
    //gpio_enable_interrupts(ult->txport,ult->txpin);

// which timer to use?
// ie TIM5
	// setup timer to count with overflow interrupt (for detection)
	// to be able to count from x us to y ms

/*
	rcc_periph_clock_enable(RCC_TIM5);
	timer_reset(TIM5);
    timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // value - http://luckylarry.co.uk/arduino-projects/arduino-sonic-range-finder-with-srf05/
    // 5764us = 1m -->
    // 57.64us = 1cm
    timer_set_prescaler(TIM5, 1);   // 20.5kHz = 49us (=48.7804878049us)
    timer_set_period(TIM5, 0xFFF);
    timer_set_counter(TIM5, 0);
    timer_set_repetition_counter(TIM5, 0);
    timer_continuous_mode(TIM5);

    // start counter
    timer_enable_preload(TIM5);
    timer_enable_break_main_output(TIM5);

    timer_enable_irq(TIM5, TIM_DIER_CC4IE);
    timer_enable_counter(TIM1);
    nvic_enable_irq(NVIC_TIM5_IRQ);
*/
#endif // __NOT_IMPORTANT

    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
