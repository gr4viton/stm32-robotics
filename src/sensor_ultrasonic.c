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
 {.clk=RCC_GPIOD, .txport=GPIOD, .rxport=GPIOD, .txpin=GPIO2, .rxpin=GPIO0, .irq=NVIC_EXTI0_IRQ, .exti=EXTI0}
,{.clk=RCC_GPIOD, .txport=GPIOD, .rxport=GPIOD, .txpin=GPIO3, .rxpin=GPIO1, .irq=NVIC_EXTI1_IRQ, .exti=EXTI1}
,{.clk=RCC_GPIOD, .txport=GPIOD, .rxport=GPIOD, .txpin=GPIO6, .rxpin=GPIO4, .irq=NVIC_EXTI4_IRQ, .exti=EXTI4}
,{.clk=RCC_GPIOD, .txport=GPIOD, .rxport=GPIOD, .txpin=GPIO7, .rxpin=GPIO5, .irq=NVIC_EXTI9_5_IRQ, .exti=EXTI5}
//,{.clk=RCC_GPIOE, .txport=GPIOE, .rxport=GPIOE, .txpin=GPIO5, .rxpin=GPIO4, .irq=NVIC_EXTI4_IRQ, .exti=EXTI5}
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
    ult->ticksStart = 0;
    ult->ticksEnd = 0;
    ult->nTicks = 0;

    // get RCC from port and enable
    //rcc_periph_clock_enable(GET_rcc_from_port(ult->txport))); //????
	rcc_periph_clock_enable(ult->clk);
	gpio_mode_setup(ult->txport, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, ult->txpin);
	gpio_mode_setup(ult->rxport, GPIO_MODE_INPUT, GPIO_PUPD_NONE, ult->rxpin);
	gpio_clear(ult->txport, ult->txpin);

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
        ult->dist += ult->nTicks * pow(ult->coef[a], a);
    }
    return ult->dist;
}

void ULTRA_signalSend(S_sensor_ultra *ult)
{
    gpio_set(ult->txport, ult->txpin);
    //ult->ticksStart = timer5;
    // wait
    mswait(1);
    gpio_clear(ult->txport, ult->txpin);
}

void ULTRA_echoStarted(S_sensor_ultra *ult)
{
    ult->ticksStart = _tic();
}

void ULTRA_echoEnded(S_sensor_ultra *ult)
{
    #if __PREMATURE_OPTIMALIZATION
    ult->nTicks = (ult->ticksEnd - ult->ticksStart); //+ ult->nOverflow * ult->..; OF in 49 days ;)
    #endif // __PREMATURE_OPTIMALIZATION
    ult->nTicks = _tocFrom(ult->ticksStart);
    ult->ticksStart = 0; // for interrupt handler distinction between echoStart and echoEnd
    ULTRA_calcDist(ult);
}

void ULTRA_handleEcho(S_sensor_ultra* ult)
{
    if( exti_get_flag_status(ult->exti) )
    { // some of the ultras responded with up or down edge
        if(ult->ticksStart == 0)
        { // start of measuring period
            ULTRA_echoStarted(ult);
        }
        else
        { // end of measuring period
            ULTRA_echoEnded(ult);
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
