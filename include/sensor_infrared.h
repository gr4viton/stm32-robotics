/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_infrared.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

#ifndef _SENSOR_INFRARED_H_
#define _SENSOR_INFRARED_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/vector.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include  <libopencm3/stm32/exti.h>

#include <math.h>

//_________> project includes
#include "defines.h"
#include "waitin.h"
//#include "robot_config.h"
#include "main_debug.h"
//_________> local includes
//_________> forward includes

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
// stored samples from adc
#define INFRA_SAMPLES       16
// stored samples from blackwhite sensor
#define BLACKWHITE_SAMPLES  16
//____________________________________________________
//constants (do not change)
//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations

/****************
 @brief It depends on the infrasensor connection
 If the CNY70 transistor connection is inversed it generates
  - lower voltages for white
  - higher voltages for black
 and vice versa.
 (Maybe it is exactly the oposite inversed vs. non-inversed, you should check circuit)
 ****************/
typedef enum _E_infraBrightVoltage
{
    white_highVoltage = 0,
    black_lowVoltage = 0,
    black_highVoltage = 1,
    white_lowVoltage = 1
}E_infraBrightVoltage;

/****************
 @brief On which brightness should the interrupt be triggered
 ****************/
typedef enum _E_infraInterruptBrightness
{
    interruptOn_white = 0,
    interruptOn_black = 1
}E_infraInterruptBrightness;

/****************
 @brief On which voltage should the interrupt be triggered
 ****************/
typedef enum _E_infraVoltageTresholdType
{
    treshVoltage_high = 1,
    treshVoltage_low = 0
}E_infraVoltageTresholdType;


//____________________________________________________
// structs
/****************
 @brief
 ****************/
// another possibility how to solve it->
typedef struct _S_sensor_blackNwhite
{
    volatile bool free; // true if the sensor is over a free space (SUMO-black,LINE-white)

    // rewrite to use the values bit-wise (with mapping)
    volatile bool values[BLACKWHITE_SAMPLES];
}S_sensor_blackNwhite;


/****************
 @brief Structure encapsulating ultrasonic sensor
 ****************/
typedef struct _S_sensor_infra
{
    uint32_t clk;
    uint32_t port;
    uint16_t pin;

    E_infraBrightVoltage brightVolt;          // depends on connection of infra sensor
    E_infraInterruptBrightness treshInterruptOn; // depends on lifeStyle (SUMO->interrupt on white)
    E_infraVoltageTresholdType treshVoltage;     // this is conjuction of the 2 above -> watchdog settings

    uint16_t triggerVal; // value of the adc watchdog trigger interupt

    S_sensor_blackNwhite bnw;
#if __NOT_IMPLEMENTED_YET
    uint8_t adc_setting;
#endif // __NOT_IMPLEMENTED_YET


    uint16_t channel; // chanels to scan (injected)
    volatile double val; // counted last value of the ADC
    double stdev; // standard deviation
    double nStds; // number of standard deviations

    volatile uint8_t indADC; // index of active element in samples, values arrays
    volatile uint16_t samplesADC[INFRA_SAMPLES]; // input from AD conversion
    volatile uint16_t valuesADC[INFRA_SAMPLES]; // previous counted value (mean)
} S_sensor_infra;


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
 S_sensor_infra* INIT_infraPredef(uint8_t index);

/****************
 \brief
 \param
 \retval
 ****************/
void INFRA_setupInjectedTIM2(void);

/****************
 \brief
 \param
 \retval
 ****************/
void INFRA_setupInjectedADC1wTIM2(uint8_t* channel_array, uint8_t nChannels);

/****************
 \brief
 \param
 \retval
 ****************/
void INFRA_addSample(S_sensor_infra* inf, uint16_t sample);

/****************
 \brief
 \param
 \retval
 ****************/
void INFRA_countMean(S_sensor_infra* inf);

/****************
 \brief
 \param
 \retval
 ****************/
void INFRA_addSampleCountMean(S_sensor_infra* inf, uint16_t sample);

/****************
 \brief Sets interrupt trigger treshold value and orientation
 find out if the interrupt is generated on higher voltage from treshold or vice versa
 and set the treshold value to the last value masured
 this is done, because of the setting of adc watchdog (not sure yet..)
 \param
 \retval
 ****************/
void INFRA_setTresholdLastValue(S_sensor_infra* inf, E_infraInterruptBrightness interruptOn);

/****************
 \brief
 \param
 \retval
 ****************/
void adc_finish(uint16_t values[]);

#if __NOT_USED_ANYMORE
void current_init(void);
void current_update(void);
uint16_t INFRA_readNaiive(uint8_t channel);
#endif // __NOT_USED_ANYMORE

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES


#endif  // _SENSOR_INFRARED_H_
