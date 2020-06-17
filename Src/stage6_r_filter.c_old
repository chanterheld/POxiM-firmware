/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter6 = SimulatedFIR([-560384, -2084096, 1233920, 15193088, 11409408, -49801472, -85861120, 95791104, 472342016, 678930944, 472342016, 95791104, -85861120, -49801472, 11409408, 15193088, 1233920, -2084096, -560384], bit_depth=32)


#include "filters.h"

#define c0	(-560384)
#define c1	(-2084096)
#define c2	(1233920)
#define c3	(15193088)
#define c4	(11409408)
#define c5	(-49801472)
#define c6	(-85861120)
#define c7	(95791104)
#define c8	(472342016)
#define c9	(678930944)
#define c10	(472342016)
#define c11	(95791104)
#define c12	(-85861120)
#define c13	(-49801472)
#define c14	(11409408)
#define c15	(15193088)
#define c16	(1233920)
#define c17	(-2084096)
#define c18	(-560384)

#define FILTER_NAME				stage6_r
#define FILTER_SIZE				18
#define filter_input_type		int32_t
#define filter_output_type		int32_t


FILTER_MEMORY_MEM_SAVE(FILTER_NAME, filter_output_type, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	int64_t temp;

	if(output_cycle == 1){

		FILTER_FIRST_LINE_MEM_SAVE(FILTER_NAME, int64_t);

		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 2, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 4, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 6, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 8, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 10, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 12, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 14, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 16, filter_output_type);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN_CAST(FILTER_NAME, 18, filter_output_type);

	    return FILTER_RETURN_MEM_SAVE_CAST(filter_output_type);
	}else{

		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 1, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 3, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 5, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 7, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 9, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 11, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 13, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 15, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 17, filter_output_type);
	    return 0;
	}

}


