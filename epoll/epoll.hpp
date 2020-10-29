#include <cstdio>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>
#include "TCPsocket.hpp"
class Epoll {
  public:
    Epoll(){
      // epoll_creat(size) size已经被忽略，大于 0 即可
      _epfd = epoll_create(1);
      if(_epfd < 0) {
        perror("epoll creat error");
        _exit(0);
      }
    }

    bool Add(TcpSocket &sock) {
      int fd = sock.GetFd();
      // 定义一个描述符对应的事件结构体 struct epoll_event
      struct epoll_event ev;
      // fd描述符就绪后，返回这个结构体中的data.fd就是我们要操作的描述符
      ev.data.fd = fd;
      // 表示监控可读事件 EPOLLIN   可写事件 EPOLLOUT
      ev.events = EPOLLIN;
      // epoll_ctl(epoll句柄， 操作类型， 监控的描述符， 描述符对应事件结构)
      int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
      if(ret < 0) {
        perror("epoll_ctl_add error");
        return false;
      }
      return true;
    }

    bool Del(TcpSocket &sock) {
      int fd = sock.GetFd();
      int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
      if(ret < 0) {
        perror("epoll_ctl_del error");
        return false;
      }
      return true;
    }

    bool wait(std::vector<TcpSocket> *list, int timeout = 3000) {
      // epoll_wait(句柄，接受就绪结构体的数组，数组节点数量，超时时间);
      // 返回值大于 0就表示就绪的个数
      struct epoll_event evs[10];
      int ret = epoll_wait(_epfd, evs, 10, timeout);
      if(ret < 0) {
        perror("epoll_wait error");
      }
      else if(ret == 0) {
        printf("epoll timeout\n");
        return false;
      }
      for(int i = 0; i < ret; i++) {
        // 判断是否就绪的是可读事件
        if(evs[i].events & EPOLLIN) {
          TcpSocket sock;
          sock.SetFd(evs[i].data.fd);
          list->push_back(sock);
        }
      }
      return true;
    }

  private:
    int _epfd;
};
