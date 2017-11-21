#include <stdio.h>
#include <stdlib.h>

void allocate(int value) {
    int* ptr = NULL;
    ptr = malloc(1024*1024*sizeof(int));
    if( ptr == NULL){
        perror("malloc had an error");
        exit(-1);
    }
    *ptr = value;
    printf("test of allocated memory: %i\n", *ptr);
}
