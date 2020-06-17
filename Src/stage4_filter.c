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


#define FILTER_NAME				stage4
#define FILTER_SIZE				8
#define filter_input_type		int32_t
#define filter_output_type		int32_t

DEC_FIR_FILTER_ADVANCE_MEM_IN(FILTER_NAME, filter_input_type, filter_output_type){

	int64_t temp;

	if(output_cycle == 1){

		FILTER_FIRST_LINE_MEM_SAVE(, int64_t);

		FILTER_LINE_MEM_SAVE_MOVE_CAST(, 2, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(, 4, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(, 6, filter_output_type);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN_CAST(, 8, filter_output_type);

	    return FILTER_RETURN_MEM_SAVE_CAST(filter_output_type);
	}else{

		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 1, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 3, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 5, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(, 7, filter_output_type);
	    return 0;
	}

}


