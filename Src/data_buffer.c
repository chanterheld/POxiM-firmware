/*
 * data_buffer.c
 *
 *  Created on: Jun 18, 2020
 *      Author: daandegroot
 */

#include "stm8s.h"
#include "stm8s_it.h"

volatile bool databuffer_overflow = 0;
volatile int32_t data_buffer[32];
const char databuffer_size = sizeof(data_buffer)/sizeof(data_buffer[0]);
volatile char databuffer_write_index = 0;
volatile char databuffer_read_index = 0;

int32_t databuffer_get_new_data(void){
	if(databuffer_overflow){
		disableInterrupts();
		while(1){
			//overflow of data buffer, endless loop;
		}
	}

	while(databuffer_read_index == databuffer_write_index){
	}
	int32_t retval =  data_buffer[databuffer_read_index];

	if((databuffer_read_index + 1) == databuffer_size){
		databuffer_read_index = 0;
	}else{
		databuffer_read_index++;
	}

	return retval;
}

void databuffer_write_new_data(int32_t input){
	//check for overflow condition
	if(databuffer_read_index == (databuffer_write_index + 1)){
		databuffer_overflow = 1;
		return;
	}
	data_buffer[databuffer_write_index] = input;

	if((databuffer_write_index + 1) == databuffer_size){
		databuffer_write_index = 0;
	}else{
		databuffer_write_index++;
	}

	return;
}



