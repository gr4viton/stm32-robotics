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
    double coef[ROB_ULTRA_COEF_COUNT] =
    {3, 11, 0.00};

    uint8_t a = 0;
    S_robot_ultras* u = &(r->ults);

    for(a=0; a<ROB_ULTRA_MAX_COUNT; a++)
    {
        u->u[a] = INIT_ultraPredef(a);
        ROBOT_initIsr(u->u[a]->rxport, u->u[a]->exti, u->u[a]->irq,
                      u->u[a]->priority, EXTI_TRIGGER_BOTH);
        ULTRA_setCoefs(u->u[a], coef);
    }

    // connection of the PE pins may be alike the position of the sensors on the robot
                                // trig|echo
    u->uL  = u->u[0]; // PE0|PE1
    u->uR  = u->u[1]; // PE1
    u->uFL = u->u[2]; // PE2
    u->uFR = u->u[3]; // PE3

}

void ROBOT_initButtons(S_robot* r)
{
    S_robot_buttons* b = &(r->btns);

    b->btnStartEnabled = 0;
    // Initialize gpio for buttons
    b->bStart = INIT_buttonPredef(0);
    b->bSumo = INIT_buttonPredef(1);
    b->bLine = INIT_buttonPredef(2);

    // Initialize IRQ for button bStart
    S_sensor_button* bb = b->bStart;
    ROBOT_initIsr(bb->port, bb->exti, bb->irq, ROB_PRIORITY_BUTTON_START, EXTI_TRIGGER_BOTH);
}

void ROBOT_initLcd(S_robot* r)
{
    uint8_t ilcd = 1;
    uint8_t iPins = 0;
    r->flcd = fopenLCD(ilcd, iPins, 16, 1, (two_lines|font_5x7|bus_4bit), //|bus_8bit),
        (writes2right_cursorMovesOnScreen),
        (display_on | cursor_on | cursor_notBlinking),
         r->lcd_dbuf, ROB_LCD_DBUFSZ);

    r->lcd = &(lcds_predef[ilcd]);
}

void ROBOT_initUsart(S_robot* r)
{
    // uarts[3] = UART4 = tC10,rC11
    r->fus = fopenserial(3, 9600, r->tbuf, ROB_US_TBUFSZ, r->rbuf, ROB_US_RBUFSZ);
}

void ROBOT_initBuzzers(S_robot* r)
{
    r->buzs.bz1 = INIT_buzzerPredef(0);
}

void ROBOT_initDcmotors(S_robot* r)
{
    r->dcs.mFL = INIT_dcmotorPredef(0);
    r->dcs.mFR = INIT_dcmotorPredef(1);
    r->dcs.mBL = INIT_dcmotorPredef(2);
    r->dcs.mBR = INIT_dcmotorPredef(3);
}

void ROBOT_initLinCam(S_robot* r)
{
    if(r->life == IAM_LINE_SNIFFER)
        r->cam = INIT_lincamPredef(0);
}

void ROBOT_initInfraArrayAndChannels(S_robot* r, const uint8_t nInfras)
{
    uint8_t a=0;
    S_robot_infras* in = &(r->infs);
    in->nInfs = nInfras;

    for(a=0; a < in->nInfs; a++)
        in->i[a] = INIT_infraPredef(a);

    // Select the channels we want to convert.
    // for injected sampling [max 4 channels], for regular [max 16 ch]
    E_infraBrightVoltage bv;
    E_infraInterruptBrightness ib;
    switch(r->life)
    {
        case(IAM_SUMO_WARRIOR):
            bv = white_highVoltage;
            ib = interruptOn_white;
            break;
        case(IAM_BUGGED_ROBOT):
        case(IAM_LINE_SNIFFER):
        default:
            bv = white_highVoltage;
            ib = interruptOn_black;
    }

    for(a=0;a<nInfras;a++)
    {
        in->channelArray[a] = in->i[a]->channel;
        in->i[a]->brightVolt = bv;
        in->i[a]->treshInterruptBright = ib;
        INFRA_refreshTriggerType(in->i[a]);
    }
}

void ROBOT_initInfras(S_robot* r)
{
    //uint8_t a=0;
    S_robot_infras* in= &(r->infs);
    switch(r->life)
    {
        case(IAM_BUGGED_ROBOT):
            // init 4 corner infra sensors:
            // scanning on channels IN10-IN13 = PC0-PC3 (Extension connectors table)
            ROBOT_initInfraArrayAndChannels(r,4);
            // connection of the PC pins is alike as position of the sensors
            in->iFL = in->i[1];in->iFR = in->i[0]; // FL-> |PC1 PC0| <-FR
            in->iBL = in->i[3];in->iBR = in->i[2]; // BL-> |PC3 PC2| <-BR
            break;
        case(IAM_SUMO_WARRIOR):
            // init 4 corner infra sensors:
            // scanning on channels IN10-IN13 = PC0-PC3 (Extension connectors table)
            ROBOT_initInfraArrayAndChannels(r,4);
            // connection of the PC pins is alike as position of the sensors
            in->iFL = in->i[1];in->iFR = in->i[0]; // FL-> |PC1 PC0| <-FR
            in->iBL = in->i[3];in->iBR = in->i[2]; // BL-> |PC3 PC2| <-BR
            break;
        case(IAM_LINE_SNIFFER):
            // init "100" infra sensors strip
            //for(;a<20;a++)
              //  i->i[a] = INIT_infraPredef(a);
            break;
        default:
            break;
    }

    INFRA_setupInjectedTIM2();
    INFRA_setupInjectedADC1wTIM2(in->channelArray, in->nInfs);
}

void ROBOT_initLifeDebug(S_robot* r)
{
    INIT_leds();
    r->oneSec = 970;
    ROBOT_initBuzzers(r);
    ROBOT_initUsart(r);
    ROBOT_initLcd(r);

    // sensors
    ROBOT_initButtons(r);
    ROBOT_initUltras(r);
    ROBOT_initInfras(r);
    //ROBOT_initLinCam(r);

    //ROBOT_initDcmotors(r);
}

void ROBOT_initLifeSumo(S_robot* r)
{
    INIT_leds();
    r->oneSec = 970;
    ROBOT_initBuzzers(r);
    ROBOT_initUsart(r);
    ROBOT_initLcd(r);

    // sensors
    //ROBOT_initButtons(r);
    ROBOT_initUltras(r);
    ROBOT_initInfras(r);
    //ROBOT_initLinCam(r);

    ROBOT_initDcmotors(r);
}

void ROBOT_initLifeLine(S_robot* r)
{
    INIT_leds();
    r->oneSec = 970;
    ROBOT_initBuzzers(r);
    ROBOT_initUsart(r);
    ROBOT_initLcd(r);

    // sensors
    //ROBOT_initButtons(r);
    ROBOT_initUltras(r);
    ROBOT_initInfras(r);
    ROBOT_initLinCam(r);

    ROBOT_initDcmotors(r);
}


    //____________________________________________________
    // others
E_lifeStyleSelector ROBOT_getLifeStyle(S_robot* r)
{
    E_lifeStyleSelector newLife;

    REFRESH_buttonState(r->btns.bLine);
    REFRESH_buttonState(r->btns.bSumo);

    if(r->btns.bLine->state!= 0 && r->btns.bSumo->state== 0)
    {
        newLife = IAM_LINE_SNIFFER;
    }
    else if(r->btns.bLine->state== 0 && r->btns.bSumo->state!= 0)
    {
        newLife = IAM_SUMO_WARRIOR;
    }
    else
    {
        newLife = IAM_BUGGED_ROBOT;
    }
    return newLife;

}
    //____________________________________________________
    // tictoc
void _tocPrint(FILE* f)
{
    uint32_t t = _toc();
    fprintf(f, "%lums|", t);
}

void _tocPrintFrom(FILE* f,uint32_t start)
{
    uint32_t t = _tocFrom(start);
    fprintf(f, "%lums|", t);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
