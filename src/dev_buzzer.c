/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	sensor_ultrasonic.c
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
#include "dev_buzzer.h"

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
 \brief Predefined buzzer sensors ports & clocks [tx=out;rx=in]
 ****************/
S_dev_buzzer buzzer_predef[3] = {
    {.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO2 }
    ,{.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO3 }
    ,{.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO4 }
};

/*
 SONG format =
 for now there are periods stored as interrupts for frequencies are not implemented yet
 so - { 0.5.period_of_tone , number_of_repetitions}
 !! last element must be 0,0 -> for defining the end of a song - analogous of string
 type uint16_t
 LATER -> with interrupt
 - for frequency - frequencies in Hz = 1-32768Hz = sufficient hearable
 - for lenght of song - time in ms = 1-32.768 sec lenght of longest tone
 in case of more songs - create separate headerfile songs.h
 */
uint16_t SONG_r2d2[100][2] =
{
    {1, 100}, {2, 100}, {1, 10}, {4, 10}, {6, 50}, {5, 90}, {8, 190},{1, 100}, {8, 190},{1, 100},
    {8, 190},{1, 100},{8, 190},{1, 100},{6, 50}, {5, 90},{6, 50}, {5, 90}, {0, 0}
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE

S_dev_buzzer* INIT_buzzer(uint8_t index)
{
    S_dev_buzzer* buz = &buzzer_predef[index];

	rcc_periph_clock_enable(buz->clk);
	gpio_mode_setup(buz->port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, buz->pin);
	gpio_clear(buz->port, buz->pin);

    // future - timer setup

    return buz;
}

void BUZZ_R2D2song(S_dev_buzzer* buz)
{
    uint16_t* p = SONG_r2d2[0];
    // p = 0.5 tone_period
    // p+1 = how many repetitions of period
    while(*p!=0)
    {
        BUZZ_beep2(buz,*p,*(p+1));
        p+=2;
    }
}

void BUZZ_beep2(S_dev_buzzer* buz, uint16_t half_period, uint16_t repetitions)
{
    for(repetitions++;repetitions>1;repetitions--)
    {
        gpio_toggle(buz->port, buz->pin);
        mswait(half_period);
        gpio_toggle(buz->port, buz->pin);
    }

}

#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE

void tim3_isr(void)
{
    if (timer_interrupt_source(TIM5, TIM_SR_CC4IF))
    {
        //if (++counter == 0)
        //{
            //gpio_toggle(PLED,LEDBLUE3);
        //}
    }
}
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING


    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES

