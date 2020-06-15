/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

//filter7 = SimulatedFIR([5120, 53504, 110336, -148992, -819968, -543232, 2222336, 4315392, -1463808, -12609280, -9195520, 19376640, 38183424, -4659968, -83977472, -71265280, 128718848, 417668352, 556807936, 417668352, 128718848, -71265280, -83977472, -4659968, 38183424, 19376640, -9195520, -12609280, -1463808, 4315392, 2222336, -543232, -819968, -148992, 110336, 53504, 5120], bit_depth=32)

#include "filters.h"

#define c0	(5120)
#define c1	(53504)
#define c2	(110336)
#define c3	(-148992)
#define c4	(-819968)
#define c5	(-543232)
#define c6	(2222336)
#define c7	(4315392)
#define c8	(-1463808)
#define c9	(-12609280)
#define c10	(-9195520)
#define c11	(19376640)
#define c12	(38183424)
#define c13	(-4659968)
#define c14	(-83977472)
#define c15	(-71265280)
#define c16	(128718848)
#define c17	(417668352)
#define c18	(556807936)
#define c19	(417668352)
#define c20	(128718848)
#define c21	(-71265280)
#define c22	(-83977472)
#define c23	(-4659968)
#define c24	(38183424)
#define c25	(19376640)
#define c26	(-9195520)
#define c27	(-12609280)
#define c28	(-1463808)
#define c29	(4315392)
#define c30	(2222336)
#define c31	(-543232)
#define c32	(-819968)
#define c33	(-148992)
#define c34	(110336)
#define c35	(53504)
#define c36	(5120)


#define FILTER_NAME				stage7_ir
#define FILTER_SIZE				36
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
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 18, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 20, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 22, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 24, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 26, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 28, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 30, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 32, filter_output_type);
		FILTER_LINE_MEM_SAVE_MOVE_CAST(FILTER_NAME, 34, filter_output_type);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE_MEM_SAVE_EVEN_CAST(FILTER_NAME, 36, filter_output_type);

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
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 19, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 21, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 23, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 25, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 27, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 29, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 31, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 33, filter_output_type);
		FILTER_LINE_MEM_SAVE_STATIONARY_CAST(FILTER_NAME, 35, filter_output_type);

		return 0;
	}

}


