#include "measure.h"

togi_t Voltage = {
	.omega = 50.0f*2.0f*3.1415f,
};
togi_t Current = {
};

static uint32_t Result_Measure[2] = {0};

float Voltage_measure = 0.0f;
float Current_measure = 0.0f;
float filtred_1 = 0.0f;


void measure_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODE1 | GPIO_MODER_MODE2);
	GPIOA->MODER |= (GPIO_MODER_ANALOG << GPIO_MODER_MODE1_Pos) | (GPIO_MODER_ANALOG << GPIO_MODER_MODE2_Pos);
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	
	GPIOC->MODER = (GPIO_MODER_OUTPUT << GPIO_MODER_MODE13_Pos) | (GPIO_MODER_OUTPUT << GPIO_MODER_MODE14_Pos);
	GPIOC->ODR |= GPIO_ODR_OD13;
	GPIOC->ODR &= ~GPIO_ODR_OD14;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->ARR = 10000;
	TIM3->PSC = 0;
	TIM3->CCMR1 |= (TIM_CCMR1_CC1S_OUTPUT << TIM_CCMR1_CC1S_Pos) | (TIM_CCMR1_PWM_MODE_1 << TIM_CCMR1_OC1M_Pos);
	TIM3->CCER |= TIM_CCER_CC1E;
	TIM3->CCR1 = (TIM3->ARR)/2UL; // Select fot PWM
	TIM3->CR2 |= 2 << TIM_CR2_MMS_Pos; // Master mode select: update
	TIM3->CR1 |= TIM_CR1_CEN;
	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR1 |= (0 << ADC_CR1_RES_Pos) | ADC_CR1_SCAN;
	ADC1->CR2 |= (1 << ADC_CR2_EXTEN_Pos) | (8 << ADC_CR2_EXTSEL_Pos) | ADC_CR2_ADON | ADC_CR2_EOCS | ADC_CR2_DMA | ADC_CR2_DDS;
	ADC1->SQR1 |= 1 << ADC_SQR1_L_Pos;
	ADC1->SQR3 |= (1 << ADC_SQR3_SQ1_Pos) | (2 << ADC_SQR3_SQ2_Pos);
	ADC1->SMPR2 |= (7 << ADC_SMPR2_SMP1_Pos) | (7 << ADC_SMPR2_SMP2_Pos);
	
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream0->CR |= (0 << DMA_SxCR_CHSEL_Pos) | (0 << DMA_SxCR_DIR_Pos) | (2 << DMA_SxCR_PSIZE_Pos) 
	| (2 << DMA_SxCR_MSIZE_Pos) | (1 << DMA_SxCR_MINC_Pos) | (0 << DMA_SxCR_PINC_Pos) | DMA_SxCR_CIRC | DMA_SxCR_TCIE;
	DMA2_Stream0->M0AR |= (uint32_t)Result_Measure;
	DMA2_Stream0->PAR |= (uint32_t)&ADC1->DR;
	DMA2_Stream0->NDTR = 2;
	DMA2_Stream0->CR |= DMA_SxCR_EN;
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void DMA2_Stream0_IRQHandler(void){
	DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
	GPIOC->BSRR |= GPIO_BSRR_BS14; // Start check calculation time
	
	// Normalization of measured signals
	Voltage_measure = 3.3f*Result_Measure[0]/4095.0f;
	Current_measure = 3.3f*Result_Measure[1]/4095.0f;
	
	
	TOGI();
	
	GPIOC->BSRR |= GPIO_BSRR_BR14; // Stop check calculation time
}
void TOGI(void){
	// Voltage Pll
	Voltage.error = Voltage_measure - (Voltage.offset + Voltage.alfa);
	Voltage.offset += Voltage.error*SAMPLING_STEP;
	Voltage.alfa += Voltage.omega*(Voltage.error - Voltage.beta)*SAMPLING_STEP;
	Voltage.beta += Voltage.alfa*Voltage.omega*SAMPLING_STEP;
	Voltage.omega += Voltage.beta*Voltage.error*(-SOGI_GAIN)*SAMPLING_STEP;
	// Current PLL
	Current.error = Current_measure - (Current.offset + Current.alfa);
	Current.offset += Current.error*SAMPLING_STEP;
	Current.alfa += Voltage.omega*(Current.error - Current.beta)*SAMPLING_STEP;
	Current.beta += Current.alfa*Voltage.omega*SAMPLING_STEP;
}
