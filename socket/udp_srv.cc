#include<iostream>
#include<string>
#include"UDPsocket.hpp"
using namespace std;
#define CHECKRET(q) if((q)==false){return -1;}
int main(int argc, char* argv[]){
  if(argc != 3){
    cout << "Usage:./UDPsrv IP Port" << endl;
    return -1;

  }
  uint16_t port = stoi(argv[2]);
  string ip = argv[1];

  UDPsocket srvsock;
  // 1.创建套接字
  CHECKRET(srvsock.Socket());
  // 2.为套接字绑定地址信息
  CHECKRET(srvsock.Bind(ip, port));
  while(1) {
    // 3.接收数据
    string buf;
    string peer_ip;
    uint16_t peer_port;
    CHECKRET(srvsock.Recv(&buf, &peer_ip, &peer_port));
    cout << "client[" << peer_ip << ":" << peer_port << "]say:" << buf << endl;
    // 4.发送数据
    buf.clear();
    cout << "server say: ";
    cin >> buf;
    CHECKRET(srvsock.Send(buf, peer_ip, peer_port));
  }

  // 5.关闭套接字
  srvsock.Close();
  return 0;

}
