//gcc main.c io.c -lcomedi -pthread -lrt -lm -D_GNU_SOURCE -o test

#include "io.h"
#include <stdlib.h>
#include <sched.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void timespec_add_us(struct timespec *t, long us)
{
	// add microseconds to timespecs nanosecond counter
	t->tv_nsec += us*1000;

	// if wrapping nanosecond counter, increment second counter
	if (t->tv_nsec > 1000000000) {
	t->tv_nsec = t->tv_nsec-1000000000;
	t->tv_sec += 1;
	}
}

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
	struct timespec next;
	int i = *(int*)param;
	printf("task %d \n", i);
	while(1){
		while(io_read(i)){
			clock_gettime(CLOCK_REALTIME, &next);
			timespec_add_us(&next, 100);
			clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
		}
		
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
	int integers [3] = {1, 2, 3};
	pthread_t tasks[3];
    pthread_t disturbances[10];
   
    for(i= 0; i<3; i++){
    	printf("%d", i);
    	pthread_create(&tasks[i], NULL, task, &integers[i]);
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



/*pthread_t task;
	pthread_create(&task, NULL, task2, NULL);
	pthread_join(task, NULL);

void* task2(){
	struct timespec next;
	while(1){
		printf("500ms \n");
		clock_gettime(CLOCK_REALTIME, &next);
		timespec_add_us(&next, 500000);
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &next, NULL);
	}
}*/