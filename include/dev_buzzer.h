/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_ultrasonic.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef _DEV_BUZZER_H_
#define _DEV_BUZZER_H_

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
//#include "robot_config.h"

//_________> local includes
//_________> forward includes




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
/****************
 @brief Structure encapsulating ultrasonic sensor
 ****************/
typedef struct _S_dev_buzzer
{
    // VOLATILE !!!!!!!!!!! ??????
    uint32_t clk;
    uint32_t port;
    uint16_t pin;
    // later it will run through interrupts - for now only busy waiting
    uint32_t period;
} S_dev_buzzer;

//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_dev_buzzer predef_buzzers[3];
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
/****************
 \brief     Initializes MCU ports for ultrasensor
 \param[in]
 \retval    initialized buzzer
 ****************/
S_dev_buzzer* INIT_buzzerPredef(uint8_t index);

/****************
 \brief   Plays R2D2 song with busy - active waiting
 \param
 ****************/
void BUZZ_R2D2song(S_dev_buzzer* buz);

/****************
 \brief  Toggles pin of buzzer - "plays a tone"
 it is played only with square signal, so the sound is not clean in lower frequencies
 \param  half_period - T = half_period/2 --> f = 1/T = 2/half_period
 \param  repetitions how many periods should be repeated
 \retval
 ****************/
void BUZZ_beep2(S_dev_buzzer* buz, uint16_t half_period, uint16_t repetitions);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES




#endif // _DEV_BUZZER_H_
