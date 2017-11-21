#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t mut0;
pthread_mutex_t mut1;
pthread_mutex_t mut2;
pthread_mutex_t mut3;
pthread_mutex_t mut4;

void eat(int k){
	printf("I'm havin a snack! \nI'm pil #%i \n",k);
}

void phil0(){
	while(1){
		if (pthread_mutex_trylock(&mut4) && pthread_mutex_trylock(&mut0)){
			eat(0);
			pthread_mutex_unlock(&mut0);
			pthread_mutex_unlock(&mut4);
		}
		sleep(1);
	}
}

void phil1(){
	while(1){
		if (pthread_mutex_trylock(&mut0) && pthread_mutex_trylock(&mut1)){
			eat(1);
			pthread_mutex_unlock(&mut1);
			pthread_mutex_unlock(&mut0);
		}
		sleep(1);
	}
}

void phil2(){
	while(1){
		if(pthread_mutex_trylock(&mut1) && pthread_mutex_trylock(&mut2)){
			eat(2);
			pthread_mutex_unlock(&mut2);
			pthread_mutex_unlock(&mut1);
		}
		sleep(1);
	}
}

void phil3(){
	while(1){
		if(pthread_mutex_trylock(&mut2) && pthread_mutex_trylock(&mut3)){
			eat(3);
			pthread_mutex_unlock(&mut2);
			pthread_mutex_unlock(&mut3);
		}
		sleep(1);
	}
}

void phil4(){
	while(1){
		if(pthread_mutex_trylock(&mut3) && pthread_mutex_trylock(&mut4)){
			eat(4);
			pthread_mutex_unlock(&mut3);
			pthread_mutex_unlock(&mut4);
		}
		sleep(1);
	}
}


int main(){
	pthread_mutex_init(&mut0,NULL);
	pthread_mutex_init(&mut1,NULL);
	pthread_mutex_init(&mut2,NULL);
	pthread_mutex_init(&mut3,NULL);
	pthread_mutex_init(&mut4,NULL);

	pthread_t t0;
	pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    
	pthread_create(&t0, NULL, phil0, NULL);
    pthread_create(&t1, NULL, phil1, NULL);
    pthread_create(&t2, NULL, phil2, NULL);
    pthread_create(&t3, NULL, phil3, NULL);
    pthread_create(&t4, NULL, phil4, NULL);
    

    pthread_join(t0, NULL);
	pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    
    return 0;
}