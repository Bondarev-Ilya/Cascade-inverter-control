#ifndef MEASURE_H_
#define	MEASURE_H_

#define MEASURE_VOLTAGE_GAIN 0.0054f
#define MEASURE_CURRENT_GAIN 1.0f

#define TOGI_GAIN 100
#define SAMPLING_STEP 1.0f/10000.0f

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
}togi_t;

extern togi_t Voltage;
extern togi_t Current;

extern togi_t Voltage_norm;
extern togi_t Current_norm;

#endif