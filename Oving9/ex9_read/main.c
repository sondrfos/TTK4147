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

int main(int argc, char *argv[]) {
	int dis = shm_open("/sharedpid", O_RDWR, S_IRWXU);

	struct pid_data* ptr = mmap(0, sizeof(struct pid_data), PROT_READ | PROT_WRITE, MAP_SHARED, dis, 0);

	pthread_mutex_lock(&(ptr->pid_mutex));
	printf("%d", ptr->pid);
	pthread_mutex_unlock(&(ptr->pid_mutex));

	return EXIT_SUCCESS;
}
