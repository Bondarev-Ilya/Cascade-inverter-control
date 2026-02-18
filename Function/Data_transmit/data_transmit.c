#include "data_transmit.h"

#define GPIO_MODER_ALTARNATE 2

uint32_t counter = 0;
uint8_t index_array = HEADER;
uint8_t flag_array = 0;

float angel[HEADER+NUMBER_OF_POINTS+TAIL] = {0};
float demon[HEADER+NUMBER_OF_POINTS+TAIL] = {0};
	
void transmit_init(void){
	package_init();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_ALTARNATE << GPIO_MODER_MODE6_Pos;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2;
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR3 |= USART_CR3_DMAT;
	// 100000000 / (9600 * 8 * 2) = 651.0416 (0.0416 * 16 = 0.6656 = 1)
	USART1->BRR |= (651 << USART_BRR_DIV_Mantissa_Pos) | (1 << USART_BRR_DIV_Fraction_Pos);
	USART1->CR1 |= USART_CR1_UE;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream7->CR |= (4 << DMA_SxCR_CHSEL_Pos) | (1 << DMA_SxCR_DIR_Pos) | (1 << DMA_SxCR_MINC_Pos);
	DMA2_Stream7->PAR = (uint32_t)&USART1->DR;
}

void package_init(void){
	angel[0] = FIRST_HEAD;
	angel[1] = SECOND_HEAD;
	angel[HEADER+NUMBER_OF_POINTS+TAIL-1] = FIRST_TAIL;
	
	demon[0] = FIRST_HEAD;
	demon[1] = SECOND_HEAD;
	demon[HEADER+NUMBER_OF_POINTS+TAIL-1] = FIRST_TAIL;
}

void data_transmit(void){
	if(counter == NUMBER_OF_MEASUREMENTS){
		if(flag_array == 0){
			DMA2_Stream7->CR &= ~DMA_SxCR_EN;
			DMA2_Stream7->M0AR = (uint32_t)&angel;
			DMA2_Stream7->NDTR = (HEADER+NUMBER_OF_POINTS+TAIL)*SIZE_FLOAT;
			DMA2->HIFCR = DMA_HIFCR_CTCIF7;
			DMA2_Stream7->CR |= DMA_SxCR_EN;
		}
		else{
			DMA2_Stream7->CR &= ~DMA_SxCR_EN;
			DMA2_Stream7->M0AR = (uint32_t)&demon;
			DMA2_Stream7->NDTR = (HEADER+NUMBER_OF_POINTS+TAIL)*SIZE_FLOAT;
			DMA2->HIFCR = DMA_HIFCR_CTCIF7;
			DMA2_Stream7->CR |= DMA_SxCR_EN;
		}
		counter = 0;
		index_array = HEADER;
		flag_array ^= 1;
	}
	if(counter%(NUMBER_OF_MEASUREMENTS/NUMBER_OF_POINTS) == 0){
		if(flag_array == 0){
			angel[index_array] = Voltage_Set; 
		}
		else{
			demon[index_array] = Voltage_Set;
		}
		index_array++;
	}
	counter++;
}
