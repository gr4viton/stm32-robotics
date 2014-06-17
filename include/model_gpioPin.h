/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */
#ifndef _MODEL_GPIOPIN_H_
#define _MODEL_GPIOPIN_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/systick.h>

#include <math.h> // pow
//_________> project includes
#include "defines.h"
#include "waitin.h"
//_________> local includes
//_________> forward includes
/****************
 @brief
 ****************/
typedef struct _S_model_gpioPin
{
    uint32_t preidx; // index from predef array

    uint32_t rcc; // gpio port RCC
    uint32_t port; // port address
    uint32_t pin; // pin selection
    uint32_t state; // pin state - 0 1 , hiZ, etc - af??
    uint8_t mode; // gpio_mode (GPIO_MODE_OUTPUT/GPIO_MODE_INPUT)
    uint8_t pull;  // pull Up/Down/None settings

    // isr
    uint8_t exti_enabled; // whether to setup exti line for this pin - def=0
    uint32_t exti; // exti line
    uint8_t irq;   // NVIC irq
    uint8_t priority;

    enum exti_trigger_type trig; // on which edge to trigger isr

} S_model_gpioPin;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//____________________________________________________
//constants (do not change)
/****************
 @brief number of pins defined in predef_gpioPin array
 ****************/
#define PREDEFCOUNT_GPIOPIN 100
/****************
 @brief defines of predef_gpioPin array indexes
 now as the predef_gpioPin is not ordered - it is defined one by one
 later implement the next #if statements and order predef_gpioPin
 ****************/
#define PA15    0
#define PB3     1
#define PA3     2
#define PA1     3
#define PD6     4
#define PB7     5
#define PA7     6
#define PA5     7
#define PE5     8
#define PE1     9

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// LATER ON PRODUCE THIS , and fill the predef_gpioPin

#if __NOT_IMPLEMENTED_YET
// as the predef_gpioPin is not ordered
#define PORTS_BASE      0x0020
//#define PORTS_BASE      0x0000
//will be 0
#define PORTS_OFFSET    0x000F
#define PORT_BASE(PORT) ((PORTS_BASE)+(PORT)*(PORTS_OFFSET))

// port bases
#define PA_BASE         PORT_BASE(0)
#define PB_BASE         PORT_BASE(1)
//...

// pin bases
#define PA(PIN)     ((PA_BASE)+(PIN))
#define PB(PIN)     ((PB_BASE)+(PIN))
//...

// pin indexes
#define PA0         PA(0)
#define PA1         PA(1)
#endif


// exti multilines
// for stm32f4
#define NVIC_EXTI5_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI6_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI7_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI8_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI9_IRQ      NVIC_EXTI9_5_IRQ
#define NVIC_EXTI10_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI11_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI12_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI13_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI14_IRQ     NVIC_EXTI15_10_IRQ
#define NVIC_EXTI15_IRQ     NVIC_EXTI15_10_IRQ


//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
    //____________________________________________________
/****************
 @brief Initialization of a pin as a whole
 -> if exti enabled -> init exti too.. etc.
 ****************/
 S_model_gpioPin* model_gpioPin_INIT(S_model_gpioPin* pin);

/****************
 @brief Initialization of a pin as a gpio (rcc, mode..)
 ****************/
S_model_gpioPin* model_gpioPin_INIT_gpio(S_model_gpioPin* p);

/****************
 @brief Initialization of a pin's exti
 Enables interrupt routine on selected line
 ****************/
S_model_gpioPin* model_gpioPin_INIT_exti(S_model_gpioPin* pin);

void model_gpioPin_INIT_exti2(uint32_t port, uint32_t exti, uint8_t irqn, uint8_t priority, enum exti_trigger_type trig);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#endif  // _XX_H_

