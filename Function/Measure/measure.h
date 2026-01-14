#ifndef MEASURE_H_
#define	MEASURE_H_

#define SOGI_GAIN 100
#define FILTER_ALFA 0.1f
#define SAMPLING_STEP 1.0f/10000.0f
#define GPIO_MODER_OUTPUT 1
#define GPIO_MODER_ANALOG 3
#define TIM_CCMR1_PWM_MODE_1 6
#define TIM_CCMR1_CC1S_OUTPUT 0

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

#endif