/*
 * generic_iir.c
 *
 *  Created on: Jun 18, 2020
 *      Author: daandegroot
 */

#include "stm8s.h"
#include "stm8s_it.h"
#include "filters.h"

#define a1		(coeff[0])
#define a2		(coeff[1])

#define b0		(coeff[2])
#define b1		(coeff[3])
#define b2		(coeff[4])

int32_t generic_2ord_iir_advance(int32_t input, int32_t *filter_memory, int32_t *coeff){
	int64_t input_64 = input - (filter_memory[0] * a1) - (filter_memory[1] * a2);

	int32_t intermediate = ((int32_t *)(&input_64))[0];

	int64_t output = (intermediate * b0) + (filter_memory[0] * b1) + (filter_memory[1] * b2);

	filter_memory[1] = filter_memory[0];
	filter_memory[0] = intermediate;

	return ((int32_t *)(&output))[0];
}
