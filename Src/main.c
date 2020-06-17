#include "stm8s.h"
#include "stm8s_it.h"



/*
 * filter headers:
 */

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

#define databuffer_type long

void main(void)
{

///*
// * HSI PRESCALER
// */
//	/*dunno how overdone this is but should make sure it runs at 16Mhz*/
//	//CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
//
//	  /* Clear High speed internal clock prescaler */
//	  CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
//
//	  /* Set High speed internal clock prescaler */
//	  CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;




	  /*
	   * GPIO SET MODE
	   */
//	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_OD_HIZ_FAST);
//	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_OD_HIZ_FAST);
//
//	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_FAST);
//	GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_OD_HIZ_FAST);
//	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_OUT_OD_HIZ_FAST);
//
//	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST);
//	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
//
//	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);
//	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_OD_HIZ_FAST);

//
//	/* not sure if needed but set adc input pin to floating input*/
//	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
//


	/* UART1 SETUP: */
			  /* Clear the Idle Line Detected bit in the status register by a read
			  to the UART1_SR register followed by a Read to the UART1_DR register */
		//	  (void)UART1->SR;
		//	  (void)UART1->DR;
		//
		//	  UART1->BRR2 = (0x0E);
		//	  UART1->BRR1 = (0x03);
		//
		//	  UART1->CR1 = UART1_CR1_RESET_VALUE;  /* Set UART1_CR1 to reset value 0x00: 8 bit no parity*/
		//	  UART1->CR2 = UART1_CR2_RESET_VALUE;  /* Set UART1_CR2 to reset value 0x00 */
		//	  UART1->CR3 = UART1_CR3_RESET_VALUE;  /* Set UART1_CR3 to reset value 0x00 */
		//	  UART1->CR4 = UART1_CR4_RESET_VALUE;  /* Set UART1_CR4 to reset value 0x00 */
		//	  UART1->CR5 = UART1_CR5_RESET_VALUE;  /* Set UART1_CR5 to reset value 0x00 */
		//
		//	  UART1->GTR = UART1_GTR_RESET_VALUE;
		//	  UART1->PSCR = UART1_PSCR_RESET_VALUE;
		//
		//	  UART1->CR2 |= (uint8_t)UART1_CR2_TEN;


//	ADC1_configuration();
//
//	Timer1_configuration();

//	Timer2_configuration();
//
//	/* enable interrupts */
//	enableInterrupts();
//
//	/* Enable timer 1*/
//	TIM1_Cmd(ENABLE);

	while (1)
	{

	}

}


///**
// * Initialize timer 1:
// * Timer 1 is the main system timer triggers the ADC and creates the PWM signals for led modulation
// */
//void Timer1_configuration(void){
//	/**
//	 * set to default state
//	 */
//	TIM1_DeInit();
//
//	/**
//	 * timer1:
//	 * prescaler 1;
//	 * upcounting;
//	 * period 2500 (6.4Khz@16Mhz clock);
//	 * repetion counter 0)
//	 */
//	TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP,  2500,  0);
//
//
//	/**
//	 * timer 1 channel 3 settings:
//	 * 			channel 4 has no inverse output
//	 *
//	 * pwm2: cnt < ccr => inactive
//	 * enable normal output
//	 * disable inverted output
//	 * set ccr register 0
//	 * normal output => active high
//	 * inverted output => active high
//	 * idle state low (only relevant for dead time insertion)
//	 * idle state low (only relevant for dead time insertion)
//	 */
//
//	/**
//	 * timer 1 channel 3 settings: led IR
//	 */
//	TIM1_OC3Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 3000, \
//			TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);
//
//	/**
//	 * timer 1 channel 4 settings: led R
//	 */
//	TIM1_OC4Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, 3000, \
//			TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET);
//
//
//	/**
//	 * enable preload voor ccr 3,4
//	 * needed since this registers will be written to while the timer is running
//	 */
//	TIM1_OC3PreloadConfig(ENABLE);
//	TIM1_OC4PreloadConfig(ENABLE);
//
//
//	/*
//	 * enable interrupt to update led modulation ccr registers
//	 */
//	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
//
//	/* Enable pwm outputs */
//	TIM1_CtrlPWMOutputs(ENABLE);
//
//	/*
//	 * Create output trigger signal on timer update (overflow) for adc
//	 */
//	TIM1_SelectOutputTrigger(TIM1_TRGOSOURCE_UPDATE);
//}


//void Timer2_configuration(void){
//	/**
//	 * set to default state
//	 */
//	TIM2_DeInit();
//
//	/**
//	 * timer1: prescaler 1;
//	 * period as explained earlier
//	 */
//	TIM2_TimeBaseInit(TIM2_PRESCALER_1, TIM2_RELOAD_VALUE);
//
//	/**
//	 * timer 1 channel 2 settings: adc feedback pin
//	 *
//	 * pwm1: cnt < ccr => inactive
//	 * enable normal output
//	 * set ccr register 0
//	 * normal output => active high
//	 */
//	TIM2_OC1Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE, 0, TIM2_OCPOLARITY_HIGH);
//
//	//set counter to 0 to force pwm out low
//	TIM2_SetCounter(0);
//
//	//disable pre-load of ccr register since we manually disable and enable the timer around ccr writes
//	TIM2_OC1PreloadConfig(DISABLE);
//
//	//enable update event interrupts
//	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
//}



//void ADC1_configuration(void){
//
//	/**
//	 * single conversion
//	 * channel 6
//	 * prescaler of 8 => 2Mhz adc clock: total converstion time is 14 adc clock ticks so 7uS
//	 * disable schmitttrigger on channel 6 pin gpio
//	 *
//	 */
//
//	ADC1->CSR  = (uint8_t)(ADC1_CHANNEL_6) | (uint8_t)ADC1_IT_EOCIE; // channel 6 + end of conversion interrupt
//	ADC1->CR1  = (uint8_t)(ADC1_PRESSEL_FCPU_D8); // Continuous conversion + adc_clk = cpu_clk/8
//	ADC1->CR2  = (uint8_t)(ADC1_CR2_EXTTRIG) | (uint8_t)(ADC1_EXTTRIG_TIM); //start conversion on timer trigger + left allign
//	ADC1->CR3  = ADC1_CR3_RESET_VALUE;
//	ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
//	ADC1->TDRL = (uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SCHMITTTRIG_CHANNEL6); // disable schmitt trigger on input
//	ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
//	ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
//	ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
//	ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
//	ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
//	ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;
//
//	ADC1->CR1 |= ADC1_CR1_ADON; /*enable adc*/
//
//	//ADC1->DRH holds MSB data in left align mode
//}


//
//#define LED_R_VALUE_FROM_MOD_TABLE(table_entry)			((uint8_t)((table_entry >> 4) & 0xf))
//#define LED_IR_VALUE_FROM_MOD_TABLE(table_entry)		((uint8_t)(table_entry & 0xf))
//
//volatile uint8_t led_r_brightness = 160;
//volatile uint8_t led_ir_brightness = 160;
//
//static const uint8_t led_mod_table[] = {136,136,153,169,186,202,219,219,236,236,253,253,254,254,254,254,239,239,223,223,207,191,175,159,143,142,126,110,94,77,61,44,44,27,27,10,10,9,9,8,8,23,22,38,37,53,68,84,99,115,130,130,145,161,177,193,208,208,224,224,240,240,240,240,240,241,225,225,209,210,194,179,163,148,132,133,117,102,86,71,56,40,41,25,26,10,11,11,12,12,13,29,30,46,46,62,79,95,111,127,143,143,159,175,191,206,222,222,238,237,253,252,252,251,251,250,234,233,217,216,200,183,166,150,133,133,116,100,83,67,50,34,33,17,17,1,0,0,0,0,0,16,16,32,32,49,65,81,97,114,130,131,147,164,180,197,213,214,230,231,248,248,249,249,250,250,235,235,220,220,205,189,174,158,142,142,127,111,95,79,63,47,47,31,31,14,14,14,14,13,13,28,28,43,43,58,74,89,105,120,136,135,150,166,181,197,212,212,227,227,242,242,241,241,241,241,224,224,208,208,192,176,160,144,128,129,113,97,81,66,50,35,35,20,20,5,5,6,6,7,8,24,25,41,42,58,75,91,108,124,141,141,158,174,190,206,223,223,239,239,255,255,255,255,255,254,238,238,222,221,205,188,172,155,139,138,122,105,89,72,56,39,38,22,21,5,4,4,3,3,2,18,17,33,33,49,64,80,96,112,128,128,144,160,176,193,209,209,225,226,242,243,243,244,244,245,229,230,214,215,200,184,169,153,138,138,123,107,92,76,61,45,46,30,30,14,15,15,15,15,15,31,31,47,47,62,78,94,110,125,141,140,156,171,187,202,218,217,233,232,248,247,246,246,245,245,228,228,211,211,194,178,161,145,129,129,112,96,80,64,48,32,32,16,16,1,1,1,1,2,2,19,19,36,36,53,69,86,102,119};
//static const uint16_t led_mod_table_size = sizeof(led_mod_table)/sizeof(led_mod_table[0]);
//volatile uint16_t led_mod_table_idx = 0;
//
//
///**
// *  Timer 1 update event interrupt
// *
// *	Write new values timer 1 CCR 3,4 to modulate leds
// */
//INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, ITC_IRQ_TIM1_OVF)
//{
//	//clear interrupt pending flag
//	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
//
//	// write new values from lookup table to compare registers to modulate leds
//	TIM1_SetCompare4((LED_R_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_r_brightness));
//	TIM1_SetCompare3((LED_IR_VALUE_FROM_MOD_TABLE(led_mod_table[led_mod_table_idx]) * led_ir_brightness));
//
//	//increment index
//	++led_mod_table_idx;
//	if(led_mod_table_idx == led_mod_table_size){
//		led_mod_table_idx = 0;
//	}
//}
//
//
//
//
///**
// *  ADC end of conversion interrupt
// *
// *	Calculate and write values to timer 2 CCR 1 for ADC feedback generation
// *	Start timer 2
// *	Write sampled value to UART
// *	Further High speed calculations would be implemented here
// */
//INTERRUPT_HANDLER(ADC1_IRQHandler, ITC_IRQ_ADC1)
//{
//	//clear interrupt pending flag
//	ADC1_ClearITPendingBit(ADC1_IT_EOC);
//
//	//extract adc value
//	unsigned char adc_val = ADC1->DRH;
//
//	//set time 2 to create pulse of size (adc_val * 8)
//	TIM2_SetCompare1(TIM2_RELOAD_VALUE - (adc_val * 8) + 1);
//
//	//enable timer 2
//	TIM2_Cmd(ENABLE);
//
//	//send ADC value to UART
//	UART1->DR = adc_val;
//
//	/**
//	 * at this point further calculations will take place
//	 */
//}
//
///**
// * Timer 2 update event interrupt
// *
// * Stop timer and force output low
// */
//INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, ITC_IRQ_TIM2_OVF)
//{
//	//disable timer 2
//	TIM2_Cmd(DISABLE);
//
//	//set counter value to 0 to ensure the pwm output is low
//	TIM2_SetCounter(0);
//
//	//clear interrupt pending flag
//	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
//
//}

