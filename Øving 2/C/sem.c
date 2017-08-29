#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sema;

void do_stuff(char a){
	sem_wait(&sema);

	printf("Thread number: %i doing the first thing\n", a);
	sleep(1);
	printf("Thread number: %i doing the second thing\n", a);
	sleep(1);

	sem_post(&sema);
}
	


int main(){

	sem_init(&sema, 0, 3);

	pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;

    pthread_create(&t1, NULL, do_stuff, 1);
    pthread_create(&t2, NULL, do_stuff, 2);
    pthread_create(&t3, NULL, do_stuff, 3);
    pthread_create(&t4, NULL, do_stuff, 4);
    pthread_create(&t5, NULL, do_stuff, 5);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    



    return 0;
}