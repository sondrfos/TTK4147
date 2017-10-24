#include <stdio.h>
#include <stdlib.h>
#include "controller.h"


double PI_controller(double y){
	uint16_t 	K_p = 10;
	uint16_t 	K_i = 800;
	static double integral = 0;
	
	double e = R - y; 
	integral = integral + e*P;
	
	return K_p * e + K_i * integral;
}