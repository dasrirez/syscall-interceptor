#include <sys/syscall.h>
#include <sys/types.h>
#include <string.h>
#include "interceptor.h"
int main() {
	pid_t pid;
	int status;
	char    buf[256];
	system("insmod interceptor.ko");
	syscall(MY_CUSTOM_SYSCALL, REQUEST_START_MONITORING, SYS_open, 0);
	syscall(MY_CUSTOM_SYSCALL, REQUEST_SYSCALL_INTERCEPT, SYS_open);
	syscall(MY_CUSTOM_SYSCALL, REQUEST_STOP_MONITORING, SYS_open, getpid());
	pid = fork();
	syscall(SYS_open);
	wait(&status);
	if (pid != 0) {
		//sprintf("Parent [%d] :: Child [%d]", getpid(), pid);
		sprintf(buf, "Parent [%lx]\n", (long unsigned int) getpid());
		write(1, buf, strlen(buf));
		sprintf(buf, "Child [%lx]\n", (long unsigned int) pid);
		write(1, buf, strlen(buf));
		system("rmmod interceptor.ko");
	}
}
