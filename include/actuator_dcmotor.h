/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	actuator_dcmotor.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef _ACTUATOR_DCMOTOR_H_
#define _ACTUATOR_DCMOTOR_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//_________> project includes
#include "defines.h"
#include "waitin.h"
//_________> local includes
#include "model_gpioPin.h"
//_________> forward includes


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
/****************
 @brief number of pins each dcmotor actuator has
 ****************/
#define ACTUATOR_DCMOTOR_PINS_COUNT     2
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
typedef struct _S_actuator_dcmotor
{
    uint32_t* conArr;
    S_model_gpioPin *pins[2];
    S_model_gpioPin* A; // actuator pin IN1
    S_model_gpioPin* B; // actuator pin IN2

    float dutyCycle;
    uint32_t dcTicks; // duty cycle tics -> counted from dutyCycle and pwm timer period

    // tick settings
    S_timer_setup* tim_s;  // pointer to the timer structure for tick counting
    uint32_t TIMX;         // address of timer for tick counting
    uint8_t indx;          // index of compare register in timer
    enum tim_oc_id timOCX; // compare register of the ultra sensor in the timer
    uint16_t nTriggerTicks; // trigger interval

    // tick counting
    uint16_t nOwerflow; // number of periods from echoStart to echoEnd
    uint32_t ticksStart; // ticks in timer on EchoStart
    uint32_t ticksEnd;   // ticks in timer on EchoEnd
    uint32_t nTicks;     // number of ticks from echoStart to echoEnd

    /// not used
    //uint16_t EN;  // actuator pin ENable
    //uint16_t uADC // hbridge voltage as a function of current going through actuator

}S_actuator_dcmotor;
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS

//extern S_actuator_dcmotor dcmotor_predef[];
//extern S_model_gpioPin predef_gpioPin[PREDEFCOUNT_GPIOPIN];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
    //____________________________________________________
    // ..
/****************
 @brief
 ****************/
S_actuator_dcmotor* INIT_dcmotorPredef(uint8_t index, S_timer_setup* a_tim_s);
/****************
 @brief
 ****************/
S_timer_setup* INIT_dcmotorTimer(uint8_t indx);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#endif  // _ACTUATOR_DCMOTOR_H_

