#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLINE 80

int main(void)
{
	int n;
	int fd[2];
        int abc=10;
	pid_t pid;
	char line[MAXLINE];

	if (pipe(fd) < 0) {     //pipe open fd[0].fd[1] 
		perror("pipe");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	if (pid > 0) { /* parent */
		close(fd[0]);       //父进程向管道写入字符串
		write(fd[1], "hello world\n", 12);
		abc = 100;    //两个进程无法直接通信
		wait(NULL);
	} else {       /* child */
		close(fd[1]);                   //父进程close不影响子进程
		n = read(fd[0], line, MAXLINE);//子进程从管道读取数据
		write(STDOUT_FILENO, line, n);
                printf("abc=%d\n", abc);
	}
	return 0;
}
