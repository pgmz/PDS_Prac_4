/*
 * Digital_Signal_Processing.h
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#ifndef SOURCE_DIGITAL_SIGNAL_PROCESSING_H_
#define SOURCE_DIGITAL_SIGNAL_PROCESSING_H_

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "External_mod_process.h"

#define DAC_QUEUE_LENGTH	20
#define DAC_QUEUE_ITEM_SIZE	sizeof(float)

#define SAMPLE_RATE			50000
#define SAMPLE_PERIOD		1/SAMPLE_RATE
#define PI 					3.14159265

#define MAX_BUFFER			1650

typedef struct{
	float delay_length;
	float modulation_depth;
	float modulation_rate;
	float feedback;
	float mix_balance;
}DSP_Chorus_SF_Params_Type;

#define TREMOLO_INDEX_MAX	104

typedef struct{
	float rate;
	float alpha;
	uint8_t index;
}DSP_Tremolo_SF_Params_Type;

void DSP_task ();
void DSP_Chorus_SF();
void DSP_LF();

#endif /* SOURCE_DIGITAL_SIGNAL_PROCESSING_H_ */
