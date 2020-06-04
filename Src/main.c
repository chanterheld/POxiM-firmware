#include "stm8s.h"
#include "stm8s_it.h"

/*
 * This value needs to be between 2500 - (nr of adc clock cycles) and (255 * max adc multiplication factor for feedback)
 * this is thus 2388 and 2040 (for 112 and 8 respectively)
 * Choosing this value higher makes more time for timer 2 interrupt to stop the timer after a cycle but less time for the ADC EOC interrupt to start timer 2
 *
 */
#define TIM2_RELOAD_VALUE		(2232)

void Timer1_configuration(void);
void Timer2_configuration(void);
void ADC1_configuration(void);

void main(void)
{

	/*dunno how overdone this is but should make sure it runs at 16Mhz*/
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_CPUDIV1);


	/**
	 * set both commons to highZ to disable leds:
	 */
	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_OD_HIZ_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_OD_HIZ_FAST);


	/* not sure if needed but set adc input pin to floating input*/
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);


	UART1_DeInit();
	UART1_Init((uint32_t)256000, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
			UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TX_ENABLE);


	ADC1_configuration();

	Timer1_configuration();

	Timer2_configuration();

	/* enable interrupts */
	enableInterrupts();

	/* Enable timer 1*/
	TIM1_Cmd(ENABLE);

	while (1)
	{

	}

}





/**
 * Initialize timer 1:
 * Timer 1 is the main system timer triggers the ADC and creates the PWM signals for led modulation
 */
void Timer1_configuration(void){
	/**
	 * set to default state
	 */
	TIM1_DeInit();

	/**
	 * timer1:
	 * prescaler 1;
	 * upcounting;
	 * period 2500 (6.4Khz@16Mhz clock);
	 * repetion counter 0)
	 */
	TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP,  2500,  0);


	/**
	 * timer 1 channel 3 settings:
	 * 			channel 4 has no inverse output
	 *
	 * pwm2: cnt < ccr => inactive
	 * enable normal output
	 * disable inverted output
	 * set ccr register 0
	 * normal output => active high
	 * inverted output => active high
	 * idle state low (only relevant for dead time insertion)
	 * idle state low (only relevant for dead time insertion)
	 */

	/**
	 * timer 1 channel 3 settings: led IR
	 */
	TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 3000, \
			TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);

	/**
	 * timer 1 channel 4 settings: led R
	 */
	TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, 3000, \
			TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET);


	/**
	 * enable preload voor ccr 3,4
	 * needed since this registers will be written to while the timer is running
	 */
	TIM1_OC3PreloadConfig(ENABLE);
	TIM1_OC4PreloadConfig(ENABLE);


	/*
	 * enable interrupt to update led modulation ccr registers
	 */
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);

	/* Enable pwm outputs */
	TIM1_CtrlPWMOutputs(ENABLE);

	/*
	 * Create output trigger signal on timer update (overflow) for adc
	 */
	TIM1_SelectOutputTrigger(TIM1_TRGOSOURCE_UPDATE);
}


void Timer2_configuration(void){
	/**
	 * set to default state
	 */
	TIM2_DeInit();

	/**
	 * timer1: prescaler 1;
	 * period as explained earlier
	 */
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM2_RELOAD_VALUE);

	/**
	 * timer 1 channel 2 settings: adc feedback pin
	 *
	 * pwm1: cnt < ccr => inactive
	 * enable normal output
	 * set ccr register 0
	 * normal output => active high
	 */
	TIM2_OC1Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);

	//set counter to 0 to force pwm out low
	TIM2_SetCounter(0);

	//disable pre-load of ccr register since we manually disable and enable the timer around ccr writes
	TIM2_OC1PreloadConfig(DISABLE);

	//enable update event interrupts
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
}


void ADC1_configuration(void){
	ADC1_DeInit();

	/**
	 * single conversion
	 * channel 6
	 * prescaler of 8 => 2Mhz adc clock: total converstion time is 14 adc clock ticks so 7uS
	 * disable schmitttrigger on channel 6 pin gpio
	 *
	 */
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D8, ADC1_EXTTRIG_TIM, \
			ENABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL6, DISABLE);

	//enable end of conversion interrupt
	ADC1_ITConfig(ADC1_IT_EOCIE, ENABLE);

	//enable ADC
	ADC1_Cmd(ENABLE);

	//ADC1->DRH holds MSB data in left align mode
}

#define LED_R_VALUE_FROM_MOD_TABLE(table_entry)			((table_entry >> 4) & 0xf)
#define LED_IR_VALUE_FROM_MOD_TABLE(table_entry)		(table_entry & 0xf)

volatile unsigned char led_r_brightness = 160;
volatile unsigned char led_ir_brightness = 160;

static const unsigned char led_mod_table[] = {136,135,152,152,169,168,185,185,202,201,218,218,234,235,234,235,251,251,251,251,252,252,252,252,253,236,237,237,237,222,221,206,206,189,190,174,174,158,158,142,142,126,110,111,94,95,79,79,63,63,47,46,31,31,30,31,14,15,14,15,14,14,14,15,14,30,29,30,29,45,46,61,61,77,77,93,92,108,109,124,124,139,155,155,171,171,187,186,202,202,218,218,233,233,232,233,248,248,248,248,248,247,247,247,246,231,230,230,230,213,214,197,197,180,181,164,164,147,148,131,132,115,99,99,82,83,66,66,50,50,33,34,17,17,18,17,1,1,1,1,0,1,0,1,0,16,17,16,16,33,32,48,49,64,64,81,80,97,96,113,113,128,145,145,161,161,177,177,194,193,210,210,226,226,226,226,243,243,243,243,243,243,244,244,244,228,229,229,229,213,213,197,198,182,182,166,167,151,151,135,135,120,104,104,88,89,73,73,57,57,42,43,26,27,27,26,11,11,12,11,12,12,12,12,12,28,29,29,29,45,45,62,62,77,78,94,94,110,111,126,127,142,159,158,175,174,191,190,207,206,223,223,238,239,239,239,254,255,254,254,255,254,254,254,255,238,238,237,238,221,221,205,205,189,189,173,172,157,156,140,139,124,107,107,91,91,74,74,59,58,42,41,25,25,25,25,9,9,8,7,7,7,7,7,7,22,22,22,22,37,37,53,53,69,68,84,84,100,100,116,115,131,147,147,163,162,178,178,194,194,210,210,225,225,225,225,241,241,241,240,241,240,240,241,241,224,224,225,224,208,209,193,192,176,177,160,161,144,144,129,129,113,96,97,81,81,65,65,49,50,34,34,18,18,18,18,3,3,3,3,3,3,4,4,4,20,21,21,20,37,38,53,54,70,70,87,86,103,103,119,120,135,152,152,169,168,185,185,201,202,218,218,234,234,234,235,251,252,251,252,252,252,252,252,252,237,237,237,236,221,222,205,206,189,190,174,174,158,159,142,142,127,110,111,94,95,79,78,63,63,46,47,31,31,30,30,15,15,14,14,15,14,14,14,14,30,30,30,30,46,45,61,61,77,76,93,92,108,108,124,124,139,156,155,171,171,186,187,202,202,218,217,234,233,233,232,249,248,248,247,248,247,247,247,247,230,230,230,229,214,213,197,197,181,181,164,164,148,147,131,116,115,99,99,82,83,66,66,50,50,34,33,17,18,17,17,1,1,1,0,1,0,1,1,0,16,17,16,16,33,32,48,49,64,65,80,80,97,96,113,113,128,145,145,161,161,177,177,193,194,210,210,226,226,226,226,243,243,243,243,243,244,244,243,244,228,229,229,229,213,213,197,197,182,182,167,167,151,151,135,135,120,104,104,89,88,73,73,58,58,41,42,26,26,27,27,11,11,11,12,12,11,12,12,13,28,29,29,29,46,45,62,61,78,78,93,94,110,110,127,126,143,158,159,175,174,191,190,207,206,223,223,239,238,239,239,254,255,254,254,255,254,254,254,254,239,238,237,238,221,221,205,206,189,188,173,172,156,156,140,124,124,107,107,91,91,75,74,58,58,42,42,25,25,25,24,8,8,8,8,8,7,7,7,6,23,22,22,22,37,37,53,53,69,69,85,84,99,100,115,115,131,147,146,163,162,178,178,195,194,210,209,225,225,226,225,241,241,241,241,240,241,240,241,240,224,225,224,225,208,208,192,193,176,176,161,160,145,144,129,129,112,97,97,81,80,65,66,49,50,34,34,18,18,19,18,3,2,3,3,3,4,3,4,4,20,21,21,21,36,37,54,54,70,70,86,86,103,103,119,120,136,152,153,168,169,185,185,201,202,217,218,234,234,235,235,251,251,251,251,252,252,252,252,252,237,237,237,237,221,222,206,205,190,190,174,174,158,158,142,127,126,111,110,94,95,78,79,62,63,47,46,31,31,31,30,15,15,14,15,14,14,14,15,14,30,30,29,30,45,45,61,61,77,77,93,93,108,108,124,123,140,155,155,171,170,187,186,203,202,218,217,233,233,233,233,249,248,248,248,247,247,247,247,247,231,230,230,229,213,213,197,197,181,180,164,164,148,148,132,131,115,99,99,82,83,66,66,50,50,34,33,17,18,17,17,1,1,1,0,1,1,0,0,1,16,17,16,17,32,32,48,49,64,65,80,80,97,96,113,112,129,145,144,161,161,178,178,193,194,209,210,226,226,226,227,243,242,243,243,243,244,243,244,244,228,228,229,229,213,214,197,198,182,182,166,167,151,151,135,119,120,104,104,89,89,73,73,57,57,41,42,26,27,27,27,11,11,11,12,11,12,12,13,12,29,29,29,28,45,46,61,62,78,77,94,94,110,111,126,142,143,159,158,174,175,191,190,207,206,223,223,239,238,239,238,255,254,255,255,254,254,254,255,254,238,238,237,238,221,221,205,205,189,189,173,172,156,156,140,140,124,108,107,91,90,75,74,58,58,41,41,26,25,25,25,9,8,8,8,7,7,7,7,7,22,22,22,22,38,37,53,52,69,68,84,84,100,100,116,115,131,147,147,162,162,179,178,194,194,210,209,226,225,225,225,241,241,241,240,241,241,240,240,241,224,225,224,224,209,208,193,192,176,176,161,160,145,144,129,128,113,97,97,81,81,65,65,50,49,34,34,18,18,18,18,3,3,3,3,4,3,4,3,4,20,21,20,21,37,37,54,54,70,70,86,87,103,103,119,119,136,152,152,169,168,185,185,202,202,217,218,234,234,235,235,251,251,251,251,252,252,252,253,252,237,237,237,236,221,221,206,206,190,190,173,174,158,158,143,143,126,110,111,95,94,79,78,63,62,47,47,31,30,31,30,15,15,14,14,15,14,14,15,14,30,29,30,30,45,45,61,61,77,77,93,93,108,108,123,124,140,155,155,171,171,186,186,203,202,218,217,234,233,233,232,249,248,248,247,247,248,247,247,247,230,230,230,230,213,213,197,197,181,181,164,164,148,147,132,131,115,99,99,82,82,66,67,50,50,34,33,18,17,17,17,1,1,1,0,1,0,1,0,1,16,17,16,17,32,32,49,48,64,65,80,80,97,96,112,129,129,145,145,161,161,177,177,193,194,210,210,226,226,226,227,243,243,242,243,243,243,244,244,244,228,229,229,228,213,213,198,198,182,182,166,167,151,151,135,135,120,104,104,89,88,73,73,58,58,41,42,26,26,27,27,11,11,11,11,12,12,12,13,12,29,29,28,29,45,46,61,62,78,78,93,94,110,111,126,126,143,158,159,175,174,190,191,207,207,222,223,239,238,239,238,255,255,254,254,255,254,254,255,254,238,237,238,237,222,221,205,206,189,188,173,172,156,156,140,140,124,108,107,91,90,75,74,58,58,41,41,26,25,25,25,8,9,8,8,7,7,7,7,7,22,22,22,22,38,37,53,53,68,68,85,84,99,100,115,115,131,147,147,163,163,179,178,194,193,209,210,225,226,225,225,241,241,241,241,240,240,241,240,241,224,225,224,225,208,208,193,192,176,176,161,160,145,144,129,128,113,97,96,81,81,66,66,49,50,33,34,18,18,19,18,2,3,3,3,3,4,3,4,4,20,21,20,21,37,37,54,54,70,70,86,87,103,103,119};
static const short led_mod_table_size = sizeof(led_mod_table)/sizeof(led_mod_table[0]);
volatile unsigned short led_mod_table_idx = 0;


/**
 *  Timer 1 update event interrupt
 *
 *	Write new values timer 1 CCR 3,4 to modulate leds
 */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, ITC_IRQ_TIM1_OVF)
{
	//clear interrupt pending flag
	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);

	// write new values from lookup table to compare registers to modulate leds
	TIM1_SetCompare4((unsigned short)(LED_R_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_r_brightness));
	TIM1_SetCompare3((unsigned short)(LED_IR_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_ir_brightness));

	//increment index
	if(++led_mod_table_idx == led_mod_table_size){
		led_mod_table_idx = 0;
	}
}



/**
 *  ADC end of conversion interrupt
 *
 *	Calculate and write values to timer 2 CCR 1 for ADC feedback generation
 *	Start timer 2
 *	Write sampled value to UART
 *	Further High speed calculations would be implemented here
 */
INTERRUPT_HANDLER(ADC1_IRQHandler, ITC_IRQ_ADC1)
{
	//clear interrupt pending flag
	ADC1_ClearITPendingBit(ADC1_IT_EOC);

	//extract adc value
	unsigned char adc_val = ADC1->DRH;

	//set time 2 to create pulse of size (adc_val * 8)
	TIM2_SetCompare1(TIM2_RELOAD_VALUE - (adc_val * 8) + 1);

	//enable timer 2
	TIM2_Cmd(ENABLE);

	//send ADC value to UART
	UART1->DR = adc_val;

	/**
	 * at this point further calculations will take place
	 */
}

/**
 * Timer 2 update event interrupt
 *
 * Stop timer and force output low
 */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, ITC_IRQ_TIM2_OVF)
{
	//disable timer 2
	TIM2_Cmd(DISABLE);

	//set counter value to 0 to ensure the pwm output is low
	TIM2_SetCounter(0);

	//clear interrupt pending flag
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);

}





