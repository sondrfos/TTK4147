#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <sys/dispatch.h>
#include <sys/mman.h>

struct pid_data{
	pthread_mutex_t pid_mutex;
	pid_t pid;
};

void* create_file(){
	int dis = shm_open("/sharedpid", O_RDWR | O_CREAT, S_IRWXU);
	ftruncate(dis, sizeof(struct pid_data));

	return mmap(0, sizeof(struct pid_data), PROT_READ | PROT_WRITE, MAP_SHARED, dis, 0);
}

void store_pid() {
	printf("You're a wizzard Harry\n");
	struct pid_data* ptr = create_file();

	pthread_mutex_t pid_mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutexattr_t myattr;
	pthread_mutexattr_init(&myattr);
	pthread_mutexattr_setpshared(&myattr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&ptr->pid_mutex, &myattr);

	struct pid_data data = {pid_mutex, getpid()};

	pthread_mutex_lock(&pid_mutex);
	*ptr = data;
	pthread_mutex_unlock(&pid_mutex);
}
