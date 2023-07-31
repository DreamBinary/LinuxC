#include "stdio.h"
#include <unistd.h>
#include <sys/types.h>
int main() {
	printf("Hello process id: %ld\n", (long)getpid());
	puts("Hello World!");
	return 0;
}


