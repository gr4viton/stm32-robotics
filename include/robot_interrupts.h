/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	robot_interrupts.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */
#ifndef _ROBOT_INTERRUPTS_H_
#define _ROBOT_INTERRUPTS_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes

//_________> local includes
//_________> forward includes
#include "robot_config.h"
/*
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>

#include <stdlib.h>

#include "defines.h"
#include "waitin.h"

//#include "robot_config.h"

// devices = I/O
#include "dev_serial.h"
#include "dev_LCD_HD44780.h"
#include "LCD_HD44780.h"
#include "dev_buzzer.h"
#include "led_f4.h"

// sensors
#include "sensor_button.h"
#include "sensor_ultrasonic.h"
#include "sensor_lineCamera.h"
#include "sensor_infrared.h"

// actuators
#include "actuator_dcmotor.h"
*/
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

/****************
 \brief Go through all ultras and check wheter their exti is the one being triggered
 \param exti line from caller interrupt which is being triggered
 ****************/
void ROBOT_handleUltraEchoOnExti(uint8_t exti);

/****************
 \brief Enable interrupt on selected line
 \param
 ****************/
void ROBOT_initIsr(uint32_t port, uint32_t exti, uint8_t irqn, uint8_t priority, enum exti_trigger_type trig);

/****************
 \brief Start clock of SYSCFG setup register - exti line port selection
 must be called before ROBOT_initIsr for it to work!
 ****************/
void ROBOT_initClkIsr(void);

/****************
 @brief Common function for exti isr for echo line of ultrasonic sensors
 ultra exti on lines: 2,3,6,7
 ****************/
void extiX_isr(uint8_t actExti);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#endif  // _ROBOT_INTERRUPTS_H_

