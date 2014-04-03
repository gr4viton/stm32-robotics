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

#ifndef SENSOR_BUTTON_H_INCLUDED
#define SENSOR_BUTTON_H_INCLUDED

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
//_________> project includes
#include "main.h"
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
    IAM_BUGGED_ROBOT =0,            // for debugging and development
    IAM_SUMO_WARRIOR ,              // for mini-sumo competition
    IAM_SHEEP_FOLLOWING_THE_LINE    // for line-follower
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
    uint16_t state; // state of the button 0 or 1
}S_sensor_button;

/****************
 @brief Structure defining all input buttons of this project
 ****************/
typedef struct _S_sensor_buttonsInput
{
    S_sensor_button start_btn;
    S_sensor_button sumo_btn;
    S_sensor_button line_btn;
}S_sensor_buttonsInput;
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_sensor_buttonsInput btns;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS
/****************
 \brief   Initializes all buttons of the project
 ****************/
void INIT_buttons(void);

/****************
 \brief   Activates clock and pin in port defined in btn
 \param   btn Structure of the button
 ****************/
void ACTIVATE_button(S_sensor_button* btn);

/****************
 \brief   Refreshes the state of the button
 \param   btn Structure of the button
 ****************/
void REFRESH_buttonState(S_sensor_button* btn);

/****************
 \brief   Finds out which life should be lived by pushed "buttons"
 \returns life Selected lifestyle
 ****************/
E_lifeStyleSelector GET_lifeStyle(void);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES




#endif // SENSOR_GPIO
