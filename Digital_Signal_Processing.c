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
int n = 0;
int nx = 0;
int ny = 0;
float mod_sample = 0.0;
float query_sample = 0.0;
float y_n;
float y_n_filtered;
float x_n;


extern float ADC_data;

DSP_Chorus_SF_Params_Type DSP_Chorus_SF_Params = {
		0.013, 0.003, 1, 0.7, 0.4
};

void DSP_task (){
	/*Valor leído por ADC, es x_n **/
	x_n = ADC_data - 0x160;
	DSP_Chorus_SF();
	DSP_LF();
}

void DSP_Chorus_SF(){
	mod_sample = (int)(DSP_Chorus_SF_Params.modulation_depth * SAMPLE_RATE + 0.5);
	mod_sample = mod_sample * sin(2 * PI * DSP_Chorus_SF_Params.modulation_rate * (1/SAMPLE_RATE) * n);
	mod_sample = mod_sample + (int)(DSP_Chorus_SF_Params.delay_length * SAMPLE_RATE + 0.5);
	query_sample = mod_sample - (int)(mod_sample);
	y_n = DATA_BUFFER[(int)(mod_sample)] + (DATA_BUFFER[(int)(mod_sample + 0.9)] - DATA_BUFFER[(int)(mod_sample)])*query_sample;

	DATA_BUFFER[n] = y_n*DSP_Chorus_SF_Params.feedback + x_n;

	n = (n >= MAX_BUFFER)?(0):(n + 1);

}

void DSP_LF(){

	y_n_filtered = Y_REG[ny]*1.5573 - Y_REG[(ny == 1)?(0):(1)]*0.6275 + X_REG[nx]*0.9037 - X_REG[(nx == 0)?(1):((nx == 1)?(2):(0))]*1.5669 + X_REG[(nx == 0)?(2):((nx == 1)?(0):(1))]*0.6785;

	X_REG[nx] = y_n;
	Y_REG[ny] = y_n_filtered;

	nx = (nx >= 3)?(0):(nx + 1);
	ny = (ny >= 2)?(0):(ny + 1);

}
