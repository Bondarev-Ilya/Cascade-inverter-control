#include "main.h"

int main(void){
	Freq_up_to_100_MHz();
	measure_init();
	pwm_init();
	GPIOC->BSRR = GPIO_BRR_BR13;
	while(1==1){
		
	}
}
