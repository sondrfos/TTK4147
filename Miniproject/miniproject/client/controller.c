#include <stdio.h>
#include <stdlib.h>
#include "controller.h"


double PID_controller(double y){
	uint16_t 	K_p 		= 5;
	uint16_t 	K_i 		= 800;
	uint16_t	K_d 		= 0.025;
	static double integral 	= 0;
	static double e_old		= 0;
	
	double e = R - y; 
	integral = integral + e*P;
	double derivat = (e-e_old)/P;
	e_old = e;
	
	return K_p * e + K_d * derivat + K_i * integral;
}