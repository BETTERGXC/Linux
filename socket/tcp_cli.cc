#include <iostream>
#include "TCPsocket.hpp"
 // 流程
 // 创建套接字
 // 绑定地址信息（不推荐）
 // 向服务端发起连接请求
 // 收发数据
 // 关闭套接字
 
int main(int argc, char *argv[]) {
  if(argc != 3) {
    std::cout << "Usage: ./tcp_cli ip port" << std::endl;
    return -1;
  }
  std::string srv_ip = argv[1];
  uint16_t srv_port = std::stoi(argv[2]);

  TcpSocket sock;
  CHECK_RET(sock.Socket());
  CHECK_RET(sock.Connect(srv_ip, srv_port));
  while(1) {
    std::string buf;
    std::cout << "client say:";
    std::cin >> buf;
    sock.Send(buf);

    buf.clear();
    sock.Recv(&buf);
    std::cout << "server say:" << buf << std::endl;
  }
  sock.Close();
  return 0;
}
