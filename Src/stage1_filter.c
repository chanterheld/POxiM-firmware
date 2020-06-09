/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */


#include "filters.h"

//filter1 = SimulatedFIR([4, 24, 60, 80, 60, 24, 4], bit_depth=8, bipolar=False, map_down=False)


#define c0	(4)
#define c1	(24)
#define c2	(60)
#define c3	(80)
#define c4	(60)
#define c5	(24)
#define c6	(4)

#define FILTER_NAME				stage1
#define FILTER_SIZE				6
#define filter_input_type		int8_t
#define filter_output_type		int16_t

FILTER_MEMORY(FILTER_NAME, filter_output_type, FILTER_SIZE);


DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE(FILTER_NAME, filter_output_type);
		FILTER_LINE(FILTER_NAME, 2);
		FILTER_LINE(FILTER_NAME, 4);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE(FILTER_NAME, 6);

	    return retval;
	}else{
		FILTER_LINE(FILTER_NAME, 1);
		FILTER_LINE(FILTER_NAME, 3);
		FILTER_LINE(FILTER_NAME, 5);
	    return 0;
	}

}


