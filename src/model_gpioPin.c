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

//#define PIN_SET(PORT,PIN) {.rcc=RCC_GPIO(PORT),.port=GPIO(PORT),.pin=GPIO(PIN),.exti=EXTI(PIN),.irq=NVIC_EXTI(PIN)_IRQ}

/****************
 @brief
 defaultly is every pin:
 output with non-pull resistors
 ****************/

/*
TIM2_CH1 = A0;A15
TIM2_CH2 = A1;B3
TIM2_CH3 = A2;B10
TIM2_CH4 = A3;B11*/
S_model_gpioPin predef_gpioPin[] =
{
/*0*/ {.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*1*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*2*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*3*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*4*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*5*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ} // u3 echo
/*6*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*7*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ} // u3 trig
/*8*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*9*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*10*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*11*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*12*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*13*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*14*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*15*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOA,.port=GPIOA,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
/*16*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*17*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*18*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*19*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*20*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*21*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*22*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*23*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*24*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*25*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*26*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*27*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*28*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*29*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*30*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*31*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOB,.port=GPIOB,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
/*32*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*33*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*34*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*35*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*36*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*37*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*38*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*39*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*40*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*41*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*42*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*43*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*44*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*45*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*46*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*47*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOC,.port=GPIOC,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
/*48*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*49*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*50*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*51*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*52*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*53*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*54*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*55*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*56*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*57*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*58*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*59*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*60*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*61*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*62*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*63*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOD,.port=GPIOD,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}

#if __IT_IS_TOO_BIG_WITH_IT //maybee
/*64*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*65*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*66*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*67*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*68*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*69*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*70*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*71*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*72*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*73*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*74*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*75*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*76*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*77*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*78*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*79*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOE,.port=GPIOE,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}

/*80*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*81*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*82*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*83*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*84*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*85*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*86*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*87*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*88*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*89*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*90*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*91*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*92*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*93*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*94*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*95*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOH,.port=GPIOH,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
/*96*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO0,.exti_enabled=0,.exti=EXTI0,.irq=NVIC_EXTI0_IRQ}
/*97*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO1,.exti_enabled=0,.exti=EXTI1,.irq=NVIC_EXTI1_IRQ}
/*98*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO2,.exti_enabled=0,.exti=EXTI2,.irq=NVIC_EXTI2_IRQ}
/*99*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO3,.exti_enabled=0,.exti=EXTI3,.irq=NVIC_EXTI3_IRQ}
/*100*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO4,.exti_enabled=0,.exti=EXTI4,.irq=NVIC_EXTI4_IRQ}
/*101*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO5,.exti_enabled=0,.exti=EXTI5,.irq=NVIC_EXTI5_IRQ}
/*102*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO6,.exti_enabled=0,.exti=EXTI6,.irq=NVIC_EXTI6_IRQ}
/*103*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO7,.exti_enabled=0,.exti=EXTI7,.irq=NVIC_EXTI7_IRQ}
/*104*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO8,.exti_enabled=0,.exti=EXTI8,.irq=NVIC_EXTI8_IRQ}
/*105*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO9,.exti_enabled=0,.exti=EXTI9,.irq=NVIC_EXTI9_IRQ}
/*106*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO10,.exti_enabled=0,.exti=EXTI10,.irq=NVIC_EXTI10_IRQ}
/*107*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO11,.exti_enabled=0,.exti=EXTI11,.irq=NVIC_EXTI11_IRQ}
/*108*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO12,.exti_enabled=0,.exti=EXTI12,.irq=NVIC_EXTI12_IRQ}
/*109*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO13,.exti_enabled=0,.exti=EXTI13,.irq=NVIC_EXTI13_IRQ}
/*110*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO14,.exti_enabled=0,.exti=EXTI14,.irq=NVIC_EXTI14_IRQ}
/*111*/,{.pull=GPIO_PUPD_NONE,.mode=GPIO_MODE_OUTPUT,.rcc=RCC_GPIOI,.port=GPIOI,.pin=GPIO15,.exti_enabled=0,.exti=EXTI15,.irq=NVIC_EXTI15_IRQ}
#endif // 0

};


#if __NOT_USED_ANYMORE
S_model_gpioPin predef_gpioPin[] =
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
#endif // __NOT_USED_ANYMORE
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
    // INITs
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
	exti_set_trigger(p->exti, p->exti_trig);
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
    pin.exti_trig = trig;
    model_gpioPin_INIT_exti(&pin);
    return;
}
    //____________________________________________________
    // gpio operations
void model_gpioPin_set(S_model_gpioPin* p){     gpio_set(p->port, p->pin); }
void model_gpioPin_clear(S_model_gpioPin* p){   gpio_clear(p->port, p->pin); }
void model_gpioPin_toggle(S_model_gpioPin* p){  gpio_toggle(p->port, p->pin);}

uint16_t model_gpioPin_get(S_model_gpioPin* p){ return gpio_get(p->port, p->pin); }
    //____________________________________________________
    // connection
void model_gpioMorePins_connectPins(S_model_gpioPin** pins,uint32_t* conArr, uint32_t num_of_pins)
{
    uint8_t q=0;
    for(q=0;q<num_of_pins;q++)
    {
        pins[q] = &( predef_gpioPin[ conArr[q] ] );
        // need some mechanism of collision detection! -> everything should be cli -> do not use GUI!
    }
}


void model_gpioMorePins_installConfig(S_model_gpioPin** pins, uint32_t num_of_pins)
{
    uint8_t q = 0;
    for(q=0;q<num_of_pins;q++)
    {
        model_gpioPin_INIT(pins[q]);
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
