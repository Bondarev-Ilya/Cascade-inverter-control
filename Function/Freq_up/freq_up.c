#include "freq_up.h"

void Freq_up_to_100_MHz(void){
	// Enable external high-speed clock
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	// disable PLL for setup
	RCC->CR &= ~RCC_CR_PLLON;
	// config PLL timings
	RCC->PLLCFGR = (25UL << RCC_PLLCFGR_PLLM_Pos) | (200UL << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos) | RCC_PLLCFGR_PLLSRC_HSE;
	// enable PLL
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	// make flash read slower
	do{
	FLASH->ACR |= 3;  
	}while((FLASH->ACR & FLASH_ACR_LATENCY) != 3);
	// conf APB1 and APB2 prescalers and set PLL as SYSCLOCK SOURCE
	RCC->CFGR = RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_SW_PLL;
}