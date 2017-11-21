//gcc main.c io.c -lcomedi -pthread -lrt -lm -D_GNU_SOURCE -o test

#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <native/task.h>
#include <native/timer.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#define T_CLOCK 2670 // tick/µs

int set_cpu(int cpu_number){
	// setting cpu set to the selected cpu
	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_number, &cpu);
	// set cpu set to current thread and return
	return pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t),&cpu);

}

void* task(void* param){
	//set_cpu(1);

	int i = *(int*)param;
	rt_printf("task %d \n", i);
	while(1){
		while(io_read(i));
		
		io_write(i,1);
		usleep(10);
		io_write(i,0);
		rt_task_wait_period(NULL);
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
	mlockall(MCL_CURRENT|MCL_FUTURE);
	io_init();
	rt_print_auto_init(1);
	int i;
	int integer [3] = {1, 2, 3};
	RT_TASK tasks[3];
    pthread_t disturbances[10];
   
    for(i= 0; i<3; i++){
    	rt_task_create(&tasks[i], "task "+i, 0, 5, T_JOINABLE | T_CPU(1));
    	rt_task_set_periodic(&tasks[i], TM_NOW, 10000);
    	rt_task_start(&tasks[i], task, &integer[i]);
    }
    for(i= 0; i<10; i++){
    	pthread_create(&disturbances[i], NULL, disturbance, NULL);
    }
    for(i= 0; i<3; i++){
    	rt_task_join(&tasks[i]);
    }
    for(i= 0; i<10; i++){
    	pthread_join(disturbances[i], NULL);
    }
    return 0;
}