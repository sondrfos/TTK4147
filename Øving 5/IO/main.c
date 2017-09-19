//gcc main.c io.c -lcomedi -pthread -lrt -lm -o test

#include "io.h"
#include <stdlib.h>

void task(int i){

	while(1){
		while(io_read(i));
		
		io_write(i,0);
		nanosleep(1000);
		io_write(i,1);
	}
}


int main(){
	io_init();

	pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    printf("I'M GONNA DO STUFF!\n");
    
    pthread_create(&t1, NULL, task, 1);
    pthread_create(&t3, NULL, task, 2);
    pthread_create(&t2, NULL, task, 3);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

}

