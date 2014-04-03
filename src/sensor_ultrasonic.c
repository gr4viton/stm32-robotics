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
#include "sensor_ultrasonic.h"

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
 \brief Predefined ultrasonic sensors ports & clocks [tx=out;rx=in]
 ****************/
S_sensor_ultra ultras_predef[3] = {
    {.clk=RCC_GPIOE, .txport=GPIOE, .rxport=GPIOE, .txpin=GPIO4, .rxpin=GPIO5 }
    ,{.clk=RCC_GPIOH, .txport=GPIOH, .rxport=GPIOH, .txpin=GPIO0, .rxpin=GPIO1 }
    ,{.clk=RCC_GPIOB, .txport=GPIOB, .rxport=GPIOB, .txpin=GPIO0, .rxpin=GPIO1 }
};
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
S_sensor_ultra* INIT_ultra(uint8_t index, double prop)
{
    S_sensor_ultra* dev = &ultras_predef[index];

    dev->proportion = prop;

    // get RCC from port and enable
    //rcc_periph_clock_enable(GET_rcc_from_port(dev->txport))); //????
	rcc_periph_clock_enable(dev->clk);
	gpio_mode_setup(dev->txport, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, dev->txpin);
	gpio_mode_setup(dev->rxport, GPIO_MODE_INPUT, GPIO_PUPD_NONE, dev->rxpin);
	gpio_clear(dev->txport, dev->txpin);

//gpio_enable_interrupts(dev->rxport,dev->rxpin);
	// txport interrupt enable
    //gpio_enable_interrupts(dev->txport,dev->txpin);

// which timer to use?
// ie TIM5
	// setup timer to count with overflow interrupt (for detection)
	// to be able to count from x us to y ms

/*
	rcc_periph_clock_enable(RCC_TIM5);
	timer_reset(TIM5);
    timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // value - http://luckylarry.co.uk/arduino-projects/arduino-sonic-range-finder-with-srf05/
    // 5764us = 1m -->
    // 57.64us = 1cm
    timer_set_prescaler(TIM5, 1);   // 20.5kHz = 49us (=48.7804878049us)
    timer_set_period(TIM5, 0xFFF);
    timer_set_counter(TIM5, 0);
    timer_set_repetition_counter(TIM5, 0);
    timer_continuous_mode(TIM5);

    // start counter
    timer_enable_preload(TIM5);
    timer_enable_break_main_output(TIM5);

    timer_enable_irq(TIM5, TIM_DIER_CC4IE);
    timer_enable_counter(TIM1);
    nvic_enable_irq(NVIC_TIM5_IRQ);
*/
    return dev;
}

double ULTRA_getDist(uint8_t index)
{
    return ultras_predef[index].dist;
}

void ULTRA_signalSend(S_sensor_ultra *dev)
{
    gpio_set(dev->txport, dev->txpin);
    //dev->ticksStart = timer5;
    // wait
    twait(100);
    gpio_clear(dev->txport, dev->txpin);
}

void ULTRA_signalAcquired(S_sensor_ultra *dev)
{
    // proportion
    dev->nTicks = (dev->ticksEnd - dev->ticksStart); //+ (dev->nOverflow * dev->..;
    dev->dist = dev->proportion * ( dev->nTicks / dev->clk );
}

void ULTRA_debug_try(void)
{

    uint8_t i_ultra = 0;
    INIT_ultra(i_ultra ,0);
    uint8_t ilcd = 0;
    S_dev_lcd* lcd_dev = &(lcds[ilcd]);

    S_sensor_ultra* ultra = &(ultras_predef[i_ultra]);
    ULTRA_signalSend(ultra);
    while(1)
    {
        ultra->dist = 0;
        while( !gpio_get(ultra->rxport,ultra->rxpin) )
        {
            __asm__("nop");
        }
        while( gpio_get(ultra->rxport,ultra->rxpin) )
        {
            (ultra->dist)++;
        }

        //u30+4;u10+1
        ultra->dist *= 1.1;//1.111;//1.33333333;//(1 + 4.0/30);

        LCD_clear(lcd_dev);
        fprintf(flcd, "dist[cm]=%.2f", ultra->dist/100);//ULTRA_getDist(i_ultra));
        mswait(500);
    }

}
#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void gpiod_isr(void)
{
    S_sensor_ultra *dev;
    //switch(INTERRUPTED_PIN){
    //case(ultras[0]):
        dev=&ultras_predef[0];
    //}

    //!!!!
    //dev->ticksEnd = timer5a;
    ULTRA_signalAcquired(dev);
}
#endif // __NOT_IMPLEMENTED_YET__BUSY_WAITING

void tim5_isr(void)
{
    if (timer_interrupt_source(TIM5, TIM_SR_CC4IF))
    {
        //if (++counter == 0)
        //{
            gpio_toggle(PLED,LEDBLUE3);
        //}
    }
}



#if __NOT_IMPLEMENTED_YET__INTERRUPT_WAITING_TOBE
void timer3a_isr(){
    // find out which counting is active and increment overflow
    //dev->overflow++
}
#endif

    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
