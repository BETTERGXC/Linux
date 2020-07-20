#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
  pid_t pid = fork();
  if(pid == 0) {
    sleep(5);
    exit(0);
  }
  int ret = waitpid(-1,NULL,WNOHANG);
  while(1) {
    printf("我在打游戏，不要烦我,%d\n",ret);
    sleep(2);
  }
  return 0;
}
