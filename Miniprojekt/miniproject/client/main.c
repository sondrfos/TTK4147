#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "miniproject.h"
#include "controller.h"

#define IP "192.168.0.1"
#define PORT 9999 

pthread_mutex_t yMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t synchContMut = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t synchSignMut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t synchCont = PTHREAD_COND_INITIALIZER;
pthread_cond_t synchSign = PTHREAD_COND_INITIALIZER;
struct udp_conn	udp;
char *yMsg;
int running = 1;
int flag = 0;


void timerCountdown(){
	struct timespec endTime;
	clock_gettime(CLOCK_REALTIME, &endTime);
	timespec_add_us(&endTime, 500000);
	clock_nanosleep(&endTime);
	running = 0;
}

void startSim(){

	udp_send(&udp, "START", 5);
}

void stopSim(){

	udp_send(&udp,"STOP",4);
}

void setU(){
	char y [7]; 
	while(running){
		pthread_mutex_lock(&synchContMut);
		pthread_cond_wait(&synchCont, &synchContMut);
		pthread_mutex_unlock(&synchContMut);

		pthread_mutex_lock(&yMut);
		sscanf(yMsg, "GET_ACK:%s\n", y);
		pthread_mutex_unlock(&yMut);
		
		double yVal = atof(y);
		double u = PID_controller(yVal);
		char umsg[20];
		sprintf(umsg,"SET:%0.5f", u);
		udp_send(&udp,umsg,20);
	}
}

void getY(){
	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);
	while(running){
		udp_send(&udp,"GET",4);
		timespec_add_us(&waitTime, P*1000000);
		clock_nanosleep(&waitTime);
	}
}

void setSignal(){
	while(running){
		pthread_mutex_lock(&synchSignMut);
		pthread_cond_wait(&synchSign, &synchSignMut);
		pthread_mutex_unlock(&synchSignMut);
		udp_send(&udp,"SIGNAL_ACK",11);
	}
}

void listenUDP(){
	struct timespec waitTime;
	clock_gettime(CLOCK_REALTIME, &waitTime);

	while(running){
		char msg [15];
		if (udp_receive(&udp, &msg, 15) <= 0){
			printf("sleep \n");
			timespec_add_us(&waitTime, 10);
			clock_nanosleep(&waitTime);
			break;
		}
		if (msg[0] == 'G'){
			pthread_mutex_lock(&yMut);
			yMsg = msg;
			pthread_mutex_unlock(&yMut);
			pthread_cond_broadcast(&synchCont);
		}
		else if (msg[0] == 'S'){
			pthread_cond_broadcast(&synchSign);
		}
	}
}

int main(){
	pthread_t pollUDP;
    pthread_t computeU;
    pthread_t requestY;
    pthread_t timer;
    pthread_t sendSignal;

	if(udp_init_client(&udp, PORT, IP) == -1){
		return -1;
	}

	pthread_create(&pollUDP, NULL, listenUDP, NULL);
    pthread_create(&computeU, NULL, setU, NULL);
    pthread_create(&sendSignal, NULL, setSignal,NULL);
    pthread_create(&timer, NULL, timerCountdown, NULL);
    pthread_create(&requestY, NULL, getY, NULL);

	startSim(&udp);

	while(running);

	stopSim(&udp);
	udp_close(&udp);
	printf("%d\n", flag);
	return 0; //Too easy!
}




		//saturation
		/*if (u>10){
			u=10;
		} else if(u<-10){
			u = -10;
		}

		Tu = 0.02s
		Ku = 19



		*/
		