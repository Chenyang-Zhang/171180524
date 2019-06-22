#include <stdio.h>          //编译时候要加-lrt
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
 
void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
 
int main(int argc, char *argv[]) {
  int r;
 
  const char *memname = "sample"; //有名字的共享内存，名字为sample
  const size_t region_size = sysconf(_SC_PAGE_SIZE);//共享内存大小
 
  int fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, 0666);//读写方式打开，若不存在则创建
  if (fd == -1)
    error_and_die("shm_open");
 
  r = ftruncate(fd, region_size);//对文件大小做截断操作
  if (r != 0)//返回值不为0就出错了
    error_and_die("ftruncate");
 
  void *ptr = mmap(0, region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//映射到内存得到一个指针ptr，指针指向了这片共享内存
  if (ptr == MAP_FAILED)
    error_and_die("mmap");
  close(fd);
 
  pid_t pid = fork();//创建新进程
 
  if (pid == 0) {   //在子进程对共享内存写入数据
    u_long *d = (u_long *) ptr;
    *d = 0xdbeebee;
    exit(0);
  }
  else {           //等待子进程结束后，打印共享内存数据
    int status;
    waitpid(pid, &status, 0);
    printf("child wrote %#lx\n", *(u_long *) ptr);
  }
 
  r = munmap(ptr, region_size);  //取消映射，释放内存
  if (r != 0)
    error_and_die("munmap");
 
  r = shm_unlink(memname);   //
  if (r != 0)
    error_and_die("shm_unlink");
 
  return 0;
}
