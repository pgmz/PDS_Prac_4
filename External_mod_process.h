/*
 * External_mod_process.h
 *
 *  Created on: 12/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_EXTERNAL_MOD_PROCESS_H_
#define SOURCE_EXTERNAL_MOD_PROCESS_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_pit.h"
#include "board.h"

typedef enum {
	TREMOLO_RATE,
	TREMOLO_ALPHA,
	AMP_PARAM
}Mod_State_type;

#define AMP_PARAM_MAX 1
#define AMP_PARAM_MIN 0.1
#define AMP_PARAM_INC 0.1

#define TREMOLO_RATE_MAX	20
#define TREMOLO_RATE_MIN	2
#define TREMOLO_RATE_INC	2

#define TREMOLO_ALPHA_MAX	0.9
#define TREMOLO_ALPHA_MIN	0.1
#define TREMOLO_ALPHA_INC	0.1

typedef struct {
	Mod_State_type Mod_State;
	Mod_State_type Mod_Next;
	float Modifier;
	float Modifier_inc;
	float Modifier_max;
	float Modifier_min;
	uint8_t led1 :1;
	uint8_t led2 :1;
	uint8_t led3 :1;
}Mod_SM_type;

void External_mod_process_init();

void Modify_amplitude(uint8_t Increment);

#endif /* SOURCE_EXTERNAL_MOD_PROCESS_H_ */
