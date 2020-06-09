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


FILTER_MEMORY(FILTER_NAME, int64_t, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE(FILTER_NAME, int64_t);

		FILTER_LINE(FILTER_NAME, 2);
		FILTER_LINE(FILTER_NAME, 4);
		FILTER_LINE(FILTER_NAME, 6);
		FILTER_LINE(FILTER_NAME, 8);
		FILTER_LINE(FILTER_NAME, 10);
		FILTER_LINE(FILTER_NAME, 12);
		FILTER_LINE(FILTER_NAME, 14);
		FILTER_LINE(FILTER_NAME, 16);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE(FILTER_NAME, 18);

	    return ((filter_output_type *)(&retval))[0];
	}else{

		FILTER_LINE(FILTER_NAME, 1);
		FILTER_LINE(FILTER_NAME, 3);
		FILTER_LINE(FILTER_NAME, 5);
		FILTER_LINE(FILTER_NAME, 7);
		FILTER_LINE(FILTER_NAME, 9);
		FILTER_LINE(FILTER_NAME, 11);
		FILTER_LINE(FILTER_NAME, 13);
		FILTER_LINE(FILTER_NAME, 15);
		FILTER_LINE(FILTER_NAME, 17);
	    return 0;
	}

}


