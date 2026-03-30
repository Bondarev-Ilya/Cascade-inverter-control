#include "power_control.h"

float P_loc = 0.0f;
float Q_loc = 0.0f;
static float P = 0.0f;
static float Q = 0.0f;
static float S = 0.0f;
static float P_Error = 0.0f;
static float P_Int = 0.0f;
static float Q_Error = 0.0f;
static float Q_Int = 0.0f;

float Voltage_Amp = 0.0f;
float Voltage_Set = 0.0f;
float Current_Set = 0.0f;
float P_ref = 0.5f;
float Q_ref = 0.0f;
float Kp = 1.0f;
float Ki = 0.5f;
 
void Power_Control(void){
	
	Voltage_Amp = ((Voltage_norm.alfa*Voltage_norm.alfa)+(Voltage_norm.beta*Voltage_norm.beta));
	if(Voltage_Amp < 1) Voltage_Amp = 1;
	S = sqrt((P*P)+(Q*Q));
	if(S < 1) S = 1;
	
  Current_Set = (Voltage_norm.alfa*P + Voltage_norm.beta*Q)/Voltage_Amp;
	Voltage_Set = 1.0f*Current_Set + Voltage_norm.alfa;
		
	if(Voltage_Set > 0.0f){
		TIM1->CCR2 = 0;
		TIM1->CCR1 = (uint32_t)(TIM1->ARR * Voltage_Set/SECONDARY_WINDING);
	}
	else{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = (uint32_t)(TIM1->ARR * (-1.0f*Voltage_Set/SECONDARY_WINDING));
	}
	
	P_loc = (Voltage_norm.alfa*Current_norm.alfa+Voltage_norm.beta*Current_norm.beta);
	Q_loc = (Voltage_norm.beta*Current_norm.alfa-Voltage_norm.alfa*Current_norm.beta);
	
	P_Error = P_ref-P_loc;
	P_Int += Ki*P_Error*SAMPLING_STEP;
	P = P_Int + Kp*P_Error;
	
	Q_Error = Q_ref-Q_loc;
	Q_Int += Ki*Q_Error*SAMPLING_STEP;
	Q = Q_Int + Kp*Q_Error;
	
	data_transmit();
}
