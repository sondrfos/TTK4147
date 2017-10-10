#include <stdio.h>
#include <stdlib.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <rtdk.h>

RT_SEM 		synchr;
RT_MUTEX 	A;
RT_MUTEX 	B;

#define TU 10

void busy_wait_ms(unsigned long delay){
	unsigned long count = 0;
 	while (count <= delay*10){
 		rt_timer_spin(1000*100);
 		count++;
 	}
}

int rt_task_sleep_ms(unsigned long delay){
 	return rt_task_sleep(1000*1000*delay);
}

void L(){
	rt_sem_p(&synchr, TM_INFINITE);
	rt_mutex_acquire(&A, TM_INFINITE);

	busy_wait_ms(3*TU);

	rt_mutex_acquire(&B, TM_INFINITE);

	busy_wait_ms(3*TU);

	rt_mutex_release(&B);
	rt_mutex_release(&A);

	busy_wait_ms(1*TU);
}


void H(){
	rt_sem_p(&synchr, TM_INFINITE);

	rt_task_sleep_ms(1*TU);
	rt_mutex_acquire(&B);

	busy_wait_ms(1*TU);

	rt_mutex_acquire(&A);

	busy_wait_ms(2*TU);

	rt_mutex_release(&A);
	rt_mutex_release(&B);

	busy_wait_ms(1*TU);

}

int main(){
	mlockall(MCL_CURRENT|MCL_FUTURE);
	rt_print_auto_init(1);

	rt_sem_create(&synchr, NULL, 0, S_PRIO);
	rt_mutex_create(&A, NULL);
	rt_mutex_create(&B, NULL);
	rt_task_shadow(NULL, "main", 3, T_JOINABLE | T_CPU(1));

	RT_TASK tasks[2];

	rt_task_create(&tasks[0], "task 1", 0, 1, T_JOINABLE | T_CPU(1));
	rt_task_create(&tasks[1], "task 2", 0, 2, T_JOINABLE | T_CPU(1));
	rt_task_start(&tasks[0], L, NULL);
	rt_task_start(&tasks[1], H, NULL);



	usleep(100000);
	rt_sem_broadcast(&synchr);
	usleep(100000);


	rt_task_join(&tasks[0]);
	rt_task_join(&tasks[1]);

	rt_mutex_delete(&A);
	rt_mutex_delete(&B);
	rt_sem_delete(&synchr);
	return 1;

}