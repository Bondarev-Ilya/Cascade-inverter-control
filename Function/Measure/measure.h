#ifndef MEASURE_H_
#define	MEASURE_H_

#define SIN_FI 0.9527f
#define COS_FI 0.304f
#define AMP_GAIN (1.0f/0.302)
#define FILTER_FREQ 100.0f

#define TOGI_GAIN 100
#define SAMPLING_STEP 1.0f/10000.0f
#define FREQ 50.0f*2.0f*3.1415f
#define FREQ_SAT 30.0f

#define SECONDARY_WINDING_COEFFICIENT 0.030631f // 6.8/222
#define SECONDARY_WINDING (15.0f)

#define MEASURE_VOLTAGE_GAIN 0.0038596f
/*
	Uin = 240 V, Rin = 26 kOhm, Iin = (Uin*1.41)/Rin = 13.1 mA
	K1 = 2.5, Iout = Iin*K1 = 32.75 mA, Rout = 100 Ohm, Uout = 3.275 V
	K2 = 0.4, Ugain = (Uout*K2)+1.65 = 2.96 V
	Kgain = (Uout*K2)/(Uin*1.41) = 0.0038596
*/

#define MEASURE_CURRENT_GAIN (13.0f*0.5f*0.470f)
/*
	Iin = 500mA, R = 470 mOhm, Vr = Iin*R = 235 mV, 
	K1 = 13, K2 = 0.5, Vdc = 1.65 V, Vout = Vr*K1*K2+Vdc = 3.19 V
	Kgain = K1*K2*R
*/

#include <stm32f411xe.h>
#include "power_control.h"

void measure_init(void);
void TOGI(void);
void DMA2_Stream0_IRQHandler(void);

extern float Voltage_measure;
extern float Current_measure;

typedef struct{
	float offset;
	float error;
	float alfa;
	float beta;
	float omega;
	float omega_dif;
	float omega_no_sat;
}togi_t;

extern togi_t Voltage_norm;
extern togi_t Current_norm;

#endif