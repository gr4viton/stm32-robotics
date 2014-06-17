/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	actuator_dcmotor.c
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
#include "actuator_dcmotor.h"


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

/* in c++ it would be soo much Nicer
 - possible no conarr needed as the struct can be constant
 - some of the pointers would be references eliminating nasty ampersands
 -> rewrite the damn whole thing! */

/****************
 @brief connection array
 pins[x] == predef_pins[dcmotor_conarr[x]]
 order of pins of actuator in conarr (as in INIT_dcmotorPredef) is:
 { .a, .b }
 ****************/
uint32_t dcmotor_conarr[] = { PA15, PB3 };

/****************
 \brief Predefined actuator dc motor h-bridge connections ports & clocks
 ****************/
S_actuator_dcmotor dcmotor_predef[] =
{
//2014_06_16 - board v1
/*0*/ {.conarr=dcmotor_conarr}
///*0*/ {.a=(predef_gpioPin[0]),.b=(predef_gpioPin[1])} // cannot initialize as initializer element is not constant - in c struct cannot be const
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_model_gpioPin predef_gpioPin[PREDEFCOUNT_GPIOPIN];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________
S_actuator_dcmotor* INIT_dcmotorPredef(uint8_t index, S_timer_setup* a_tim_s)
{
    S_actuator_dcmotor* d = &(dcmotor_predef[index]);
    uint8_t q=0;
    uint8_t qmax = 2;
    // fill the pins array from predef_gpioPin array and initialize the pins

    //model_gpioPin_INIT()
    for(q=0;q<qmax;q++)
    {
        d->pins[q] = &( predef_gpioPin[ d->conarr[q] ] );
        model_gpioPin_INIT(d->pins[q]);
        // need some mechanism of collision detection! -> everything should be cli -> do not use GUI!
    }
    // map pointers of pins
    d->a = d->pins[0];
    d->b = d->pins[1];
    (void)a_tim_s;
	return d;
}


S_timer_setup* INIT_dcmotorTimer(uint8_t indx)
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
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
