/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_button.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief      Initialization of start button, game selecting shortcut etc.
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */
/*
 inspiration:
 * https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/stm32-h103/exti_both/exti_both.c
 =EXTI NVIC interrupts
*/
#ifndef SENSOR_BUTTON_H_INCLUDED
#define SENSOR_BUTTON_H_INCLUDED

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>

//_________> project includes
#include "defines.h"
//_________> local includes
//_________> forward includes




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
//#define RCC_BUTTON RCC_GPIOA
//#define
//____________________________________________________
//constants (do not change)
//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
/****************
 @brief Enumeration of programmed behaviour of robot
 ****************/
typedef enum _E_lifeStyleSelector
{
    IAM_BUGGED_ROBOT =0,  // for debugging and development
    IAM_SUMO_WARRIOR =42, // for mini-sumo competition
    IAM_LINE_SNIFFER =69  // for line-follower
} E_lifeStyleSelector;
//____________________________________________________
// structs
/****************
 @brief Structure defining one button
 ****************/
typedef struct _S_sensor_button
{
    uint32_t pclk; // button port RCC
    uint32_t port; // button port address
    uint8_t pull; // button pull resistor
    uint16_t pin; // button pin

    uint8_t priority; // priority of isr
    uint8_t irq; // button NVIC irq
    uint32_t exti; // button EXTI
    enum exti_trigger_type exti_triggerDir; // button interrupt triger - rising / falling / both


    uint16_t state; // state of the button 0 or 1
    // activeZero?
}S_sensor_button;
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_sensor_button buttons_predef[];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS


/****************
 \brief   Activates clock and pin in port defined in buttons_predef
 \param   btn Structure of the button
 ****************/
S_sensor_button* INIT_buttonPredef(uint8_t index);

/****************
 \brief   Refreshes the state of the button
 \param   btn Structure of the button
 ****************/
void REFRESH_buttonState(S_sensor_button* btn);

/****************
 \brief   Refreshes and returnes the state of the button
 \param   btn Structure of the button
 ****************/
uint8_t GET_buttonState(S_sensor_button* btn);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES




#endif // SENSOR_GPIO
