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
/*
 inspiration on systick:
 * https://github.com/libopencm3/libopencm3-examples/tree/master/examples/stm32/f4/stm32f4-discovery/tick_blink
*/
#ifndef WAITIN_H_INCLUDED
#define WAITIN_H_INCLUDED

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/timer.h>
#include "defines.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)

#define WAITIN_SYSCLK_1MS
//#define WAITIN_SYSCLK_100NS

// for using of wwait and twait
#define LEGACY_WAITIN
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
 @brief Timer setup for setting a timer (of ultrasound sensor mainly)
 ****************/
typedef struct _S_timer_setup
{
    uint8_t inited; // 0 if not initialized yet; 1 othervise
    uint32_t TIMX; // timer address
    uint32_t period;
    uint32_t full_freq; // it depends on the mode and to which apb1/2 is the timer connected
    uint32_t prsc;
    uint32_t clk;
    uint32_t apbclk;
    uint32_t nvic;
    /*
    // mode
    uint32_t clock_div;
    uint32_t alignment;
    uint32_t direction;
    */
} S_timer_setup;
//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern volatile uint32_t tic_toc_start;
extern S_timer_setup predef_timers[];

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
/****************
 \brief
 \param
 \retval
 ****************/
void INIT_clk(void);

/****************
 \brief
 \param
 \retval
 ****************/
void mswait(uint32_t delay);

/****************
 \brief Stores systick stamp to global variable (MATLAB alike)
 \retval also returns the stamp
 ****************/
uint32_t _tic(void);

/****************
 \brief Count ticks of systick from the last call of _tic
 \retval interval between _tic and _toc in systick units
 ****************/
uint32_t _toc(void);

/****************
 \brief Count ticks of systick from the start time stamp
 \param start time stamp
 \retval interval between start time stamp and _tocFrom in systick units
 ****************/
uint32_t _tocFrom(uint32_t start);


#ifdef LEGACY_WAITIN
/****************
 \brief
 \param
 \retval
 ****************/
void wwait(void);

/****************
 \brief
 \param
 \retval
 ****************/
void twait(uint32_t times);

/****************
 \brief
 \param
 \retval
 ****************/
void tnop(uint32_t times);
#endif // LEGACY_WAITIN

/****************
 \brief   Sets the timer3 for 100ms intervals
 ****************/
//void timtick_setup(void);

#ifdef WAITIN_SYSCLK_100NS
/****************
 \brief
 \param
 \retval
 ****************/
void ns100(uint32_t times);
#endif // WAITIN_SYSCLK_100NS

#endif // WAITIN_H_INCLUDED
