#include <iostream>
#include<winsock.h>
#pragma comment (lib,"ws2_32.lib")
using namespace std;

int main()
{
    //下面初始化依赖库
    WSADATA mywsadata;//这是库版本信息结构
    int error = WSAStartup(MAKEWORD(2, 2), &mywsadata);//这个函数用来加载套接字库。需要版本类型（第一个参数）和一个版本信息结构指针。
    //MAKEWORD（）函数用来创建一个word类型的版本类型数据。
    if (error != 0)
    {
        cout << "套接字库加载失败" << endl;
        return 0;
    }
    if (LOBYTE(mywsadata.wVersion) != 2 || HIBYTE(mywsadata.wVersion) != 2)//检查版本
    {
        WSACleanup();//如果版本不对，先关闭套接字库，这个函数可以关闭套接字库。
        return 0;
    }
    //依赖库初始化完毕



    //首先填写服务器的信息。这个数据结构是SOCKADDR_IN，用来保存服务器信息。
    SOCKADDR_IN server_data;
    server_data.sin_family = AF_INET;//sin_family 是地址类型。AF_INET是tcp_ip ipv4的协议族，我这里的程序就是基于这个协议的。
    server_data.sin_addr.S_un.S_addr = inet_addr("192.168.1.105");/*地址信息应被以长整型的形式存储在server_data的sin_addr变量中（其中sin_addr也是一个结构体，里面有一个union联合体变量S_un（什么是联合体？
                                                                  问谭浩强去！）,s_un联合体中有一个32位整型变量S_addr,这个变量就是用来存ip地址的了。inet_addr 函数可以把一个我们平时用的ip地址转换为长整型数。
                                                                  另外，这个结构体的定义者为了方便使用，对S_un.S_addr这个变量做了宏定义，宏定义为s_addr(注意大小写！！！），也就是你说s_addr实际上就是
                                                                  S_un.addr。所以这条代码也可以写为
                                                                  serverdata.sin_addr.s_addr=inet_addr("192.168.1.105")*/

                                                                  /*扩充*/
                                                                  /*补充一个于inet_aton功能相反的函数inet_ntoa(),这个函数把32位整数转化为我们熟悉的ip地址。函数原型为 char * inet_ntoa(struct in_addr adr);*/
                                                                  /*用法如下：*/
                                                                  /*char *temp_ip;
                                                                  temp_ip=inet_ntoa(server_data.sin_addr);
                                                                  char *ip;
                                                                  strcpy(ip,temp_ip)*/
                                                                  /*为什么要用temp_ip呢？因为函数的返回值是在函数内部申请的指针，无论什么时候，多少次调用这个函数，返回的地址都是一样的。当我们第二次调用这个函数时，第一次的数据就会被覆盖
                                                                  为了保存每一次的数据，就要把得到的结果复制到我们自己指定的空间*/

    server_data.sin_port = htons(5000);//服务器对应程序的端口号，是个以网络字节顺序的16位整型数据。我们计算机的整型储存顺序可能不是网络字节顺序，换句话说我们电脑的5000可能不是网络字节顺序下的5000，
    //因此要进行转换，用到的函数就是htons(),(host to net (short))
    /*到这里，服务器的信息已经写好了，至于服务器的信息怎么来，可以通过命令行（cmd)的ipconfig/all 命令查看服务器的ip地址，服务器对应程序的窗口号由写服务器端程序的人自己定的，可以查看服务器端程序的代码获取*/


    //下面开始创建客户端套接字
    SOCKET client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);//socket函数用来根据指定信息创建一个套接字，第一个参数为协议，这里用tcp/ip ipv4协议，代号是AF_INET,第二个参数是socket的类型，socket的类型有三个，这里我们创建流式
    //套接字，也就是SOCK_STREAM,如果第一个和第二个参数都有的话，第三个参数就填0，至于第三个参数是干啥的，还没进行研究。

    /*这里区别于服务器程序的地方是，服务器还需手动绑定端口，因为客户端访问服务器需要知道程序的端口号，服务器的端口必须要由程序员定义，如果程序员不定义，在使用connect函数时，电脑会随机绑定一个端口。
    客户端的端口就不需要程序员知道了，由电脑随机绑定一个端口号就行，向服务器发消息时，根据协议，会连同ip号和端口号一起发送。当于服务器连接成功时，服务器会知道客户端的ip和端口号的。*/

    //下面开始向服务器发送连接请求
    if (connect(client_socket, (SOCKADDR*)&server_data, sizeof(SOCKADDR)) == SOCKET_ERROR)/*这个函数用来发送请求，第一个参数为发送请求的socket，第二个参数为连接对象的信息，这个信息要以指针的形式
                                                                                          给出，指针类型为SOCKADDR，而我们写信息的时候是用SOCKADDR_IN写的，因此要把取地址后的指针转换为函数参数
                                                                                          要求的SOCKADDR指针*/
    {
        cout << "连接失败，可能原因时服务器没有开" << endl;
        closesocket(client_socket);//释放套接字资源
        WSACleanup();//关闭依赖库。
        return 0;

    }

    //连接成功了，开始聊天
    while (true)
    {
        char send_message[100];
        char receive_message[100];
        cout << "请输入你要对服务器说的话：";
        cin >> send_message;
        int sendlen = send(client_socket, send_message, 100, 0);//sendlen为实际发送的字符串长度。

        if (sendlen < 0)
        {
            cout << "发送失败" << endl;
            break;
        }
        cout << "你说：" << send_message << endl;
        cout << "等待服务器的回信" << endl;
        int receive_len = recv(client_socket, receive_message, 100, 0);
        if (receive_len < 0)
        {
            cout << "接受失败" << endl;
            break;
        }
        cout << "服务器说：" << receive_message << endl;

    }
    cout << "聊天结束" << endl;
    closesocket(client_socket);//释放套接字资源
    WSACleanup();//关闭依赖库。
    return 0;

}

