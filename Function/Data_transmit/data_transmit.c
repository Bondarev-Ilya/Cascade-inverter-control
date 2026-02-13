#include "data_transmit.h"

#define GPIO_MODER_ALTARNATE 2

uint8_t counter = 0;
uint8_t index_array = 0;
uint8_t flag_array = 0;

float angel[NUMBER_OF_POINTS] = {0};
float demon[NUMBER_OF_POINTS] = {0};
	
void transmit_init(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_ALTARNATE << GPIO_MODER_MODE6_Pos;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2;
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR3 |= USART_CR3_DMAT;
	USART1->BRR |= 651;
	USART1->CR1 |= USART_CR1_UE;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream7->CR |= (4 << DMA_SxCR_CHSEL_Pos) | (1 << DMA_SxCR_DIR_Pos) | (1 << DMA_SxCR_MINC_Pos) | (1 << DMA_SxCR_DBM_Pos);
	DMA2_Stream7->NDTR = NUMBER_OF_POINTS;
	DMA2_Stream7->PAR = USART1->DR;
	DMA2_Stream7->M0AR = (uint32_t)angel;
	DMA2_Stream7->M1AR = (uint32_t)demon;
	DMA2_Stream7->CR |= DMA_SxCR_EN;
}

void my_fun(void){
	if(counter%(NUMBER_OF_MEASUREMENTS/NUMBER_OF_POINTS) == 0){
		if(flag_array == 0){
			angel[index_array] = Voltage_Set; 
		}
		else{
			demon[index_array] = Voltage_Set;
		}
		index_array++;
	}
	if(counter == NUMBER_OF_MEASUREMENTS){
		counter = 0;
		index_array = 0;
		flag_array ^= 1;
	}
	counter++;
}
