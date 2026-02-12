#include "data_transmit.h"

uint8_t counter = 0;
uint8_t index_array = 0;
uint8_t flag_array = 0;

float angel[NUMBER_OF_POINTS] = {0};
float demon[NUMBER_OF_POINTS] = {0};

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
