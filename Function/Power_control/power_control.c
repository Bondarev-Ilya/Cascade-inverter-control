#include "power_control.h"

static float Voltage_Amp = 0.0f;
static float P_loc = 0.0f;
static float Q_loc = 0.0f;
static float P = 0.0f;
static float Q = 0.0f;
static float S = 0.0f;
static float Voltage_Set = 0.0f;
static float P_Error = 0.0f;
static float P_Int = 0.0f;
static float Q_Error = 0.0f;
static float Q_Int = 0.0f;

float P_ref = 200.0f;
float Q_ref = 10.0f;
float Kp = 1.0f;
float Ki = 1.0f;

void Power_Control(void){
	Voltage_Amp = (Voltage.alfa*Voltage.alfa)+(Voltage.beta*Voltage.beta);
	if(Voltage_Amp < 1) Voltage_Amp = 1;
	S = sqrt((P*P)+(Q*Q));
	if(S < 1) S = 1;
	Voltage_Set = (Voltage.alfa*(P/S) + Voltage.alfa*(Q/S))*(S/Voltage_Amp);
	P_loc = (Voltage.alfa*Current.alfa+Voltage.beta*Current.beta);
	Q_loc = (Voltage.beta*Current.alfa-Voltage.alfa*Current.beta);
	
	P_Error = P_ref-P_loc;
	P_Int += Ki*P_Error*SAMPLING_STEP;
	P = P_Int + Kp*P_Error;
	
	Q_Error = Q_ref-Q_loc;
	Q_Int += Ki*Q_Error*SAMPLING_STEP;
	Q = Q_Int + Kp*Q_Error;
}
