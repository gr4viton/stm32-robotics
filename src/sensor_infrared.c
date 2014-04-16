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
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 As in "arm - Refernce manual - DM00031020.pdf" STM32F4 has 12-bit ADC,
 capable of measuring signals from up to:
   - 16 external sources
   - 2 internal sources
   - VBAT channel
 A/D conversion can be performed in:
   - single / continuous / scan / discontinuous modes
 Stored in 16-bit data register.
 The analog watchdog feature allows the application to detect if the input
 voltage goes beyond the user-defined, higher or lower thresholds.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 inspiration:
 * https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f4/stm32f4-discovery/adc-dac-printf/adc-dac-printf.c
 * https://github.com/uamt-brno/inductor-tool/tree/master/fw/f4discovery
 * https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/lisa-m-2/adc_injec_timtrig_irq_4ch/adc_injec_timtrig_irq_4ch.c
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 As in "user manual_hw_UM1472-STM32F4DISCOVERY_DM00039084.pdf"
 Extension connectors:
  PA0 - ADC123_IN0
  PA1 - ADC123_IN1
  PA2 - ADC123_IN2
  PA3 - ADC123_IN3
  PA4 - ADC12_IN4
  PA5 - ADC12_IN5
  PA6 - ADC12_IN6
  PA7 - ADC12_IN7

  PB0 - ADC12_IN8
  PA1 - ADC12_IN9

  PC0 - ADC123_IN10
  PC1 - ADC123_IN11
  PC2 - ADC123_IN12
  PC3 - ADC123_IN13
  PC4 - ADC12_IN14
  PC5 - ADC12_IN15
  temperature_sensor    - IN16
  Vrefint               - IN17
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
correct interrupt continual sequence adc settings:
 RCC_ADC1
 RCC_PORT
 gpio_mode ANALOG
 adc_off
 adc_set_clk_prescale
 adc_enable_scan_mode
 adc_set_single_conversion_mode
 adc_enable_external_trigger_injected
 adc_set_right_aligned
 adc_set_resolution
 adc_set_injected_sequence
 adc_enable_eoc_interrupt_injected
 adc_power_on
 nvic_enable_irq

+ setting of the timer to trigger the adc readings
*/

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// INCLUDES
//_________> project includes
#include "sensor_infrared.h"



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
 \brief Predefined infra sensors ports & clocks [tx=out;rx=in]
 ****************/
S_sensor_infra infras_predef[4] = {
    {.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO0, .channel=10},
    {.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO1, .channel=11},
    {.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO2, .channel=12},
    {.clk=RCC_GPIOC, .port=GPIOC, .pin=GPIO3, .channel=13}
};
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
    // initialization
void INFRA_setupInjectedTIM2(void)
{
	/* Set up the timer TIM2 for injected sampling */
	uint32_t timer;

	timer   = TIM2;
	rcc_periph_clock_enable(RCC_TIM2);

	/* Time Base configuration */
    timer_reset(timer);
    timer_set_mode(timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_period(timer, 0xFF);
    timer_set_prescaler(timer, 0x8);
    timer_set_clock_division(timer, 0x0);
    /* Generate TRGO on every update. */
    timer_set_master_mode(timer, TIM_CR2_MMS_UPDATE);
    timer_enable_counter(timer);
}


S_sensor_infra* INIT_infraPredef(uint8_t index)
{
    S_sensor_infra* inf = &infras_predef[index];

    // empty the measured values
    inf->val = 0;
    for( inf->indADC = 0; inf->indADC < INFRA_SAMPLES; inf->indADC++)
    {
        inf->samplesADC[inf->indADC] = 0;
    }
    inf->indADC = 0;

	// RCC digital interface clock enable
	rcc_periph_clock_enable(inf->clk);
    gpio_mode_setup(inf->port, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, inf->pin);
	return inf;
}

void INFRA_setupInjectedADC1wTIM2(uint8_t* channel_array, uint8_t nChannels)
{
    rcc_periph_clock_enable(RCC_ADC1);              // RCC digital interface clock enable

	adc_off(ADC1);                                  // adc OFF
	/* We configure everything for one single timer triggered injected conversion with interrupt generation. */
	/* While not needed for a single channel, try out scan mode which does all channels in one sweep and
	 * generates the interrupt/EOC/JEOC flags set at the end of all channels, not each one.
	 */
	adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);       // PRESCALER
	adc_enable_scan_mode(ADC1);                     // scan mode
    adc_set_single_conversion_mode(ADC1);           // single
    //adc_set_continuous_conversion_mode(ADC1);

	/* We want to start the injected conversion with the TIM2 TRGO */
	adc_enable_external_trigger_injected(ADC1, ADC_CR2_JEXTSEL_TIM2_TRGO, ADC_CR2_JEXTEN_FALLING_EDGE);  // get interrupts from TIM2

	/* Generate the ADC1_2_IRQ */
	adc_enable_eoc_interrupt_injected(ADC1);        // enable interrupt injected (from TIM2)

    adc_set_right_aligned(ADC1);                    // align
	adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);    // resolution

	/* We want to read the temperature sensor, so we have to enable it. */
	adc_enable_temperature_sensor();                // temperature - (big offset -> measure difference only!)

	adc_set_injected_sequence(ADC1, nChannels, channel_array);

	adc_power_on(ADC1);

	/* Enable the adc1_2_isr() routine */
    nvic_set_priority(NVIC_ADC_IRQ, 0);
    nvic_enable_irq(NVIC_ADC_IRQ);

    // Wait for ADC starting up.
    mswait(1); // From the datasheet for the medium density devices, the stabilisation period will not exceed 1us.

}

void INFRA_setTresholdLastValue(S_sensor_infra* inf, E_infraInterruptBrightness interruptOn)
{
    //____________________________________________________
    // find out if the interrupt is generated on higher voltage from treshold or vice versa
    // this is done, because of the setting of adc watchdog (not sure yet..)
    inf->treshInterruptOn = interruptOn;
    // white generates higher voltages from CNY70, if it is not connected inversed
    switch(inf->brightVolt)
    {
        case(white_highVoltage): // == black_lowVoltage
            if(interruptOn == interruptOn_white) inf->treshVoltage = treshVoltage_high;
            if(interruptOn == interruptOn_black) inf->treshVoltage = treshVoltage_low;
            break;
        case(white_lowVoltage): // == black_highVoltage
            if(interruptOn == interruptOn_white) inf->treshVoltage = treshVoltage_low;
            if(interruptOn == interruptOn_black) inf->treshVoltage = treshVoltage_high;
            break;
        default: return;
    }
    //____________________________________________________
    // set treshold value from the last value
    // future-> maybe to have it as a mean value of samples
    // (instead of the last value which is counted a little differently)

    // add or subtract nStds standard deviations of samples
    // -> to be sure the trigger is different enaugh
    inf->stdev = 0; // standard deviation
    inf->nStds = 5; // number of standard deviations added
    uint8_t a=0;
    for(a=0; a<INFRA_SAMPLES; a++)
    {
        inf->stdev += (inf->valuesADC[a] - inf->val) * (inf->valuesADC[a] - inf->val);
    }
    inf->stdev = sqrt(inf->stdev / (INFRA_SAMPLES-1) );
    if(inf->treshVoltage == treshVoltage_high) inf->triggerVal = inf->val + inf->nStds * inf->stdev;
    if(inf->treshVoltage == treshVoltage_low) inf->triggerVal = inf->val - inf->nStds * inf->stdev;
}

    //____________________________________________________
    // sampling
void INFRA_addSample(S_sensor_infra* inf, uint16_t sample)
{
    inf->samplesADC[inf->indADC++] = sample;
    if(inf->indADC >= INFRA_SAMPLES) inf->indADC = 0;
}

void INFRA_countMean(S_sensor_infra* inf)
{
    // count this step mean
    // values = y
    // samples = u
    /*  From control theory
        We assume the output as discrete stable system:
        y(k) = u(k) + [ y(k-1)+y(k-2)+...+y(k-INFRA_SAMPLES+1) ] / (INFRA_SAMPLES-1)
    */
    uint8_t i = 0;
    double y_k = 0;
    for(i=0; i<INFRA_SAMPLES; i++)
    {
        // y(k) = [ y(k-1)+y(k-2)+...+y(k-INFRA_SAMPLES+1) ]
        if(i!=inf->indADC)
            y_k += inf->valuesADC[i];
    }

    y_k += inf->samplesADC[inf->indADC];
    y_k /= INFRA_SAMPLES;
    /*
    // y(k) = [ y(k-1)+y(k-2)+...+y(k-INFRA_SAMPLES+1) ] / (INFRA_SAMPLES-1)
    y_k /= INFRA_SAMPLES-1;
    // y(k) = u(k) + [ y(k-1)+y(k-2)+...+y(k-INFRA_SAMPLES+1) ] / (INFRA_SAMPLES-1)
    y_k += inf->samples[inf->cnt];
    y_k /= 2;
    */

    inf->val = inf->valuesADC[inf->indADC] = y_k;
    #if __NOT_USED_ANYMORE
    /*  From control theory
        We assume the output as discrete stable system:
        y(k) = [u(k) + u(k-1) + u(k-2) +...+ u(k-INFRA_SAMPLES) ] / (INFRA_SAMPLES)
        the system function is quick but has high frequencies (derivatives)
    */
    for(i=0; i<INFRA_SAMPLES; i++)
    {
        inf->valuesADC[inf->indADC] += inf->samplesADC[i];
    }
    inf->valuesADC[inf->indADC] /= INFRA_SAMPLES;
    #endif // __NOT_USED_ANYMORE
}


void INFRA_addSampleCountMean(S_sensor_infra* inf, uint16_t sample)
{
    INFRA_addSample(inf, sample);
    // count mean value every sample
    INFRA_countMean(inf);
}
    //____________________________________________________
    // interrupts
// future: interrupt handler can be defined in robot_config / main_sumo ..
void adc_isr(void)
{
    uint8_t a=0;
    uint16_t sample = 0;
    if (adc_eoc_injected(ADC1))
    {
        // the order of channels in adc channelArray is the same as in array inf->i[]
        for(a=0; a<R.infs.nInfs; a++)
        {
            //gpio_toggle(PLED,LEDRED2);
            sample = adc_read_injected(ADC1, a+1); // indexed from 1
            INFRA_addSampleCountMean(R.infs.i[a], sample);
        }
        ADC_SR(ADC1) = ~ADC_SR_JEOC; // clean the flag
    }
}

#if __NOT_USED_ANYMORE
void adc_finish(uint16_t values[])
{
    // from main_debug
    DBG_adc_finish();
}
#endif // __NOT_USED_ANYMORE




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// EXTERNAL REFERENCES





#if __NOT_USED_ANYMORE

uint16_t INFRA_readNaiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

void current_init(void)
{


	rcc_periph_clock_enable(RCC_ADC1);
	rcc_periph_clock_enable(RCC_ADC2);
	rcc_periph_clock_enable(RCC_ADC3);

	rcc_periph_clock_enable(RCC_GPIOC);

	gpio_mode_setup(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3);

	adc_off(ADC1);
	adc_off(ADC2);
	adc_off(ADC3);

	adc_set_clk_prescale(ADC_CCR_ADCPRE_BY2);

	adc_enable_scan_mode(ADC1);
    adc_enable_scan_mode(ADC2);
    adc_enable_scan_mode(ADC3);

    adc_set_single_conversion_mode(ADC1);
    adc_set_single_conversion_mode(ADC2);
    adc_set_single_conversion_mode(ADC3);

    adc_enable_external_trigger_injected(ADC1, ADC_CR2_JEXTSEL_TIM5_CC4, ADC_CR2_JEXTEN_FALLING_EDGE); // in center of pwm=low
    adc_enable_external_trigger_injected(ADC2, ADC_CR2_JEXTSEL_TIM5_CC4, ADC_CR2_JEXTEN_FALLING_EDGE); // in center of pwm=low
    adc_enable_external_trigger_injected(ADC3, ADC_CR2_JEXTSEL_TIM5_CC4, ADC_CR2_JEXTEN_FALLING_EDGE); // in center of pwm=low

    adc_set_right_aligned(ADC1);
    adc_set_right_aligned(ADC2);
    adc_set_right_aligned(ADC3);

	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_3CYC);
	adc_set_sample_time_on_all_channels(ADC2, ADC_SMPR_SMP_3CYC);
	adc_set_sample_time_on_all_channels(ADC3, ADC_SMPR_SMP_15CYC);

	adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
    adc_set_resolution(ADC2, ADC_CR1_RES_12BIT);
    adc_set_resolution(ADC3, ADC_CR1_RES_12BIT);

    uint8_t adc1_channel = ADC_CHANNEL10;
    uint8_t adc2_channel = ADC_CHANNEL11;
    uint8_t adc3_channel = ADC_CHANNEL12;

	adc_set_injected_sequence(ADC1, 1, &adc1_channel);
	adc_set_injected_sequence(ADC2, 1, &adc2_channel);
	adc_set_injected_sequence(ADC3, 1, &adc3_channel);

    adc_enable_eoc_interrupt_injected(ADC1);
	adc_enable_eoc_interrupt_injected(ADC2);
	adc_enable_eoc_interrupt_injected(ADC3);

	adc_power_on(ADC1);
	adc_power_on(ADC2);
	adc_power_on(ADC3);

	nvic_enable_irq(NVIC_ADC_IRQ);

	/* Wait for ADC starting up. */
    for (int i = 0; i < 800000; i++)    /* Wait a bit. */
        __asm__("nop");

}

#endif // __NOT_USED_ANYMORE
