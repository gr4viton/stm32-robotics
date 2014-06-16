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
/****************
 \brief Predefined actuator dc motor h-bridge connections ports & clocks
 ****************/
S_actuator_dcmotor dcmotor_predef[] =
{
//2014_06_16 - board v1
/*0*/ {.priority=0,.clk=RCC_GPIOA,.rxport=GPIOA,.txport=GPIOA,.rxpin=GPIO3,.txpin=GPIO1,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ,  .timOCX=TIM_OC1}
/*1*/,{.priority=0,.clk=RCC_GPIOB,.rxport=GPIOD,.txport=GPIOB,.rxpin=GPIO6,.txpin=GPIO7,.exti=EXTI6,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC2}
/*2*/,{.priority=0,.clk=RCC_GPIOA,.rxport=GPIOA,.txport=GPIOA,.rxpin=GPIO7,.txpin=GPIO5,.exti=EXTI7,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC3}
/*3*/,{.priority=0,.clk=RCC_GPIOE,.rxport=GPIOE,.txport=GPIOE,.rxpin=GPIO5,.txpin=GPIO1,.exti=EXTI5,.irq=NVIC_EXTI9_5_IRQ,.timOCX=TIM_OC4}
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________
    // ..
S_actuator_dcmotor* INIT_dcmotorPredef(uint8_t index, S_timer_setup* a_tim_s)
{
    S_actuator_dcmotor* d = &(dcmotor_predef[index]);
    //S_timer_setup* tim_s d->tim_s;

    rcc_periph_clock_enable(d->pclk);
	gpio_mode_setup(d->port, GPIO_MODE_INPUT, d->pull, d->pEN);
	gpio_mode_setup(d->port, GPIO_MODE_INPUT, d->pull, d->pPWM);

    // timer setup
    d->tim_s = a_tim_s;
    d->TIMX = a_tim_s->TIMX;

/* setup timer:
    - external trigger start rising edge
    - upcounting
    - external trigger stop falling edge::
    - good prescalers for measurining signal between <0.2; 12> [ms]
*/
    /* -------- timer settings -------- */
    uint8_t q = d->indx;
    uint32_t t = d->tim_s->TIMX;

    /* Disable outputs. */
    timer_disable_oc_output(t, d->timOCX);

    /* -- OCX configuration -- */

    /* Configure global mode of line 1. */
    timer_disable_oc_clear(t, d->timOCX);
    timer_disable_oc_preload(t, d->timOCX);
    timer_set_oc_slow_mode(t, d->timOCX);
    timer_set_oc_mode(t, d->timOCX, TIM_OCM_FROZEN);

    /* Set the capture compare value for OC1. */
    timer_set_oc_value(t, d->timOCX, d->nTriggerTicks);

    /* Enable commutation interrupt. */
    timer_enable_irq(t, TIM_DIER_CC1IE<<q);
    /* ---- */
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
