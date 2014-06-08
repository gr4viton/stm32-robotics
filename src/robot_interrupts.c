/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	robot_interrupts.c
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
#include "robot_interrupts.h"


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


    //____________________________________________________
    // interrupts request handlers = interrupt handler functions
void exti0_isr(void)
{
    // btnStart was pressed or ultraL triggered
    uint8_t actExti = EXTI0;
    if( exti_get_flag_status(actExti) != 0)
    {

        // btn and ultra
        if( R.STARTED == 0 )
        {
            R.STARTED = 1;
        }
        else
        { // the robot is already alive - button does not work!
            ROBOT_handleUltraEchoOnExti(actExti);
        }
        exti_reset_request(actExti); // reset flag
    }
}

void exti1_isr(void)
{
    uint8_t actExti = EXTI0;
    if( exti_get_flag_status(actExti) != 0)
    {
        //gpio_toggle(PLED,LEDBLUE3);

        if( R.STARTED == 1 )
        { // the robot is already alive - button does not work!
            ROBOT_handleUltraEchoOnExti(EXTI1);
            // flag reseting is done in ULTRA_handleEcho(..)
        }

        exti_reset_request(actExti); // reset flag
    }
}

void exti9_5_isr(void)
{
    gpio_toggle(PLED, LEDGREEN0);
	exti_reset_request(EXTI5);
	exti_reset_request(EXTI6);
	exti_reset_request(EXTI7);
	exti_reset_request(EXTI8);
	exti_reset_request(EXTI9);
}


/*
void exti4_isr(void)
{
    if( R.STARTED == 1 )
    { // the robot is already alive - button does not work!
        ROBOT_handleUltraEchoOnExti(EXTI4);
        // flag reseting is done in ULTRA_handleEcho(..)
    }
    exti_reset_request(EXTI4); // reset flag
}
*/


void ROBOT_initIsr(uint32_t port, uint32_t exti, uint8_t irqn, uint8_t priority, enum exti_trigger_type trig)
{
    // exti
	exti_select_source(exti, port);
	exti_set_trigger(exti, trig);
	exti_enable_request(exti);
    // enable interrupt
	nvic_enable_irq(irqn);
	nvic_set_priority(irqn,priority);
}


void ROBOT_handleUltraEchoOnExti(uint8_t exti)
{
    uint8_t a=0;
    for(a=0;a<ROB_ULTRA_MAX_COUNT; a++)
    {
        if(exti == R.ults.u[a]->exti)
            ULTRA_handleEcho(R.ults.u[a]);
    }
}

void ROBOT_initClkIsr(void)
{
    // for enabling changing SYSCFG - exti line port selection
    // rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_SYSCFGEN);// deprecated
    rcc_periph_clock_enable(RCC_SYSCFG);
}

uint32_t world_time = 0;
uint32_t cmp = 1000;

void tim3_isr()
{
    static uint32_t t = TIM3;
    S_robot_ultras* us = R.ults;
    S_sensor_ultra* u = R.ults.u[0];

    gpio_toggle(PLED,LEDGREEN0);

        u = us->u[0];

    if (timer_get_flag(t, TIM_SR_CC1IF))
    {
        timer_clear_flag(t, TIM_SR_CC1IF);

        switch(u->state)
        {
        case(s1_sending_trigger):
            gpio_clear(PLED,LEDORANGE1);
            u->state = s2_waiting_for_echo;

            break;

        }

        cmp = timer_get_counter(t) + 1000;

        timer_set_oc_value(t, TIM_OC1, cmp);

        gpio_toggle(PLED,LEDBLUE3);
    }

    if( timer_get_flag(t,TIM_SR_UIF) != 0 )
    {
        timer_clear_flag(t, TIM_SR_UIF);

        switch(u->state)
        {
        case(s0_idle_before_trigger):
            // send 10us trigger signal
            cmp = 2000;
            u->cnt_period = 0;
            gpio_set(PLED,LEDORANGE1);
            u->state = s1_sending_trigger;

            break;

        }
        u->cnt_period++;
        //if(u->cnt_period)
        //cnt_period

        cmp = 1;
        world_time++;
        gpio_toggle(PLED,LEDRED2);
        /*
        nvic_clear_pending_irq(NVIC_TIM3_IRQ); // reset flag
        */
    }
    /*

    if( nvic_get_pending_irq(NVIC_TIM3_IRQ) != 0)
    {
        a>10? a++: 0;
        if(a==0) gpio_toggle(PLED,LEDORANGE1);

        nvic_clear_pending_irq(NVIC_TIM3_IRQ); // reset flag
    }
    */
    //timer_clear_flag(t,0xFFFF);
}

#if __NOT_IMPLEMENTED_YET
void exti9_5_isr(void)
{
    if( R.STARTED == 1 )
    {
        ULTRA_checkEchoOnExti(EXTI9_5);
        exti_reset_request(EXTI9_5); // reset flag
    }
}
#endif // __NOT_IMPLEMENTED_YET
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES



#if __NOT_USED_ANYMORE

    static uint16_t actPA;
    static uint16_t actPH;

    if( exti_get_flag_status(actExti) != 0)
    {
        actPA = GPIOA_IDR;
        actPH = GPIOH_IDR;

        if ((actPA & GPIO0) != 0)
        { // PA0 = HI
            gpio_set(PLED, LEDGREEN0);
        }
        else
        { // PA0 = LO
            gpio_clear(PLED, LEDGREEN0);
        }

        if ((actPH & GPIO0) != 0)
        { // PH0 = HI
            gpio_set(PLED, LEDBLUE3);
        }
        else
        { // PH0 = LO
            gpio_clear(PLED, LEDBLUE3);
        }


        gpio_toggle(PLED, LEDGREEN0);

        exti_reset_request(actExti);
    }

#endif // __NOT_USED_ANYMORE
