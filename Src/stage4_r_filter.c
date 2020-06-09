/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter4 = SimulatedFIR([-21671168, -57446912, 86228736, 515199488, 786390528, 515199488, 86228736, -57446912, -21671168], bit_depth=32)


#include "filters.h"

#define c0	(-21671168)
#define c1	(-57446912)
#define c2	(86228736)
#define c3	(515199488)
#define c4	(786390528)
#define c5	(515199488)
#define c6	(86228736)
#define c7	(-57446912)
#define c8	(-21671168)


#define FILTER_NAME				stage4_r
#define FILTER_SIZE				8
#define filter_input_type		int32_t
#define filter_output_type		int32_t


FILTER_MEMORY(FILTER_NAME, int64_t, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE(FILTER_NAME, int64_t);

		FILTER_LINE(FILTER_NAME, 2);
		FILTER_LINE(FILTER_NAME, 4);
		FILTER_LINE(FILTER_NAME, 6);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE(FILTER_NAME, 8);

	    return ((filter_output_type *)(&retval))[0];
	}else{

		FILTER_LINE(FILTER_NAME, 1);
		FILTER_LINE(FILTER_NAME, 3);
		FILTER_LINE(FILTER_NAME, 5);
		FILTER_LINE(FILTER_NAME, 7);
	    return 0;
	}

}


