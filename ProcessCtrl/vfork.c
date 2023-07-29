#include "stdio.h"
#include "unistd.h"
#include <sys/types.h>
int gvar = 2;

int main() {
	pid_t pid;
	int var = 5;
	printf("process id: %ld\n", (long)getpid());
	printf("gvar = %d var = %d\n", gvar, var);

	if ((pid = vfork()) < 0) {
		printf("error");
		return 0;
	} else if (pid == 0) {
		gvar--;
		var++;
		printf("the child process id: %ld\ngvar = %d var = %d\n", (long)getpid(), gvar, var);
		_exit(0);
	} else {
		printf("the parent process id: %ld\ngvar = %d var = %d\n", (long)getpid(), gvar, var);
		return 0;
	}
}
