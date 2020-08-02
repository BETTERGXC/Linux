#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
int main() {
  // 匿名管道只能用于具有亲缘关系的进程间通信
  // 注意：创建管道一定要放在创建子进程之前
  int pipefd[2] = {-1};
  int ret = pipe(pipefd);
  if(ret < 0) {
    perror("pipe error");
    return -1;
  }
  pid_t pid = fork();
  if(pid == 0) {
    // 子进程从管道中读取数据
    while(1) {
    char buf[1024] = {0};
    read(pipefd[0],buf,1023);
    printf("child:buf[%s]\n",buf);
    sleep(1);
    }
  }else if (pid > 0) {
    // 父进程向管道写入数据
    while(1) {
    char *ptr = "这两天好热~";
    write(pipefd[1],ptr,strlen(ptr));
    sleep(1);
    }
  }
  return 0;
}
