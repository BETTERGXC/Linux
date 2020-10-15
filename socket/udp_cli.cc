#include<iostream>
#include<string>
#include"UDPsocket.hpp"
using namespace std;
#define CHECKRET(q) if((q) == false){return -1;}

int main(int argc, char* argv[]){
  // 客户端获取的IP地址是服务端绑定的，也就是客户端发送的目标地址
  if(argc != 3){
    cout << "Usage: ./UDPcli ip port" << endl;
    return -1;

  }
  string srv_ip = argv[1];
  uint16_t srv_port = stoi(argv[2]);

  UDPsocket clisock;
  // 1.创建套接字
  CHECKRET(clisock.Socket());
  // 2.为套接字绑定地址信息
  while(1){

    // 3.发送数据
    cout << "client say:";
    string buf;
    cin >> buf;
    CHECKRET(clisock.Send(buf, srv_ip, srv_port));
    // 4.接收数据
    buf.clear();
    CHECKRET(clisock.Recv(&buf));
    cout << "server say: " << buf << endl;

  }


  // 5.关闭套接字
  clisock.Close();
  return 0;
}

