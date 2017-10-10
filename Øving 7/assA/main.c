#include <stdio.h>
#include <stdlib.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <rtdk.h>

RT_SEM sem;

void waitTask(void* param){
	int i = *(int*)param;

	rt_printf( "Task %d is waiting \n", i);
	rt_sem_p(&sem, TM_INFINITE);
	rt_printf( "Task %d is finished \n", i);


}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);
	rt_sem_create(&sem, NULL, 0, S_PRIO);
	rt_task_shadow(NULL, "main", 3, T_JOINABLE | T_CPU(1));

	RT_TASK tasks[2];
	int i;
	int integer[2] = {1, 2};
	for( i= 0; i < 2; i++){
		rt_task_create(&tasks[i], "task "+i, 0, 1, T_JOINABLE | T_CPU(1));
	    rt_task_start(&tasks[i], waitTask, &integer[i]);
	}

	usleep(100000);
	rt_sem_broadcast(&sem);
	usleep(100000);

	rt_sem_delete(&sem);
	return 1;

}