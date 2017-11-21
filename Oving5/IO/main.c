//gcc main.c io.c -lcomedi -pthread -lrt -lm -D_GNU_SOURCE -o test

#include "io.h"
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);

}

void* task(void* param){
	set_cpu(1);

	int i = *(int*)param;
	printf("task %d \n", i);
	while(1){
		while(io_read(i));
		
		io_write(i,1);
		usleep(10);
		io_write(i,0);
	}

}

void* disturbance(){
	set_cpu(1);
	int i = 0;
	while(1){
		i++;
	}
}


int main(){
	io_init();
	int i;
	pthread_t tasks[3]
    pthread_t disturbances[10];
   
    for(i= 1; i<4; i++){
    	pthread_create(&tasks[i-1], NULL, task, &i);
    }
    for(i= 0; i<10; i++){
    	pthread_create(&disturbances[i], NULL, disturbance, NULL);
    }
    for(i= 0; i<3; i++){
    	pthread_join(tasks[i], NULL);
    }
    for(i= 0; i<10; i++){
    	pthread_join(disturbances[i], NULL);
    }

}

