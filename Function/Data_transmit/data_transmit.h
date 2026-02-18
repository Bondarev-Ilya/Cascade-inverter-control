#ifndef TRANSMIT_H_
#define	TRANSMIT_H_

#include <stm32f411xe.h>
#include "measure.h"
#include "power_control.h"


#define NUMBER_OF_MEASUREMENTS 200 // point (50Hz/10kHz) !8bit!
#define NUMBER_OF_POINTS 20

#define HEADER 2
#define TAIL 1
#define FIRST_HEAD 10.10f
#define SECOND_HEAD 1.414f
#define FIRST_TAIL 3.141f

#define SIZE_FLOAT 4

void transmit_init(void);
void package_init(void);
void data_transmit(void);

extern float angel[HEADER+NUMBER_OF_POINTS+TAIL];
extern float demon[HEADER+NUMBER_OF_POINTS+TAIL];

#endif