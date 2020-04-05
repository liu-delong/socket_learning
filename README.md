# c-c-socket
这个库是我学习c/c++socket编程的源代码，需要用带ws2_32.lib这个库，我连同他一起上传。
如果你的IDE是VC,不需要理会lib这个文件夹。也不需要做任何处理，直接编译运行就行。
如果不是，其他IDE是没有ws2_32.lib这个库的，需要自行载入。拿codeblocks举例，你先把lib文件夹整个文件夹复制到codeblocks创建的工程文件夹中，然后在settings->Complier->Linker_settings->add中输入lib文件夹中那个文件的路径。比如我的是E:\my university\learning\cpp\myc++\socket\lib\WSOCK32.LIB。这个路径从头到尾不能有中文。

socket_server socket_client 是我的第一个，最简单的客户端与服务器程序。
echo_server echo_client 的是服务器逐一和5个客户端进行回声通讯，客户端发什么，服务器回复什么。