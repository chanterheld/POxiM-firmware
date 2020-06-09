/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter3 = SimulatedFIR([-59783424, 178432, 536870656, 953818368, 536870656, 178432, -59783424], bit_depth=32)

#include "filters.h"

#define c0	(-59783424)
#define c1	(178432)
#define c2	(536870656)
#define c3	(953818368)
#define c4	(536870656)
#define c5	(178432)
#define c6	(-59783424)

#define FILTER_NAME				stage3_ir
#define FILTER_SIZE				6
#define filter_input_type		int32_t
#define filter_output_type		int32_t


FILTER_MEMORY(FILTER_NAME, int64_t, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE(FILTER_NAME, int64_t);

		FILTER_LINE(FILTER_NAME, 2);
		FILTER_LINE(FILTER_NAME, 4);
		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE(FILTER_NAME, 6);

	    return ((filter_output_type *)(&retval))[0];
	}else{

		FILTER_LINE(FILTER_NAME, 1);
		FILTER_LINE(FILTER_NAME, 3);
		FILTER_LINE(FILTER_NAME, 5);
	    return 0;
	}

}


