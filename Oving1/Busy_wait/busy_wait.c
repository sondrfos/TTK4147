#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void busy_wait_delay(int seconds){
    int i, dummy;
    int tps = sysconf(_SC_CLK_TCK);
    clock_t start;
    struct tms exec_time;
    times(&exec_time);
    start = exec_time.tms_utime;
    while( (exec_time.tms_utime-start) < (seconds * tps)){
        for(i=0; i<1000; i++){
            dummy = i;
        }
        times(&exec_time);
    }
}


void printAndWait(){
    printf("This is the start of the thread \n");
    busy_wait_delay(5);
    printf("This is the end of the thread \n");
}

int main(){
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, printAndWait, NULL);
    pthread_create(&t2, NULL, printAndWait, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
