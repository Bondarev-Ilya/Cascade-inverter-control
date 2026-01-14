#ifndef CONTROL_H_
#define	CONTROL_H_

#include <stm32f411xe.h>
#include "measure.h"
#include "math.h"

void Power_Control(void);

extern float P_ref;
extern float Q_ref;
extern float Kp;
extern float Ki;

#endif