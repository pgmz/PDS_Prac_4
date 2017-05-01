/*
 * Digital_Signal_Processing.c
 *
 *  Created on: 11/03/2017
 *      Author: Patricio Gomez
 */

#include "Digital_Signal_Processing.h"
#include <math.h>

float DATA_BUFFER[MAX_BUFFER] = {0};
float X_REG[3] = {0};
float Y_REG[2] = {0};
uint16_t n = 0;
uint8_t nx = 0;
uint8_t ny = 0;
float mod_sample = 0.0;
float query_sample = 0.0;
float y_n;
float y_n_filtered;
float x_n;


extern float ADC_data;

DSP_Chorus_SF_Params_Type DSP_Chorus_SF_Params = {
		0.013, 0.003, 1, 0.7, 0.4
};

DSP_Tremolo_SF_Params_Type DSP_Tremolo_SF_Params = {
		20, 0.5, 0
};

DSP_Ring_Mod_SF_Params_Type DSP_Ring_Mod_SF_Params = {
		440, 0
};

void DSP_task (){
	/*Valor leído por ADC, es x_n **/
	x_n = ADC_data - 0x155;
	DSP_Ring_Mod_SF();
}

void DSP_Chorus_SF(){
	mod_sample = (int)(DSP_Chorus_SF_Params.modulation_depth * SAMPLE_RATE + 0.5);
	mod_sample = mod_sample * sin(2 * PI * DSP_Chorus_SF_Params.modulation_rate * SAMPLE_PERIOD * n);
	mod_sample = mod_sample + (int)(DSP_Chorus_SF_Params.delay_length * SAMPLE_RATE + 0.5);
	query_sample = mod_sample - (int)(mod_sample);
	y_n = DATA_BUFFER[(int)(mod_sample)] + (DATA_BUFFER[(int)(mod_sample + 0.9)] - DATA_BUFFER[(int)(mod_sample)])*query_sample;

	DATA_BUFFER[n] = y_n*DSP_Chorus_SF_Params.feedback + x_n;

	n = (n >= MAX_BUFFER)?(0):(n + 1);

	if(n == MAX_BUFFER)
	{
		int aux = 0;
	}

}

void DSP_LF(){

	y_n_filtered = Y_REG[ny]*1.5573 - Y_REG[(ny == 1)?(0):(1)]*0.6275 + X_REG[nx]*0.9037 - X_REG[(nx == 0)?(1):((nx == 1)?(2):(0))]*1.5669 + X_REG[(nx == 0)?(2):((nx == 1)?(0):(1))]*0.6785;

	X_REG[nx] = y_n;
	Y_REG[ny] = y_n_filtered;

	nx = (nx >= 2)?(0):(nx + 1);
	ny = (ny >= 1)?(0):(ny + 1);

}

void DSP_Tremolo_SF(){
	y_n = x_n * (1 + DSP_Tremolo_SF_Params.alpha*sin(2*PI*DSP_Tremolo_SF_Params.index*DSP_Tremolo_SF_Params.rate*SAMPLE_PERIOD + PI/2));
	DSP_Tremolo_SF_Params.index++;
}

void DSP_Ring_Mod_SF(){
	y_n = x_n * (sin(2*PI*DSP_Ring_Mod_SF_Params.index*DSP_Ring_Mod_SF_Params.rate*SAMPLE_PERIOD));
	DSP_Ring_Mod_SF_Params.index++;
}
