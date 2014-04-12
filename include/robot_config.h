/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	robot_config.h
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */


#ifndef _ROBOT_CONFIG_H_
#define _ROBOT_CONFIG_H_


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> system includes
//_________> project includes
//_________> local includes
//_________> forward includes

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>

#include "defines.h"
#include "waitin.h"

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


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// MACRO DEFINITIONS
//____________________________________________________
//constants (user-defined)
// USART recieve buffer size
#define RBUFSZ 1024
// USART transmission buffer size
#define TBUFSZ 1024
// LCD data buffer size
#define LCD_DBUFSZ 1024
//____________________________________________________
//constants (do not change)
#define btnStart_isr exti0_isr
//____________________________________________________
// macro functions (do not use often!)
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs

/****************
 @brief Structure defining all input buttons of the robot
 ****************/
typedef struct _S_robot_buttons
{
    S_sensor_button* bStart;
    S_sensor_button* bSumo;
    S_sensor_button* bLine;

    //-> future: maybe in separate state structure
    uint8_t btnStartEnabled; // 1=for accepting interrupts
}S_robot_buttons;

/****************
 @brief Structure defining all ultrasonic sensors of the robot
 ****************/
typedef struct _S_robot_ultras
{
    S_sensor_ultra* uFL;
    S_sensor_ultra* uFR;
    S_sensor_ultra* uL;
    S_sensor_ultra* uR;
} S_robot_ultras;

/****************
 @brief Structure defining all buzzers of the robot
 ****************/
typedef struct _S_robot_buzzers
{
    S_dev_buzzer* bz1;
} S_robot_buzzers;

/****************
 @brief Structure defining all dcmotors of the robot
 ****************/
typedef struct _S_robot_dcmotors
{
    S_actuator_dcmotor* mFL; // motor front left
    S_actuator_dcmotor* mFR; // motor front right
    S_actuator_dcmotor* mBL; // motor back left
    S_actuator_dcmotor* mBR; // motor back right
} S_robot_dcmotors;


typedef struct _S_robot_infras
{
    S_sensor_infra* iFL; // infra front left
    S_sensor_infra* iFR; // infra front right
    S_sensor_infra* iBL; // infra back left
    S_sensor_infra* iBR; // infra back right
} S_robot_infras;


/****************
 @brief Structure defining all modules of the robot
 ****************/
typedef struct _S_robot
{
    // sensors
    S_robot_buttons btns;
    S_robot_ultras ults;
    S_robot_infras infs;
    S_sensor_lincam* cam;

    // devices
    S_dev_lcd* lcd;
    S_robot_buzzers buzs;

    // actuators
    S_robot_dcmotors dcs;

    // state machine
    E_lifeStyleSelector life;
    //-> future: maybe in separate state structure
    uint8_t STARTED; // 1= when the STARTbutton was pushed - program has started


    // usart
    FILE *fus; // FILE USART - pointer to usart device to write strings to
    uint8_t rbuf[RBUFSZ]; // recieve buffer (using ring buffer logic)
    uint8_t tbuf[TBUFSZ]; // transmission buffer (using ring buffer logic)
    // lcd
    FILE *flcd; // FILE LCD - pointer to lcd device display to write strings to
    uint8_t lcd_dbuf[LCD_DBUFSZ]; // lcd data buffer (will use ring buffer logic)
}S_robot;

//____________________________________________________
// unions

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
extern S_robot R;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DECLARATIONS
    //____________________________________________________
    // INITS

/****************
 \brief   Initializes all ultrasonic sensors of the robot
 ****************/
void ROBOT_initUltras(S_robot* r);

/****************
 \brief   Initializes all buttons of the robot
 ****************/
void ROBOT_initButtons(S_robot* r);

/****************
 \brief   Initializes all buzzers of the robot
 ****************/
void ROBOT_initBuzzers(S_robot* r);

/****************
 \brief  Initializes lcd display
 \retval pointer to the LCD display device - maybe for debug only in future
 ****************/
void ROBOT_initLcd(S_robot* r);

/****************
 \brief  Initializes usart communication
 ****************/
void ROBOT_initUsart(S_robot* r);

/****************
 \brief  Initializes linear camera
 ****************/
void ROBOT_initLinCam(S_robot* r);

/****************
 \brief
 ****************/
void ROBOT_initDcmotors(S_robot* r);

/****************
 \brief
 ****************/
void ROBOT_initInfras(S_robot* r);

/****************
 @brief Initializes whole robot
 ****************/
void ROBOT_initAll(S_robot* r);
    //____________________________________________________
    // others
/****************
 \brief   Finds out which life should be lived by pushed "buttons"
 \retval life Selected lifestyle
 ****************/
E_lifeStyleSelector ROBOT_getLifeStyle(S_robot* r);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

#include "sensor_infrared.h"

#endif  // _ROBOT_CONFIG_H_
