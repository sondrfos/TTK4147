#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int A = 0;

void do_stuff(){
	int b = 0;
	int i = 0;
	for (i = 0;i<1000000;i++){
		b++;
		A++;
	}

	printf("b = %i\n",b );
	printf("A = %i\n",A );
}
	


int main(){
	pthread_t t1;
    pthread_t t2;

    printf("I'M GONNA DO STUFF!\n");
    
    pthread_create(&t2, NULL, do_stuff, NULL);
    pthread_create(&t1, NULL, do_stuff, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    // Ting kjører hulter til bulter
    return 0;
}