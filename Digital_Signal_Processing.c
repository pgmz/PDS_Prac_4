/*
 * Digital_Signal_Processing.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "Digital_Signal_Processing.h"
#include <math.h>

float DATA_BUFFER[MAX_BUFFER] = {0};
float Xkm = 0.0, Xkm1 = 0.0, Xkm2 = 0.0,
		Ykm = 0.0, Ykm1 = 0.0, Ykm2 = 0.0,
			Xkm_ = 0.0;
uint16_t n = 0;
uint8_t nx = 0;
uint8_t ny = 0;
float mod_sample = 0.0;
float query_sample = 0.0;
float y_n;
float y_n_filtered;
float x_n;

extern float ADC_data;
extern float *DSP_Ring_Mod_SF_Rate;

extern float *Effect_Amplitude;
extern float *Effect_Alpha;
extern float *Effect_Rate;

void DSP_task (){
	/*Valor leído por ADC, es x_n **/
	Xkm = ADC_data - 0x3be;
	DSP_Tremolo_SF();
}

void DSP_Tremolo_SF(){
	y_n = Xkm * (1 +
			(*Effect_Alpha)*sin(2*PI*(n)*(*Effect_Rate)*SAMPLE_PERIOD + PI/2));
	y_n = y_n * (*Effect_Amplitude);
	n++;
}


#if 0
DSP_Chorus_SF_Params_Type DSP_Chorus_SF_Params = {
		0.013, 0.003, 1, 0.7, 0.4
};

DSP_Tremolo_SF_Params_Type DSP_Tremolo_SF_Params = {
		20, 0.9, 0
};

DSP_Ring_Mod_SF_Params_Type DSP_Ring_Mod_SF_Params = {
		20, 0
};

void DSP_Ring_Mod_SF(){
	y_n = Xkm * (sin(2*PI*DSP_Ring_Mod_SF_Params.index*(DSP_Ring_Mod_SF_Params.rate)*SAMPLE_PERIOD));
	DSP_Ring_Mod_SF_Params.index++;
}

void DSP_Chorus_SF(){
	mod_sample = (int)(DSP_Chorus_SF_Params.modulation_depth * SAMPLE_RATE + 0.5);
	mod_sample = mod_sample * sin(2 * PI * DSP_Chorus_SF_Params.modulation_rate * SAMPLE_PERIOD * n);
	mod_sample = mod_sample + (int)(DSP_Chorus_SF_Params.delay_length * SAMPLE_RATE + 0.5);
	query_sample = mod_sample - (int)(mod_sample);
	y_n = DATA_BUFFER[(int)(mod_sample)] + (DATA_BUFFER[(int)(mod_sample + 0.9)] - DATA_BUFFER[(int)(mod_sample)])*query_sample;

	DATA_BUFFER[n] = y_n*DSP_Chorus_SF_Params.feedback + Xkm;

	n = (n >= MAX_BUFFER)?(0):(n + 1);

	if(n == MAX_BUFFER)
	{
		int aux = 0;
	}

}

void DSP_LF(){

	y_n = Xkm*0.97503 - Xkm1*1.8509 + Xkm2*0.87794 + Ykm1*1.84951 - Ykm2*0.85506;

	Ykm2 = Ykm1;
	Ykm1 = y_n;
	Xkm2 = Xkm1;
	Xkm1 = Xkm;

}
#endif
