#include<WinSock.h>
#include <iostream>
#pragma comment (lib,"ws2_32.lib")
using namespace std;
int main()
{
	WSADATA wsadata;
	int error = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (error != 0)
	{
		cout << "依赖库初始化失败，退出程序" << endl;
		return 0;
	}
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		cout << "版本有误，退出程序" << endl;
		WSACleanup();
		return 0;
	}
	SOCKADDR_IN server_data;
	server_data.sin_family = AF_INET;
	server_data.sin_addr.S_un.S_addr = inet_addr("192.168.1.105");
	server_data.sin_port = htons(6000);
	SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == SOCKET_ERROR)
	{
		cout << "套接字建立失败，退出程序" << endl;
		WSACleanup();
		return 0;
	}
	int length = sizeof(SOCKADDR);
	if (connect(client_socket, (SOCKADDR*)&server_data,length) != 0)
	{
		cout << "连接服务器失败，可能原因是服务未打开或者拒绝连接" << endl;
		closesocket(client_socket);
		WSACleanup();
		return 0;
	}
	while (true)
	{
		char sendbuf[1000];
		cout << "请输入你想要说的话(若输入Q,服务器会断开你的连接）：";
		cin.getline(sendbuf, 1000);
		int sendlen = send(client_socket, sendbuf, 1000, 0);
		if (sendlen < 0)
		{
			cout << "失去与服务器的连接" << endl;
			break;
		}
		cout << "等待服务器回信" << endl;
		char receivebuf[1000];
		int receivelen = recv(client_socket, receivebuf, 1000, 0);
		if (receivelen < 0)
		{
			cout << "失去与服务器的连接" << endl;
			break;
		}
		cout << "服务器回声：" << receivebuf<<endl;
	}
	cout << "程序结束，退出";
	closesocket(client_socket);
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
