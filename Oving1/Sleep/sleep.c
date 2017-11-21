#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void printAndWait(){
    printf("This is the start of the thread \n");
    sleep(5);
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
