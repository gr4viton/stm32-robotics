/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_button.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "sensor_button.h"

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DEFINITIONS
//____________________________________________________
// static variables
//____________________________________________________
// other variables
S_sensor_buttonsInput btns;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE

void INIT_buttons(void)
{
    S_sensor_button* btn;

    // start button - PA0
    btn = &(btns.start_btn);
	// Set GPIO0 (in GPIO port A) = blue button to 'input open-drain'
    btn->pin=GPIO0; btn->port=GPIOA; btn->pclk=RCC_GPIOA; btn->pull=GPIO_PUPD_NONE;
    ACTIVATE_button(btn);

    // sumobot button selector - PC7
    btn = &(btns.sumo_btn);
    btn->pin=GPIO7; btn->port=GPIOC; btn->pclk=RCC_GPIOC; btn->pull=GPIO_PUPD_PULLUP;
    ACTIVATE_button(btn);

    // line follower button selector - PC8
    btn = &(btns.line_btn);
    btn->pin=GPIO8; btn->port=GPIOC; btn->pclk=RCC_GPIOC; btn->pull=GPIO_PUPD_PULLUP;
    ACTIVATE_button(btn);
}

void ACTIVATE_button(S_sensor_button* btn)
{
    rcc_periph_clock_enable(btn->pclk);
	gpio_mode_setup(btn->port, GPIO_MODE_INPUT, btn->pull, btn->pin);
}

void REFRESH_buttonState(S_sensor_button* btn)
{
    btn->state = GPIO_IDR(btn->port) & btn->pin;
}

E_lifeStyleSelector GET_lifeStyle(void)
{
    REFRESH_buttonState(&(btns.start_btn));
    REFRESH_buttonState(&(btns.line_btn));
    REFRESH_buttonState(&(btns.sumo_btn));
    gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);
    if(btns.start_btn.state != 0)
        gpio_set(PLED,LEDGREEN0);
    if(btns.line_btn.state != 0)
        gpio_set(PLED,LEDBLUE3);
    if(btns.sumo_btn.state != 0)
        gpio_set(PLED,LEDRED2);

    return(IAM_BUGGED_ROBOT);
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
