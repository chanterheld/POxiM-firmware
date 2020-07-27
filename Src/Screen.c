/*
 * Screen.c
 *
 *  Created on: Jul 27, 2020
 *      Author: daandegroot
 */

#include "stm8s.h"
#include "stm8s_it.h"
#include "defines.h"


void SetNr(uint8_t idx, uint8_t nr);


#define SEGMENTS_NR0		(0b1110111)
#define SEGMENTS_NR1		(0b0100100)
#define SEGMENTS_NR2		(0b1011101)
#define SEGMENTS_NR3		(0b1101101)
#define SEGMENTS_NR4		(0b0101110)
#define SEGMENTS_NR5		(0b1101011)
#define SEGMENTS_NR6		(0b1111011)
#define SEGMENTS_NR7		(0b0100101)
#define SEGMENTS_NR8		(0b1111111)
#define SEGMENTS_NR9		(0b1101111)



uint8_t screen_leds[4] = {0};

GPIO_TypeDef* segment_ports[7] = {SEGMENT0_PORT, SEGMENT1_PORT, SEGMENT2_PORT, SEGMENT3_PORT, SEGMENT4_PORT,SEGMENT5_PORT, SEGMENT6_PORT};
GPIO_Pin_TypeDef segments_pins[7] = {SEGMENT0_PIN, SEGMENT1_PIN, SEGMENT2_PIN, SEGMENT3_PIN, SEGMENT4_PIN, SEGMENT5_PIN, SEGMENT6_PIN};

GPIO_TypeDef* common_ports[3] = {COMMON0_PORT, COMMON1_PORT, COMMON2_PORT};
GPIO_Pin_TypeDef common_pins[3] = {COMMON0_PIN, COMMON1_PIN, COMMON2_PIN};


void SetScreen(uint8_t value, uint8_t bitfield){

	uint8_t dvsr = 100;
	uint8_t idx = 0;

	// display number
    while(dvsr){
        SetNr(idx++, value / dvsr);
        value %= dvsr;
        dvsr /= 10;
    }

    //screen_leds[3] = bitfield;
    (void)bitfield;

}


uint8_t screen_disp_index = 0;

void ScreenDisplayNext(void){

	for(uint8_t i = 0; i < 3; i++){
		GPIO_WriteLow(common_ports[i], common_pins[i]);
	}

	if(screen_disp_index != 3){
		GPIO_WriteHigh(common_ports[screen_disp_index], common_pins[screen_disp_index]);
	}

	uint8_t bitfield = screen_leds[screen_disp_index];

	for(uint8_t j = 0; j < 7; j++){
		if(bitfield & 0b1){
			GPIO_WriteHigh(segment_ports[j], segments_pins[j]);
		}else{
			GPIO_WriteLow(segment_ports[j], segments_pins[j]);
		}
		bitfield >>= 1;
	}

	if(++screen_disp_index == 4){
		screen_disp_index = 0;
	}
}

void ScreenInit(void){

	for(uint8_t i = 0; i < 7; i++){
		GPIO_Init(segment_ports[i], segments_pins[i], GPIO_MODE_OUT_OD_HIZ_FAST);
	}

	for(uint8_t i = 0; i < 3; i++){
		GPIO_Init(common_ports[i], common_pins[i], GPIO_MODE_OUT_PP_LOW_FAST);
	}

}


void SetNr(uint8_t idx, uint8_t nr){
	uint8_t bitfield;
	switch(nr){
	case (0):
		bitfield = SEGMENTS_NR0;
		break;
	case (1):
		bitfield = SEGMENTS_NR1;
		break;
	case (2):
		bitfield = SEGMENTS_NR2;
		break;
	case (3):
		bitfield = SEGMENTS_NR3;
		break;
	case (4):
		bitfield = SEGMENTS_NR4;
		break;
	case (5):
		bitfield = SEGMENTS_NR5;
		break;
	case (6):
		bitfield = SEGMENTS_NR6;
		break;
	case (7):
		bitfield = SEGMENTS_NR7;
		break;
	case (8):
		bitfield = SEGMENTS_NR8;
		break;
	case (9):
		bitfield = SEGMENTS_NR9;
		break;
	default:
		return;
		break;
	}

	screen_leds[idx] = bitfield;

	return;
}

