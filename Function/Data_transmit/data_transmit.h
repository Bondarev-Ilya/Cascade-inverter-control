#ifndef TRANSMIT_H_
#define	TRANSMIT_H_

#include <stm32f411xe.h>
#include "measure.h"
#include "power_control.h"


#define NUMBER_OF_MEASUREMENTS 400 // 200 point (50Hz/10kHz) !32bit!
#define NUMBER_OF_POINTS 50

#define HEADER 3
#define TAIL 1
#define SIZE_FLOAT 4
#define LENGTH_ARR HEADER+(NUMBER_OF_POINTS*SIZE_FLOAT)+TAIL

#define FIRST_HEAD 0x8F // 143
#define SECOND_HEAD 0x0D //13
#define THIRD_HEAD	0x1F //31
#define FIRST_TAIL 0xFB // 251

void transmit_init(void);
void package_init(void);
void data_transmit(void);
void search_points(void);
void end_put_on(void);
void put_on_array(uint8_t* tmp_arr);
void dma_road_select(void);

extern uint8_t angel[LENGTH_ARR];
extern uint8_t demon[LENGTH_ARR];

#endif