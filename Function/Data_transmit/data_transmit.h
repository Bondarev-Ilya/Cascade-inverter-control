#ifndef TRANSMIT_H_
#define	TRANSMIT_H_

#include <stm32f411xe.h>
#include "measure.h"
#include "power_control.h"

#define NUMBER_OF_MEASUREMENTS 200 // point (50Hz/10kHz) !8bit!
#define NUMBER_OF_POINTS 20

void transmit_init(void);
void my_fun(void);

extern float angel[NUMBER_OF_POINTS];
extern float demon[NUMBER_OF_POINTS];

#endif