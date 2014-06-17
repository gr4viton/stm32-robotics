/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	model_gpioPin.c
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
#include "model_gpioPin.h"
// add definition as a function of a board where:
// #define NVIC_EXTI15 NVIC_EXTI15_10_IRQ
// #define NVIC_EXTI14 NVIC_EXTI15_10_IRQ
// ...etc.

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
//#define PIN_SET(PORT,PIN) {.rcc=RCC_GPIO(PORT),.port=GPIO(PORT),.pin=GPIO(PIN),.exti=EXTI(PIN),.irq=NVIC_EXTI(PIN)_IRQ}

/****************
 @brief
 defaultly is every pin:
 output with non-pull resistors
 ****************/
S_model_gpioPin const predef_gpioPin[] =
{
/*0*/ {.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
/*1*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
//ultras
/*2*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ} //rx
/*3*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}

/*4*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ} //rx
/*5*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI5_IRQ}

/*6*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ} //rx
/*7*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}

/*8*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ} //rx
/*9*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
};

/*
TIM2_CH1 = A0;A15
TIM2_CH2 = A1;B3
TIM2_CH3 = A2;B10
TIM2_CH4 = A3;B11*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________

S_model_gpioPin* model_gpioPin_INIT(S_model_gpioPin* p)
{ // init all
    model_gpioPin_INIT_gpio(p);
    if(p->exti_enabled != 0) model_gpioPin_INIT_exti(p);
	return p;
}

S_model_gpioPin* model_gpioPin_INIT_gpio(S_model_gpioPin* p)
{
    rcc_periph_clock_enable(p->rcc);
	gpio_mode_setup(p->port, p->mode, p->pull, p->pin);
	return p;
}


S_model_gpioPin* model_gpioPin_INIT_exti(S_model_gpioPin* p)
{
    // exti
	exti_select_source(p->exti, p->port);
	exti_set_trigger(p->exti, p->trig);
	exti_enable_request(p->exti);
    // enable interrupt
	nvic_enable_irq(p->irq);
	nvic_set_priority(p->irq, p->priority);
	return p;
}

void model_gpioPin_INIT_exti2(uint32_t port, uint32_t exti, uint8_t irqn, uint8_t priority, enum exti_trigger_type trig)
{
    S_model_gpioPin pin;
    pin.exti = exti;
    pin.port = port;
    pin.irq = irqn;
    pin.priority = priority;
    pin.trig = trig;
    model_gpioPin_INIT_exti(&pin);
    return;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
