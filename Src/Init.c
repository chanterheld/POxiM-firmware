/*
 * Init.c
 *
 *  Created on: Jul 1, 2020
 *      Author: daandegroot
 */

#include "stm8s.h"
#include "stm8s_it.h"
#include "defines.h"


void Initialize(void){

	/* UART1 SETUP: */
	/* Clear the Idle Line Detected bit in the status register by a read
			  to the UART1_SR register followed by a Read to the UART1_DR register */

	(void)UART1->SR;
	(void)UART1->DR;

	UART1->BRR2 = (0x0E);
	UART1->BRR1 = (0x03);

	UART1->CR1 = UART1_CR1_RESET_VALUE;  /* Set UART1_CR1 to reset value 0x00: 8 bit no parity*/
	UART1->CR2 = UART1_CR2_RESET_VALUE;  /* Set UART1_CR2 to reset value 0x00 */
	UART1->CR3 = UART1_CR3_RESET_VALUE;  /* Set UART1_CR3 to reset value 0x00 */
	UART1->CR4 = UART1_CR4_RESET_VALUE;  /* Set UART1_CR4 to reset value 0x00 */
	UART1->CR5 = UART1_CR5_RESET_VALUE;  /* Set UART1_CR5 to reset value 0x00 */

	UART1->GTR = UART1_GTR_RESET_VALUE;
	UART1->PSCR = UART1_PSCR_RESET_VALUE;

	UART1->CR2 |= (uint8_t)UART1_CR2_TEN;



	/* ADC SETUP: */
	/* single conversion
	 * channel 6
	 * prescaler of 8 => 2Mhz adc clock: total converstion time is 14 adc clock ticks so 7uS
	 * disable schmitttrigger on channel 6 pin gpio
	 *
	 */

	ADC1->CSR  = (uint8_t)(ADC1_CHANNEL_6) | (uint8_t)ADC1_IT_EOCIE; // channel 6 + end of conversion interrupt
	ADC1->CR1  = (uint8_t)(ADC1_PRESSEL_FCPU_D8); // Continuous conversion + adc_clk = cpu_clk/8
	ADC1->CR2  = (uint8_t)(ADC1_CR2_EXTTRIG) | (uint8_t)(ADC1_EXTTRIG_TIM); //start conversion on timer trigger + left allign
	ADC1->CR3  = ADC1_CR3_RESET_VALUE;
	ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
	ADC1->TDRL = (uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SCHMITTTRIG_CHANNEL6); // disable schmitt trigger on input
	ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
	ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
	ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
	ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
	ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
	ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;

	ADC1->CR1 |= ADC1_CR1_ADON; /*enable adc*/

	//ADC1->DRH holds MSB data in left align mode




	/*Timer1 Init */

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





	/*Timer2 Init */

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

