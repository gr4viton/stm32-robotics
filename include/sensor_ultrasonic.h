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

#ifndef DEV_ULTRASONIC_H_INCLUDED
#define DEV_ULTRASONIC_H_INCLUDED

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
typedef struct
{
    // VOLATILE !!!!!!!!!!! ??????
    uint32_t clk;
    uint8_t irq;
    uint32_t txport;
    uint32_t rxport;
    uint16_t txpin;
    uint16_t rxpin;

    double dist;
    double proportion;
    uint32_t nOverflows;
    uint32_t ticksStart;
    uint32_t ticksEnd;
    uint32_t nTicks;

} S_sensor_ultra;

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
 \brief Initializes MCU ports for ultrasensor
 \param[in]
 \retval
 ****************/
S_sensor_ultra* INIT_ultra(uint8_t index, double prop);

/****************
 \brief
 \param
 \retval
 ****************/
double ULTRA_getDist(S_sensor_ultra* ult);

/****************
 \brief
 \param
 \retval
 ****************/
void ULTRA_signalSend(S_sensor_ultra *dev);

/****************
 \brief
 \param
 \retval
 ****************/
void ULTRA_signalAcquired(S_sensor_ultra *dev);

/****************
 \brief   Try ultrasonic sensor function without interrupts
 \param
 \retval
 ****************/
 void ULTRA_debug_try(void);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES




#endif // DEV_ULTRASONIC_H_INCLUDED
