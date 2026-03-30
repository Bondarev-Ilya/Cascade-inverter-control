#include "data_transmit.h"

#define GPIO_MODER_ALTARNATE 2

static uint32_t counter = 0;
static uint8_t flag_array = 0;
uint8_t index_array = HEADER;
uint8_t index_test = 0;

float test[NUMBER_OF_POINTS] = {0};
uint8_t angel[LENGTH_ARR] = {0};
uint8_t demon[LENGTH_ARR] = {0};
	
void transmit_init(void){
	package_init();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	GPIOB->MODER |= GPIO_MODER_ALTARNATE << GPIO_MODER_MODE6_Pos;
	GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_0 | GPIO_AFRL_AFRL6_1 | GPIO_AFRL_AFRL6_2;
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR3 |= USART_CR3_DMAT;
	// 100000000 / (115200 * 8 * 2) = 54.25347 (0.25347 * 16 = 4)
	USART1->BRR |= (54 << USART_BRR_DIV_Mantissa_Pos) | (4 << USART_BRR_DIV_Fraction_Pos);
	USART1->CR1 |= USART_CR1_UE;
	
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
	DMA2_Stream7->CR |= (4 << DMA_SxCR_CHSEL_Pos) | (1 << DMA_SxCR_DIR_Pos) | (1 << DMA_SxCR_MINC_Pos);
	DMA2_Stream7->PAR = (uint32_t)&USART1->DR;
}

void package_init(void){
	angel[0] = FIRST_HEAD;
	angel[1] = SECOND_HEAD;
	angel[2] = THIRD_HEAD;
	angel[LENGTH_ARR-1] = FIRST_TAIL;
	
	demon[0] = FIRST_HEAD;
	demon[1] = SECOND_HEAD;
	demon[2] = THIRD_HEAD;
	demon[LENGTH_ARR-1] = FIRST_TAIL;
}

void data_transmit(void){
	end_put_on();
	search_points();
	counter++;
}

void end_put_on(void){
	if(counter == NUMBER_OF_MEASUREMENTS){
		dma_road_select();
		counter = 0;
		index_array = HEADER;
		index_test = 0;
		flag_array ^= 1;
	}
}

void dma_road_select(void){
	if(flag_array == 0){
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		DMA2_Stream7->M0AR = (uint32_t)&angel;
		DMA2_Stream7->NDTR = LENGTH_ARR;
		DMA2->HIFCR = DMA_HIFCR_CTCIF7;
		DMA2_Stream7->CR |= DMA_SxCR_EN;
	}
	else{
		DMA2_Stream7->CR &= ~DMA_SxCR_EN;
		DMA2_Stream7->M0AR = (uint32_t)&demon;
		DMA2_Stream7->NDTR = LENGTH_ARR;
		DMA2->HIFCR = DMA_HIFCR_CTCIF7;
		DMA2_Stream7->CR |= DMA_SxCR_EN;
	}
}

void search_points(void){
	if(counter%(NUMBER_OF_MEASUREMENTS/NUMBER_OF_POINTS) == 0){
		if(flag_array == 0){
			put_on_array(angel);
			test[index_test] = Voltage_Set;
		}
		else{
			put_on_array(demon);
			test[index_test] = Voltage_Set;			
		}
		index_test++;
	}
}

void put_on_array(uint8_t* tmp_arr){
	uint32_t tmp;
	tmp = *(uint32_t*)&Current_norm.alfa;
	//tmp = *(uint32_t*)&Voltage_Set;
	//tmp = *(uint32_t*)&P_loc;
	for(uint8_t i = 0; i<4; i++){
		uint8_t val;
		val = (tmp & (0xFF<<(8*i)))>>(8*i);
		tmp_arr[index_array+i] = val;
	}
	index_array += 4;
}
