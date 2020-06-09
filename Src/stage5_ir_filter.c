/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter5 = SimulatedFIR([4534784, 10265344, -26586880, -84064768, 65555456, 500018688, 765431552, 500018688, 65555456, -84064768, -26586880, 10265344, 4534784], bit_depth=32)

#include "filters.h"

#define c0	(4534784)
#define c1	(10265344)
#define c2	(-26586880)
#define c3	(-84064768)
#define c4	(65555456)
#define c5	(500018688)
#define c6	(765431552)
#define c7	(500018688)
#define c8	(65555456)
#define c9	(-84064768)
#define c10	(-26586880)
#define c11	(10265344)
#define c12	(4534784)


#define FILTER_NAME				stage5_ir
#define FILTER_SIZE				12
#define filter_input_type		int32_t
#define filter_output_type		int32_t


FILTER_MEMORY_MEM_SAVE(FILTER_NAME, int64_t, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE_MEM_SAVE(FILTER_NAME, int64_t);

		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 2);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 4);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 6);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 8);
		FILTER_LINE_MEM_SAVE_MOVE(FILTER_NAME, 10);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN(FILTER_NAME, 12);

	    return ((filter_output_type *)(&retval))[0];
	}else{

		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 1);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 3);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 5);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 7);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 9);
		FILTER_LINE_MEM_SAVE_STATIONARY(FILTER_NAME, 11);
	    return 0;
	}

}


