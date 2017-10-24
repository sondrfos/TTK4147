#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#include "miniproject.h"
#include "controller.h"

#define IP "192.168.0.1"
#define PORT 9999 

pthread_mutex_t yMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t synchMut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t synch = PTHREAD_COND_INITIALIZER;
struct udp_conn	udp;
double y;
int fresh = 0;





void startSim(){
	udp_send(&udp, "START", 5);
}

void stopSim(){
	udp_send(&udp,"STOP",4);
}

void* getY(){
	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);

	while(1){
		
		udp_send(&udp, "GET", 5);
		char msg [15];
		int status = udp_receive(&udp, &msg, 15);
		char y_val [7]; 
		sscanf(msg, "GET_ACK:%s\n", &y_val);
		pthread_mutex_lock(&yMut);
		y = atof(y_val);
		pthread_mutex_unlock(&yMut);

		pthread_cond_broadcast(&synch);
		timespec_add_us(&waitTime, 1000);
		clock_nanosleep(&waitTime);


	}
}

void* setU(){
	while(1){
		pthread_mutex_lock(&synchMut);
		pthread_cond_wait(&synch, &synchMut);
		pthread_mutex_unlock(&synchMut);

		pthread_mutex_lock(&yMut);
		double u = PI_controller(y);
		if (u>10){
			u=10;
		} else if(u<-10){
			u = -10;
		}
		pthread_mutex_unlock(&yMut);

		char umsg[4 + sizeof(u)];
		sprintf(umsg,"SET:%0.3f", u);
		printf(umsg);
		printf("\n");

		udp_send(&udp,umsg,sizeof(umsg));
	}

	
}

int main(){

	struct timespec waitTime;
	struct timespec endTime;
	pthread_t requestY;
    pthread_t computeU;

	if(udp_init_client(&udp, PORT, IP) == -1){
		return -1;
	}
	pthread_create(&requestY, NULL, getY, NULL);
    pthread_create(&computeU, NULL, setU, NULL);

	startSim(&udp);

	while(1){
		
	}
	stopSim(&udp);

	udp_close(&udp);



	return 0; //Too easy!
}







	/*

	while(1){
		printf("Loop\n");
		
	}
	return 0;*/