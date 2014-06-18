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
#include "model_gpioPin_defines.h"


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
    uint8_t state; // pin state - 0 1 , hiZ, etc - af??
    uint8_t mode; // gpio_mode (GPIO_MODE_OUTPUT/GPIO_MODE_INPUT)
    uint8_t pull;  // pull Up/Down/None settings

    // isr
    uint8_t exti_enabled; // whether to setup exti line for this pin - def=0
    uint32_t exti; // exti line
    uint8_t irq;   // NVIC irq
    uint8_t priority;

    enum exti_trigger_type exti_trig; // on which edge to trigger isr

} S_model_gpioPin;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//____________________________________________________
//constants (do not change)
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
    // INITs
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

/****************
 @brief legacy
 ****************/
void model_gpioPin_INIT_exti2(uint32_t port, uint32_t exti, uint8_t irqn, uint8_t priority, enum exti_trigger_type trig);

    //____________________________________________________
    // gpio operations

uint16_t model_gpioPin_get(S_model_gpioPin* p);
void model_gpioPin_set(S_model_gpioPin* p);
void model_gpioPin_clear(S_model_gpioPin* p);
void model_gpioPin_toggle(S_model_gpioPin* p);

    //____________________________________________________
    // connections
/****************
 @brief Connects pins (redistribute pointers) according to connection array [conArr]
 later will be in separate model_gpioMorePins?
 ****************/
void model_gpioMorePins_connectPins(S_model_gpioPin** pins,uint32_t* conArr, uint32_t num_of_pins);

/****************
 @brief Installs gpio periphery from [pins] configuration
 later will be in separate model_gpioMorePins?
 ****************/
void model_gpioMorePins_installConfig(S_model_gpioPin** pins, uint32_t num_of_pins);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#endif  // _XX_H_

