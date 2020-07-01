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

	UART1->CR1 =  (uint8_t)UART1_WORDLENGTH_9D ;  /* 9 dbit no parity*/
	UART1->CR2 =  (uint8_t)(1U << 6);  	/* Set Transmission complete interrupt enable */
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
	//TIM1_DeInit();

	/////////////////////


	  TIM1->CR1  = TIM1_CR1_RESET_VALUE;	//edge-allign upcounting mode
	  TIM1->CR2  = (uint8_t)TIM1_TRGOSOURCE_UPDATE;
	  TIM1->SMCR = TIM1_SMCR_RESET_VALUE;
	  TIM1->ETR  = TIM1_ETR_RESET_VALUE;
	  TIM1->IER  = (uint8_t)TIM1_IT_UPDATE;
	  TIM1->SR2  = TIM1_SR2_RESET_VALUE;
	  /* Disable channels */
	  TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
	  TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
	  /* Configure channels as inputs: it is necessary if lock level is equal to 2 or 3 */
	  TIM1->CCMR1 = 0x01;
	  TIM1->CCMR2 = 0x01;
//	  TIM1->CCMR3 = 0x01;
//	  TIM1->CCMR4 = 0x01;
	  TIM1->CCMR3 = (uint8_t)TIM1_OCMODE_PWM2 | (uint8_t)TIM1_CCMR_OCxPE;
	  TIM1->CCMR4 = (uint8_t)TIM1_OCMODE_PWM2 | (uint8_t)TIM1_CCMR_OCxPE;
	  /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
	  TIM1->CCER1 = TIM1_CCER1_RESET_VALUE;
//	  TIM1->CCER2 = TIM1_CCER2_RESET_VALUE;
	  TIM1->CCER2 = (uint8_t)(TIM1_OUTPUTSTATE_ENABLE  & TIM1_CCER2_CC3E) | (uint8_t)(TIM1_OUTPUTSTATE_ENABLE & TIM1_CCER2_CC4E) ;
	  TIM1->CCMR1 = TIM1_CCMR1_RESET_VALUE;
	  TIM1->CCMR2 = TIM1_CCMR2_RESET_VALUE;
//	  TIM1->CCMR3 = TIM1_CCMR3_RESET_VALUE;
//	  TIM1->CCMR4 = TIM1_CCMR4_RESET_VALUE;
	  TIM1->CNTRH = TIM1_CNTRH_RESET_VALUE;
	  TIM1->CNTRL = TIM1_CNTRL_RESET_VALUE;
	  TIM1->PSCRH = TIM1_PSCRH_RESET_VALUE;
	  TIM1->PSCRL = TIM1_PSCRL_RESET_VALUE;
	  TIM1->ARRH = (uint8_t)(2500 >> 8);
	  TIM1->ARRL = (uint8_t)(2500);
	  TIM1->CCR1H = TIM1_CCR1H_RESET_VALUE;
	  TIM1->CCR1L = TIM1_CCR1L_RESET_VALUE;
	  TIM1->CCR2H = TIM1_CCR2H_RESET_VALUE;
	  TIM1->CCR2L = TIM1_CCR2L_RESET_VALUE;
	  TIM1->CCR3H = (uint8_t)(3000 >> 8);
	  TIM1->CCR3L = (uint8_t)(3000);
	  TIM1->CCR4H = (uint8_t)(3000 >> 8);
	  TIM1->CCR4L = (uint8_t)(3000);
	  TIM1->OISR  = TIM1_OISR_RESET_VALUE;
	  TIM1->EGR   = 0x01; /* TIM1_EGR_UG */
	  TIM1->DTR   = TIM1_DTR_RESET_VALUE;
//	  TIM1->BKR   = TIM1_BKR_RESET_VALUE;
	  TIM1->BKR	  = TIM1_BKR_MOE;
	  TIM1->RCR   = TIM1_RCR_RESET_VALUE;
	  TIM1->SR1   = TIM1_SR1_RESET_VALUE;

//	/////////////////////
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

