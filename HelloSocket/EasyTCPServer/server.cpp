#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLengh;	// 数据长度
	short cmd;			// 命令		
};

// DataPackage
struct Login : public DataHeader
{
	Login() {
		dataLengh = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char PassWord[32];
};

struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLengh = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;

	}
	int result;
	
};

struct LogOut : public DataHeader
{
	LogOut() {
		dataLengh = sizeof(LogOut);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};

struct LogOutResult : public DataHeader
{
	LogOutResult() {
		dataLengh = sizeof(LogOutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
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

	printf("新客户端加入：socket = %d, IP = %s \n", (int)(_cSock), inet_ntoa(clientAddr.sin_addr)); //这个IP地址时客户端链接过来的地址，不是客户端的地址
	//send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
	
	while (true)
	{
		
		char szRev[1024];
		// 5 接收客户端的请求数据
		int nLen = recv(_cSock, szRev, sizeof(DataHeader), 0);
		DataHeader *header = (DataHeader*)szRev;

		if (nLen <= 0)
		{
			printf("客户端已经退出！,任务结束\n");
			break;
		}
		else
		{
			printf("收到命令：%d 数据长度：%d \n", header->cmd, header->dataLengh);
		}
		// 6 处理请求
		switch (header->cmd)
		{
		case CMD_LOGIN:
		{
			Login *login;
			recv(_cSock, szRev +sizeof(DataHeader), header->dataLengh - sizeof(DataHeader), 0);
			login = (Login*)szRev;
			printf("收到命令：CMD_LOGIN,数据长度：%d, userName = %s, PassWord = %s\n", login->dataLengh, login->userName, login->PassWord);

			// 忽略判断用户名密码
			LoginResult ret = {};

			send(_cSock, (char*)&ret, sizeof(LoginResult), 0);

		}
			break;
		case CMD_LOGOUT:
			{
				LogOut *logout;
				recv(_cSock, szRev+sizeof(DataHeader), header->dataLengh - sizeof(DataHeader), 0);
				logout = (LogOut *)szRev;
				printf("收到命令：CMD_LOGOUT,数据长度：%d, userName = %s,\n", logout->dataLengh, logout->userName);
				// 忽略判断用户名密码
				//printf();
				LogOutResult ret = {};
				send(_cSock, (char*)&ret, sizeof(LogOutResult), 0);

			}
			break;

		default:
			header->cmd = CMD_ERROR;
			header->dataLengh = 0;
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