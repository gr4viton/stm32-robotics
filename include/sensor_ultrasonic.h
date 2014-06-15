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
#include <libopencm3/stm32/exti.h>

#include <libopencm3/cm3/systick.h>

#include <math.h> // pow
//_________> project includes
#include "defines.h"
#include "waitin.h"
//_________> local includes
//_________> forward includes




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
// degree of the polynomial

#define ROB_ULTRA_COEF_COUNT 3
//____________________________________________________
//constants (do not change)
//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
/****************
 @brief Enumerator of ultra sensor states
  ****************/
 typedef enum _E_sensor_ultra_state
 {
     s0_idle_before_trigger = 0,
     s1_sending_trigger,
     s2_waiting_for_echo,
     s3_waiting_for_echo_end
 } E_sensor_ultra_state;
//____________________________________________________
// structs
/****************
 @brief Structure encapsulating ultrasonic sensor
 ****************/
typedef struct _S_sensor_ultra
{
    // pin settings
    uint32_t clk;
    uint32_t txport;
    uint32_t rxport;
    uint16_t txpin;
    uint16_t rxpin;

    // pin interrupts
    uint32_t exti; // exti line
    uint8_t irq;   // NVIC irq
    uint8_t priority;

    uint8_t echoState;          // memory for distinguishing between rising and falling edges
    E_sensor_ultra_state state; // state in which the sensor is


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

    // distance measurement
    double dist;
    double coef[ROB_ULTRA_COEF_COUNT];
    // dist = coef[0] + coef[1]*nTicks + coef[2]*nTicks^2 + .. + coef[N]*nTicks^N
    // .. where N = ROB_ULTRA_COEF_COUNT
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
 ****************/
S_sensor_ultra* INIT_ultraPredef(uint8_t index, S_timer_setup* tim_s);

/****************
 @brief Initializes timer for 4 ultrasensors
 ****************/
S_timer_setup* INIT_ultraTimer(uint8_t indx);

/****************
 \brief Calculates the distance from measured period (nTicks)
 ****************/
double ULTRA_calcDist(S_sensor_ultra* ult);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// deprecated
/****************
 \brief Generate a pulse on trigger signal line
 ****************/
void ULTRA_signalSend(S_sensor_ultra *dev);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



/****************
 \brief On start of sending trigger signal
 ****************/

void ULTRA_tiggerStart(S_sensor_ultra *u);
/****************
 \brief On end of sending trigger signal
 ****************/
void ULTRA_tiggerEnd(S_sensor_ultra *u);

/****************
 \brief On echo signal started
 ****************/
void ULTRA_echoStarted(S_sensor_ultra *ult);

/****************
 \brief On echo signal end
 ****************/
void ULTRA_echoEnded(S_sensor_ultra *ult);

/****************
 \brief The echo signal changed - handle it
 ****************/
void ULTRA_handleEcho(S_sensor_ultra* ult);

/****************
 \brief Sets coeficients of polynomial function modeling sensor distance
 \param coef array of coeficients
 ****************/
void ULTRA_setCoefs(S_sensor_ultra* ult, double coef[]);


/****************
 \brief   Try ultrasonic sensor function without interrupts
 ****************/
 void ULTRA_debug_try(void);

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

/*
void exti0_1_isr(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    // Toggle LED2
    STM_EVAL_LEDToggle(LED2);

    // Clear the EXTI line 0 pending bit
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}
*/

#endif // DEV_ULTRASONIC_H_INCLUDED
