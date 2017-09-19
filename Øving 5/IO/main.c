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
	printf("set_cpu\n");
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);

}

void* task(void* param){
	set_cpu(1);

	int i = (long) param;
	printf("task \n");
	while(1){
		while(io_read(i));
		
		io_write(i,1);
		nanosleep(10);
		io_write(i,0);
	}

}


int main(){
	io_init();
	printf("yooo\n");
	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
	pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    printf("I'M GONNA DO STUFF!\n");
    
    pthread_create(&t1, NULL, task, &i1);
    pthread_create(&t3, NULL, task, &i2);
    pthread_create(&t2, NULL, task, &i3);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

}

