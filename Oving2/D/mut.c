#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

long long int running = 1;
int var1 = 0;
int var2 = 0;
pthread_mutex_t mut;



void thread1(){
	while(running){
		pthread_mutex_lock(&mut);
		var1++;
		var2 = var1;
		pthread_mutex_unlock(&mut);
	}
}

void thread2(){
	int i;
	for(i = 0; i<20; i++){
		pthread_mutex_lock(&mut);
		printf("Number 1 is %i, number 2 is %i \n", var1, var2);
		printf("%i\n",(var1-var2) );
		pthread_mutex_unlock(&mut);
		sleep(1);
	}
	running = 0;
}
	


int main(){
	pthread_mutex_init(&mut,NULL);
	pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}