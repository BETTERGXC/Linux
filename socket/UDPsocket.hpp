#include<cstdio>
#include<string>
#include<sys/socket.h> // 套接字接口信息
#include<netinet/in.h> // 包含地址结构
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;
class UDPsocket {
  public:
    UDPsocket()
      :_sockfd(-1)
    {}
    // 1.创建套接字
    bool Socket() {
      _sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
      if(_sockfd < 0) {
        perror("socket error");
        return false;
      }
      return true;
    }

    // 2.为套接字绑定地址信息
    bool Bind(const string& ip,uint16_t port) {
      // 定义IPV4地址结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      // 将主机字节序短整型转换为网络字节序短整型
      addr.sin_port = htons(port);
      // 将字符串ip转换为网络字节序ip地址
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      // 绑定
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("bind error");
        return false;

      }
      return true;
    }
    // 3.接收数据并且获取发送端的地址信息
    bool Recv(string* buf, string* ip = NULL, uint16_t* port = NULL){
      struct sockaddr_in peer_addr;
      socklen_t len = sizeof(struct sockaddr_in);
      char tmp[4096] = {0};
      int ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&peer_addr, &len);
      if(ret < 0){
        perror("recv error");
        return false;

      }
      // 从tmp中截取ret个字节放到buf中
      buf->assign(tmp, ret);
      if(port != NULL){
        // 网络字节序转为主机字节序
        *port = ntohs(peer_addr.sin_port);
      }
      if(ip != NULL) {
        // 网络字节序到字符串IP地址的转换
        *ip = inet_ntoa(peer_addr.sin_addr);
      }
      return true;
    }

    // 4.发送数据
    bool Send(const string& data, const string& ip, const uint16_t port){
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("send error");
        return false;

      }
      return true;

    }

    // 5.关闭套接字
    bool Close(){
      if(_sockfd > 0){
        close(_sockfd);
        _sockfd = -1;

      }
      return true;

    }
  private:
    int _sockfd;
};
