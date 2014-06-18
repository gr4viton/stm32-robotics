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
 - possible no conArr needed as the struct can be constant
 - some of the pointers would be references eliminating nasty ampersands
 -> rewrite the damn whole thing! */

/****************
 @brief connection array
 pins[x] == predef_pins[conarr_dcmotor[x]]
 order of pins of actuator in conArr (as in INIT_dcmotorPredef) is:
 { .a, .b }
 ****************/
uint32_t conarr_dcmotor[][ACTUATOR_DCMOTOR_PINS_COUNT] =
{
/*0,1,2,3*/{PA1,PC2},{PA2,PA2},{PC1,PC2},{PA1,PA2}
///*0,1,2,3*/{PC12,PC11},{PA14,PA15},{PA10,PA13},{PA8,PA9}
///*0,1,2,3*/{PC12,PC11},{PA1,PA2},{PA3,PA4},{PA5,PA5}
///*0,1,2,3*/{PA15,PB3},{PA1,PA2},{PA3,PA4},{PA5,PA5}
};



/****************
 \brief Predefined actuator dc motor h-bridge connections ports & clocks
 ****************/
S_actuator_dcmotor dcmotor_predef[] =
{
//2014_06_16 - board v1
/*0*/ {.conArr=conarr_dcmotor[0],.timOCX=TIM_OC2}
/*1*/,{.conArr=conarr_dcmotor[1],.timOCX=TIM_OC3}
/*2*/,{.conArr=conarr_dcmotor[2]}
/*3*/,{.conArr=conarr_dcmotor[3]}
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
    #include "servo.h"
    #include "libopencm3_add/gpio.h"



void DCMOT_INIT_timerChannel(S_actuator_dcmotor* d);

S_actuator_dcmotor* INIT_dcmotorPredef(uint8_t index, S_timer_setup* a_tim_s)
{
    S_actuator_dcmotor* d = &(dcmotor_predef[index]);
    uint32_t nPins = ACTUATOR_DCMOTOR_PINS_COUNT;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // pin settings
    // fill the pins array from predef_gpioPin array and initialize the pins
    //____________________________________________________
    // connection
    model_gpioMorePins_connectPins(d->pins, d->conArr, nPins);
    //____________________________________________________
    // map pointers of pins
    d->A = d->pins[0];
    d->B = d->pins[1];
    //____________________________________________________
    // create configuration

    d->A->pull = GPIO_PUPD_NONE;


    d->B->pull = GPIO_PUPD_NONE;
    //gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    //____________________________________________________
    // isntall configuration from [pins] to gpio periphery
    model_gpioMorePins_installConfig(d->pins, nPins);


     //rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);

    // init
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

    uint8_t q=0;
    for(q=0;q<nPins;q++)
    {
        S_model_gpioPin *p = d->pins[q];
        gpio_set_af(p->port,GPIO_AF1,p->pin);
        gpio_mode_setup(p->port,GPIO_MODE_AF,GPIO_PUPD_NONE,p->pin);
    }

    /* timer setup */
    d->tim_s = a_tim_s;
    d->TIMX = a_tim_s->TIMX;
    DCMOT_INIT_timerChannel(d);

	return d;
}

/*
void DCMOT_INSTALL_dutyCycle(S_actuator_dcmotor* m)
{
    model_timer_SET_outputCompare(m->tim_s->TIMX, m->OCval);
}*/

void DCMOT_SET_dutyCycle(S_actuator_dcmotor* m, float dutyCycle)
{
    m->dutyCycle = dutyCycle;
    //count value of OutputCompare
    m->OCval = m->tim_s->period * m->dutyCycle;
    model_timer_SET_outputCompare(m->tim_s->TIMX, m->timOCX, m->OCval);
}

void model_timer_SET_outputCompare(uint32_t TIMX, enum tim_oc_id timOCX, uint32_t OCval)
{
     timer_set_oc_value(TIMX, timOCX, OCval);
}


void DCMOT_INIT_timerChannel(S_actuator_dcmotor* d)
{
    S_timer_setup* ts = d->tim_s;
    uint32_t t = d->tim_s->TIMX;

    timer_disable_oc_output(t, d->timOCX);
    timer_set_oc_mode(t, d->timOCX, TIM_OCM_PWM1);
    timer_set_oc_value(t, d->timOCX, 0);
    timer_enable_oc_output(t, d->timOCX);

    model_timer_SET_outputCompare(t, d->timOCX, ts->period/2);
}

S_timer_setup* INIT_dcmotorTimer(uint8_t indx)
{
    // initialize timer X for 1ns ticking
    S_timer_setup* tim_s = &(predef_timers[indx]);

    //____________________________________________________
    // clock initialization
	//rcc_periph_clock_enable(tim_s->apbclk);
	//rcc_periph_clock_enable(tim_s->clk);

    timer_reset(tim_s->TIMX);

    /* Set the timers global mode to:
    * - use no divider
    * - alignment edge
    * - count direction up
    */
    timer_set_mode(tim_s->TIMX,
                TIM_CR1_CKD_CK_INT,
                TIM_CR1_CMS_EDGE,
                TIM_CR1_DIR_UP);

    timer_set_prescaler(tim_s->TIMX, tim_s->prsc);
    timer_set_repetition_counter(tim_s->TIMX, 0);
    timer_enable_preload(tim_s->TIMX);
    timer_continuous_mode(tim_s->TIMX);

    timer_set_period(tim_s->TIMX, tim_s->period);

    gpio_set(GPIOC, GPIO12);
    tim_s->inited = 1;
    return tim_s;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
