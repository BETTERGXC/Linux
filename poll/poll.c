#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main() {
  // 定义描述符的事件结构体数组（这里因为只要监控一个描述符，因此定义了一个结构体）
  struct pollfd poll_fd;
  // 要监控的描述符
  poll_fd.fd = 0;
  // 对于描述符要进行监控的事件  POLLIN--可读
  poll_fd.events = POLLIN;
  for(;;) {
    // 开始监控
    int ret = poll(&poll_fd, 1, 3000);
    // 返回值小于 0 表示出错
    if(ret < 0) {
      perror("poll");
      continue;
    } 
    // 返回值等于 0 表示超时
    if(ret == 0) {
      printf("poll timeout\n");
      continue;
    }
    // revents 不需要手动还原，因为下次监控的时候没有就绪就会自动置为 0
    // 调用返回时，通过每个节点的revents确定哪个描述符就绪了哪个事件
    if(poll_fd.revents == POLLIN) {
      // 可读时间进行读操作
      char buf[1024] = {0};
      read(0, buf, sizeof(buf) - 1);
      printf("stdin:%s",buf);
    }
  }
  return 0;
}
