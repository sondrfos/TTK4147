#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int a = 0;


int main(){
	
	int b = 0;
	int i =0;
	for(i = 0;i<10;i++){
		b++;
	}
	vfork();

	for(i = 0;i<20;i++){
		a++;
	}

	printf("%i\n",a );
	printf("%i\n",b );

	return 0;

	// vfork lager child som blokker parent. Derfor er a allerede blitt diiiiger når parent igjen får kjøre.
}