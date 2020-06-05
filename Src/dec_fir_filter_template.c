/*
 * filters.c
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */


#include "filters.h"

#define c0	(0)
#define c1	(0)
#define c2	(0)
#define c3	(0)
#define c4	(0)
#define c5	(0)
#define c6	(0)
#define c7	(0)
#define c8	(0)
#define c9	(0)
#define c10	(0)
#define c11	(0)
#define c12	(0)
#define c13	(0)
#define c14	(0)
#define c15	(0)
#define c16	(0)
#define c17	(0)
#define c18	(0)
#define c19	(0)
#define c20	(0)
#define c21	(0)
#define c22	(0)
#define c23	(0)
#define c24	(0)
#define c25	(0)
#define c26	(0)
#define c27	(0)
#define c28	(0)
#define c29	(0)
#define c30	(0)

#define FILTER_NAME				dec_template
#define FILTER_SIZE				30
#define filter_input_type		short
#define filter_output_type		short


FILTER_MEMORY(FILTER_NAME, filter_output_type, FILTER_SIZE);

DEC_FIR_FILTER_ADVANCE(FILTER_NAME, filter_input_type, filter_output_type){

	if(output_cycle == 1){

		FILTER_FIRST_LINE(FILTER_NAME, filter_output_type);

		FILTER_LINE(FILTER_NAME, 2);
		FILTER_LINE(FILTER_NAME, 4);
		FILTER_LINE(FILTER_NAME, 6);
		FILTER_LINE(FILTER_NAME, 8);
		FILTER_LINE(FILTER_NAME, 10);
		FILTER_LINE(FILTER_NAME, 12);
		FILTER_LINE(FILTER_NAME, 14);
		FILTER_LINE(FILTER_NAME, 16);
		FILTER_LINE(FILTER_NAME, 18);
		FILTER_LINE(FILTER_NAME, 20);
		FILTER_LINE(FILTER_NAME, 22);
		FILTER_LINE(FILTER_NAME, 24);
		FILTER_LINE(FILTER_NAME, 26);
		FILTER_LINE(FILTER_NAME, 28);

		/*this line might have to go to the other block depending on filter size(even/uneven), the */
		FILTER_LAST_LINE(FILTER_NAME, 30);

	    return retval;
	}else{

		FILTER_LINE(FILTER_NAME, 1);
		FILTER_LINE(FILTER_NAME, 3);
		FILTER_LINE(FILTER_NAME, 5);
		FILTER_LINE(FILTER_NAME, 7);
		FILTER_LINE(FILTER_NAME, 9);i
		FILTER_LINE(FILTER_NAME, 11);
		FILTER_LINE(FILTER_NAME, 13);
		FILTER_LINE(FILTER_NAME, 15);
		FILTER_LINE(FILTER_NAME, 17);
		FILTER_LINE(FILTER_NAME, 19);
		FILTER_LINE(FILTER_NAME, 21);
		FILTER_LINE(FILTER_NAME, 23);
		FILTER_LINE(FILTER_NAME, 25);
		FILTER_LINE(FILTER_NAME, 27);
		FILTER_LINE(FILTER_NAME, 29);
	    return 0;
	}

}


