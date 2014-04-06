/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	robot_config.c
\contacts	Bc. Daniel DAVIDEK	<danieldavidek@gmail.com>
            Bc. Jiri SLIZ       <xslizj00@stud.feec.vutbr.cz>
            Bc. Michal Dvorak   <xdvora0u@stud.feec.vutbr.cz>
\date		2014_03_30
\brief      Choice of which button and sensor predefs are selected = configuration
\descrptn
\license    LGPL License Terms \ref lgpl_license
***********/
/* DOCSTYLE: gr4viton_2014_A <goo.gl/1deDBa> */

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "robot_config.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// TYPE DEFINITIONS
//____________________________________________________
// enumerations
//____________________________________________________
// structs
#if __NOT_USED_ANYMORE
struct _S_robot
{
    // sensors
    S_robot_buttons btns;
    S_robot_ultras ults;
    //S_robot_infras infs;

    // devices
    S_dev_lcd* lcd;
    S_robot_buzzers buzs;

    // state machine
    //-> future: maybe in separate state structure
    uint8_t STARTED; // 1= when the STARTbutton was pushed - program has started
    // usart
    FILE *fus; // FILE USART - pointer to usart device to write strings to
    uint8_t rbuf[RBUFSZ]; // recieve buffer (using ring buffer logic)
    uint8_t tbuf[TBUFSZ]; // transmission buffer (using ring buffer logic)
    // lcd
    FILE *flcd; // FILE LCD - pointer to lcd device display to write strings to
    uint8_t lcd_dbuf[LCD_DBUFSZ]; // lcd data buffer (will use ring buffer logic)
};
#endif // __NOT_USED_ANYMORE
//____________________________________________________
// unions
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// VARIABLE DEFINITIONS
//____________________________________________________
// static variables
//____________________________________________________
// other variables
/****************
 @brief Global robot variable
 must be global for access from interrupts
 ****************/
S_robot R;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
    //____________________________________________________


void ROBOT_initUltras(S_robot* r)
{
    r->ults.uFL = INIT_ultra(0,1.1);
    r->ults.uFR = INIT_ultra(1,1.1);
    r->ults.uL = INIT_ultra(2,1.1);
    r->ults.uR = INIT_ultra(3,1.1);
}
void ROBOT_initButtons(S_robot* r)
{
    r->btns.btnStartEnabled = 0;

    r->btns.bStart = INIT_button(0);
    r->btns.sumo_btn = INIT_button(1);
    r->btns.line_btn = INIT_button(2);
}

void ROBOT_initLcd(S_robot* r)
{
    int ilcd = 0;
    r->flcd = fopenLCD(ilcd, 16,
                   LCD_C_8BIT_2L_5x7_LIGHT,
                   LCD_C_ENTRY_RIGHT_CMOVE,
                   LCD_C_CUR_VIS_STATIC,
                   r->lcd_dbuf, LCD_DBUFSZ);

    r->lcd = &(lcds_predef[ilcd]);
}

void ROBOT_initUsart(S_robot* r)
{
    // uarts[3] = UART4 = tC10,rC11
    r->fus = fopenserial(3, 9600, r->tbuf, TBUFSZ, r->rbuf, RBUFSZ);
}

void ROBOT_initBuzzers(S_robot* r)
{
    r->buzs.bz1 = INIT_buzzer(0);
}

void ROBOT_initDcmotors(S_robot* r)
{
    r->dcs.mFL = INIT_dcmotor(0);
    r->dcs.mFR = INIT_dcmotor(1);
    r->dcs.mBL = INIT_dcmotor(2);
    r->dcs.mBR = INIT_dcmotor(3);
}

void ROBOT_initAll(S_robot* r)
{
    INIT_leds();
    r->STARTED = 0;
    ROBOT_initButtons(r);
    ROBOT_initBuzzers(r);
    ROBOT_initLcd(r);
    ROBOT_initUltras(r);
    ROBOT_initUsart(r);
    ROBOT_initDcmotors(r);

}

    //____________________________________________________
    // interrupt handler functions
    //____________________________________________________
void gpioa_isr()
{
    #if __NOT_IMPLEMENTED_YET
    if(R.btns.btnStartEnabled == 1)
    {
        if(GET_buttonState(R.btns.bStart)) R.STARTED = 1;
    }
    #endif // __NOT_IMPLEMENTED_YET
}
    //____________________________________________________
    // others
E_lifeStyleSelector ROBOT_getLifeStyle(S_robot* r)
{
    //REFRESH_buttonState(r->btns.bStart);
    REFRESH_buttonState(r->btns.line_btn);
    REFRESH_buttonState(r->btns.sumo_btn);
    gpio_clear(PLED,LEDGREEN0|LEDORANGE1|LEDRED2|LEDBLUE3);
    //if(r->btns.bStart->state!= 0) gpio_set(PLED,LEDGREEN0);
    if(r->btns.line_btn->state!= 0) gpio_set(PLED,LEDBLUE3);
    if(r->btns.sumo_btn->state!= 0) gpio_set(PLED,LEDRED2);

    if(r->btns.line_btn->state!= 0 && r->btns.sumo_btn->state== 0)
    {
        return IAM_SHEEP_FOLLOWING_THE_LINE;
    }
    else if(r->btns.line_btn->state== 0 && r->btns.sumo_btn->state!= 0)
    {
        return IAM_SUMO_WARRIOR;
    }
    else
    {
        return IAM_BUGGED_ROBOT;
    }

}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
