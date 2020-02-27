#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

enum CMD
{
	CMD_LOGIN,
	CMD_LOGOUT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLengh;	// 数据长度
	short cmd;			// 命令		
};

// DataPackage
struct Login
{
	char userName[32];
	char PassWord[32];
};

struct LoginResult
{
	int result;
	
};

struct LogOut
{
	char userName[32];
};

struct LogOutResult
{
	int result;

};

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

	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);

	if (INVALID_SOCKET == _cSock)
	{
		printf("错误， 接受到无效的客户端链接...\n");

	}

	printf("新客户端加入：socket = %d, IP = %s \n", (int)(_cSock), inet_ntoa(clientAddr.sin_addr));
	//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	
	while (true)
	{
		DataHeader header = {};
		// 5 接收客户端的请求数据
		int nLen = recv(_cSock, (char*)&header, sizeof(header), 0);
		if (nLen <= 0)
		{
			printf("客户端已经退出！,任务结束\n");
			break;
		}
		else
		{
			printf("收到命令：%d 数据长度：%d \n", header.cmd, header.dataLengh);
		}
		// 6 处理请求
		switch (header.cmd)
		{
		case CMD_LOGIN:
		{
			Login login = {};
			recv(_cSock, (char*)&login, sizeof(Login), 0);
			// 忽略判断用户名密码
			//printf();
			LoginResult ret = {0};
			send(_cSock, (char*)&header, sizeof(DataHeader), 0);

			send(_cSock, (char*)&ret, sizeof(LoginResult), 0);

		}
			break;
		case CMD_LOGOUT:
			{
				LogOut logout = {};
				recv(_cSock, (char*)&logout, sizeof(LogOut), 0);
				// 忽略判断用户名密码
				//printf();
				LogOutResult ret = { 1 };
				send(_cSock, (char*)&header, sizeof(DataHeader), 0);
				send(_cSock, (char*)&ret, sizeof(LogOutResult), 0);

			}
			break;

		default:
			header.cmd = CMD_ERROR;
			header.dataLengh = 0;
			send(_cSock, (char*)&header, sizeof(DataHeader), 0);
			break;
		}

		//if (0 == strcmp(cmdBuf, "getInfo"))
		//{
		//	DataPackage dp = {80, "张国荣"};
		//	
		//	send(_cSock, (char*)&dp, sizeof(DataPackage), 0);
		//}
		//else
		//{
		//	char msgBuf[] = "?????...\n";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);

		//}

		

	}


	closesocket(_sock);

	getchar();

	WSACleanup();
	return 0;
	
}