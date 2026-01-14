#include "main.h"

int main(void){
	Freq_up_to_100_MHz();
	measure_init();
	GPIOC->ODR &= ~GPIO_ODR_OD13;
	while(1==1){
		
	}
}
