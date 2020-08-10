/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */


#include "filters.h"

#define FILTER_NAME				generic_fir_rom_opt
#define filter_input_type		int32_t
#define filter_output_type		int32_t

filter_output_type generic_fir_rom_opt_advance(filter_input_type input, char output_cycle, int32_t *filter_memory, int32_t *filter_coeff, uint8_t filter_order){

	if(output_cycle == 1){

		int64_t retval	= input * filter_coeff[0] + filter_memory[0];

		for(uint8_t i = 2; i < filter_order; i+=2){
			int64_t temp = 	input * filter_coeff[i] + filter_memory[(i/2)];
			filter_memory[(i/2) - 1] = ((filter_output_type *)(&temp))[0];
		}

		/* final line using only input and no older values*/
		int64_t temp = input * filter_coeff[filter_order];
		filter_memory[(filter_order/2) - 1] = ((filter_output_type *)(&temp))[0];

	    return FILTER_RETURN_MEM_SAVE_CAST(filter_output_type);
	}else{

		for(uint8_t i = 1; i < filter_order; i+=2){
			int64_t temp = input * filter_coeff[i] + filter_memory[((i - 1)/2)];
			filter_memory[((i - 1)/2)] = ((filter_output_type *)(&temp))[0];
		}

		return 0;
	}

}


