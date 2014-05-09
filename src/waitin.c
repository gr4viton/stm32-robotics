/***********
\project    MRBT - Robotický den 2014
\author 	xdavid10, xslizj00, xdvora0u @ FEEC-VUTBR
\filename	.c
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
 inspiration here:
 https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f4/stm32f4-discovery/tick_blink/tick_blink.c

// systick_config
    // f_CPU = SYSCLK(Hz) = 168MHz
    // HCLK(Hz) = SYSCLK = 168MHz
    // AHB Presc = 1
    // APB1 Presc = 4
    // APB2 Presc = 2
    // 168 000 000 / 4 = 42 000 000 Hz
    // 1 Hz = 42 000 000 asmwaits
    // 1nHz = 42 asmwaits
    // 1nHz = 4200 asmwaits
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "waitin.h"

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
 @brief monotonically increasing
 number of milliseconds/100ns from reset
 overflows every 49 days(for WAITIN_SYSCLK_1MS) if you're wondering
 ****************/
volatile uint32_t system_tick;
volatile uint32_t tic_toc_start;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL VARIABLE DECLARATIONS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DECLARATIONS
static void systick_setup(void);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// STATIC FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INLINE FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// OTHER FUNCTION DEFINITIONS - doxygen description should be in HEADERFILE

void sys_tick_handler(void)
{   /* Called when systick fires */
	system_tick++;
}

uint32_t _tic(void)
{
    tic_toc_start = system_tick;
    return tic_toc_start;
}

uint32_t _toc(void)
{
    return system_tick - tic_toc_start;
}

uint32_t _tocFrom(uint32_t start)
{
    return system_tick - start;
}

#ifdef WAITIN_SYSCLK_1MS
// sleep for delay milliseconds
void mswait(uint32_t delay)
{
	uint32_t wake = system_tick + delay;
	while (wake > system_tick);
}
#endif // WAITIN_SYSCLK_1MS

#ifdef WAITIN_SYSCLK_100NS
// sleep for delay milliseconds
void mswait(uint32_t delay)
	uint32_t wake = system_tick + delay*10;
	while (wake > system_tick);
}
/* sleep for delay 100ns */
void ns100(uint32_t delay)
{
	uint32_t wake = system_Cns + delay;
	while (wake > system_Cns);
}
#endif

void INIT_clk()
{
    // Set STM32 to 168 MHz.
	rcc_clock_setup_hse_3v3(&hse_8mhz_3v3[CLOCK_3V3_168MHZ]);

	systick_setup();

#if __NOT_IMPORTANT
	// how about like this??
	clock_scale_t myclock_config = {
		.hpre = RCC_CFGR_HPRE_SYSCLK_NODIV,
		.ppre1 = RCC_CFGR_PPRE1_HCLK_NODIV,
		.ppre2 = RCC_CFGR_PPRE2_HCLK_NODIV,
		.voltage_scale = RANGE2,
		.flash_config = FLASH_ACR_LATENCY_0WS,
		.apb1_frequency = 4194000,
		.apb2_frequency = 4194000,
		.msi_range = RCC_ICSCR_MSIRANGE_4MHZ,
	};
	rcc_clock_setup_msi(&myclock_config);
#endif // __NOT_IMPORTANTb
}

void systick_setup(void)
{
#ifdef WAITIN_SYSCLK_1MS
	/* clock rate / 1000 to get 1mS interrupt rate */
	systick_set_reload(168000);
#endif
#ifdef WAITIN_SYSCLK_100NS
	/* clock rate / 100 to get 0.1mS=100ns interrupt rate */
	systick_set_reload(16800);
#endif
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	/* this done last */
	systick_interrupt_enable();
}

void timtick_setup(void)
{
    // initialize timer X for 1ns ticking
    uint32_t timer;
	timer   = TIM3;

    //____________________________________________________
    // clock initialization
	rcc_periph_clock_enable(RCC_APB1ENR_TIM3EN);
	rcc_periph_clock_enable(RCC_TIM3);
    timer_reset(timer); /* Time Base configuration */

    // CK_INT = f_periph(TIM3=APB1) = 30[MHz]??          //RM0090.pdf
    //____________________________________________________
    // MODE SETTING
    // - use internal clock as a trigger
    timer_set_mode(timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    // TIM_CR1_CKD_CK_INT = Clock Division Ratio
    //  - set CK_PSC = CK_INT = f_periph = 30[MHz]
    // TIM_CR1_CMS_EDGE = Center-aligned Mode Selection
    //  - edge..??
    // TIM_CR1_DIR_UP = counting direction up

    /*

    */
    // PSC = 0 --> CK_CNT = CK_PSC = 30[MHz]
    timer_set_prescaler(timer, 0x0);

    // Input Filter clock prescaler -
    timer_set_clock_division(timer, 0x0); // 30[MHz] down to 30[Mhz] .= 33.3 [ns]

    // TIMx_ARR - Period in counter clock ticks.
    timer_set_period(timer, 3000-1); // ctj?? 30MHz/3000Hz -1 ??? = theoreticly generates 100[ms] intervals

    /* Generate TRGO on every update. */
    timer_set_master_mode(timer, TIM_CR2_MMS_UPDATE);

    // start
    timer_enable_counter(timer);

    //____________________________________________________
    // NVIC isr setting

	//exti_select_source(exti, port);
	//exti_set_trigger(exti, trig);
	//exti_enable_request(exti);

    // enable interrupt in NestedVectorInterruptCoo?
	nvic_enable_irq(NVIC_TIM3_IRQ);
	//nvic_set_priority(NVIC_TIM3_IRQ,15);
}

#ifdef LEGACY_WAITIN
void wwait(void){ // wait for a while
    twait(100);
}

void twait(uint32_t times){
    for (;times>0; times--)
        for (int i = 0; i < 10000; i++)
            __asm__("nop");
}

void tnop(uint32_t times)
{
    for(;times>0;times--) __asm__("nop");
}
#endif // LEGACY_WAITIN


    //____________________________________________________
    // ..
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES
