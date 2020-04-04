#include <iostream>
#include<winsock.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

int main()
{
    char sendbuf[100]; //发送数据缓冲区
    char receivebuf[100]; //接受数据缓冲区
    int sendlen; //发送数据的长度
    int receivelen; //接受数据的长度。
    int length;   //SOCKADDR的大小
    SOCKET socket_server; //服务器套接字
    SOCKET socket_receive; //客户端套接字
    SOCKADDR_IN server_add; //服务器的地址信息
    SOCKADDR_IN receive_add; //客户端的地址信息
    //下面开始初始化套接字库
    WORD version;
    WSADATA wsadata;  //库版本信息结构
    //下面开始初始化套接字
    version = MAKEWORD(2, 2);
    int error = WSAStartup(version, &wsadata);
    if (error != 0)
    {
        cout << "套接字加载失败！";
        return 0;
    }
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    {
        cout << "版本错误！";
        WSACleanup();//关闭套接字
        return 0;
    }
    //下面开始设置连接地址。
    server_add.sin_family = AF_INET;//地址家族，必须是AF_INET,注意只有它不是网络字节顺序。
    server_add.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//主机地址
    server_add.sin_port = htons(5000);

    socket_server = socket(AF_INET, SOCK_STREAM, 0); //创建套接字
    if (bind(socket_server, (SOCKADDR*)&server_add, sizeof(SOCKADDR)) == SOCKET_ERROR)//绑定套接字到服务器端口上
    {
        cout << "绑定失败" << endl;
    }
    if (listen(socket_server, 5) < 0)//设置监听模式
    {
        cout << "设置监听失败" << endl;
    }
    //下面接受客户端的链接请求
    length = sizeof(SOCKADDR);//因为下面的函数传参要求要传入一个指针。
    cout << "等待连接中" << endl;
    socket_receive = accept(socket_server, (SOCKADDR*)&receive_add, &length/*这里&是取地址，而不是引用*/);
    if (socket_receive == SOCKET_ERROR)
    {
        cout << "接受连接失败" << endl;
        closesocket(socket_receive);
        closesocket(socket_server);
        WSACleanup();
        return 0;
    }
    cout << "连接成功！开始聊天" << endl;
    while (1)
    {
        receivelen = recv(socket_receive, receivebuf, 100, 0);
        if (receivelen < 0)
        {
            cout << "接收数据失败，程序退出！" << endl;
            break;
        }
        else
        {
            cout << "客户端说：" << receivebuf << endl;
        }
        //下面开始说话
        cout << "请输入你要说的话:";
        cin.getline(sendbuf, 100);
        sendlen = send(socket_receive, sendbuf, 100, 0);
        if (sendlen < 0)
        {
            cout << "发送失败！等待接收客户端的来信" << endl;
        }
        cout << "你说：" << sendbuf << endl;
    }
    closesocket(socket_receive);
    closesocket(socket_server);
    WSACleanup();
    return 0;

}
