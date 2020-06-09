/*
 * filters.h
 *
 *  Created on: Jun 4, 2020
 *      Author: daandegroot
 */

#include "stm8s.h"


#ifndef INC_FILTERS_H_
#define INC_FILTERS_H_

typedef   signed long long     int64_t;


#define CONCAT(a,b)											a ## b
#define CONCAT2(a,b)										CONCAT(a,b)

#define FIR_FILTER_ADVANCE(name, type_in, type_out)			type_out CONCAT(name, _fir_filter_advance)(type_in input)
#define DEC_FIR_FILTER_ADVANCE(name, type_in, type_out)   	type_out CONCAT(name, _fir_filter_advance)(type_in input, char output_cycle)

#define FILTER_MEMORY(name, type, size)						volatile type CONCAT(name,_filter_memory)[size] = {0}

#define FILTER_LINE(name, nr)								CONCAT(name,_filter_memory)[nr - 1] = input * (c ## nr) + CONCAT(name,_filter_memory)[nr]
#define FILTER_LAST_LINE(name, nr)							CONCAT(name,_filter_memory)[nr - 1] = input * (c ## nr)
#define FILTER_FIRST_LINE(name, type)						type retval	= input * c0 + CONCAT(name,_filter_memory)[0]

/*
 * mem save
 */


#define FILTER_MEMORY_MEM_SAVE(name, type, size)			volatile type CONCAT(name,_filter_memory)[(size/2) + 1] = {0}

#define FILTER_FIRST_LINE_MEM_SAVE(name, type)				FILTER_FIRST_LINE(name, type)

/* even*/
#define FILTER_LINE_MEM_SAVE_MOVE(name, nr)					CONCAT(name,_filter_memory)[(nr/2) - 1] = input * (c ## nr) + CONCAT(name,_filter_memory)[(nr/2)]

/*odd*/
#define FILTER_LINE_MEM_SAVE_STATIONARY(name, nr)			CONCAT(name,_filter_memory)[((nr - 1)/2)] = input * (c ## nr) + CONCAT(name,_filter_memory)[((nr - 1)/2)]

#define FILTER_LAST_LINE_MEM_SAVE_EVEN(name, nr)			CONCAT(name,_filter_memory)[(nr/2) - 1] = input * (c ## nr)
#define FILTER_LAST_LINE_MEM_SAVE_ODD(name, nr)				FILTER_LINE_MEM_SAVE_STATIONARY(name, nr)

#define FILTER_MEMORY_ACCES(name, indx)						CONCAT(name,_filter_memory)[indx]




#endif /* INC_FILTERS_H_ */
