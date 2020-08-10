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

#define FILTER_NAME				stage3
#define FILTER_SIZE				6
#define filter_input_type		int32_t
#define filter_output_type		int32_t


DEC_FIR_FILTER_ADVANCE_MEM_IN(FILTER_NAME, filter_input_type, filter_output_type){

	int64_t temp;

	if(output_cycle == 1){

		FILTER_FIRST_LINE_MEM_SAVE_CAST(, int64_t, filter_output_type);

		FILTER_LINE_MEM_SAVE_MOVE_CAST(, 2, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(, 4, filter_output_type);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN_CAST(, 6, filter_output_type);

	    return FILTER_RETURN_MEM_SAVE_CAST(filter_output_type);
	}else{

		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 1, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 3, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 5, filter_output_type);
	    return 0;
	}

}


