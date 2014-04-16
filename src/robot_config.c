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
    r->ults.uFL = INIT_ultraPredef(0,1.1);
    r->ults.uFR = INIT_ultraPredef(1,1.1);
    r->ults.uL = INIT_ultraPredef(2,1.1);
    r->ults.uR = INIT_ultraPredef(3,1.1);
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
    INIT_buttonInterrupt(b->bStart);
}

void ROBOT_initLcd(S_robot* r)
{
    int ilcd = 0;
    r->flcd = fopenLCD(ilcd, 16,
                   LCD_C_8BIT_2L_5x7_LIGHT,
                   LCD_C_ENTRY_RIGHT_CMOVE,
                   LCD_C_CUR_VIS_STATIC,
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
    if(r->life == IAM_SHEEP_FOLLOWING_THE_LINE)
        r->cam = INIT_lincamPredef(0);
}

void ROBOT_initInfraArrayAndChannels(S_robot* r, const uint8_t nInfras)
{
    uint8_t a=0;
    S_robot_infras* in = &(r->infs);
    in->nInfs = nInfras;

    // allocate space - dendent on number of infras
    //*(in->i) = (S_sensor_infra*) malloc(in->nInfs);
    for(a=0; a < in->nInfs; a++)
        in->i[a] = INIT_infraPredef(a);

    // Select the channels we want to convert.
    // for injected sampling, 4 channels max, for regular, 16 max
    for(a=0;a<nInfras;a++)
    {
        in->channelArray[a] = in->i[a]->channel;
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
        case(IAM_SHEEP_FOLLOWING_THE_LINE):
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

void ROBOT_initAll(S_robot* r)
{
    INIT_leds();
    r->STARTED = 0;
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

    //____________________________________________________
    // others
E_lifeStyleSelector ROBOT_getLifeStyle(S_robot* r)
{
    E_lifeStyleSelector newLife;

    REFRESH_buttonState(r->btns.bLine);
    REFRESH_buttonState(r->btns.bSumo);

    if(r->btns.bLine->state!= 0 && r->btns.bSumo->state== 0)
    {
        newLife = IAM_SHEEP_FOLLOWING_THE_LINE;
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
    // interrupts request handlers = interrupt handler functions
void btnStart_isr(void)
{
    // btnStart was pressed
	exti_reset_request(R.btns.bStart->exti); // reset flag
	R.STARTED = 1;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
