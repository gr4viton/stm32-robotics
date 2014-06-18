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
/*0,1,2,3*/{PC12,PC11},{PC12,PC11},{PC12,PC11},{PC12,PC11}
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
    //d->a
    /*
    d->a->mode = GPIO_MODE_INPUT;
    d->a->pull = GPIO_PUPD_NONE;
    d->a->exti_enabled = true;
    d->a->exti_trig = EXTI_TRIGGER_BOTH;
*/
    //d->b->pull = GPIO_PUPD_PULLDOWN;
    //gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    //____________________________________________________
    // isntall configuration from [pins] to gpio periphery
    model_gpioMorePins_installConfig(d->pins, nPins);

    d->tim_s = a_tim_s;
    d->TIMX = a_tim_s->TIMX;

    // init
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

    /* timer setup */
    DCMOT_INIT_timerChannel(d);

	return d;
}


void pwm_set_pulse_width(uint32_t timer_peripheral, enum tim_oc_id oc_id, uint32_t pulse_width)
{
     timer_set_oc_value(timer_peripheral, oc_id, pulse_width);
}


void servo_set_position(enum tim_oc_id ch, uint32_t pos_us)
{
     pwm_set_pulse_width(TIM2, ch, pos_us);
}

void DCMOT_INIT_timerChannel(S_actuator_dcmotor* d)
{
    uint8_t q = d->indx;
    S_timer_setup* ts = d->tim_s;
    uint32_t t = d->tim_s->TIMX;

/*
}

void pwm_init_output_channel(uint32_t timer_peripheral, enum tim_oc_id oc_id, volatile uint32_t *gpio_reg, uint32_t gpio_en, uint32_t gpio_port, uint16_t gpio_pin)
{*/
     /* Enable GPIO clock. */
     //rcc_peripheral_enable_clock(gpio_reg, gpio_en);

     /* Set timer channel to output */
     /*
     gpio_set_mode(gpio_port, GPIO_MODE_OUTPUT_50_MHZ,
                   GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
                   gpio_pin);
*/

//pin
     //gpio_mode_setup(gpio_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, gpio_pin);



     timer_disable_oc_output(t, d->timOCX);
     timer_set_oc_mode(t, d->timOCX, TIM_OCM_PWM1);
     timer_set_oc_value(t, d->timOCX, 0);
     timer_enable_oc_output(t, d->timOCX);

    pwm_set_pulse_width(t, d->timOCX, ts->period/2);
}
/*
void pwm_init_timer(volatile uint32_t *reg, uint32_t en, uint32_t timer_peripheral, uint32_t prescaler, uint32_t period)
{

    pwm_init_timer(&RCC_APB1ENR, tim_s->apbclk, tim_s->TIMX, tim_s->prsc, tim_s->period);

}
*/

S_timer_setup* INIT_dcmotorTimer(uint8_t indx)
{
    // initialize timer X for 1ns ticking
    S_timer_setup* tim_s = &(predef_timers[indx]);

    //____________________________________________________
    // clock initialization
	rcc_periph_clock_enable(tim_s->apbclk);
	rcc_periph_clock_enable(tim_s->clk);

/* // wtf?
uint32_t en = tim_s->apbclk; //??%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// RCC_APB1ENR??

     // Enable timer clock.  // wtf?
     rcc_peripheral_enable_clock(&RCC_APB1ENR, en);
*/

     /* Reset TIM1 peripheral */
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


#if __NOT_IMPLEMENTED_YET
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

    // NVIC isr setting
    // enable interrupt in NestedVectorInterrupt
    // -> if some of the timer interrupts is enabled -> it will call the tim isr function
	nvic_enable_irq(tim_s->nvic);
#endif // __NOT_IMPLEMENTED_YET
    return tim_s;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
