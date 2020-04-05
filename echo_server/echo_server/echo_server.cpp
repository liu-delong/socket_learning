#include <iostream>
#include<WinSock2.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
int main()
{
    WSADATA wsadata;
    int error=WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (error != 0)
    {
        cout << "依赖库初始化失败，退出程序" << endl;
        return 0;
    }
    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    {
        cout << "版本不对，退出程序" << endl;
        WSACleanup();
        return 0;
    }
    SOCKADDR_IN server_data;
    server_data.sin_family = AF_INET;
    server_data.sin_addr.S_un.S_addr = htons(INADDR_ANY);
    server_data.sin_port = htons(6000);
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(server_socket, (SOCKADDR*)&server_data, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        cout << "绑定端口失败，退出程序" << endl;
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    if (listen(server_socket, 5) < 0)
    {
        cout << "设置监听模式失败，退出程序" << endl;
        closesocket(server_socket);
        WSACleanup();
        return 0;
    }
    SOCKADDR_IN client_data;
    SOCKET client_socket;
    int length = sizeof(SOCKADDR);
    for(int i = 0; i < 5; i++)
    {
        client_socket=accept(server_socket, (SOCKADDR*)&client_data,&length);
        char receivebuf[1000];
        while (1)
        {
            cout << "等待" << i + 1 << "号客户端发来消息" << endl;
            int receivelen = recv(client_socket, receivebuf, 1000, 0);
            if (receivelen < 0)
            {
                cout << i + 1 << "号客户端断开连接" << endl;
                break;
            }
            cout << i + 1 << "号客户端发来消息:" << "“" << receivebuf << "”" << endl;
            if (strcmp(receivebuf, "Q") == 0)
            {
                cout << "断开" << i + 1 << "号客户端的连接" << endl;
                break;
            }
            int sendlen = send(client_socket, receivebuf, 1000, 0);
            if (sendlen < 0)
            {
                cout << i + 1 << "号客户端断开连接" << endl;
                break;
            }
            cout << "已经回复" << i + 1 << "号客户端" << endl;
        }
        closesocket(client_socket);

    }
    cout << "已经和5个客户端进行逐一通讯，关闭客户端" << endl;
    closesocket(server_socket);
    WSACleanup();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
