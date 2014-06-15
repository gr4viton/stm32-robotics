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
 \brief Predefined ultrasonic sensors ports & clocks [tx=out;rx=in]
 ****************/
S_sensor_ultra ultras_predef[] = {
/*0*/ {.clk=RCC_GPIOD,.rxport=GPIOD, .txport=GPIOD, .rxpin=GPIO2, .txpin=GPIO0, .irq=NVIC_EXTI2_IRQ, .exti=EXTI2, .timOCX=TIM_OC1}
/*1*/,{.clk=RCC_GPIOD,.rxport=GPIOD, .txport=GPIOD, .rxpin=GPIO3, .txpin=GPIO1, .irq=NVIC_EXTI3_IRQ, .exti=EXTI3, .timOCX=TIM_OC2}
/*2*/,{.clk=RCC_GPIOD,.rxport=GPIOD, .txport=GPIOD, .rxpin=GPIO6, .txpin=GPIO4, .irq=NVIC_EXTI9_5_IRQ, .exti=EXTI6, .timOCX=TIM_OC3}
/*3*/,{.clk=RCC_GPIOD,.rxport=GPIOD, .txport=GPIOD, .rxpin=GPIO7, .txpin=GPIO5, .irq=NVIC_EXTI9_5_IRQ, .exti=EXTI7, .timOCX=TIM_OC4}
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


S_sensor_ultra* INIT_ultraPredef(uint8_t index)
{
    S_sensor_ultra* ult = &ultras_predef[index];
    ult->indx = index;
    ult->echoState = 0;
    ult->ticksStart = 0;
    ult->ticksEnd = 0;
    ult->nTicks = 0;
    ult->nOwerflow = 0;
    ult->state = s0_idle_before_trigger;
    ult->timOCX = TIM_OC1;


    uint32_t sum_tick_in_period = 65536;
    uint32_t us1 = 1000; // count from frequency of timer and its period
    ult->interval_trigger = us1; // should be at least 10us

    // get RCC from port and enable
    //rcc_periph_clock_enable(GET_rcc_from_port(ult->txport))); //????
	rcc_periph_clock_enable(ult->clk);
	gpio_mode_setup(ult->txport, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, ult->txpin);
	gpio_mode_setup(ult->rxport, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ult->rxpin);
	gpio_clear(ult->txport, ult->txpin);

/* setup timer:
    - external trigger start rising edge
    - upcounting
    - external trigger stop falling edge::
    - good prescalers for measurining signal between <0.2; 12> [ms]
*/

    return ult;
}

void ULTRA_setCoefs(S_sensor_ultra* ult, double coef[])
{
    uint8_t a=0;
    for(a=0;a<ROB_ULTRA_COEF_COUNT;a++)
        ult->coef[a] = coef[a];
}

double ULTRA_calcDist(S_sensor_ultra* ult)
{
    // in future there may be linearization or preprocessing
    uint8_t a=0;
    ult->dist = ult->coef[0];
    for(a=1;a<ROB_ULTRA_COEF_COUNT;a++)
    {
        ult->dist += ult->coef[a] * pow((double)(ult->nTicks), a);
    }
    return ult->dist;
}

void ULTRA_tiggerStart(S_sensor_ultra *u)
{
    static uint32_t t = TIM3;
    // send 10us trigger signal

    /* enable compare interrupt */
    timer_enable_irq(t, ((TIM_DIER_CC1IE)<<(u->indx)) );
    /* set value of compare register to produce trigger signal of given interval */
    timer_set_oc_value(t, u->timOCX, u->interval_trigger);

    gpio_set(u->txport, u->txpin);

    u->nOwerflow = 0;
    u->state = s1_sending_trigger;
}

void ULTRA_tiggerEnd(S_sensor_ultra *u)
{
    static uint32_t t = TIM3;
    gpio_clear(u->txport, u->txpin);
    u->state = s2_waiting_for_echo;
    u->nOwerflow = 0;
    /* disable compare interrupt */
    timer_disable_irq(t, ((TIM_DIER_CC1IE)<<(u->indx)) );
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
    static uint32_t t = TIM3;
    //u->ticksStart = _tic();
    u->ticksStart = timer_get_counter(t);
    u->state = s3_waiting_for_echo_end;
    u->nOwerflow = 0;
}

void ULTRA_echoEnded(S_sensor_ultra *u)
{
    #if __PREMATURE_OPTIMALIZATION
    u->nTicks = (u->ticksEnd - u->ticksStart); //+ u->nOverflow * u->..; OF in 49 days ;)
    #endif // __PREMATURE_OPTIMALIZATION
    //u->nTicks = _tocFrom(u->ticksStart);
    static uint32_t t = TIM3;
    u->ticksEnd = timer_get_counter(t);
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
        u->echoState = BIT_TOGGLE(u->echoState,0);

        if(u->state == s2_waiting_for_echo)
        { // start of measuring period
            if(u->echoState != 0) ULTRA_echoStarted(u);


        }
        else if(u->state == s3_waiting_for_echo_end)
        { // end of measuring period
            if(u->echoState == 0) ULTRA_echoEnded(u);
        }
    }
}

#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void gpiod_isr(void)
{
    S_sensor_ultra *dev;
    //switch(INTERRUPTED_PIN){
    //case(ultras[0]):
        dev=&ultras_predef[0];
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
