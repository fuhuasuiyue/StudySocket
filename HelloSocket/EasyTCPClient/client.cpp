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

	SOCKET _sock = socket(AF_INET, SOCK_STREAM, 0);

	if (_sock == INVALID_SOCKET)
	{
		printf("建立SOCKET失败...\n");
	}
	else
	{
		printf("建立SOCKET成功...\n");
	}

	// 2、连接服务器
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//_sin.sin_addr.S_un.S_addr = inet_addr("192.168.109.1");
	int re = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));

	if (re == SOCKET_ERROR)
	{
		printf("错误， 链接错误...\n");
	}
	else
	{
		printf("链接成功...\n");
	}


	char cmdBuf[128] = {};
	while (true)
	{

		// 3 输入命令
		scanf("%s", cmdBuf);
		printf("\n\n输入的命令是： %s\n\n", cmdBuf);
		// 4 处理请求命令
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("收到Exit退出命令\n");
			break;
		}
		else if (0 == strcmp(cmdBuf, "login"))
		{
			printf("我发送的消息是：%s \n", cmdBuf);
			Login login = {"lyd", "lydmm"};
			DataHeader header = { sizeof(Login),CMD_LOGIN};
			send(_sock, (char*)&header, sizeof(DataHeader), 0);
			send(_sock, (char*)&login, sizeof(Login), 0);

			// 接收服务器返回数据
			DataHeader retHeader = {};

			LoginResult retLogin = {};
			recv(_sock, (char*)&retHeader, sizeof(DataHeader), 0);
			recv(_sock, (char*)&retLogin, sizeof(LoginResult), 0);

			printf("LogInResult: %d\n", retLogin.result);
			
		}
		else if (0 == strcmp(cmdBuf, "logout"))
		{
			printf("我发送的消息是：%s \n", cmdBuf);
			LogOut logout = {"lyd"};
			DataHeader dh = { sizeof(logout), CMD_LOGOUT};
			send(_sock, (char*)&dh, sizeof(DataHeader), 0);
			send(_sock, (char*)&logout, sizeof(LogOut), 0);

			// 接收服务器返回数据
			DataHeader retHeader = {};

			LogOutResult retLogout = {};
			recv(_sock, (char*)&retHeader, sizeof(DataHeader), 0);
			recv(_sock, (char*)&retLogout, sizeof(LogOutResult), 0);

			printf("LogOutResult: %d\n", retLogout.result);
			
		}
		else
		{
			printf("不支持的命令，请重新输入！\n");
		}

		

	}

	closesocket(_sock);
	getchar();




	WSACleanup();
	return 0;

}