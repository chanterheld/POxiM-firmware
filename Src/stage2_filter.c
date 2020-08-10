/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter2 = SimulatedFIR([126, 1016, 3576, 7176, 8980, 7176, 3576, 1016, 126], bit_depth=16, map_down=False)


#include "filters.h"

#define c0	(126/2)
#define c1	(1016/2)
#define c2	(3576/2)
#define c3	(7176/2)
#define c4	(8980/2)
#define c5	(7176/2)
#define c6	(3576/2)
#define c7	(1016/2)
#define c8	(126/2)

#define FILTER_NAME				stage2
#define FILTER_SIZE				8
#define filter_input_type		uint16_t
#define filter_output_type		int32_t


FILTER_MEMORY_MEM_SAVE(FILTER_NAME, filter_output_type, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE_MEM_SAVE(FILTER_NAME, filter_output_type);

		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 2);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 4);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 6);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN(FILTER_NAME, 8);

	    return retval;
	}else{

		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 1);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 3);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 5);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 7);
	    return 0;
	}

}


