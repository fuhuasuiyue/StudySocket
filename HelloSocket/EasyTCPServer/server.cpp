#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;
	int re = bind(_sock, (sockaddr*)&_sin, sizeof(_sin));
	if (re == SOCKET_ERROR)
	{
		printf("ERROR, 绑定用于接手客户端的链接的网络端口失败\n");
		return -1;
	}
	{
		printf("绑定网络端口成功...\n");
	}

	re = listen(_sock, 5);
	if (re == SOCKET_ERROR)
	{
		printf("ERROR, 监听用于接手客户端的链接的网络端口失败\n");
		return -1;
	}
	{
		printf("监听网络端口成功...\n");
	}
	// 4 accept 等待接收客户端链接
	sockaddr_in clientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	char msgBuf[] = "Hello, I'm Server!\n";

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);

	if (INVALID_SOCKET == _cSock)
	{
		printf("错误， 接受到无效的客户端链接...\n");

	}

	printf("新客户端加入：socket = %d, IP = %s \n", (int)(_cSock), inet_ntoa(clientAddr.sin_addr));
	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	
	while (true)
	{
		char cmdBuf[128] = {};
		// 5 接收客户端的请求数据
		int nLen = recv(_cSock, cmdBuf, 128, 0);
		if (nLen <= 0)
		{
			printf("客户端已经退出！,任务结束\n");
			break;
		}
		else
		{
			printf("服务器接收的数据为：%s \n", cmdBuf);
		}
		// 6 处理请求
		if (0 == strcmp(cmdBuf, "getName"))
		{
			char msgBuf[] = "xiaoqiang\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else if (0 == strcmp(cmdBuf, "getAge"))
		{
			char msgBuf[] = "80岁\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else
		{
			char msgBuf[] = "?????...\n";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);

		}

		

	}


	closesocket(_sock);

	getchar();

	WSACleanup();
	return 0;
	
}